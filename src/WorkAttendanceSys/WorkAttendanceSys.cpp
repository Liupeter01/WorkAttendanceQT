#include "WorkAttendanceSys.h"

WorkAttendanceSys::WorkAttendanceSys(QWidget* parent)
          : QDialog(parent), Interface(),
          ui_sys(new Ui::WorkAttendanceSys),
          ui_admin(new Ui::WorkAttendanceAdmin),
          m_globalTimer(new QDateTime)
{
          ui_sys->setupUi(this);
          setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint); // ������󻯹ر�
          setFixedSize(1180, 744);                                            // ��ֹ�ı䴰�ڴ�С
          this->initAdminUi();                                                  //��ʼ��ADMIN UI��

          /*��ʼ���źŲ�CONNECT*/
          this->initSysConnectSlot();                                      //����Sys�źŲ�
          this->initAdminConnectSlot();                                 //����Admin�źŲ�

          /*��ʼ��ͨ��������ϵͳ*/
          this->initSysDeptComboBox();                               //����ϵͳ��ʼ��ͨ��������
          this->initAdminDeptComboBox();                         //����ϵͳ��ʼ������Ա������

          /*��ʼ������ϵͳ�Ĳ���*/
          this->initAdminParamSetting();                              //����ϵͳ��ʼ������ϵͳ�Ĳ���

          this->initWorkAttendanceSys();                              //��ʼ����פ����
          this->initProcessBarSetting();                                 //��ʼ��������
          this->initButtonSetting();                                        //��������
}

WorkAttendanceSys::~WorkAttendanceSys()
{
          this->QTcloseVideo();                                            //�ر���Ƶ
          delete m_qDialog;
          delete m_globalTimer;                                            //�ر�ʱ��ϵͳ
          for (auto& i : this->m_threadPool) {                      //�ر��������߳�
                    if (i.joinable()) {
                              i.join();
                    }
          }
          if (this->ui_admin != nullptr) {                             //ɾ��ADMIN UIϵͳ
                    delete this->ui_admin;
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
          this->ui_sys->AdminOnly->setDisabled(true);                         //Ĭ�Ͻ��� ���ʹ����Ű�ť
          this->ui_sys->TranningSetInput->setDisabled(true);                //Ĭ�Ͻ��� ע�Ტ¼��������ť
          this->ui_sys->CloseVideo->setDisabled(true);                         //Ĭ�Ͻ��� �ر���Ƶ����İ�ť
          this->ui_sys->TakePicture->update();                                       //������������״̬
          this->ui_sys->ConfirmTranningSet->update();                         //����
          this->ui_sys->IgnoreTranningSet->update();                            //����
          this->ui_sys->InitTranning->update();                                      //����
          this->ui_sys->AdminOnly->update();                                       //����
          this->ui_sys->TranningSetInput->update();                              //����
          this->ui_sys->CloseVideo->update();                                       //����
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
* @name :   initSysDeptComboBox
* @funtion : ��ʼ�����������ڲ��ŵ�ѡ��
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initSysDeptComboBox()
{
          std::vector<std::string> dbRes = this->initDepartmentSetting();                                       //�����ݿ��л�ȡ����
          for (int i = 0; i < dbRes.size(); ++i) {
                    this->ui_sys->comboBox->addItem(QString::fromLocal8Bit(dbRes[i].c_str()));            //�����õĲ�����Ϣ��������ѡ��
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

/*------------------------------------------------------------------------------------------------------
 * ����ѵ����صİ�ť����
 * @name : disableTranningButton
 * @funtion : ���ÿռ�ͺ����������ϵ
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::disableTranningButton()
{
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
          this->ui_sys->PremissionStatus->clear();                                            //���Ȩ��״̬
}

/*------------------------------------------------------------------------------------------------------
 * ɾ������ͨUI
 * @name : deleteSysUi
 * @funtion : ɾ������ͨUI
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::deleteSysUi()
{
          delete ui_sys;                                              //ɾ����ͨϵͳ
}

/*------------------------------------------------------------------------------------------------------
 * ɾ��ADMINϵͳUI
 * @name : deleteSysUi
 * @funtion : ɾ��ADMINϵͳUI
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::deleteAdminUi()
{
          if (this->m_dataDisplay != nullptr) {                         //ɾ��QDialog����
                    delete this->m_dataDisplay;
                    this->m_dataDisplay = nullptr;                        //��ֵ��ʼֵ׼���´η���
          }
          if (this->m_qDialog != nullptr) {                              //ɾ��QDialog����
                    delete this->m_qDialog;
                    this->m_qDialog = nullptr;                             //��ֵ��ʼֵ׼���´η���
          }
}

/*------------------------------------------------------------------------------------------------------
 * ��ʼ��ADMINϵͳUI
 * @name : deleteSysUi
 * @funtion : ��ʼ��ADMINϵͳUI(�������·���UI)
 * @Correction: 2022-8-19 ����QDialog�������ڴ������Ҫ����Admin UI�źŲ۵�����
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initAdminUi()
{
          if (this->m_qDialog == nullptr) {                                                                       //��ǰ��QDialog���ͷ� 
                    this->m_qDialog = new QDialog();                                                           //���·����ڴ�
                    this->ui_admin->setupUi(m_qDialog);                                                       //ΪQDialog����������UI�ļ�
                    this->m_qDialog->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint); // ���ý�ֹ���
                    this->m_qDialog->setFixedSize(1180, 744);                                            // ��ֹ�ı䴰�ڴ�С
                    this->initAdminConnectSlot();                                                                 //����WorkAttendanceAdmin�źŲ۵����ó���
          }
          this->initAdminParamSetting();                                                                            //��ʼ������Աǩ��ϵͳ��������
          if (this->m_dataDisplay == nullptr) {                                                                   //��ǰ�� DataDisplay���ͷ� 
                    this->m_dataDisplay = new DataDisplay(                                                  //���·����ڴ�
                              this->m_qDialog, 
                              this->ui_admin->AttendenceRecord,
                              this->ui_admin->newEmployee,
                              this->ui_admin->graphicsView
                    );    
          }
}