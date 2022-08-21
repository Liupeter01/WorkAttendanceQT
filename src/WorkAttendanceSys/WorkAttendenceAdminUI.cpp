#include"WorkAttendanceSys.h"

/*---------------------WorkAttendanceAdmin����Աϵͳ��ť�ۺ���----------------------*/
/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceAdmin����ϵͳ��ʼ��������
* @name :   initSysDeptComboBox
* @funtion : ��ʼ�����������ڲ��ŵ�ѡ��
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initAdminDeptComboBox()
{
          std::vector<std::string> dbRes = this->initDepartmentSetting();                                       //�����ݿ��л�ȡ����
          for (int i = 0; i < dbRes.size(); ++i) {
                    this->ui_admin->comboBox->addItem(QString::fromLocal8Bit(dbRes[i].c_str()));      //�����õĲ�����Ϣ��������ѡ��
          }
}

/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceAdmin����ϵͳ��ʼ������ϵͳ�Ĳ���
* @name :    initAdminParamSetting
* @funtion : ��ʼ������ϵͳ�Ĳ���
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initAdminParamSetting()
{
          this->ui_admin->TranningSet->setText(QString::number(this->initTranningSetting()));                       //ѵ�������趨
          this->ui_admin->TranningSimilarity->setText(QString::number(this->initTrainningSimilarity()));      //ѵ�����ƶ�
          this->ui_admin->LateTimeSet->setTime(this->getMorningShiftTime());                                              //�ٵ�ʱ��
          this->ui_admin->LeaveEarilyTimeSet->setTime(this->getNightshiftTime());                                       //����ʱ��
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---��ʾ��Ӧ��ͳ����Ϣ
 * @name : adminManagementLogin
 * @funtion : ��ʾ��Ӧ��ͳ����Ϣ�򿨼�¼��ͼ����ʾ
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::displayStatisticsInfo()
{
          this->QTAdminStatisticsInterface(
                    this->ui_admin->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_admin->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_admin->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->ui_admin->AttendenceTable,                                                                        //ǩ����¼AttendenceTable
                    this->ui_admin->SignOutTable,                                                                              //ǩ�˼�¼SignOutTable
                    this->ui_admin->isTimeEnabled,                                                                           //�Ƿ�ѡ��ʱ��
                    this->ui_admin->LeftTime->dateTime(),                                                               //ʱ��������
                    this->ui_admin->RightTime->dateTime(),                                                              //ʱ����ҽ���
                    this->m_globalTimer,                                                                                             //����ȫ�ּ�ʱ��
                    this->m_dataDisplay,                                                                                              //DataDisplay
                    this->ui_admin->SystemStatusInfo                                                                        //�������
          );
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---��ʾ��Ա�����
 * @name : displayNewEmployeeList
 * @funtion : ��ʾ��Ա�����
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::displayNewEmployeeList()
{
          this->QTGetNewEmployeeInterface(this->m_dataDisplay);                //��ʾ��Ա�����
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---ͨ��Ա������
 * @name : ApproveEmployee
 * @funtion : ͨ��Ա������
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::approveEmployee()
{
          this->QTDeniedAndApprove(
                    true,
                    this->ui_admin->newEmployee,
                    this->m_globalTimer,
                    this->ui_admin->SystemStatusInfo
          );
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---����Ա������
 * @name : deniedEmployee
 * @funtion : ����Ա������
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::deniedEmployee()
{
          this->QTDeniedAndApprove(
                    false, 
                    this->ui_admin->newEmployee,
                    this->m_globalTimer,
                    this->ui_admin->SystemStatusInfo
          );
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---����ϵͳ����
 * @name : adminParamSetting
 * @funtion : ��ʾ��Ӧ��ͳ����Ϣ�򿨼�¼��ͼ����ʾ
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::adminParamSetting()
{
          this->QTAdminParamSettingInterface(
                    this->ui_admin->TranningSet->toPlainText().toLocal8Bit().constData(),                                                    //ѵ�������趨
                    this->ui_admin->TranningSimilarity->toPlainText().toLocal8Bit().constData(),                                         //ѵ�����ƶ�
                    this->ui_admin->LateTimeSet->dateTime().toString("hh:mm:ss").toLocal8Bit().constData(),
                    this->ui_admin->LeaveEarilyTimeSet->dateTime().toString("hh:mm:ss").toLocal8Bit().constData(),
                    this->m_currentAdmin,                                                                                          //���뵱ǰ�ĵ�¼�Ĺ���ԱID
                    this->m_globalTimer,                                                                                             //����ȫ�ּ�ʱ��
                    this->ui_admin->SystemStatusInfo                                                                        //�������
          );
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---�ر�WorkAttendanceAdminϵͳ��UIϵͳ
 * @name : closeAdminUI
 * @funtion : �ر�WorkAttendanceAdminϵͳ��UIϵͳ
 * @Correction: ������һ�ΰ�ťͬʱ�������ε����
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::closeAdminUI()
{
          this->deleteAdminUi();
          [=]() {
                    this->ui_sys->AdministerLogin->setEnabled(true);                                             //�������õ�¼������ϵͳ   
                    this->ui_sys->AdministerLogin->update();                                                          //����
                    this->ui_sys->AdminOnly->setDisabled(true);                                                     //���÷��ʹ�����ϵͳϵͳ
                    this->ui_sys->AdminOnly->update();                                                                  //����
                    this->ui_sys->CloseVideo->setDisabled(true);                                                    // ���ùر���Ƶ��ʶ������ϵͳ
                    this->ui_sys->CloseVideo->update();                                                                  //����
          }();
}

/*------------------------------------------------------------------------------------------------------
 * WorkAttendanceAdmin�źŲ۵����ó���
 * @name :initAdminConnectSlot
 * @funtion : �ر�WorkAttendanceAdminϵͳ��UIϵͳ
 * @Correction: 2022-8-20 ������һ�ΰ�ťͬʱ�������ε����
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initAdminConnectSlot()
{
          /*��ʾADMINϵͳ��ͳ����Ϣ---�����򿨼�¼��ͼ��*/
          QObject::connect(this->ui_admin->DisplayStatistics, SIGNAL(clicked()), this, SLOT(displayStatisticsInfo()),Qt::UniqueConnection);          //��ʼ����ʾ��Ϣ�¼�

           /*ADMINϵͳ���˳����ܰ�*/
          QObject::connect(this->ui_admin->QuitSys, SIGNAL(clicked()), this, SLOT(closeAdminUI()), Qt::UniqueConnection);                                 //ADMINϵͳ���˳�����

          /*ADMINϵͳ��ǩ���������ð�*/
          QObject::connect(this->ui_admin->CommitParam, SIGNAL(clicked()), this, SLOT(adminParamSetting()), Qt::UniqueConnection);               //ADMINϵͳ������ϵͳ��������
          
          /*��ȡ��Ա����*/
          QObject::connect(this->ui_admin->getNewEmployee, SIGNAL(clicked()), this, SLOT(displayNewEmployeeList()));              

          /*ͨ����Ա������*/
          QObject::connect(this->ui_admin->Accept, SIGNAL(clicked()), this, SLOT(approveEmployee()));

         /*������Ա������*/
          QObject::connect(this->ui_admin->Denied, SIGNAL(clicked()), this, SLOT(deniedEmployee()));
}