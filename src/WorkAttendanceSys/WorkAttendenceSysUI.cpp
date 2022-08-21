#include"WorkAttendanceSys.h"

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
                    this->ui_sys->TranningSetInput,
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
                    this->ui_sys->AciqurePremit,
                    0
          );
          [=]() {
                    this->ui_sys->TakePicture->setEnabled(true);                         //������������
                    this->ui_sys->TakePicture->update();                                      //������������״̬
                    this->ui_sys->InitTranning->setEnabled(true);                        //������ѵ����ť
                    this->ui_sys->InitTranning->update();                                     //������ѵ����ť״̬
          }();
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---���µ�ǰ���������û���������¼ϵͳ(��ʼ����ʶ��)
 * @name : employeeAttendanceInterface
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::employeeAttendanceInterface()
{
          this->disableTranningButton();                                                                                        //����ѵ����صİ�ť����
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
          this->disableTranningButton();                                                                                        //����ѵ����صİ�ť����
          this->QTEmployeeSignOut(
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
 * �ۺ������---���µ�ǰ���������Ա�˻�����ע��Ȩ��
 * @name : employeeAskPremitInterface
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::employeeAskPremitInterface()
{
          this->disableTranningButton();                                                                                        //����ѵ����صİ�ť����
          this->QTEmployeeAskPremit(
                    this->ui_sys->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_sys->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->m_globalTimer,                                                                                             //����ȫ�ּ�ʱ��
                    this->ui_sys->SystemStatusInfo                                                                             //�������
          );
          this->ui_sys->UserID->clear();                                                           //���ID������
          this->ui_sys->NameInput->clear();                                                     //������ֵ�����
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---���µ�ǰ���ؼ��ע��Ȩ��
 * @name : employeecheckPremittion
 * @funtion : �ṩ������SLOT����signal�źŵ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::employeeCheckPremittion()
{
          this->disableTranningButton();                                                                                        //����ѵ����صİ�ť����
          this->QTEmployeeCheckPremittion(
                    this->ui_sys->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_sys->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->ui_sys->TranningSetInput,                                                                            //ע�Ტ¼������
                    this->m_globalTimer,                                                                                             //����ȫ�ּ�ʱ��
                    this->ui_sys->PremissionStatus,                                                                             //���Ȩ��״̬�ӿ�
                    this->ui_sys->SystemStatusInfo                                                                             //�������
          );
}

/*------------------------WorkAttendanceSys����ϵͳ����Աϵͳ-----------------------------*/
/*------------------------------------------------------------------------------------------------------
 * �ۺ������---��¼������ϵͳ�����֤����
 * @name : adminManagementLogin
 * @funtion : ��¼�����ŵ�Ȩ����֤ϵͳ
 * @Correction: 2022-8-18 �ú�����Ҫ�趨Ϊ�ۺ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::adminManagementLogin()
{
          [=]() {
                    this->ui_sys->SignIn->setDisabled(true);                                                                       //��������ǩ����ť
                    this->ui_sys->Logout->setDisabled(true);                                                                      //��������ǩ�˰�ť
                    this->ui_sys->AciqurePremit->setDisabled(true);                                                         //������Ա������Ȩ�ް�ť
                    this->ui_sys->checkPremittion->setDisabled(true);                                                       //���ò�ѯȨ�ް�ť
                    this->ui_sys->SignIn->update();                                                                                      //����
                    this->ui_sys->Logout->update();                                                                                     //����
                    this->ui_sys->AciqurePremit->update();                                                                         //����
                    this->ui_sys->checkPremittion->update();                                                                       //����
          }();

          this->disableTranningButton();                                                                                        //����ѵ����صİ�ť����
          this->QTAdminManagementLogin(
                    this->ui_sys->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_sys->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->m_currentAdmin,                                                                                         //��ǰ��¼�Ĺ���Ա
                    this->ui_sys->AdministerLogin,                                                                             //��¼������ϵͳ��ť
                    this->ui_sys->AdminOnly,                                                                                     //���ʹ�����ϵͳ��ť
                    this->ui_sys->CloseVideo,                                                                                     //��Ƶ��ʶ�����簴ť
                    this->m_globalTimer,                                                                                             //����ȫ�ּ�ʱ��
                    this->ui_sys->SystemStatusInfo                                                                             //�������
          );

          [=]() {
                    this->ui_sys->AdministerLogin->setEnabled(true);                                                        //���õ�¼������ϵͳ
                    this->ui_sys->SignIn->setEnabled(true);                                                                       //��������ǩ����ť
                    this->ui_sys->Logout->setEnabled(true);                                                                      //��������ǩ�˰�ť
                    this->ui_sys->AciqurePremit->setEnabled(true);                                                         //������Ա������Ȩ�ް�ť
                    this->ui_sys->checkPremittion->setEnabled(true);                                                       //���ò�ѯȨ�ް�ť
                    this->ui_sys->AdministerLogin->update();                                                                     //����
                    this->ui_sys->SignIn->update();                                                                                      //����
                    this->ui_sys->Logout->update();                                                                                     //����
                    this->ui_sys->AciqurePremit->update();                                                                         //����
                    this->ui_sys->checkPremittion->update();                                                                       //����
                    this->ui_sys->UserID->clear();                                                           //���ID������
                    this->ui_sys->NameInput->clear();                                                     //������ֵ�����
          }();
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---��¼������ϵͳ��UI����
 * @name : adminManagementLogin
 * @funtion : ���ʹ�����ϵͳUI����
 * @Correction: 2022-8-18 �ú�����Ҫ�趨Ϊ�ۺ���
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::adminManagementUI()
{
          this->initAdminUi();                                                                        //���ڷ�ֹADMIN UI�ظ��������µ�����
          m_qDialog->show();                                                                        //��ʾQDialog�Ĵ���
          [=]() {
                    this->ui_sys->AdminOnly->setDisabled(true);                              //���� ���ʹ����Ű�ť
                    this->ui_sys->AdminOnly->update();                                             //���� ���ʹ����Ű�ť
                    this->ui_sys->CloseVideo->setDisabled(true);                               //���� �ر���Ƶ����İ�ť
                    this->ui_sys->CloseVideo->update();                                              //���� �ر���Ƶ����İ�ť
          }();
}

/*------------------------------------------------------------------------------------------------------
 * WorkAttendanceSys�źŲ۵����ó���
 * @name : initSysConnectSlot
 * @funtion : ���ÿռ�ͺ����������ϵ
 * @Correction: 2022-8-20 ������һ�ΰ�ťͬʱ�������ε����
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initSysConnectSlot()
{
          /*������������Ա����֮��������ѵ���Ľ�����*/
          QObject::connect(this->ui_sys->TranningSetInput, SIGNAL(clicked()), this, SLOT(tranningSetInputFromVideo()), Qt::UniqueConnection);    //����ѵ������ģ��
          QObject::connect(this->ui_sys->TakePicture, SIGNAL(clicked()), this, SLOT(takePictureFromVideo()), Qt::UniqueConnection);                      //��������ģ��
          QObject::connect(this->ui_sys->ConfirmTranningSet, SIGNAL(clicked()), this, SLOT(savePictureForTranning()), Qt::UniqueConnection);      //��������
          QObject::connect(this->ui_sys->IgnoreTranningSet, SIGNAL(clicked()), this, SLOT(ignorePictureForTranning()), Qt::UniqueConnection);      //��������

          /*��ѵ���Ľ���������֮�󣬿��Կ���Resnet����ѵ��*/
          QObject::connect(this->ui_sys->InitTranning, SIGNAL(clicked()), this, SLOT(initTranningProcess()), Qt::UniqueConnection);                         //�����в�������

          /*�û��ĵ�¼�ǳ�����*/
          QObject::connect(this->ui_sys->SignIn, SIGNAL(clicked()), this, SLOT(employeeAttendanceInterface()), Qt::UniqueConnection);                   //����ǩ������
          QObject::connect(this->ui_sys->Logout, SIGNAL(clicked()), this, SLOT(employeeSignalOutInterface()), Qt::UniqueConnection);                    //����ǩ�˳���

          /*�ر�ϵͳ�ĺ��Ĺ���*/
          QObject::connect(this->ui_sys->CloseVideo, SIGNAL(clicked()), this, SLOT(closeVideoStream()), Qt::UniqueConnection);                              //�ر���Ƶ��

          /*�û�����ע��Ȩ��*/
          QObject::connect(this->ui_sys->AciqurePremit, SIGNAL(clicked()), this, SLOT(employeeAskPremitInterface()), Qt::UniqueConnection);        //�����Ա�˻�����ע��Ȩ��

          /*��ѯ���û���Ȩ���Ƿ�ͨ��*/
          QObject::connect(this->ui_sys->checkPremittion, SIGNAL(clicked()), this, SLOT(employeeCheckPremittion()), Qt::UniqueConnection);        //���ע��Ȩ���Ƿ�ͨ��

          /*����������ϵͳ��¼��֤*/
          QObject::connect(this->ui_sys->AdministerLogin, SIGNAL(clicked()), this, SLOT(adminManagementLogin()), Qt::UniqueConnection);         //����������ϵͳ��¼��֤
          QObject::connect(this->ui_sys->AdminOnly, SIGNAL(clicked()), this, SLOT(adminManagementUI()), Qt::UniqueConnection);                      //��������Աҳ��
}