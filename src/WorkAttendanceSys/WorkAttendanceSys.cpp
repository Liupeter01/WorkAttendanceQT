#include "WorkAttendanceSys.h"

WorkAttendanceSys::WorkAttendanceSys(QWidget* parent)
          : QDialog(parent), Interface(),
          ui_sys(new Ui::WorkAttendanceSys),
          ui_admin(new Ui::WorkAttendanceAdmin),
          m_globalTimer(new QDateTime)
{
          ui_sys->setupUi(this);
          setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint); // 设置最大化关闭
          setFixedSize(1180, 744);                                            // 禁止改变窗口大小
          this->initAdminUi();                                                  //初始化ADMIN UI层

          /*初始化信号槽CONNECT*/
          this->initSysConnectSlot();                                      //设置Sys信号槽
          this->initAdminConnectSlot();                                 //设置Admin信号槽

          /*初始化通用下拉框系统*/
          this->initSysDeptComboBox();                               //考勤系统初始化通用下拉框
          this->initAdminDeptComboBox();                         //考勤系统初始化管理员下拉框

          /*初始化管理系统的参数*/
          this->initAdminParamSetting();                              //考勤系统初始化管理系统的参数

          this->initWorkAttendanceSys();                              //初始化常驻程序
          this->initProcessBarSetting();                                 //初始化进度条
          this->initButtonSetting();                                        //禁用摁键
}

WorkAttendanceSys::~WorkAttendanceSys()
{
          this->QTcloseVideo();                                            //关闭视频
          delete m_qDialog;
          delete m_globalTimer;                                            //关闭时钟系统
          for (auto& i : this->m_threadPool) {                      //关闭其他的线程
                    if (i.joinable()) {
                              i.join();
                    }
          }
          if (this->ui_admin != nullptr) {                             //删除ADMIN UI系统
                    delete this->ui_admin;
          }
          this->deleteSysUi();                                                //关闭UI显示系统
}

/*------------------------WorkAttendanceSys考勤系统初始化-----------------------------*/
/*------------------------------------------------------------------------------------------------------
 * @WorkAttendanceSys考勤系统按钮设定
 * @name : initButtonSetting
 * @funtion : 提供操作槽SLOT供给signal信号调用
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initButtonSetting()
{
          this->ui_sys->TakePicture->setDisabled(true);                         //默认禁用拍照摁键
          this->ui_sys->ConfirmTranningSet->setDisabled(true);           //默认禁用保存按钮
          this->ui_sys->IgnoreTranningSet->setDisabled(true);              //默认禁用 忽略按钮
          this->ui_sys->InitTranning->setDisabled(true);                        //默认禁用 开启训练按钮
          this->ui_sys->AdminOnly->setDisabled(true);                         //默认禁用 访问管理部门按钮
          this->ui_sys->TranningSetInput->setDisabled(true);                //默认禁用 注册并录入人脸按钮
          this->ui_sys->CloseVideo->setDisabled(true);                         //默认禁用 关闭视频网络的按钮
          this->ui_sys->TakePicture->update();                                       //更新拍照摁键状态
          this->ui_sys->ConfirmTranningSet->update();                         //更新
          this->ui_sys->IgnoreTranningSet->update();                            //更新
          this->ui_sys->InitTranning->update();                                      //更新
          this->ui_sys->AdminOnly->update();                                       //更新
          this->ui_sys->TranningSetInput->update();                              //更新
          this->ui_sys->CloseVideo->update();                                       //更新
}

/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys考勤系统常驻程序
* @name : initWorkAttendanceSys
* @funtion : 提供操作槽SLOT供给signal信号调用
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
* @WorkAttendanceSys考勤系统初始化下拉框
* @name :   initSysDeptComboBox
* @funtion : 初始化下拉框用于部门的选择
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initSysDeptComboBox()
{
          std::vector<std::string> dbRes = this->initDepartmentSetting();                                       //从数据库中获取部门
          for (int i = 0; i < dbRes.size(); ++i) {
                    this->ui_sys->comboBox->addItem(QString::fromLocal8Bit(dbRes[i].c_str()));            //将设置的部门信息加入下拉选项
          }
}

/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys考勤系统进度条初始化
* @name :  initProcessBarSetting
* @funtion : 初始化进度条的信息
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initProcessBarSetting()
{
          this->ui_sys->progressBar->setRange(0, this->initTranningSetting());          //读取数据库中的进度最大值
          this->ui_sys->progressBar->setValue(0);                                                         //初始化数值为0
}

/*------------------------------------------------------------------------------------------------------
 * 禁用训练相关的按钮程序
 * @name : disableTranningButton
 * @funtion : 设置空间和函数的捆绑关系
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::disableTranningButton()
{
          this->ui_sys->InitTranning->setDisabled(true);                                  //禁用训练摁键
          this->ui_sys->InitTranning->update();                                                //更新拍照摁键按钮状态
          this->ui_sys->TakePicture->setDisabled(true);                                   //禁用拍照摁键
          this->ui_sys->TakePicture->update();                                                 //更新拍照摁键状态
          this->ui_sys->ConfirmTranningSet->setDisabled(true);                     //禁用保存按钮
          this->ui_sys->ConfirmTranningSet->update();                                   //更新禁用保存按钮状态
          this->ui_sys->IgnoreTranningSet->setDisabled(true);                        //禁用忽略按钮
          this->ui_sys->IgnoreTranningSet->update();                                      //更新忽略按钮状态
          this->ui_sys->TranningSetInput->setDisabled(true);                          //禁用注册并禁用人脸按钮
          this->ui_sys->TranningSetInput->update();                                        //更新注册并禁用人脸按钮状态
          this->ui_sys->PremissionStatus->clear();                                            //清空权限状态
}

/*------------------------------------------------------------------------------------------------------
 * 删除打开普通UI
 * @name : deleteSysUi
 * @funtion : 删除打开普通UI
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::deleteSysUi()
{
          delete ui_sys;                                              //删除普通系统
}

/*------------------------------------------------------------------------------------------------------
 * 删除ADMIN系统UI
 * @name : deleteSysUi
 * @funtion : 删除ADMIN系统UI
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::deleteAdminUi()
{
          if (this->m_dataDisplay != nullptr) {                         //删除QDialog类型
                    delete this->m_dataDisplay;
                    this->m_dataDisplay = nullptr;                        //赋值初始值准备下次分配
          }
          if (this->m_qDialog != nullptr) {                              //删除QDialog类型
                    delete this->m_qDialog;
                    this->m_qDialog = nullptr;                             //赋值初始值准备下次分配
          }
}

/*------------------------------------------------------------------------------------------------------
 * 初始化ADMIN系统UI
 * @name : deleteSysUi
 * @funtion : 初始化ADMIN系统UI(或者重新分配UI)
 * @Correction: 2022-8-19 由于QDialog的重新内存分配需要重置Admin UI信号槽的设置
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initAdminUi()
{
          if (this->m_qDialog == nullptr) {                                                                       //当前的QDialog被释放 
                    this->m_qDialog = new QDialog();                                                           //重新分配内存
                    this->ui_admin->setupUi(m_qDialog);                                                       //为QDialog加载真正的UI文件
                    this->m_qDialog->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint); // 设置禁止最大化
                    this->m_qDialog->setFixedSize(1180, 744);                                            // 禁止改变窗口大小
                    this->initAdminConnectSlot();                                                                 //重置WorkAttendanceAdmin信号槽的设置程序
          }
          this->initAdminParamSetting();                                                                            //初始化管理员签到系统参数设置
          if (this->m_dataDisplay == nullptr) {                                                                   //当前的 DataDisplay被释放 
                    this->m_dataDisplay = new DataDisplay(                                                  //重新分配内存
                              this->m_qDialog, 
                              this->ui_admin->AttendenceRecord,
                              this->ui_admin->newEmployee,
                              this->ui_admin->graphicsView
                    );    
          }
}