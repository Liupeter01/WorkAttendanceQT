#include "WorkAttendanceQT.h"

WorkAttendanceQT::WorkAttendanceQT(QWidget* parent)
          : QDialog(parent), Interface(10)
{
          ui.setupUi(this);
          QObject::connect(ui.Exit, SIGNAL(clicked()), this, SLOT(videoStreamClose()));                                 //¹Ø±ÕÊÓÆµÁ÷
          QObject::connect(ui.TakePicture, SIGNAL(clicked()), this, SLOT(takePictureForTranning()));             //¿ªÆôÅÄÕÕÄ£¿é
          QObject::connect(ui.Register, SIGNAL(clicked()), this, SLOT(registerEmployee()));                            //ÈËÁ³ÑµÁ·¹¦ÄÜÄ£¿é
          QObject::connect(ui.ConfirmTranningSet, SIGNAL(clicked()), this, SLOT(savePicture()));        //±£´æÈËÁ³
          QObject::connect(ui.IgnoreTranningSet, SIGNAL(clicked()), this, SLOT(ignorePicture()));           //ÉáÆúÈËÁ³
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

void WorkAttendanceQT::videoStreamClose() 
{
          this->videoClose();
}

void WorkAttendanceQT::registerEmployee()                          //Æô¶¯ÑµÁ·Ïß³Ì
{
          this->startVideoRegister(this->m_videoFlag, this->ui.SystemStatusInfo);
}

void WorkAttendanceQT::takePictureForTranning() 
{
          this->videoCameraShooting();
}

void WorkAttendanceQT::savePicture()                                    //±£´æÕÕÆ¬
{
          this->videoFrameSavingProcess();
}

void WorkAttendanceQT::ignorePicture()                                 //ºöÂÔÕÕÆ¬
{
          this->videoFrameIgnoreProcess();
}