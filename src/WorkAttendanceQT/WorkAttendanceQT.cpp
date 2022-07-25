#include "WorkAttendanceQT.h"

WorkAttendanceQT::WorkAttendanceQT(QWidget* parent)
          : QDialog(parent), Interface(10)
{
          ui.setupUi(this);
          QObject::connect(ui.Exit, SIGNAL(clicked()), this, SLOT(videoStreamClose()));                                 //关闭视频流
          QObject::connect(ui.TakePicture, SIGNAL(clicked()), this, SLOT(takePictureForTranning()));             //开启拍照模块
          QObject::connect(ui.Register, SIGNAL(clicked()), this, SLOT(registerEmployee()));                            //人脸训练功能模块
          QObject::connect(ui.ConfirmTranningSet, SIGNAL(clicked()), this, SLOT(savePicture()));        //保存人脸
          QObject::connect(ui.IgnoreTranningSet, SIGNAL(clicked()), this, SLOT(ignorePicture()));           //舍弃人脸
          this->m_threadPool.emplace_back(&Interface::QTVideoOutput, this, std::ref(this->ui.VideoDisplay), std::ref(this->ui.SystemStatusInfo));
          this->m_threadPool.emplace_back(&Interface::setLcdTimer, this, std::ref(this->ui.lcdNumber));
}

WorkAttendanceQT::~WorkAttendanceQT()
{
          this->videoClose();
          for (auto& i : this->m_threadPool) {
                    if (i.joinable()) {
                              i.join();
                    }
          }
}

/*------------------------------------------------------------------------------------------------------
* 视频流的关闭开关
* @name : videoStreamClose
* @funtion : 提供操作槽SLOT供给signal信号调用
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceQT::videoStreamClose() 
{
          this->videoClose();
}

/*------------------------------------------------------------------------------------------------------
 * 在训练模型时作为训练集的拍照开关
 * @name : takePictureForTranning
 * @funtion : 提供操作槽SLOT供给signal信号调用
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceQT::takePictureForTranning()
{
          this->videoCameraShooting();
}

/*------------------------------------------------------------------------------------------------------
 * 开始使用训练模型进行训练集的训练
 * @name : initModelTranning
 * @funtion : 提供操作槽SLOT供给signal信号调用
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceQT::initModelTranning()
{
          this->startResnetModelTranning(
                    this->m_videoFlag, 
                    this->ui.UserID->document()->toRawText().toStdString(), 
                    this->ui.NameInput->document()->toRawText().toStdString(),
                    this->ui.SystemStatusInfo
          );
}

/*------------------------------------------------------------------------------------------------------
 * 保存当前的照片图像
 * @name : savePicture
 * @funtion : 提供操作槽SLOT供给signal信号调用
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceQT::savePicture()
{
          this->videoFrameSavingProcess();
}

/*------------------------------------------------------------------------------------------------------
 * 舍弃当前的照片图像
 * @name : ignorePicture
 * @funtion : 提供操作槽SLOT供给signal信号调用
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceQT::ignorePicture()
{
          this->videoFrameIgnoreProcess();
}

/*------------------------------------------------------------------------------------------------------
 * 摁下当前开关启动人脸的训练程序
 * @name : registerEmployee
 * @funtion : 提供操作槽SLOT供给signal信号调用
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceQT::registerEmployee()                          //启动训练线程
{
          this->startVideoRegister(this->m_videoFlag, this->ui.SystemStatusInfo);
}