#include"ImageProcess.h"

/*
 * cv::Mat ----> QImage
 * @name: mat2Qimage
 * @function：将MAT类型转换为QT的QImage类型
 * @param 输入原始图像  const cv::Mat& mat
 * @retValue : 返回位于本类中QImage&引用类型
 * @Correction: 2022-7-21 在类内部引入QImage临时存储结构，可以使用引用加速
 *                        2022-7-21 删除函数中多余的部分，仅仅保留CV8U_C3下的处理方式，并引入内联函数
*/
inline QImage& ImageProcess::mat2Qimage(const cv::Mat& mat)
{
          return  (m_qimageFrameStore = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped());
}

/*
 * 实时摄像头图像+人脸检测+人脸识别输出显示接口
 * @name: realTimeFacialDisplay
 * @function：其他的功能调用通过与该程序连接的线程进行操作
 * @param  1. 图像修改读写锁  std::mutex& _writeMutex
 *                  2. 输入原始图像   cv::Mat& mat
 * 
 * @retValue: QImage &
*/
QImage &ImageProcess::realTimeFacialDisplay()
{
          [=]() {               //条件变量设置：可能需要等待其他模块的信号，才可以继续进行执行
                    std::unique_lock<std::mutex> _lckg(this->m_videoDisplaySync);
                    this->m_videoDisplayCtrl.wait(_lckg, [=]() 
                    {
                              return this->isVideoPaused();                               //视频是否开启暂停状态
                    });
          }();                     
          this->resetDataSyncSignal();                                                                                                                                        //重置数据就绪信号
          this->m_imageSync.first = this->getImageFrame(this->m_imageSync.second);                                                        //共享人脸图像
          this->setImageSyncSignal();                                                                                                                                         //当前的人脸图像已经更新
          this->m_faceRectSync.first= this->getFaceRectangle(this->m_imageSync.first, this->m_faceRectSync.second);    //共享人脸位置
          this->setRectSyncSignal();                                                                                                                                           //当前的人脸位置已经更新
          this->drawGeometryOnImage(this->m_imageSync.second, this->m_faceRectSync.first, "");
          return mat2Qimage(this->m_imageSync.first);
}


/*
 * 通过摄像头图像进行实时人脸训练功能模块
 * @name: realTimeFacialDisplay
 * @function：和实时摄像头图像协同进行人脸训练功能
 * @param:  视频开关 std::atomic<bool> &
 * @Correction: 2022-7-24 添加函数参数修复防止线程无法正确的停止运转
*/
void ImageProcess::videoSyncFacialTranning(std::atomic<bool>& _videoFlag)
{
          while (!_videoFlag)
          {
                    /*检查当前拍照信号，循环等待外部的摄像头是否启动拍照*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_cameraSwitchSync.second);
                              this->m_cameraCtrl.wait(_lckg, [=]() {
                                                  return this->getCameraState();
                              });
                    }();

                    this->setvideoPause();                                                                                                         //暂停显示线程的正常输出

                    /*检查人脸图像是否准备好，避免死锁情况的发生*/
                    [=]() {            
                              std::unique_lock<std::mutex> _lckg(this->m_imageSync.second);
                              this->m_imageReady.wait(_lckg, [=](){
                                                  return this->getImageSyncStatus();
                              });
                              this->m_copiedImage = this->m_imageSync.first;
                    }();
                    this->resetImageSyncStatus();                                                                                             //取消人脸图像的就绪状态

                    /*检查人脸位置是否准备好，避免死锁情况的发生*/
                    [=]() {              
                              std::unique_lock<std::mutex> _lckg(this->m_faceRectSync.second);
                              this->m_rectReady.wait(_lckg, [=]() {
                                                  return this->getRectSyncStatus();
                              });
                              this->m_copiedfaceRect = this->m_faceRectSync.first;
                    }();
                    this->resetRectSyncStatus();                                                                                              //取消人脸位置的就绪状态

                    /*检查当前保存信号，循环等待外部是保存或删除信号*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_savingSwitchSync.second);
                              this->m_saving = this->m_savingSwitchSync.first;                                                 //保存当前控制状态机
                              this->m_savingCtrl.wait(_lckg, [=]()  {
                                                  return this->getImageSavingState();
                               });
                    }();

                    this->setvideoContinue();                                                                                                  //恢复显示线程的正常输出

                    if (this->m_saving == SavingSwitch::SAVING_FACE)
                    {
                              this->m_faceLandmark = this->getFaceLandmark(this->m_copiedImage, this->m_copiedfaceRect);   //获取特征点
                              if (!externalInput(this->m_copiedImage, this->m_faceLandmark)) {       //返回值若为false则代表训练集输入完毕
                                        break;
                              }
                    }
          }
}

/*
 * 启动人脸的显示线程
 * @name:  startVideoDisplay
 * @function: 实时摄像头图像+人脸检测+人脸识别输出显示接口
 * @retValue: QImage &
*/
QImage& ImageProcess::startVideoDisplay()
{
          return this->realTimeFacialDisplay();
}

/*
 * 启动人脸训练程序
 * @name:  startVideoRegister
 * @function: 开启当前视频拍摄，启动人脸训练程序
 * @param:  视频开关 std::atomic<bool> &
 * @Correction: 2022-7-24 添加函数参数修复防止线程无法正确的停止运转
*/
void  ImageProcess::startVideoRegister(std::atomic<bool>& _videoFlag)
{
          this->m_threadPool.emplace_back(&ImageProcess::videoSyncFacialTranning, this, std::ref(_videoFlag));
}