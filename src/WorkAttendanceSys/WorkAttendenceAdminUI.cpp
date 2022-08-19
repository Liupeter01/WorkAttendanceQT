#include"WorkAttendanceSys.h"

/*---------------------WorkAttendanceAdmin管理员系统按钮槽函数----------------------*/
/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceAdmin考勤系统初始化下拉框
* @name :   initSysDeptComboBox
* @funtion : 初始化下拉框用于部门的选择
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initAdminDeptComboBox()
{
          std::vector<std::string> dbRes = this->initDepartmentSetting();                                       //从数据库中获取部门
          for (int i = 0; i < dbRes.size(); ++i) {
                    this->ui_admin->comboBox->addItem(QString::fromLocal8Bit(dbRes[i].c_str()));      //将设置的部门信息加入下拉选项
          }
}

/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceAdmin考勤系统初始化管理系统的参数
* @name :    initAdminParamSetting
* @funtion : 初始化管理系统的参数
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initAdminParamSetting()
{
          this->ui_admin->TranningSet->setText(QString::number(this->initTranningSetting()));                       //训练集数设定
          this->ui_admin->TranningSimilarity->setText(QString::number(this->initTrainningSimilarity()));      //训练相似度
          this->ui_admin->LateTimeSet->setTime(this->getMorningShiftTime());                                              //迟到时间
          this->ui_admin->LeaveEarilyTimeSet->setTime(this->getNightshiftTime());                                       //早退时间
}

/*------------------------------------------------------------------------------------------------------
 * 槽函数类别---显示相应的统计信息
 * @name : adminManagementLogin
 * @funtion : 显示相应的统计信息打卡记录和图表显示
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::displayStatisticsInfo()
{
          this->QTAdminStatisticsInterface(
                    this->ui_admin->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_admin->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_admin->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->ui_admin->AttendenceTable,                                                                        //签到记录AttendenceTable
                    this->ui_admin->SignOutTable,                                                                              //签退记录SignOutTable
                    this->ui_admin->isTimeEnabled,                                                                           //是否选择时间
                    this->ui_admin->LeftTime->dateTime(),                                                               //时间的左界限
                    this->ui_admin->RightTime->dateTime(),                                                              //时间的右界限
                    this->m_globalTimer,                                                                                             //加载全局计时器
                    this->ui_admin->SystemStatusInfo                                                                        //输出窗口
          );
}

/*------------------------------------------------------------------------------------------------------
 * 槽函数类别---设置系统参数
 * @name : adminParamSetting
 * @funtion : 显示相应的统计信息打卡记录和图表显示
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::adminParamSetting()
{
          this->QTAdminParamSettingInterface(
                    this->ui_admin->TranningSet->toPlainText().toLocal8Bit().constData(),                                                    //训练集数设定
                    this->ui_admin->TranningSimilarity->toPlainText().toLocal8Bit().constData(),                                         //训练相似度
                    this->ui_admin->LateTimeSet->dateTime().toString("hh:mm:ss").toLocal8Bit().constData(),
                    this->ui_admin->LeaveEarilyTimeSet->dateTime().toString("hh:mm:ss").toLocal8Bit().constData()
          );
}

/*------------------------------------------------------------------------------------------------------
 * 槽函数类别---关闭WorkAttendanceAdmin系统的UI系统
 * @name : closeAdminUI
 * @funtion : 关闭WorkAttendanceAdmin系统的UI系统
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::closeAdminUI()
{
          this->deleteAdminUi();
          [=]() {
                    this->ui_sys->AdministerLogin->setEnabled(true);                                             //允许启用登录管理部门系统   
                    this->ui_sys->AdministerLogin->update();                                                          //更新
                    this->ui_sys->AdminOnly->setDisabled(true);                                                     //禁用访问管理部门系统系统
                    this->ui_sys->AdminOnly->update();                                                                  //更新
                    this->ui_sys->CloseVideo->setDisabled(true);                                                    // 禁用关闭视频和识别网络系统
                    this->ui_sys->CloseVideo->update();                                                                  //更新
          }();
}

/*------------------------------------------------------------------------------------------------------
 * WorkAttendanceAdmin信号槽的设置程序
 * @name : initSysConnectSlot
 * @funtion : 设置空间和函数的捆绑关系
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initAdminConnectSlot()
{
          /*显示ADMIN系统的统计信息---包含打卡记录和图表*/
          QObject::connect(this->ui_admin->DisplayStatistics, SIGNAL(clicked()), this, SLOT(displayStatisticsInfo()));          //初始化显示信息事件

           /*ADMIN系统的退出功能绑定*/
          QObject::connect(this->ui_admin->QuitSys, SIGNAL(clicked()), this, SLOT(closeAdminUI()));                                 //ADMIN系统的退出功能

          /*ADMIN系统的签到参数设置绑定*/
          QObject::connect(this->ui_admin->CommitParam, SIGNAL(clicked()), this, SLOT(adminParamSetting()));                                                //ADMIN系统的设置系统参数功能
}