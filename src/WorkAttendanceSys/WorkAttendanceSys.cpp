#include "WorkAttendanceSys.h"

WorkAttendanceSys::WorkAttendanceSys(QWidget* parent)
          : QDialog(parent), Interface(),
          ui_sys(new Ui::WorkAttendanceSys),
          ui_admin(new  Ui::WorkAttendanceAdmin)
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
          this->videoClose();                                                  //关闭视频
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
          this->ui_sys->ConfirmTranningSet->setDisabled(true);          //默认禁用保存按钮
          this->ui_sys->IgnoreTranningSet->setDisabled(true);          //默认禁用 忽略按钮
          this->ui_sys->InitTranning->setDisabled(true);                        //默认禁用 开启训练按钮
          this->ui_sys->TakePicture->update();                                  //更新
          this->ui_sys->ConfirmTranningSet->update();
          this->ui_sys->IgnoreTranningSet->update();
          this->ui_sys->InitTranning->update();
}

/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys考勤系统常驻程序
* @name : initWorkAttendanceSys
* @funtion : 提供操作槽SLOT供给signal信号调用
*------------------------------------------------------------------------------------------------------*/
void  WorkAttendanceSys::initWorkAttendanceSys()
{
          this->m_threadPool.emplace_back(&Interface::QTVideoOutput, this, std::ref(this->ui_sys->VideoDisplay), std::ref(this->ui_sys->SystemStatusInfo));
          this->m_threadPool.emplace_back(&Interface::setLcdTimer, this, std::ref(this->ui_sys->lcdNumber));
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
          this->ui_sys->progressBar->setRange(0, 10);                      //
          this->ui_sys->progressBar->setValue(0);                             //初始化数值为0
}

/*------------------------------------------------------------------------------------------------------
* 视频流的关闭开关
* @name : videoStreamClose
* @funtion : 提供操作槽SLOT供给signal信号调用
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::videoStreamClose()
{
          this->videoClose();
}

/*------------------------------------------------------------------------------------------------------
 * 在训练模型时作为训练集的拍照开关
 * @name : takePictureForTranning
 * @funtion : 提供操作槽SLOT供给signal信号调用
 * @Correction : 2022-7-30 只有摁下拍摄按钮之后才可以选择图像的保存逻辑
 *                                                没有做出保存决定之前不可拍摄下一张照片
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::takePictureForTranning()
{
          this->videoCameraShooting();
          this->ui_sys->ConfirmTranningSet->setEnabled(true);          //启用保存按钮
          this->ui_sys->IgnoreTranningSet->setEnabled(true);             //启用忽略按钮
          this->ui_sys->TakePicture->setDisabled(true);                      //没有做出保存决定之前不可拍摄下一张照片
          this->ui_sys->ConfirmTranningSet->update();
          this->ui_sys->IgnoreTranningSet->update();
          this->ui_sys->TakePicture->update();
}

/*------------------------------------------------------------------------------------------------------
 * 开始使用训练模型进行训练集的训练
 * @name : initModelTranning
 * @funtion : 提供操作槽SLOT供给signal信号调用
 * @Correction: 2022-7-29 修复无法宽字符无法正常输入数据库的bug
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initModelTranning()
{
          this->QTResnetTranning(
                    this->ui_sys->UserID->document()->toRawText().toStdString(),                         //UserID
                    this->ui_sys->NameInput->document()->toRawText().toStdString(),                   //UserName
                    this->ui_sys->comboBox->currentText().toLocal8Bit().constData(),                    //Department
                    this->ui_sys->SystemStatusInfo                                                                             //输出窗口
          );
}

/*------------------------------------------------------------------------------------------------------
 * 保存当前的照片图像
 * @name : savePicture
 * @funtion : 提供操作槽SLOT供给signal信号调用
 * @Correction : 2022-7-30 只有摁下拍摄按钮之后才可以选择继续拍照
 *                                                没有拍照之前不可以选择保存逻辑
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::savePicture()
{
          this->videoFrameSavingProcess();
          this->ui_sys->ConfirmTranningSet->setDisabled(true);          //禁用保存按钮
          this->ui_sys->IgnoreTranningSet->setDisabled(true);             //禁用忽略按钮
          this->ui_sys->TakePicture->setEnabled(true);                         //启用拍摄按钮
          this->ui_sys->ConfirmTranningSet->update();
          this->ui_sys->IgnoreTranningSet->update();
          this->ui_sys->TakePicture->update();
}

/*------------------------------------------------------------------------------------------------------
 * 舍弃当前的照片图像
 * @name : ignorePicture
 * @funtion : 提供操作槽SLOT供给signal信号调用
 * @Correction : 2022-7-30 只有摁下拍摄按钮之后才可以选择继续拍照
 *                                                没有拍照之前不可以选择保存逻辑
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::ignorePicture()
{
          this->videoFrameIgnoreProcess();
          this->ui_sys->ConfirmTranningSet->setDisabled(true);          //禁用保存按钮
          this->ui_sys->IgnoreTranningSet->setDisabled(true);             //禁用忽略按钮
          this->ui_sys->TakePicture->setEnabled(true);                         //启用拍摄按钮
          this->ui_sys->ConfirmTranningSet->update();
          this->ui_sys->IgnoreTranningSet->update();
          this->ui_sys->TakePicture->update();
}

/*------------------------------------------------------------------------------------------------------
 * 摁下当前开关启动人脸的训练拍照程序
 * @name : registerEmployee
 * @funtion : 提供操作槽SLOT供给signal信号调用
 * @Correction : 2022-7-30 启动训练拍照程序之后系统允许按钮的操作
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::registerEmployee()                          //启动训练线程
{
          this->ui_sys->TakePicture->setEnabled(true);                         //启用拍照摁键
          this->ui_sys->InitTranning->setEnabled(true);                         //？启用开启训练按钮
          this->ui_sys->TakePicture->update();                                      //更新
          this->ui_sys->InitTranning->update();
          this->startVideoRegister(
                    this->m_videoFlag,
                    this->ui_sys->SystemStatusInfo,
                    this->ui_sys->progressBar,
                    0);
}

/*------------------------------------------------------------------------------------------------------
 * 信号槽的设置程序
 * @name : connectSlotSet
 * @funtion : 设置空间和函数的捆绑关系
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::connectSlotSet()
{
          QObject::connect(ui_sys->CloseVideo, SIGNAL(clicked()), this, SLOT(videoStreamClose()));                                 //关闭视频流
          QObject::connect(ui_sys->TakePicture, SIGNAL(clicked()), this, SLOT(takePictureForTranning()));             //开启拍照模块
          QObject::connect(ui_sys->Register, SIGNAL(clicked()), this, SLOT(registerEmployee()));                            //人脸训练功能模块
          QObject::connect(ui_sys->ConfirmTranningSet, SIGNAL(clicked()), this, SLOT(savePicture()));                 //保存人脸
          QObject::connect(ui_sys->IgnoreTranningSet, SIGNAL(clicked()), this, SLOT(ignorePicture()));                 //舍弃人脸
          QObject::connect(ui_sys->InitTranning, SIGNAL(clicked()), this, SLOT(initModelTranning()));                   //开启残差神经网络
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