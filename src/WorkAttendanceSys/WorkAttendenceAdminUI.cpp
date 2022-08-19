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
                    this->ui_admin->SystemStatusInfo                                                                        //�������
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
                    this->ui_admin->LeaveEarilyTimeSet->dateTime().toString("hh:mm:ss").toLocal8Bit().constData()
          );
}

/*------------------------------------------------------------------------------------------------------
 * �ۺ������---�ر�WorkAttendanceAdminϵͳ��UIϵͳ
 * @name : closeAdminUI
 * @funtion : �ر�WorkAttendanceAdminϵͳ��UIϵͳ
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
 * @name : initSysConnectSlot
 * @funtion : ���ÿռ�ͺ����������ϵ
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initAdminConnectSlot()
{
          /*��ʾADMINϵͳ��ͳ����Ϣ---�����򿨼�¼��ͼ��*/
          QObject::connect(this->ui_admin->DisplayStatistics, SIGNAL(clicked()), this, SLOT(displayStatisticsInfo()));          //��ʼ����ʾ��Ϣ�¼�

           /*ADMINϵͳ���˳����ܰ�*/
          QObject::connect(this->ui_admin->QuitSys, SIGNAL(clicked()), this, SLOT(closeAdminUI()));                                 //ADMINϵͳ���˳�����

          /*ADMINϵͳ��ǩ���������ð�*/
          QObject::connect(this->ui_admin->CommitParam, SIGNAL(clicked()), this, SLOT(adminParamSetting()));                                                //ADMINϵͳ������ϵͳ��������
}