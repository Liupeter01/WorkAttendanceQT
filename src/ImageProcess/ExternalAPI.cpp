#include"ImageProcess.h"

/*------------------------------为外部函数提供的ExternalAPI--------------------------------*/
/*------------------------------------------------------------------------------------------------------
 * 启动视频的显示线程(与外部GUI连接)
 * @name:  startVideoDisplay
 * @function: 实时摄像头图像+人脸检测+人脸识别输出显示接口
 * @param: 输出窗口接口：QTextBrowser*& _systemOutput
 * @retValue: QImage &
 *------------------------------------------------------------------------------------------------------*/
QImage& ImageProcess::startVideoDisplay(QTextBrowser*& _systemOutput)
{
          return this->realTimeFacialDisplay(_systemOutput);
}

/*------------------------------------------------------------------------------------------------------
 * 启动人脸注册函数(与外部GUI连接)
 * @name:  startVideoRegister
 * @function: 开启当前视频拍摄，启动人脸训练程序
 * @param:  1.视频开关 std::atomic<bool> &
 *                  2.输出窗口接口：QTextBrowser*& _systemOutput
 *
 * @Correction: 2022-7-24 添加函数参数修复防止线程无法正确的停止运转
*------------------------------------------------------------------------------------------------------*/
void  ImageProcess::startVideoRegister(std::atomic<bool>& _videoFlag, QTextBrowser*& _systemOutput)
{
          this->m_threadPool.emplace_back(&ImageProcess::videoSyncFacialTranning, this, std::ref(_videoFlag), std::ref(_systemOutput));
}


/*------------------------------------------------------------------------------------------------------
 * 启动人脸注册之后运行训练程序(与外部GUI连接)
 * @name:  startResnetModelTranning
 * @function: 启动人脸注册之后，运行训练程序
 * @param:  1.视频开关 std::atomic<bool> &
 *                  2. 用户ID的输入  const std::string& _userID
 *                  3. 用户姓名的输入    const std::string& _userName
 *                  4. 输出窗口接口：QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::startResnetModelTranning(
          std::atomic<bool>& _videoFlag,
          const std::string& _userID,
          const std::string& _userName,
          QTextBrowser*& _systemOutput
)
{
          this->m_threadPool.emplace_back(
                    &ImageProcess::modelSetTranning, this,
                    std::ref(_videoFlag), std::ref(_userID),
                    std::ref(_userName), std::ref(_systemOutput)
          );
}

/*------------------------------------------------------------------------------------------------------
 * 命令保存功能启动
 * @name:  enableSavingProcess
 * @function: 开启当前视频拍摄，用户根据相关图像进行保存
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::enableSavingProcess()
{
          //开启当前图像的保存功能
          std::lock_guard<std::mutex> _lckg(this->m_savingSwitchSync.second);
          this->m_savingSwitchSync.first = SavingSwitch::SAVING_FACE;
          this->m_savingCtrl.notify_all();                       //通知线程条件变量可以工作(可能存在Bug)
}

/*------------------------------------------------------------------------------------------------------
 * 命令舍弃功能启动
 * @name:  enableSavingProcess
 * @function: 开启当前视频拍摄，用户根据相关图像进行舍弃
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::enableIgnoreProcess()
{
          //开启当前图像的舍弃功能
          std::lock_guard<std::mutex> _lckg(this->m_savingSwitchSync.second);
          this->m_savingSwitchSync.first = SavingSwitch::DELETE_FACE;
          this->m_savingCtrl.notify_all();                       //通知线程条件变量可以工作(可能存在Bug)
}

/*------------------------------------------------------------------------------------------------------
 * 命令摄像头进行拍照
 * @name: enableCameraShooting
 * @function: 开启当前视频拍摄，拍着某一张图像
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::enableCameraShooting()
{
          //开启当前视频的拍照功能
          std::lock_guard<std::mutex> _lckg(this->m_cameraSwitchSync.second);
          this->m_cameraSwitchSync.first = CameraSwitch::TAKE_PICTURE;
          this->m_cameraCtrl.notify_all();                       //通知线程条件变量可以工作(可能存在Bug)
}