#include "WorkAttendanceSys.h"

WorkAttendanceSys::WorkAttendanceSys(QWidget* parent)
          : QDialog(parent), Interface(10)
{
          ui.setupUi(this);
          this->connectSlotSet();                                            //设置信号槽
          this->initDepartmentSetting();
          this->initWorkAttendanceSys();                              //初始化常驻程序
          this->initProcessBarSetting();                                 //初始化进度条
}

WorkAttendanceSys::~WorkAttendanceSys()
{
          this->videoClose();
          for (auto& i : this->m_threadPool) {
                    if (i.joinable()) {
                              i.join();
                    }
          }
}

/*------------------------WorkAttendanceSys考勤系统初始化-----------------------------*/
/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys考勤系统常驻程序
* @name : initWorkAttendanceSys
* @funtion : 提供操作槽SLOT供给signal信号调用
*------------------------------------------------------------------------------------------------------*/
void  WorkAttendanceSys::initWorkAttendanceSys()
{
          this->m_threadPool.emplace_back(&Interface::QTVideoOutput, this, std::ref(this->ui.VideoDisplay), std::ref(this->ui.SystemStatusInfo));
          this->m_threadPool.emplace_back(&Interface::setLcdTimer, this, std::ref(this->ui.lcdNumber));
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
                    ui.comboBox->addItem(QString::fromLocal8Bit(dbRes[i][0].c_str()));                  //将设置的部门信息加入下拉选项
          }
}

/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys考勤系统进度条初始化
* @name :  initProcessBarSetting
* @funtion : 初始化进度条的信息
*------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::initProcessBarSetting()
{
          this->ui.progressBar->setRange(0, 10);                      //
          this->ui.progressBar->setValue(0);                             //初始化数值为0
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
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::takePictureForTranning()
{
          this->videoCameraShooting();
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
                    this->ui.UserID->document()->toRawText().toStdString(),                         //UserID
                    this->ui.NameInput->document()->toRawText().toStdString(),                   //UserName
                    this->ui.comboBox->currentText().toLocal8Bit().constData(),                    //Department修
                    this->ui.SystemStatusInfo                                                                             //输出窗口
          );
}

/*------------------------------------------------------------------------------------------------------
 * 保存当前的照片图像
 * @name : savePicture
 * @funtion : 提供操作槽SLOT供给signal信号调用
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::savePicture()
{
          this->videoFrameSavingProcess();
}

/*------------------------------------------------------------------------------------------------------
 * 舍弃当前的照片图像
 * @name : ignorePicture
 * @funtion : 提供操作槽SLOT供给signal信号调用
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::ignorePicture()
{
          this->videoFrameIgnoreProcess();
}

/*------------------------------------------------------------------------------------------------------
 * 摁下当前开关启动人脸的训练程序
 * @name : registerEmployee
 * @funtion : 提供操作槽SLOT供给signal信号调用
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::registerEmployee()                          //启动训练线程
{
          this->startVideoRegister(
                    this->m_videoFlag,
                    this->ui.SystemStatusInfo,
                    this->ui.progressBar,
                    0);
}

/*------------------------------------------------------------------------------------------------------
 * 信号槽的设置程序
 * @name : connectSlotSet
 * @funtion : 设置空间和函数的捆绑关系
 *------------------------------------------------------------------------------------------------------*/
void WorkAttendanceSys::connectSlotSet()
{
          QObject::connect(ui.Exit, SIGNAL(clicked()), this, SLOT(videoStreamClose()));                                 //关闭视频流
          QObject::connect(ui.TakePicture, SIGNAL(clicked()), this, SLOT(takePictureForTranning()));             //开启拍照模块
          QObject::connect(ui.Register, SIGNAL(clicked()), this, SLOT(registerEmployee()));                            //人脸训练功能模块
          QObject::connect(ui.ConfirmTranningSet, SIGNAL(clicked()), this, SLOT(savePicture()));                 //保存人脸
          QObject::connect(ui.IgnoreTranningSet, SIGNAL(clicked()), this, SLOT(ignorePicture()));                 //舍弃人脸
          QObject::connect(ui.InitTranning, SIGNAL(clicked()), this, SLOT(initModelTranning()));                   //开启残差神经网络
}