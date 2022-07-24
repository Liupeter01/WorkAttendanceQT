#include"ImageProcess.h"

ImageProcess::ImageProcess(int TrainningSetting)
          :FaceDetecion(), ModelTrain(TrainningSetting), TakePicture() 
{
          m_threadPool.emplace_back(&ImageProcess::initImageProcess, this);
          this->setvideoContinue();                                                                            //视频的连续播放
          this->m_cameraSwitchSync.first = CameraSwitch::NO_INPUT;              //单次拍照--当前没有输入
          this->m_savingSwitchSync.first = SavingSwitch::DEFAULT;                  //保存人脸--当前没有操作
}

ImageProcess::~ImageProcess()
{
          //this->releaseImageProcess();
          for (auto& i : m_threadPool) {
                    if (i.joinable()) {
                              i.join();
                    }
          }
}

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
 * 初始化图形处理类的内存
 * @name: initImageProcess
*/
bool ImageProcess::initImageProcess()
{
          return this->initFaceDetection() && this->initResnetModel();
}

/*
 * 释放图形处理类的内存
 * @name:releaseImageProcess;
*/
void ImageProcess::releaseImageProcess()
{
          this->releaseResnetModel();
          this->releaseFaceDetection();
}

/*
 * 设置人脸就绪信号
 * @name:setImageSyncSignal
 * @function: 用来指示其他线程有关于数据更新的状态
*/
void ImageProcess::setImageSyncSignal()
{
          this->m_imageReadySign = true;
}

/*
 * 设置人脸位置就绪信号
 * @name:setRectSyncSignal
 * @function: 用来指示其他线程有关于数据更新的状态
*/
void ImageProcess::setRectSyncSignal()
{
          this->m_faceRectReadySign = true;
}

/*
 * 获取当前的图片是否被同步
 * @name: getImageSyncStatus
 * @function: 用来指示其他线程有关于数据更新的状态
 * @retValue: 1.true : 当前数据的状态为旧状态
 *                    2.false: 当前数据的状态为新状态
*/
bool  ImageProcess::getImageSyncStatus()
{
          return    this->m_imageReadySign;
}

/*
 * 获取当前人脸位置是否被同步
 * @name: getRectSyncStatus
 * @function: 用来指示其他线程有关于数据更新的状态
 * @retValue: 1.true : 当前数据的状态为旧状态
 *                    2.false: 当前数据的状态为新状态
*/
bool  ImageProcess::getRectSyncStatus()
{
          return   this->m_faceRectReadySign;
}

/*
 * 重置人脸图像就绪信号
 * @name:resetImageSyncStatus
 * @function: 用来指示其他线程有关于数据更新的状态
*/
void  ImageProcess::resetImageSyncStatus()
{
          this->m_imageReadySign = false;
}

/*
 * 重置人脸位置就绪信号
 * @name: resetRectSyncStatus;
 * @function: 用来指示其他线程有关于数据更新的状态
*/
void  ImageProcess::resetRectSyncStatus()
{
          this->m_faceRectReadySign = false;
}

/*
 * 重置人脸图像和位置就绪信号
 * @name: resetDataSyncSignal;
 * @function: 用来指示其他线程有关于数据更新的状态
*/
void  ImageProcess::resetDataSyncSignal()
{
          this->resetImageSyncStatus();
          this->resetRectSyncStatus();
}

/*
 * 设置视频显示线程的暂停
 * @name: setvideoPause
 * @function: 暂停和继续视频的显示线程
*/
void ImageProcess::setvideoPause()
{
          this->m_videoDisplaySign = false;
}

/*
 * 设置视频显示线程的继续
 * @name: setvideoContinue
 * @function: 暂停和继续视频的显示线程
*/
void ImageProcess::setvideoContinue()
{
          this->m_videoDisplaySign = true;
}

/*
* 视频的显示线程，用于控制暂停和继续
* @name: isVideoPaused
* @function: 暂停和继续视频的显示线程
* @retValue: 1.true : 启动视频的显示
*/
bool ImageProcess::isVideoPaused()
{
          return m_videoDisplaySign;
}

/*
 * 开启摄像头的拍照功能
 * @name: enableCameraPhotography
 * @function: 开启当前帧的视频拍摄功能
*/
void ImageProcess::enableCameraShooting()
{
          //开启当前视频的拍照功能
          std::lock_guard<std::mutex> _lckg(this->m_cameraSwitchSync.second);
          this->m_cameraSwitchSync.first = CameraSwitch::TAKE_PICTURE;
}

/*
 * 读取摄像头是否已经被开启
 * @name: readCameraStatus
 * @function: 读取摄像拍照的数据
 * @RetValue:1.true : 允许拍照
 *                    2.false: 不允许拍照
*/
bool ImageProcess::readCameraStatus()
{
          bool _retValue(false);
          {      //读取当前摄像头视频的锁
                    std::lock_guard<std::mutex> _lckg(this->m_cameraSwitchSync.second);
                    _retValue = static_cast<bool>(this->m_cameraSwitchSync.first);                  //读取之前的数据
                    this->m_cameraSwitchSync.first = CameraSwitch::NO_INPUT;                     //重置之前的摄像机状态
          }
          return _retValue;
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
*/
void ImageProcess::videoSyncFacialTranning()
{
          while (true) {
                    /*检查当前拍照信号，循环等待外部的拍照摁键输入*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_cameraSwitchSync.second);
                              this->m_cameraCtrl.wait(_lckg, [=]() {return true; });
                    }();

                    //暂停显示线程的正常输出

                    /*检查人脸图像是否准备好，避免死锁情况的发生*/
                    [=]() {            
                              std::unique_lock<std::mutex> _lckg(this->m_imageSync.second);
                              this->m_imageReady.wait(_lckg, [=]() {return this->getImageSyncStatus(); });
                              this->m_copiedImage = this->m_imageSync.first;
                    }();
                    this->resetImageSyncStatus();                                                                                             //取消人脸图像的就绪状态

                    /*检查人脸位置是否准备好，避免死锁情况的发生*/
                    [=]() {              
                              std::unique_lock<std::mutex> _lckg(this->m_faceRectSync.second);
                              this->m_rectReady.wait(_lckg, [=]() {return this->getRectSyncStatus(); });
                              this->m_copiedfaceRect = this->m_faceRectSync.first;
                    }();
                    this->resetRectSyncStatus();                                                                                              //取消人脸位置的就绪状态

                    /*检查当前保存信号，循环等待外部是保存或删除信号*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_savingSwitchSync.second);
                              this->m_savingCtrl.wait(_lckg, [=]() {return true; });
                    }();

                    //恢复显示线程的正常输出
                    if (false)
                    {
                              this->m_faceLandmark = this->getFaceLandmark(this->m_copiedImage, this->m_copiedfaceRect);   //获取特征点
                              if (!externalInput(this->m_copiedImage, this->m_faceLandmark)) {       //返回值若为false则代表训练集输入完毕
                                        break;
                              }
                    }
          }
}