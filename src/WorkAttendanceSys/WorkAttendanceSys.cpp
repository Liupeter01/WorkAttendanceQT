#include "WorkAttendanceSys.h"

WorkAttendanceSys::WorkAttendanceSys(QWidget* parent)
          : QDialog(parent), Interface(10)
{
          ui.setupUi(this);
          this->connectSlotSet();                                            //�����źŲ�
          this->initDepartmentSetting();
          this->initWorkAttendanceSys();                              //��ʼ����פ����
          this->initProcessBarSetting();                                 //��ʼ��������
}

WorkAttendanceSys::~WorkAttendanceSys()
{
          this->videoClose();
          for (auto& i : this->m_threadPool) {
                    if (i.joinable()) {
                              i.join();
                    }
          }
}

/*------------------------WorkAttendanceSys����ϵͳ��ʼ��-----------------------------*/
/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys����ϵͳ��פ����
* @name : initWorkAttendanceSys
* @funtion : �ṩ������SLOT����signal�źŵ���
*------------------------------------------------------------------------------------------------------*/
void  WorkAttendanceSys::initWorkAttendanceSys()
{
          this->m_threadPool.emplace_back(&Interface::QTVideoOutput, this, std::ref(this->ui.VideoDisplay), std::ref(this->ui.SystemStatusInfo));
          this->m_threadPool.emplace_back(&Interface::setLcdTimer, this, std::ref(this->ui.lcdNumber));
}

/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys����ϵͳ��ʼ��������
* @name :  initDepartmentSetting
* @funtion : ��ʼ�����������ڲ��ŵ�ѡ��
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initDepartmentSetting()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(m_SelectDepartment);       //�����ݿ��л�ȡ����
          for (int i = 0; i < dbRes.size(); ++i) {
                    ui.comboBox->addItem(QString::fromLocal8Bit(dbRes[i][0].c_str()));                  //�����õĲ�����Ϣ��������ѡ��
          }
}

/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys����ϵͳ��������ʼ��
* @name :  initProcessBarSetting
* @funtion : ��ʼ������������Ϣ
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initProcessBarSetting()
{
          this->ui.progressBar->setRange(0, 10);                      //
          this->ui.progressBar->setValue(0);                             //��ʼ����ֵΪ0
}

/*------------------------------------------------------------------------------------------------------
* ��Ƶ���Ĺرտ���
* @name : videoStreamClose
* @funtion : �ṩ������SLOT����signal�źŵ���
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::videoStreamClose()
{
          this->videoClose();
}

/*------------------------------------------------------------------------------------------------------
 * ��ѵ��ģ��ʱ��Ϊѵ���������տ���
 * @name : takePictureForTranning
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::takePictureForTranning()
{
          this->videoCameraShooting();
}

/*------------------------------------------------------------------------------------------------------
 * ��ʼʹ��ѵ��ģ�ͽ���ѵ������ѵ��
 * @name : initModelTranning
 * @funtion : �ṩ������SLOT����signal�źŵ���
 * @Correction: 2022-7-29 �޸��޷����ַ��޷������������ݿ��bug
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initModelTranning()
{
          this->QTResnetTranning(
                    this->ui.UserID->document()->toRawText().toStdString(),                         //UserID
                    this->ui.NameInput->document()->toRawText().toStdString(),                   //UserName
                    this->ui.comboBox->currentText().toLocal8Bit().constData(),                    //Department��
                    this->ui.SystemStatusInfo                                                                             //�������
          );
}

/*------------------------------------------------------------------------------------------------------
 * ���浱ǰ����Ƭͼ��
 * @name : savePicture
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::savePicture()
{
          this->videoFrameSavingProcess();
}

/*------------------------------------------------------------------------------------------------------
 * ������ǰ����Ƭͼ��
 * @name : ignorePicture
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::ignorePicture()
{
          this->videoFrameIgnoreProcess();
}

/*------------------------------------------------------------------------------------------------------
 * ���µ�ǰ��������������ѵ������
 * @name : registerEmployee
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::registerEmployee()                          //����ѵ���߳�
{
          this->startVideoRegister(
                    this->m_videoFlag,
                    this->ui.SystemStatusInfo,
                    this->ui.progressBar,
                    0);
}

/*------------------------------------------------------------------------------------------------------
 * �źŲ۵����ó���
 * @name : connectSlotSet
 * @funtion : ���ÿռ�ͺ����������ϵ
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::connectSlotSet()
{
          QObject::connect(ui.Exit, SIGNAL(clicked()), this, SLOT(videoStreamClose()));                                 //�ر���Ƶ��
          QObject::connect(ui.TakePicture, SIGNAL(clicked()), this, SLOT(takePictureForTranning()));             //��������ģ��
          QObject::connect(ui.Register, SIGNAL(clicked()), this, SLOT(registerEmployee()));                            //����ѵ������ģ��
          QObject::connect(ui.ConfirmTranningSet, SIGNAL(clicked()), this, SLOT(savePicture()));                 //��������
          QObject::connect(ui.IgnoreTranningSet, SIGNAL(clicked()), this, SLOT(ignorePicture()));                 //��������
          QObject::connect(ui.InitTranning, SIGNAL(clicked()), this, SLOT(initModelTranning()));                   //�����в�������
}