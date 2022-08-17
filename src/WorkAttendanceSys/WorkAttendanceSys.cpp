#include "WorkAttendanceSys.h"

WorkAttendanceSys::WorkAttendanceSys(QWidget* parent)
          : QDialog(parent), Interface(),
          ui_sys(new Ui::WorkAttendanceSys),
          ui_admin(new Ui::WorkAttendanceAdmin),
          m_globalTimer(new QDateTime),
          m_qDialog(new QDialog)
{
          ui_sys->setupUi(this);
          this->connectSlotSet();                                            //设置信号槽
          this->initDepartmentComboBox();                         //考勤系统初始化下拉框
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
          this->deleteSysUi();                                                //关闭UI显示系统
          this->deleteAdminUi();                                          //关闭ADMIN的UI显示系统
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
* @name :   initDepartmentComboBox
* @funtion : 初始化下拉框用于部门的选择
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initDepartmentComboBox()
{
          std::vector<std::string> dbRes = this->initDepartmentSetting();                                       //从数据库中获取部门
          for (int i = 0; i < dbRes.size(); ++i) {
                    ui_sys->comboBox->addItem(QString::fromLocal8Bit(dbRes[i].c_str()));            //将设置的部门信息加入下拉选项
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

/*----------------------WorkAttendanceSys考勤系统按钮槽函数---------------------------*/
/*------------------------------------------------------------------------------------------------------
* 槽函数类别---关闭视频流和其他的线程
* @name : closeVideoStream
* @funtion : 提供操作槽SLOT供给signal信号调用
* @Correction: 2022-8-1 可能存在有的线程卡在死循环中无法关闭的bug(现在还存在)
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::closeVideoStream()
{
          this->QTcloseVideo();
}

/*------------------------------------------------------------------------------------------------------
 * 槽函数类别---向训练线程传递信号拍照信号
 * @name : takePictureFromVideo
 * @funtion : 提供操作槽SLOT供给signal信号调用，拍照时暂停主显示线程以方便客户查看
 * @Correction : 2022-7-30 只有摁下拍摄按钮之后才可以选择图像的保存逻辑
 *                                                没有做出保存决定之前不可拍摄下一张照片
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::takePictureFromVideo()
{
          this->QTtakePicture();
          [=]() {
                    this->ui_sys->ConfirmTranningSet->setEnabled(true);          //启用保存按钮
                    this->ui_sys->ConfirmTranningSet->update();                       //更新保存按钮状态
                    this->ui_sys->IgnoreTranningSet->setEnabled(true);             //启用忽略按钮
                    this->ui_sys->IgnoreTranningSet->update();                          //更新保存忽略按钮
                    this->ui_sys->TakePicture->setDisabled(true);                      //没有做出保存决定之前不可拍摄下一张照片
                    this->ui_sys->TakePicture->update();                                     //更新拍摄下一张照片按钮状态
          }();
}

/*------------------------------------------------------------------------------------------------------
 * 槽函数类别---根据训练集开启人脸训练模式
 * @name : initTranningProcess
 * @funtion : 提供操作槽SLOT供给signal信号调用
 * @Correction: 2022-7-29 修复无法宽字符无法正常输入数据库的bug
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initTranningProcess()
{
          [=]() {
                    this->ui_sys->TakePicture->setDisabled(true);                       //禁用拍摄按钮
                    this->ui_sys->TakePicture->update();                                      //更新拍摄按钮状态
          }();
          this->QTResnetTranning(
                    this->ui_sys->UserID->document()->toPlainText().toLocal8Bit().constData(),        //UserID
                    this->ui_sys->NameInput->document()->toPlainText().toLocal8Bit().constData(),  //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                        //Department
                    this->ui_sys->SystemStatusInfo                                                                                  //输出窗口
          );
}

/*------------------------------------------------------------------------------------------------------
 * 槽函数类别---保存当前的照片图像(仅限训练)
 * @name : savePictureForTranning
 * @funtion : 提供操作槽SLOT供给signal信号调用
 * @Correction : 2022-7-30 只有摁下拍摄按钮之后才可以选择继续拍照
 *                                                没有拍照之前不可以选择保存逻辑
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::savePictureForTranning()
{
          this->QTsaveImage();
          [=]() {
                    this->ui_sys->ConfirmTranningSet->setDisabled(true);          //禁用保存按钮
                    this->ui_sys->ConfirmTranningSet->update();                        //更新保存按钮状态
                    this->ui_sys->IgnoreTranningSet->setDisabled(true);             //禁用忽略按钮
                    this->ui_sys->IgnoreTranningSet->update();                           //更新忽略按钮状态
                    this->ui_sys->TakePicture->setEnabled(true);                         //启用拍摄按钮
                    this->ui_sys->TakePicture->update();                                      //更新拍摄按钮状态
          }();
}

/*------------------------------------------------------------------------------------------------------
 * 槽函数类别---舍弃当前的照片图像(仅限训练)
 * @name : ignorePicture
 * @funtion : 提供操作槽SLOT供给signal信号调用
 * @Correction : 2022-7-30 只有摁下拍摄按钮之后才可以选择继续拍照
 *                                                没有拍照之前不可以选择保存逻辑
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::ignorePictureForTranning()
{
          this->QTignoreImage();
          [=]() {
                    this->ui_sys->ConfirmTranningSet->setDisabled(true);          //禁用保存按钮
                    this->ui_sys->ConfirmTranningSet->update();                        //更新禁用保存按钮状态
                    this->ui_sys->IgnoreTranningSet->setDisabled(true);             //禁用忽略按钮
                    this->ui_sys->IgnoreTranningSet->update();                           //更新忽略按钮状态
                    this->ui_sys->TakePicture->setEnabled(true);                         //启用拍摄按钮
                    this->ui_sys->TakePicture->update();                                      //更新拍摄按钮状态
          }();
}

/*------------------------------------------------------------------------------------------------------
 * 槽函数类别---开启人脸训练模式，从拍照线程中获取图片并加入训练集
 * @name : tranningSetInputFromVideo
 * @funtion : 提供操作槽SLOT供给signal信号调用
 * @Correction : 2022-7-30 启动训练拍照程序之后系统允许按钮的操作
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::tranningSetInputFromVideo()                              //启动训练线程
{
          [=]() {
                    this->ui_sys->TakePicture->setEnabled(true);                         //启用拍照摁键
                    this->ui_sys->TakePicture->update();                                      //更新拍照摁键状态
                    this->ui_sys->InitTranning->setEnabled(true);                        //？启用训练按钮
                    this->ui_sys->InitTranning->update();                                     //？更新训练按钮状态
          }();
          this->QTtranningSetInput(
                    this->m_videoFlag,
                    this->ui_sys->SystemStatusInfo,
                    this->ui_sys->progressBar,
                    0
          );
          [=]() {
                    this->ui_sys->TakePicture->setDisabled(true);                        //禁用拍照摁键
                    this->ui_sys->TakePicture->update();                                      //更新拍照摁键状态
                    this->ui_sys->ConfirmTranningSet->setDisabled(true);          //禁用保存摁键
                    this->ui_sys->ConfirmTranningSet->update();                        //更新保存摁键状态
                    this->ui_sys->IgnoreTranningSet->setDisabled(true);             //禁用舍弃摁键
                    this->ui_sys->IgnoreTranningSet->update();                           //更新舍弃摁键状态
                    this->ui_sys->InitTranning->setDisabled(true);                       //禁用训练按钮
                    this->ui_sys->InitTranning->update();                                     //更新训练按钮状态
                    this->ui_sys->UserID->clear();                                                //清空ID的输入
                    this->ui_sys->NameInput->clear();                                          //清空名字的输入
          }();
}

/*------------------------------------------------------------------------------------------------------
 * 槽函数类别---摁下当前开关启动用户的人脸登录系统(开始人脸识别)
 * @name : employeeAttendanceInterface
 * @funtion : 提供操作槽SLOT供给signal信号调用
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::employeeAttendanceInterface()
{
          this->disableTranningButton();                                                                                        //禁用训练相关的按钮程序
          this->QTEmployeeAttendance(
                    this->ui_sys->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_sys->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->m_globalTimer,                                                                                             //加载全局计时器
                    this->ui_sys->SystemStatusInfo                                                                             //输出窗口
          );

          [=]() {
                    this->ui_sys->checkPremittion->setEnabled(true);                            //启用查询权限按钮
                    this->ui_sys->checkPremittion->update();                                          //更新查询权限按钮状态
                    this->ui_sys->UserID->clear();                                                           //清空ID的输入
                    this->ui_sys->NameInput->clear();                                                     //清空名字的输入
          }();
}

/*------------------------------------------------------------------------------------------------------
 * 槽函数类别---摁下当前开关启动用户的人脸登出系统(开始人脸识别)
 * @name : employeeSignalOutInterface
 * @funtion : 提供操作槽SLOT供给signal信号调用
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::employeeSignalOutInterface()
{
          this->disableTranningButton();                                                                                        //禁用训练相关的按钮程序
          this->QTEmployeeSignOut(
                    this->ui_sys->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_sys->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->m_globalTimer,                                                                                             //加载全局计时器
                    this->ui_sys->SystemStatusInfo                                                                             //输出窗口
          );

          [=]() {
                    this->ui_sys->checkPremittion->setEnabled(true);                            //启用查询权限按钮
                    this->ui_sys->checkPremittion->update();                                          //更新查询权限按钮状态
                    this->ui_sys->UserID->clear();                                                           //清空ID的输入
                    this->ui_sys->NameInput->clear();                                                     //清空名字的输入
          }();
}

/*------------------------------------------------------------------------------------------------------
 * 槽函数类别---摁下当前开关向管理员账户申请注册权限
 * @name : employeeAskPremitInterface
 * @funtion : 提供操作槽SLOT供给signal信号调用
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::employeeAskPremitInterface()
{
          this->disableTranningButton();                                                                                        //禁用训练相关的按钮程序
          this->QTEmployeeAskPremit(
                    this->ui_sys->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_sys->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->m_globalTimer,                                                                                             //加载全局计时器
                    this->ui_sys->SystemStatusInfo                                                                             //输出窗口
          );
          this->ui_sys->UserID->clear();                                                           //清空ID的输入
          this->ui_sys->NameInput->clear();                                                     //清空名字的输入
}

/*------------------------------------------------------------------------------------------------------
 * 槽函数类别---摁下当前开关检查注册权限
 * @name : employeecheckPremittion
 * @funtion : 提供操作槽SLOT供给signal信号调用
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::employeeCheckPremittion()
{
          this->disableTranningButton();                                                                                        //禁用训练相关的按钮程序
          this->QTEmployeeCheckPremittion(
                    this->ui_sys->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_sys->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->ui_sys->TranningSetInput,                                                                            //注册并录入人脸
                    this->m_globalTimer,                                                                                             //加载全局计时器
                    this->ui_sys->PremissionStatus,                                                                             //输出权限状态接口
                    this->ui_sys->SystemStatusInfo                                                                             //输出窗口
          );
}

/*------------------------WorkAttendanceSys考勤系统管理员系统-----------------------------*/
/*------------------------------------------------------------------------------------------------------
 * 槽函数类别---登录管理部门系统设定
 * @name : adminManagementLogin
 * @funtion : 访问管理部门系统
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::adminManagementLogin()
{
          this->ui_sys->AdminOnly->setDisabled(true);                                                               //关闭访问管理部门系统按钮
          this->ui_sys->AdminOnly->update();
          this->ui_sys->CloseVideo->setDisabled(true);                                                                //关闭视频和识别网络按钮
          this->ui_sys->CloseVideo->update();
          this->disableTranningButton();                                                                                        //禁用训练相关的按钮程序
          this->QTAdminManagementLogin(
                    this->ui_sys->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_sys->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->ui_sys->AdminOnly,                                                                                     //访问管理部门系统按钮
                    this->ui_sys->CloseVideo,                                                                                     //视频和识别网络按钮
                    this->m_globalTimer,                                                                                             //加载全局计时器
                    this->ui_sys->SystemStatusInfo                                                                             //输出窗口
          );
}

/*------------------------------------------------------------------------------------------------------
 * 信号槽的设置程序
 * @name : connectSlotSet
 * @funtion : 设置空间和函数的捆绑关系
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::connectSlotSet()
{
          /*当经过管理人员审批之后开启积累训练的进度条*/
          QObject::connect(this->ui_sys->TranningSetInput, SIGNAL(clicked()), this, SLOT(tranningSetInputFromVideo()));    //人脸训练功能模块
          QObject::connect(this->ui_sys->TakePicture, SIGNAL(clicked()), this, SLOT(takePictureFromVideo()));                      //开启拍照模块
          QObject::connect(this->ui_sys->ConfirmTranningSet, SIGNAL(clicked()), this, SLOT(savePictureForTranning()));      //保存人脸
          QObject::connect(this->ui_sys->IgnoreTranningSet, SIGNAL(clicked()), this, SLOT(ignorePictureForTranning()));      //舍弃人脸

          /*当训练的进度条已满之后，可以开启Resnet网络训练*/
          QObject::connect(this->ui_sys->InitTranning, SIGNAL(clicked()), this, SLOT(initTranningProcess()));                         //开启残差神经网络

          /*用户的登录登出功能*/
          QObject::connect(this->ui_sys->SignIn, SIGNAL(clicked()), this, SLOT(employeeAttendanceInterface()));                   //开启签到程序
          QObject::connect(this->ui_sys->Logout, SIGNAL(clicked()), this, SLOT(employeeSignalOutInterface()));                    //开启签退程序

          /*关闭系统的核心功能*/
          QObject::connect(this->ui_sys->CloseVideo, SIGNAL(clicked()), this, SLOT(closeVideoStream()));                              //关闭视频流

          /*用户申请注册权限*/
          QObject::connect(this->ui_sys->AciqurePremit, SIGNAL(clicked()), this, SLOT(employeeAskPremitInterface()));        //向管理员账户申请注册权限
          
          /*查询新用户的权限是否通过*/
          QObject::connect(this->ui_sys->checkPremittion, SIGNAL(clicked()), this, SLOT(employeeCheckPremittion()));        //检查注册权限是否通过

          /*开启管理部门系统登录验证*/
          QObject::connect(this->ui_sys->AdministerLogin, SIGNAL(clicked()), this, SLOT(adminManagementLogin()));         //启动管理部门系统登录验证
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
          if (this->ui_admin != nullptr) {                  //删除ADMIN系统
                    delete this->ui_admin;
          }
}