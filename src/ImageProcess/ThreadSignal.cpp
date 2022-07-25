#include"ImageProcess.h"

/*-----------------------------------------------------------------------------------------------------
 * 初始化图形处理类的内存
 * @name: initImageProcess
*------------------------------------------------------------------------------------------------------*/
bool ImageProcess::initImageProcess()
{
          return this->initFaceDetection() && this->initResnetModel();
}

/*-----------------------------------------------------------------------------------------------------
 * 释放图形处理类的内存
 * @name:releaseImageProcess;
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::releaseImageProcess()
{
          this->releaseResnetModel();
          this->releaseFaceDetection();
}

/*------------------------ImageProcess的线程信号量通信函数接口-------------------------*/
/*-----------------------------------------------------------------------------------------------------
 * 设置人脸图像就绪信号
 * @name:setImageSyncSignal
 * @function: 用来指示其他线程有关于数据更新的状态
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::setImageSyncSignal()
{
          this->m_imageReadySign = true;
          this->m_imageReady.notify_all();                        //通知线程条件变量可以工作
}

/*-----------------------------------------------------------------------------------------------------
 * 设置人脸位置就绪信号
 * @name:setRectSyncSignal
 * @function: 用来指示其他线程有关于数据更新的状态
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::setRectSyncSignal()
{
          this->m_faceRectReadySign = true;
          this->m_rectReady.notify_all();                        //通知线程条件变量可以工作
}

/*-----------------------------------------------------------------------------------------------------
 * 获取当前的图片是否被同步
 * @name: getImageSyncStatus
 * @function: 用来指示其他线程有关于数据更新的状态
 * @retValue: 1.true : 当前数据的状态为旧状态
 *                    2.false: 当前数据的状态为新状态
*------------------------------------------------------------------------------------------------------*/
bool  ImageProcess::getImageSyncStatus()
{
          return    this->m_imageReadySign;
}

/*-----------------------------------------------------------------------------------------------------
 * 获取当前人脸位置是否被同步
 * @name: getRectSyncStatus
 * @function: 用来指示其他线程有关于数据更新的状态
 * @retValue: 1.true : 当前数据的状态为旧状态
 *                    2.false: 当前数据的状态为新状态
*------------------------------------------------------------------------------------------------------*/
bool  ImageProcess::getRectSyncStatus()
{
          return   this->m_faceRectReadySign;
}

/*-----------------------------------------------------------------------------------------------------
 * 重置人脸图像就绪信号
 * @name:resetImageSyncStatus
 * @function: 用来指示其他线程有关于数据更新的状态
*------------------------------------------------------------------------------------------------------*/
void  ImageProcess::resetImageSyncStatus()
{
          this->m_imageReadySign = false;
}

/*-----------------------------------------------------------------------------------------------------
 * 重置人脸位置就绪信号
 * @name: resetRectSyncStatus;
 * @function: 用来指示其他线程有关于数据更新的状态
*------------------------------------------------------------------------------------------------------*/
void  ImageProcess::resetRectSyncStatus()
{
          this->m_faceRectReadySign = false;
}

/*-----------------------------------------------------------------------------------------------------
 * 重置人脸图像和位置就绪信号
 * @name: resetDataSyncSignal;
 * @function: 用来指示其他线程有关于数据更新的状态
*------------------------------------------------------------------------------------------------------*/
void  ImageProcess::resetDataSyncSignal()
{
          this->resetImageSyncStatus();
          this->resetRectSyncStatus();
}

/*-----------------------------------------------------------------------------------------------------
 * 设置视频显示线程的暂停
 * @name: setvideoPause
 * @function: 暂停和继续视频的显示线程
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::setvideoPause()
{
          this->m_videoDisplaySign = false;
          m_videoDisplayCtrl.notify_all();                        //通知线程条件变量可以工作(可能存在Bug)
}

/*-----------------------------------------------------------------------------------------------------
 * 设置视频显示线程的继续
 * @name: setvideoContinue
 * @function: 暂停和继续视频的显示线程
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::setvideoContinue()
{
          this->m_videoDisplaySign = true;
          m_videoDisplayCtrl.notify_all();                        //通知线程条件变量可以工作(可能存在Bug)
}

/*-----------------------------------------------------------------------------------------------------
 * 视频的显示线程，用于控制暂停和继续
 * @name: isVideoPaused
 * @function: 暂停和继续视频的显示线程
 * @retValue: 1.true : 启动视频的显示
 *                    2.false: 暂停视频的显示
*------------------------------------------------------------------------------------------------------*/
bool ImageProcess::isVideoPaused()
{
          return m_videoDisplaySign;
}

/*-----------------------------------------------------------------------------------------------------
 * 其他线程判断摄像头是否启动拍照
 * @name: getCameraState
 * @function: 判断摄像头拍照
 * @RetValue:1.true : 允许拍照
 *                    2.false: 不允许拍照
*------------------------------------------------------------------------------------------------------*/
bool ImageProcess::getCameraState()
{
          bool _retValue(false);
          {      //读取当前摄像头视频的锁
                    //std::lock_guard<std::mutex> _lckg(this->m_cameraSwitchSync.second);
                    _retValue = static_cast<bool>(this->m_cameraSwitchSync.first);                  //读取之前的数据
                    this->m_cameraSwitchSync.first = CameraSwitch::NO_INPUT;                     //重置之前的摄像机状态
          }
          return _retValue;
}


/*-----------------------------------------------------------------------------------------------------
 * 其他线程判断保存功能是否启动
 * @name: getImageSavingState
 * @function: 判断摄像头拍照
 * @RetValue:1.true : 得到用户的确认(无论保存或舍弃)
 *                    2.false: 没有得到用户的确认
*------------------------------------------------------------------------------------------------------*/
bool ImageProcess::getImageSavingState()
{
          bool _retValue(false);
          {      //读取当前摄像头视频的锁
                    //std::lock_guard<std::mutex> _lckg(this->m_savingSwitchSync.second);
                    _retValue = static_cast<bool>(this->m_savingSwitchSync.first);                  //读取之前的数据
                    this->m_savingSwitchSync.first = SavingSwitch::DEFAULT;                     //重置之前的保存状态
          }
          return _retValue;
}