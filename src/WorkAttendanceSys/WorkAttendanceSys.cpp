#include "WorkAttendanceSys.h"

WorkAttendanceSys::WorkAttendanceSys(QWidget* parent)
          : QDialog(parent), Interface(),
          ui_sys(new Ui::WorkAttendanceSys),
          m_globalTimer(new QDateTime)
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
          this->QTcloseVideo();                                            //�ر���Ƶ
          delete m_globalTimer;                                            //�ر�ʱ��ϵͳ
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
          this->ui_sys->ConfirmTranningSet->setDisabled(true);           //Ĭ�Ͻ��ñ��水ť
          this->ui_sys->IgnoreTranningSet->setDisabled(true);              //Ĭ�Ͻ��� ���԰�ť
          this->ui_sys->InitTranning->setDisabled(true);                        //Ĭ�Ͻ��� ����ѵ����ť
          this->ui_sys->TakePicture->update();                                       //����
          this->ui_sys->ConfirmTranningSet->update();                         //����
          this->ui_sys->IgnoreTranningSet->update();                            //����
          this->ui_sys->InitTranning->update();                                      //����
}

/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys����ϵͳ��פ����
* @name : initWorkAttendanceSys
* @funtion : �ṩ������SLOT����signal�źŵ���
*------------------------------------------------------------------------------------------------------*/
void  WorkAttendanceSys::initWorkAttendanceSys()
{
          this->m_threadPool.emplace_back(&Interface::QTVideoOutput, this, 
                    std::ref(this->ui_sys->VideoDisplay), std::ref(this->ui_sys->SystemStatusInfo)
          );
          this->m_threadPool.emplace_back(&Interface::QTsetLcdTimer, this, 
                    std::ref(this->ui_sys->lcddate), std::ref(this->ui_sys->lcdclock),std::ref(this->m_globalTimer)
          );
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
          this->ui_sys->progressBar->setRange(0, this->initTranningSetting());          //��ȡ���ݿ��еĽ������ֵ
          this->ui_sys->progressBar->setValue(0);                                                         //��ʼ����ֵΪ0
}

/*----------------------WorkAttendanceSys����ϵͳ��ť�ۺ���---------------------------*/
/*------------------------------------------------------------------------------------------------------
* �ۺ������---�ر���Ƶ�����������߳�
* @name : closeVideoStream
* @funtion : �ṩ������SLOT����signal�źŵ���
* @Correction: 2022-8-1 ���ܴ����е��߳̿�����ѭ�����޷��رյ�bug(���ڻ�����)
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::closeVideoStream()
{
          this->QTcloseVideo();
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---��ѵ���̴߳����ź������ź�
 * @name : takePictureFromVideo
 * @funtion : �ṩ������SLOT����signal�źŵ��ã�����ʱ��ͣ����ʾ�߳��Է���ͻ��鿴
 * @Correction : 2022-7-30 ֻ���������㰴ť֮��ſ���ѡ��ͼ��ı����߼�
 *                                                û�������������֮ǰ����������һ����Ƭ
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::takePictureFromVideo()
{
          this->QTtakePicture();
          [=]() {
                    this->ui_sys->ConfirmTranningSet->setEnabled(true);          //���ñ��水ť
                    this->ui_sys->ConfirmTranningSet->update();                       //���±��水ť״̬
                    this->ui_sys->IgnoreTranningSet->setEnabled(true);             //���ú��԰�ť
                    this->ui_sys->IgnoreTranningSet->update();                          //���±�����԰�ť
                    this->ui_sys->TakePicture->setDisabled(true);                      //û�������������֮ǰ����������һ����Ƭ
                    this->ui_sys->TakePicture->update();                                     //����������һ����Ƭ��ť״̬
          }();
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---����ѵ������������ѵ��ģʽ
 * @name : initTranningProcess
 * @funtion : �ṩ������SLOT����signal�źŵ���
 * @Correction: 2022-7-29 �޸��޷����ַ��޷������������ݿ��bug
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initTranningProcess()
{
          [=]() {
                    this->ui_sys->TakePicture->setDisabled(true);                       //�������㰴ť
                    this->ui_sys->TakePicture->update();                                      //�������㰴ť״̬
          }();
          this->QTResnetTranning(
                    this->ui_sys->UserID->document()->toPlainText().toLocal8Bit().constData(),        //UserID
                    this->ui_sys->NameInput->document()->toPlainText().toLocal8Bit().constData(),  //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                        //Department
                    this->ui_sys->SystemStatusInfo                                                                                  //�������
          );
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---���浱ǰ����Ƭͼ��(����ѵ��)
 * @name : savePictureForTranning
 * @funtion : �ṩ������SLOT����signal�źŵ���
 * @Correction : 2022-7-30 ֻ���������㰴ť֮��ſ���ѡ���������
 *                                                û������֮ǰ������ѡ�񱣴��߼�
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::savePictureForTranning()
{
          this->QTsaveImage();
          [=]() {
                    this->ui_sys->ConfirmTranningSet->setDisabled(true);          //���ñ��水ť
                    this->ui_sys->ConfirmTranningSet->update();                        //���±��水ť״̬
                    this->ui_sys->IgnoreTranningSet->setDisabled(true);             //���ú��԰�ť
                    this->ui_sys->IgnoreTranningSet->update();                           //���º��԰�ť״̬
                    this->ui_sys->TakePicture->setEnabled(true);                         //�������㰴ť
                    this->ui_sys->TakePicture->update();                                      //�������㰴ť״̬
          }();
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---������ǰ����Ƭͼ��(����ѵ��)
 * @name : ignorePicture
 * @funtion : �ṩ������SLOT����signal�źŵ���
 * @Correction : 2022-7-30 ֻ���������㰴ť֮��ſ���ѡ���������
 *                                                û������֮ǰ������ѡ�񱣴��߼�
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::ignorePictureForTranning()
{
          this->QTignoreImage();
          [=]() {
                    this->ui_sys->ConfirmTranningSet->setDisabled(true);          //���ñ��水ť
                    this->ui_sys->ConfirmTranningSet->update();                        //���½��ñ��水ť״̬
                    this->ui_sys->IgnoreTranningSet->setDisabled(true);             //���ú��԰�ť
                    this->ui_sys->IgnoreTranningSet->update();                           //���º��԰�ť״̬
                    this->ui_sys->TakePicture->setEnabled(true);                         //�������㰴ť
                    this->ui_sys->TakePicture->update();                                      //�������㰴ť״̬
          }();
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---��������ѵ��ģʽ���������߳��л�ȡͼƬ������ѵ����
 * @name : tranningSetInputFromVideo
 * @funtion : �ṩ������SLOT����signal�źŵ���
 * @Correction : 2022-7-30 ����ѵ�����ճ���֮��ϵͳ����ť�Ĳ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::tranningSetInputFromVideo()                              //����ѵ���߳�
{
          [=]() {
                    this->ui_sys->TakePicture->setEnabled(true);                         //������������
                    this->ui_sys->TakePicture->update();                                      //������������״̬
                    this->ui_sys->InitTranning->setEnabled(true);                        //������ѵ����ť
                    this->ui_sys->InitTranning->update();                                     //������ѵ����ť״̬
          }();
          this->QTtranningSetInput(
                    this->m_videoFlag,
                    this->ui_sys->SystemStatusInfo,
                    this->ui_sys->progressBar,
                    0
          );
          [=]() {
                    this->ui_sys->TakePicture->setDisabled(true);                        //������������
                    this->ui_sys->TakePicture->update();                                      //������������״̬
                    this->ui_sys->ConfirmTranningSet->setDisabled(true);          //���ñ�������
                    this->ui_sys->ConfirmTranningSet->update();                        //���±�������״̬
                    this->ui_sys->IgnoreTranningSet->setDisabled(true);             //������������
                    this->ui_sys->IgnoreTranningSet->update();                           //������������״̬
                    this->ui_sys->InitTranning->setDisabled(true);                       //����ѵ����ť
                    this->ui_sys->InitTranning->update();                                     //����ѵ����ť״̬
                    this->ui_sys->UserID->clear();                                                //���ID������
                    this->ui_sys->NameInput->clear();                                          //������ֵ�����
          }();
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---���µ�ǰ���������û���������¼ϵͳ(��ʼ����ʶ��)
 * @name : employeeAttendanceInterface
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::employeeAttendanceInterface()
{
          [=]() {
                    this->ui_sys->InitTranning->setDisabled(true);                                  //����ѵ������
                    this->ui_sys->InitTranning->update();                                                //��������������ť״̬
                    this->ui_sys->TakePicture->setDisabled(true);                                   //������������
                    this->ui_sys->TakePicture->update();                                                 //������������״̬
                    this->ui_sys->ConfirmTranningSet->setDisabled(true);                     //���ñ��水ť
                    this->ui_sys->ConfirmTranningSet->update();                                   //���½��ñ��水ť״̬
                    this->ui_sys->IgnoreTranningSet->setDisabled(true);                        //���ú��԰�ť
                    this->ui_sys->IgnoreTranningSet->update();                                      //���º��԰�ť״̬
                    this->ui_sys->TranningSetInput->setDisabled(true);                          //����ע�Ტ����������ť
                    this->ui_sys->TranningSetInput->update();                                        //����ע�Ტ����������ť״̬
                    this->ui_sys->checkPremittion->setDisabled(true);                            //���ò�ѯȨ�ް�ť
                    this->ui_sys->checkPremittion->update();                                          //���²�ѯȨ�ް�ť״̬
          }();

          this->QTEmployeeAttendance(
                    this->ui_sys->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_sys->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->m_globalTimer,                                                                                             //����ȫ�ּ�ʱ��
                    this->ui_sys->SystemStatusInfo                                                                             //�������
          );

          [=]() {
                    this->ui_sys->checkPremittion->setEnabled(true);                            //���ò�ѯȨ�ް�ť
                    this->ui_sys->checkPremittion->update();                                          //���²�ѯȨ�ް�ť״̬
                    this->ui_sys->UserID->clear();                                                           //���ID������
                    this->ui_sys->NameInput->clear();                                                     //������ֵ�����
          }();
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---���µ�ǰ���������û��������ǳ�ϵͳ(��ʼ����ʶ��)
 * @name : employeeSignalOutInterface
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::employeeSignalOutInterface()
{
          [=]() {
                    this->ui_sys->InitTranning->setDisabled(true);                                  //����ѵ������
                    this->ui_sys->InitTranning->update();                                                //��������������ť״̬
                    this->ui_sys->TakePicture->setDisabled(true);                                   //������������
                    this->ui_sys->TakePicture->update();                                                 //������������״̬
                    this->ui_sys->ConfirmTranningSet->setDisabled(true);                     //���ñ��水ť
                    this->ui_sys->ConfirmTranningSet->update();                                   //���½��ñ��水ť״̬
                    this->ui_sys->IgnoreTranningSet->setDisabled(true);                        //���ú��԰�ť
                    this->ui_sys->IgnoreTranningSet->update();                                      //���º��԰�ť״̬
          }();

          this->QTEmployeeSignOut(
                    this->ui_sys->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_sys->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->m_globalTimer,                                                                                             //����ȫ�ּ�ʱ��
                    this->ui_sys->SystemStatusInfo                                                                             //�������
          );

          [=]() {
                    this->ui_sys->InitTranning->setEnabled(true);                                  //����ѵ������
                    this->ui_sys->InitTranning->update();                                                //��������������ť״̬
                    this->ui_sys->TakePicture->setEnabled(true);                                   //������������
                    this->ui_sys->TakePicture->update();                                                 //������������״̬
                    this->ui_sys->ConfirmTranningSet->setEnabled(true);                     //���ñ��水ť
                    this->ui_sys->ConfirmTranningSet->update();                                   //���½��ñ��水ť״̬
                    this->ui_sys->IgnoreTranningSet->setEnabled(true);                        //���ú��԰�ť
                    this->ui_sys->IgnoreTranningSet->update();                                      //���º��԰�ť״̬
                    this->ui_sys->UserID->clear();                                                           //���ID������
                    this->ui_sys->NameInput->clear();                                                     //������ֵ�����
          }();
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---���µ�ǰ���������Ա�˻�����ע��Ȩ��
 * @name : employeeAskPremitInterface
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::employeeAskPremitInterface()
{
          [=]() {
                    this->ui_sys->InitTranning->setDisabled(true);                                  //����ѵ������
                    this->ui_sys->InitTranning->update();                                                //��������������ť״̬
                    this->ui_sys->TakePicture->setDisabled(true);                                   //������������
                    this->ui_sys->TakePicture->update();                                                 //������������״̬
                    this->ui_sys->ConfirmTranningSet->setDisabled(true);                     //���ñ��水ť
                    this->ui_sys->ConfirmTranningSet->update();                                   //���½��ñ��水ť״̬
                    this->ui_sys->IgnoreTranningSet->setDisabled(true);                        //���ú��԰�ť
                    this->ui_sys->IgnoreTranningSet->update();                                      //���º��԰�ť״̬
                    this->ui_sys->TranningSetInput->setDisabled(true);                          //����ע�Ტ����������ť
                    this->ui_sys->TranningSetInput->update();                                        //����ע�Ტ����������ť״̬
          }();

          this->QTEmployeeAskPremit(
                    this->ui_sys->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_sys->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->ui_sys->SystemStatusInfo                                                                             //�������
          );
}

/*------------------------------------------------------------------------------------------------------
 * �źŲ۵����ó���
 * @name : connectSlotSet
 * @funtion : ���ÿռ�ͺ����������ϵ
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::connectSlotSet()
{
          /*������������Ա����֮��������ѵ���Ľ�����*/
          QObject::connect(ui_sys->TranningSetInput, SIGNAL(clicked()), this, SLOT(tranningSetInputFromVideo()));    //����ѵ������ģ��
          QObject::connect(ui_sys->TakePicture, SIGNAL(clicked()), this, SLOT(takePictureFromVideo()));                      //��������ģ��
          QObject::connect(ui_sys->ConfirmTranningSet, SIGNAL(clicked()), this, SLOT(savePictureForTranning()));      //��������
          QObject::connect(ui_sys->IgnoreTranningSet, SIGNAL(clicked()), this, SLOT(ignorePictureForTranning()));      //��������

          /*��ѵ���Ľ���������֮�󣬿��Կ���Resnet����ѵ��*/
          QObject::connect(ui_sys->InitTranning, SIGNAL(clicked()), this, SLOT(initTranningProcess()));                         //�����в�������

          /*�û��ĵ�¼�ǳ�����*/
          QObject::connect(ui_sys->SignIn, SIGNAL(clicked()), this, SLOT(employeeAttendanceInterface()));                   //����ǩ������
          QObject::connect(ui_sys->Logout, SIGNAL(clicked()), this, SLOT(employeeSignalOutInterface()));                    //����ǩ�˳���

          /*�ر�ϵͳ�ĺ��Ĺ���*/
          QObject::connect(ui_sys->CloseVideo, SIGNAL(clicked()), this, SLOT(closeVideoStream()));                              //�ر���Ƶ��

          /*�û�����ע��Ȩ��*/
          QObject::connect(ui_sys->AciqurePremit, SIGNAL(clicked()), this, SLOT(employeeAskPremitInterface()));        //�����Ա�˻�����ע��Ȩ��
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