#include "WorkAttendanceQT.h"

WorkAttendanceQT::WorkAttendanceQT(QWidget* parent)
          : QDialog(parent), Interface(10)
{
          ui.setupUi(this);
          QObject::connect(ui.Exit, SIGNAL(clicked()), this, SLOT(videoStreamClose()));                                 //�ر���Ƶ��
          QObject::connect(ui.TakePicture, SIGNAL(clicked()), this, SLOT(takePictureForTranning()));             //��������ģ��
          QObject::connect(ui.Register, SIGNAL(clicked()), this, SLOT(registerEmployee()));                            //����ѵ������ģ��
          QObject::connect(ui.ConfirmTranningSet, SIGNAL(clicked()), this, SLOT(savePicture()));        //��������
          QObject::connect(ui.IgnoreTranningSet, SIGNAL(clicked()), this, SLOT(ignorePicture()));           //��������
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

void WorkAttendanceQT::registerEmployee()                          //����ѵ���߳�
{
          this->startVideoRegister(this->m_videoFlag, this->ui.SystemStatusInfo);
}

void WorkAttendanceQT::takePictureForTranning() 
{
          this->videoCameraShooting();
}

void WorkAttendanceQT::savePicture()                                    //������Ƭ
{
          this->videoFrameSavingProcess();
}

void WorkAttendanceQT::ignorePicture()                                 //������Ƭ
{
          this->videoFrameIgnoreProcess();
}