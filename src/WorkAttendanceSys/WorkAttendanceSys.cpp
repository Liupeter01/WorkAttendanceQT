#include "WorkAttendanceSys.h"

WorkAttendanceSys::WorkAttendanceSys(QWidget* parent)
          : QDialog(parent), Interface(),
          ui_sys(new Ui::WorkAttendanceSys),
          ui_admin(new  Ui::WorkAttendanceAdmin)
{
          ui_sys->setupUi(this);
          this->connectSlotSet();                                            //�����źŲ�
          this->initDepartmentSetting();
          this->initWorkAttendanceSys();                              //��ʼ����פ����
          this->initProcessBarSetting();                                 //��ʼ��������
          this->initButtonSetting();                                        //��������
}

WorkAttendanceSys::~WorkAttendanceSys()
{
          this->videoClose();                                                  //�ر���Ƶ
          for (auto& i : this->m_threadPool) {                      //�ر��������߳�
                    if (i.joinable()) {
                              i.join();
                    }
          }
          this->deleteSysUi();                                                //�ر�UI��ʾϵͳ
}

/*------------------------WorkAttendanceSys����ϵͳ��ʼ��-----------------------------*/
/*------------------------------------------------------------------------------------------------------
 * @WorkAttendanceSys����ϵͳ��ť�趨
 * @name : initButtonSetting
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initButtonSetting()
{
          this->ui_sys->TakePicture->setDisabled(true);                         //Ĭ�Ͻ�����������
          this->ui_sys->ConfirmTranningSet->setDisabled(true);          //Ĭ�Ͻ��ñ��水ť
          this->ui_sys->IgnoreTranningSet->setDisabled(true);          //Ĭ�Ͻ��� ���԰�ť
          this->ui_sys->InitTranning->setDisabled(true);                        //Ĭ�Ͻ��� ����ѵ����ť
          this->ui_sys->TakePicture->update();                                  //����
          this->ui_sys->ConfirmTranningSet->update();
          this->ui_sys->IgnoreTranningSet->update();
          this->ui_sys->InitTranning->update();
}

/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys����ϵͳ��פ����
* @name : initWorkAttendanceSys
* @funtion : �ṩ������SLOT����signal�źŵ���
*------------------------------------------------------------------------------------------------------*/
void  WorkAttendanceSys::initWorkAttendanceSys()
{
          this->m_threadPool.emplace_back(&Interface::QTVideoOutput, this, std::ref(this->ui_sys->VideoDisplay), std::ref(this->ui_sys->SystemStatusInfo));
          this->m_threadPool.emplace_back(&Interface::setLcdTimer, this, std::ref(this->ui_sys->lcdNumber));
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
                    ui_sys->comboBox->addItem(QString::fromLocal8Bit(dbRes[i][0].c_str()));                  //�����õĲ�����Ϣ��������ѡ��
          }
}

/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys����ϵͳ��������ʼ��
* @name :  initProcessBarSetting
* @funtion : ��ʼ������������Ϣ
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initProcessBarSetting()
{
          this->ui_sys->progressBar->setRange(0, 10);                      //
          this->ui_sys->progressBar->setValue(0);                             //��ʼ����ֵΪ0
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
 * @Correction : 2022-7-30 ֻ���������㰴ť֮��ſ���ѡ��ͼ��ı����߼�
 *                                                û�������������֮ǰ����������һ����Ƭ
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::takePictureForTranning()
{
          this->videoCameraShooting();
          this->ui_sys->ConfirmTranningSet->setEnabled(true);          //���ñ��水ť
          this->ui_sys->IgnoreTranningSet->setEnabled(true);             //���ú��԰�ť
          this->ui_sys->TakePicture->setDisabled(true);                      //û�������������֮ǰ����������һ����Ƭ
          this->ui_sys->ConfirmTranningSet->update();
          this->ui_sys->IgnoreTranningSet->update();
          this->ui_sys->TakePicture->update();
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
                    this->ui_sys->UserID->document()->toRawText().toStdString(),                         //UserID
                    this->ui_sys->NameInput->document()->toRawText().toStdString(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->ui_sys->SystemStatusInfo                                                                             //�������
          );
}

/*------------------------------------------------------------------------------------------------------
 * ���浱ǰ����Ƭͼ��
 * @name : savePicture
 * @funtion : �ṩ������SLOT����signal�źŵ���
 * @Correction : 2022-7-30 ֻ���������㰴ť֮��ſ���ѡ���������
 *                                                û������֮ǰ������ѡ�񱣴��߼�
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::savePicture()
{
          this->videoFrameSavingProcess();
          this->ui_sys->ConfirmTranningSet->setDisabled(true);          //���ñ��水ť
          this->ui_sys->IgnoreTranningSet->setDisabled(true);             //���ú��԰�ť
          this->ui_sys->TakePicture->setEnabled(true);                         //�������㰴ť
          this->ui_sys->ConfirmTranningSet->update();
          this->ui_sys->IgnoreTranningSet->update();
          this->ui_sys->TakePicture->update();
}

/*------------------------------------------------------------------------------------------------------
 * ������ǰ����Ƭͼ��
 * @name : ignorePicture
 * @funtion : �ṩ������SLOT����signal�źŵ���
 * @Correction : 2022-7-30 ֻ���������㰴ť֮��ſ���ѡ���������
 *                                                û������֮ǰ������ѡ�񱣴��߼�
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::ignorePicture()
{
          this->videoFrameIgnoreProcess();
          this->ui_sys->ConfirmTranningSet->setDisabled(true);          //���ñ��水ť
          this->ui_sys->IgnoreTranningSet->setDisabled(true);             //���ú��԰�ť
          this->ui_sys->TakePicture->setEnabled(true);                         //�������㰴ť
          this->ui_sys->ConfirmTranningSet->update();
          this->ui_sys->IgnoreTranningSet->update();
          this->ui_sys->TakePicture->update();
}

/*------------------------------------------------------------------------------------------------------
 * ���µ�ǰ��������������ѵ�����ճ���
 * @name : registerEmployee
 * @funtion : �ṩ������SLOT����signal�źŵ���
 * @Correction : 2022-7-30 ����ѵ�����ճ���֮��ϵͳ����ť�Ĳ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::registerEmployee()                          //����ѵ���߳�
{
          this->ui_sys->TakePicture->setEnabled(true);                         //������������
          this->ui_sys->InitTranning->setEnabled(true);                         //�����ÿ���ѵ����ť
          this->ui_sys->TakePicture->update();                                      //����
          this->ui_sys->InitTranning->update();
          this->startVideoRegister(
                    this->m_videoFlag,
                    this->ui_sys->SystemStatusInfo,
                    this->ui_sys->progressBar,
                    0);
}

/*------------------------------------------------------------------------------------------------------
 * �źŲ۵����ó���
 * @name : connectSlotSet
 * @funtion : ���ÿռ�ͺ����������ϵ
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::connectSlotSet()
{
          QObject::connect(ui_sys->CloseVideo, SIGNAL(clicked()), this, SLOT(videoStreamClose()));                                 //�ر���Ƶ��
          QObject::connect(ui_sys->TakePicture, SIGNAL(clicked()), this, SLOT(takePictureForTranning()));             //��������ģ��
          QObject::connect(ui_sys->Register, SIGNAL(clicked()), this, SLOT(registerEmployee()));                            //����ѵ������ģ��
          QObject::connect(ui_sys->ConfirmTranningSet, SIGNAL(clicked()), this, SLOT(savePicture()));                 //��������
          QObject::connect(ui_sys->IgnoreTranningSet, SIGNAL(clicked()), this, SLOT(ignorePicture()));                 //��������
          QObject::connect(ui_sys->InitTranning, SIGNAL(clicked()), this, SLOT(initModelTranning()));                   //�����в�������
}

/*------------------------------------------------------------------------------------------------------
 * ɾ������ͨ��ADMINϵͳUI
 * @name : deleteSysUi
 * @funtion : ɾ������ͨ��ADMINϵͳUI
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::deleteSysUi()
{
          delete ui_sys;                                              //ɾ����ͨϵͳ
          if (this->ui_admin != nullptr) {                  //ɾ��ADMINϵͳ
                    delete this->ui_admin;
          }
}