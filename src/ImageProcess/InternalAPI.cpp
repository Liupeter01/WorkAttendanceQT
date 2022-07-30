#include"ImageProcess.h"

ImageProcess::ImageProcess(int _TrainningSetting, double _TrainningSimilarity)
          :FaceDetecion(), TakePicture(),
          ModelTrain(_TrainningSetting, _TrainningSimilarity)
{
          m_threadPool.emplace_back(&ImageProcess::initImageProcess, this);
          this->m_cameraSwitchSync.first = CameraSwitch::NO_INPUT;              //单次拍照--当前没有输入
          this->m_savingSwitchSync.first = SavingSwitch::DEFAULT;                  //保存人脸--当前没有操作
          this->m_videoDisplaySign = true;                                                             //视频的连续播放，防止没有意义的唤醒操作
}

ImageProcess::~ImageProcess()
{
          for (auto& i : m_threadPool) {
                    if (i.joinable()) {
                              i.join();
                    }
          }
}

/*-----------------------------------------程序的通用工具---------------------------------------*/
/*------------------------------------------------------------------------------------------------------
 * cv::Mat ----> QImage
 * @name: mat2Qimage
 * @function：将MAT类型转换为QT的QImage类型
 * @param 输入原始图像  const cv::Mat& mat
 * @retValue : 返回位于本类中QImage&引用类型
 * @Correction: 2022-7-21 在类内部引入QImage临时存储结构，可以使用引用加速
 *                        2022-7-21 删除函数中多余的部分，仅仅保留CV8U_C3下的处理方式，并引入内联函数
*------------------------------------------------------------------------------------------------------*/
inline QImage& ImageProcess::mat2Qimage(const cv::Mat& mat)
{
          return  (m_qimageFrameStore = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped());
}

/*------------------------------------------------------------------------------------------------------
 * 将启动日志信息输出到SystemStatusInfo窗口中
 * @name:  printOnTextBroswer
 * @function: 将启动日志信息输出到SystemStatusInfo窗口中
 * @param:  1. 输出窗口接口：QTextBrowser*& _systemOutput
 *                  2. 输出的wstring的字符串：const std::wstring& _wstring
*------------------------------------------------------------------------------------------------------*/
inline void ImageProcess::printOnTextBroswer(
          QTextBrowser*& _systemOutput,
          const std::wstring& _wstring
)
{
          _systemOutput->insertPlainText(QString::fromStdWString(_wstring));
}

/*-------------------------------为内部函数提供的IxternalAPI------------------------------*/
/*------------------------------------------------------------------------------------------------------
 * 实时摄像头图像+人脸检测+人脸识别输出显示接口
 * @name: realTimeFacialDisplay
 * @function：其他的功能调用通过与该程序连接的线程进行操作
 * @param : 输出窗口接口：QTextBrowser*& _systemOutput
 * @retValue: QImage &
 *------------------------------------------------------------------------------------------------------*/
QImage& ImageProcess::realTimeFacialDisplay(QTextBrowser*& _systemOutput)
{
          [=]() {               //条件变量设置：可能需要等待其他模块的信号，才可以继续进行执行
                    std::unique_lock<std::mutex> _lckg(this->m_videoDisplaySync);
                    this->m_videoDisplayCtrl.wait(_lckg, [=]() { return this->isVideoPaused();});                                               //视频是否开启暂停状态
          }();
          this->resetDataSyncSignal();                                                                                                                                        //重置数据就绪信号
          this->m_imageSync.first = this->getImageFrame(this->m_imageSync.second);                                                        //共享人脸图像
          this->setImageSyncSignal();                                                                                                                                         //当前的人脸图像已经更新
          this->m_faceRectSync.first = this->getFaceRectangle(this->m_imageSync.first, this->m_faceRectSync.second);    //共享人脸位置
          this->setRectSyncSignal();                                                                                                                                           //当前的人脸位置已经更新
          this->drawGeometryOnImage(this->m_imageSync.second, this->m_faceRectSync.first, "");
          return this->mat2Qimage(this->m_imageSync.first);                                                                                                     //需要修改！！！
}

/*------------------------------------------------------------------------------------------------------
 * 启动人脸训练程序
 * @name:  videoSyncFacialTranning
 * @function: 开启当前视频拍摄，启动人脸训练程序
 * @param:  1.视频开关 std::atomic<bool> &
 *                  2.输出窗口接口：QTextBrowser*& _systemOutput
 *                  3.进度条的输出接口：QProgressBar* &_processBar
 *                  4.进度数值显示器 : int _displayNumber
 *
 * @Correction: 2022-7-24 添加函数参数修复防止线程无法正确的停止运转
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::videoSyncFacialTranning(
          std::atomic<bool>& _videoFlag, 
          QTextBrowser*& _systemOutput,
          QProgressBar* &_processBar,
          int _displayNumber)
{
          while (!_videoFlag)
          {
                    /*检查当前拍照信号，循环等待外部的摄像头是否启动拍照*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_cameraSwitchSync.second);
                              this->m_cameraCtrl.wait(_lckg, [=]() { return this->getCameraState(); });
                    }();
                    this->setvideoPause();                                                                                                                                          //暂停显示线程的正常输出
                    /*检查人脸图像是否准备好，避免死锁情况的发生*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_imageSync.second);
                              this->m_imageReady.wait(_lckg, [=]() { return this->getImageSyncStatus();});
                              this->m_copiedImage = this->m_imageSync.first;
                    }();
                    this->resetImageSyncStatus();                                                                                                                              //取消人脸图像的就绪状态
                    /*检查人脸位置是否准备好，避免死锁情况的发生*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_faceRectSync.second);
                              this->m_rectReady.wait(_lckg, [=]() { return this->getRectSyncStatus(); });
                              this->m_copiedfaceRect = this->m_faceRectSync.first;
                    }();
                    this->resetRectSyncStatus();                                                                                                                               //取消人脸位置的就绪状态
                    /*检查当前保存信号，循环等待外部是保存或删除信号*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_savingSwitchSync.second);
                              this->m_saving = this->m_savingSwitchSync.first;                                                                                  //保存当前控制状态机
                              this->m_savingCtrl.wait(_lckg, [=]() { return this->getImageSavingState();});
                    }();
                    this->setvideoContinue();                                                                                                                                    //恢复显示线程的正常输出
                    if (this->m_saving == SavingSwitch::SAVING_FACE)
                    {
                              this->m_faceLandmark = this->getFaceLandmark(this->m_copiedImage, this->m_copiedfaceRect);   //获取特征点
                              if (!this->externalInput(_videoFlag, this->m_copiedImage, this->m_faceLandmark))                           //返回值若为false则代表训练集输入完毕
                              {      
                                        break;
                              }
                              _systemOutput->insertPlainText(QString::number(++_displayNumber) + "\n");
                    }
          }
}

/*------------------------------------------------------------------------------------------------------
 * 启动人脸注册运行训练程序
 * @name:  modelSetTranning
 * @function: 启动人脸注册之后，运行训练程序
 * @param:  
 *                  1. 输出窗口接口：QTextBrowser*& _systemOutput
 * 
 * @retValue: 返回训练好的特征向量字符串  std::string &
*------------------------------------------------------------------------------------------------------*/
std::string& ImageProcess::modelSetTranning(QTextBrowser*& _systemOutput)
{
          this->m_copidMatrix = this->resnetTrainning();                        //计算人脸模型
          this->convertMatrixToString(this->m_copidMatrix, this->m_copiedMatrixString);
          return this->m_copiedMatrixString;
}