#include "WorkAttendanceSys.h"

WorkAttendanceSys::WorkAttendanceSys(QWidget* parent)
          : QDialog(parent), Interface(),
          ui_sys(new Ui::WorkAttendanceSys),
          m_globalTimer(new QDateTime)
{
          ui_sys->setupUi(this);
          this->connectSlotSet();                                            //设置信号槽
          this->initDepartmentSetting();
          this->initWorkAttendanceSys();                              //初始化常驻程序
          this->initProcessBarSetting();                                 //初始化进度条
          this->initButtonSetting();                                        //禁用摁键
}

WorkAttendanceSys::~WorkAttendanceSys()
{
          this->QTcloseVideo();                                            //关闭视频
          delete m_globalTimer;                                            //关闭时钟系统
          for (auto& i : this->m_threadPool) {                      //关闭其他的线程
                    if (i.joinable()) {
                              i.join();
                    }
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
          this->ui_sys->TakePicture->update();                                       //更新
          this->ui_sys->ConfirmTranningSet->update();                         //更新
          this->ui_sys->IgnoreTranningSet->update();                            //更新
          this->ui_sys->InitTranning->update();                                      //更新
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
* @name :  initDepartmentSetting
* @funtion : 初始化下拉框用于部门的选择
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initDepartmentSetting()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(m_SelectDepartment);       //从数据库中获取部门
          for (int i = 0; i < dbRes.size(); ++i) {
                    ui_sys->comboBox->addItem(QString::fromLocal8Bit(dbRes[i][0].c_str()));                  //将设置的部门信息加入下拉选项
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
          [=]() {
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
                    this->ui_sys->checkPremittion->setDisabled(true);                            //禁用查询权限按钮
                    this->ui_sys->checkPremittion->update();                                          //更新查询权限按钮状态
          }();

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
          [=]() {
                    this->ui_sys->InitTranning->setDisabled(true);                                  //禁用训练摁键
                    this->ui_sys->InitTranning->update();                                                //更新拍照摁键按钮状态
                    this->ui_sys->TakePicture->setDisabled(true);                                   //禁用拍照摁键
                    this->ui_sys->TakePicture->update();                                                 //更新拍照摁键状态
                    this->ui_sys->ConfirmTranningSet->setDisabled(true);                     //禁用保存按钮
                    this->ui_sys->ConfirmTranningSet->update();                                   //更新禁用保存按钮状态
                    this->ui_sys->IgnoreTranningSet->setDisabled(true);                        //禁用忽略按钮
                    this->ui_sys->IgnoreTranningSet->update();                                      //更新忽略按钮状态
          }();

          this->QTEmployeeSignOut(
                    this->ui_sys->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_sys->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->m_globalTimer,                                                                                             //加载全局计时器
                    this->ui_sys->SystemStatusInfo                                                                             //输出窗口
          );

          [=]() {
                    this->ui_sys->InitTranning->setEnabled(true);                                  //启用训练摁键
                    this->ui_sys->InitTranning->update();                                                //更新拍照摁键按钮状态
                    this->ui_sys->TakePicture->setEnabled(true);                                   //启用拍照摁键
                    this->ui_sys->TakePicture->update();                                                 //更新拍照摁键状态
                    this->ui_sys->ConfirmTranningSet->setEnabled(true);                     //启用保存按钮
                    this->ui_sys->ConfirmTranningSet->update();                                   //更新禁用保存按钮状态
                    this->ui_sys->IgnoreTranningSet->setEnabled(true);                        //启用忽略按钮
                    this->ui_sys->IgnoreTranningSet->update();                                      //更新忽略按钮状态
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
          [=]() {
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
          }();

          this->QTEmployeeAskPremit(
                    this->ui_sys->UserID->toPlainText().toLocal8Bit().constData(),                         //UserID
                    this->ui_sys->NameInput->toPlainText().toLocal8Bit().constData(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
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
          QObject::connect(ui_sys->TranningSetInput, SIGNAL(clicked()), this, SLOT(tranningSetInputFromVideo()));    //人脸训练功能模块
          QObject::connect(ui_sys->TakePicture, SIGNAL(clicked()), this, SLOT(takePictureFromVideo()));                      //开启拍照模块
          QObject::connect(ui_sys->ConfirmTranningSet, SIGNAL(clicked()), this, SLOT(savePictureForTranning()));      //保存人脸
          QObject::connect(ui_sys->IgnoreTranningSet, SIGNAL(clicked()), this, SLOT(ignorePictureForTranning()));      //舍弃人脸

          /*当训练的进度条已满之后，可以开启Resnet网络训练*/
          QObject::connect(ui_sys->InitTranning, SIGNAL(clicked()), this, SLOT(initTranningProcess()));                         //开启残差神经网络

          /*用户的登录登出功能*/
          QObject::connect(ui_sys->SignIn, SIGNAL(clicked()), this, SLOT(employeeAttendanceInterface()));                   //开启签到程序
          QObject::connect(ui_sys->Logout, SIGNAL(clicked()), this, SLOT(employeeSignalOutInterface()));                    //开启签退程序

          /*关闭系统的核心功能*/
          QObject::connect(ui_sys->CloseVideo, SIGNAL(clicked()), this, SLOT(closeVideoStream()));                              //关闭视频流

          /*用户申请注册权限*/
          QObject::connect(ui_sys->AciqurePremit, SIGNAL(clicked()), this, SLOT(employeeAskPremitInterface()));        //向管理员账户申请注册权限
}

/*------------------------------------------------------------------------------------------------------
 * 删除打开普通和ADMIN系统UI
 * @name : deleteSysUi
 * @funtion : 删除打开普通和ADMIN系统UI
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::deleteSysUi()
{
          delete ui_sys;                                              //删除普通系统
          if (this->ui_admin != nullptr) {                  //删除ADMIN系统
                    delete this->ui_admin;
          }
}