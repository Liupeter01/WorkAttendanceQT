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

/*------------------------------------------------------------------------------------------------------
* ��Ƶ���Ĺرտ���
* @name : videoStreamClose
* @funtion : �ṩ������SLOT����signal�źŵ���
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceQT::videoStreamClose() 
{
          this->videoClose();
}

/*------------------------------------------------------------------------------------------------------
 * ��ѵ��ģ��ʱ��Ϊѵ���������տ���
 * @name : takePictureForTranning
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceQT::takePictureForTranning()
{
          this->videoCameraShooting();
}

/*------------------------------------------------------------------------------------------------------
 * ��ʼʹ��ѵ��ģ�ͽ���ѵ������ѵ��
 * @name : initModelTranning
 * @funtion : �ṩ������SLOT����signal�źŵ���
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
 * ���浱ǰ����Ƭͼ��
 * @name : savePicture
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceQT::savePicture()
{
          this->videoFrameSavingProcess();
}

/*------------------------------------------------------------------------------------------------------
 * ������ǰ����Ƭͼ��
 * @name : ignorePicture
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceQT::ignorePicture()
{
          this->videoFrameIgnoreProcess();
}

/*------------------------------------------------------------------------------------------------------
 * ���µ�ǰ��������������ѵ������
 * @name : registerEmployee
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceQT::registerEmployee()                          //����ѵ���߳�
{
          this->startVideoRegister(this->m_videoFlag, this->ui.SystemStatusInfo);
}