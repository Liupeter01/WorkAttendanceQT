#include"Interface.h"

Interface::Interface()
          :DBProcess(),
          ImageProcess(this->initTranningSetting(), this->initTrainningSimilarity())
{
          //先初始化DBProcess数据库程序，再初始化ImageProcess
          //此时的ImageProcess类已经被内部创建，无须再次初始化!!!
}

Interface::~Interface()
{
}

/*-------------------------------QTWidget层对外的封装接口----------------------------------*/
/*------------------------------------------------------------------------------------------------------
 *  QLabel图层覆写函数
 * @name: QTsetLabelImage
 * @function：将MAT类型转换为QT的QImage类型并覆盖到QLabel容器中
 * @param 1.输入右值类型的 QImage&
 *                2.输入显示图层的指针_qlabel
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTsetLabelImage(QImage& _qimage, QLabel*& _qlabel)
{
          _qimage = _qimage.scaled(_qlabel->width(), _qlabel->height());                 //适配控件分辨率
          _qlabel->setPixmap(QPixmap::fromImage(_qimage));
}

/*------------------------------------------------------------------------------------------------------
 *  QLCDNumber图层的时限显示函数
 * @name: QTsetLabelImage
 * @function：将QTimer类型作为事件数据类型传入到容器QLCDNumber中
 * @param 1.date数据类型 QLCDNumber*& _date
 *                2.时间数据类型 QLCDNumber*& _realTimeClock
 *                3.全局时钟系统的输入 QDateTime*& _timer
 *
 *  @Correction: 2022-8-2 引入全局的时钟系统
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTsetLcdTimer(
          QLCDNumber*& _date,
          QLCDNumber*& _realTimeClock,
          QDateTime*& _timer
)
{
          _date->setDigitCount(10);                                                //设置晶体管控件QLCDNumber能显示的位数
          _date->setMode(QLCDNumber::Dec);                            //设置显示的模式为十进制
          _date->setSegmentStyle(QLCDNumber::Flat);               //设置显示方式
          _realTimeClock->setDigitCount(10);                                                //设置晶体管控件QLCDNumber能显示的位数
          _realTimeClock->setMode(QLCDNumber::Dec);                            //设置显示的模式为十进制
          _realTimeClock->setSegmentStyle(QLCDNumber::Flat);               //设置显示方式
          while (!this->m_videoFlag)
          {
                    _date->display(_timer->currentDateTime().toString("yyyy-MM-dd"));
                    _realTimeClock->display(_timer->currentDateTime().toString("hh:mm:ss"));
          }
}

/*----------------------QTWidget层WorkAttendenceSys的操作接口-------------------------*/
/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层提供的视频关闭函数
 * @name: QTcloseVideo
 * @function：本质是关闭视频中的循环参数
*-----------------------------------------------------------------------------------------------------*/
void  Interface::QTcloseVideo()
{
          m_videoFlag = true;
}

/*------------------------------------------------------------------------------------------------------
 *   Interface类为QTWidget层开启当前帧拍照操作
 * @name: QTsavePicture
 * @function：本质是拍摄当前的单帧人脸
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTtakePicture()
{
          this->enableCameraShooting();
}

/*------------------------------------------------------------------------------------------------------
 *   Interface类为QTWidget层从拍照线程中获取图片并加入训练集
 * @name: QTtranningSetInput
 * @function：开启将多张实时人脸加入训练集的任务
*@param:    1.视频开关 std::atomic<bool> &
*                   2.输出窗口接口：QTextBrowser * &_systemOutput
*                   3.进度条的输出接口：QProgressBar * &_processBar
*                   4.新员工申请权限入口 QPushButton *&_pushButton
*                   5.进度数值显示器 : int _displayNumber
*
*@Correction : 2022 - 7 - 24 添加函数参数修复防止线程无法正确的停止运转
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTtranningSetInput(
          std::atomic<bool>& _videoFlag,
          QTextBrowser*& _systemOutput,
          QProgressBar*& _processBar,
          QPushButton*& _pushButton,
          int _displayNumber
)
{
          _pushButton->setDisabled(true);
          this->startImageTranningSetInput(_videoFlag, _systemOutput, _processBar, _displayNumber);
          _pushButton->setEnabled(true);
}

/*------------------------------------------------------------------------------------------------------
 *   Interface类为QTWidget层开启当前帧的保存操作
 * @name: videoFrameSavingProcess
 * @function：本质是保存当前的单帧人脸
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTsaveImage()
{
          this->enableSavingProcess();
}

/*------------------------------------------------------------------------------------------------------
 *   Interface类为QTWidget层开启当前帧的舍弃操作
 * @name: videoFrameIgnoreProcess
 * @function：本质是舍弃当前的单帧人脸
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTignoreImage()
{
          this->enableIgnoreProcess();
}

/*------------------------------------------------------------------------------------------------------
 *   Interface类为QTWidget层视频连续输出函数
 * @name: QTVideoOutput
 * @function：连续更新显示图层所显示的图像
 * @param: 1. 视频输出pixmap : QLabel*& _qlabel
 *                 2. 输出窗口接口：QTextBrowser*& _systemOutput
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTVideoOutput(QLabel*& _qlabel, QTextBrowser*& _systemOutput)
{
          while (!m_videoFlag) {
                    this->QTsetLabelImage(this->startVideoDisplayThread(_systemOutput), _qlabel);                       //适配控件分辨率
          }
          _qlabel->clear();
}


/*------------------------------------------------------------------------------------------------------
 *   Interface类为QTWidget层输入的训练集训练开关
 * @name: QTResnetTranning
 * @function：输入的训练集训练开关，并另外将信息写入数据库
 * @param:
 *                  1. 用户ID的输入  const std::string& _userID
 *                  2. 用户姓名的输入    const std::string& _userName
 *                  3. 部门的输入         const std::string & _department
 *                  4. 注册并录入人脸 QPushButton *&signin
 *                  5.输出窗口接口：QTextBrowser*& _systemOutput
 *
*------------------------------------------------------------------------------------------------------*/
void Interface::QTResnetTranning(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QPushButton*& signin,
          QTextBrowser*& _systemOutput
)
{
          signin->setDisabled(true);
          std::string& r_faceMatrix(this->startResnetModelTranning(_systemOutput));       //获取人脸矩阵的右值
          if (this->storeFaceRecord2DB(_userID, _userName, _department, r_faceMatrix)){   //将数据存储入数据库
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("的") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("人脸信息存储成功!\n")
                    );
          }
          else {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("的") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("人脸信息存储失败!\n")
                    );
          }
          signin->setEnabled(true);
}


/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层输入的人脸签到开关
 * @name: QTEmployeeAttendance
 * @function：从数据库中找到对应的人脸特征向量字符串并根据当前的实时人脸进行对比
 *                      如果满足条件则将信息输入到实时摄像头的显示层
 *
 * @param:
 *                  1. 用户ID的输入  const std::string& _userID
 *                  2. 用户姓名的输入    const std::string& _userName
 *                  3. 部门的输入         const std::string & _department
 *                  4.全局时钟系统的输入 QDateTime*& _timer
 *                  5. 输出窗口接口：QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTEmployeeAttendance(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QDateTime*& _timer,
          QTextBrowser*& _systemOutput
)
{
          try{
                    if (_userID == "" || _userName == "") {                                                                                                     //ID和名字不可以为空
                              throw  InvalidInput();                                                                                                                       //无效的登录信息输入
                    }
                    /*将人脸信息从数据库中先提取并验证是否存在*/
                    std::string dbFaceMatrix = this->readFaceRecordFromDB(_userID, _userName, _department);   //输入UserID+UserName+Department得到人脸特征矩阵的字符串
                    if (dbFaceMatrix == "" || !dbFaceMatrix.size()) {                                                                             //返回的人脸矩阵字符串为空
                              throw EmptyMatrixString();                                                                                                    //MatrixString为空的异常
                    }
                    if (this->startFacialRecognize(dbFaceMatrix, _systemOutput)) {                                                   //将数据库dbFaceMatrix和实时人脸进行比对
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                                        QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("的") +
                                        QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("人脸登陆成功!\n")
                              );
                              /*检查是否存在重复的考勤记录*/
                              if (!this->readDuplicateRecordFromDB(_userID, _department, AttendanceTable::ATTENDANCE, _timer))
                              {
                                        /*如果不存在重复的记录，则记录考勤时间*/
                                        this->storeAttendanceRecord2DB(                                                                                         //进行考勤时间的记录
                                                  _userID, _department,
                                                  ((_timer->currentDateTime().time() > this->getMorningShiftTime()) ? "员工迟到" : "正常签到"),
                                                  _timer
                                        );
                                        return;
                              }
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                                        QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("的") +
                                        QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("重复进行人脸签到\n")
                              );
                    }
                    else {
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                                        QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("的") +
                                        QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("人脸识别失败!请重新尝试\n")
                              );
                    }
          }
          catch (const InvalidInput&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("当前输入中含有非法字符，请重新输入\n")
                    );
          }
          catch (const EmptyMatrixString &) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("查无此人，人脸登陆失败!  请联系系统管理员\n")
                    );
          }
}

/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层输入的人脸登出开关
 * @name: QTEmployeeSignOut
 * @function：从数据库中找到对应的人脸特征向量字符串并根据当前的实时人脸进行对比
 *                      如果满足条件则将信息输入到实时摄像头的显示层
 *
 * @param:
 *                  1. 用户ID的输入  const std::string& _userID
 *                  2. 用户姓名的输入    const std::string& _userName
 *                  3. 部门的输入         const std::string & _department
 *                  4.全局时钟系统的输入 QDateTime*& _timer
 *                  5. 输出窗口接口：QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTEmployeeSignOut(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QDateTime*& _timer,
          QTextBrowser*& _systemOutput
)
{
          try {
                    if (_userID == "" || _userName == "") {                                                                                                     //ID和名字不可以为空
                              throw  InvalidInput();                                                                                                                       //无效的登录信息输入
                    }
                    /*将人脸信息从数据库中先提取并验证是否存在*/
                    std::string dbFaceMatrix = this->readFaceRecordFromDB(_userID, _userName, _department);   //输入UserID+UserName+Department得到人脸特征矩阵的字符串
                    if (dbFaceMatrix == "" || !dbFaceMatrix.size()) {                                                                             //返回的人脸矩阵字符串为空
                              throw EmptyMatrixString();                                                                                                    //MatrixString为空的异常
                    }
                    if (this->startFacialRecognize(dbFaceMatrix, _systemOutput)) {                                                   //将数据库dbFaceMatrix和实时人脸进行比对
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                                        QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("的") +
                                        QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("人脸签退成功!\n")
                              );
                              /*检查是否存在重复的考勤记录*/
                              if (!this->readDuplicateRecordFromDB(_userID, _department, AttendanceTable::SIGNOUT, _timer))
                              {
                                        /*如果不存在重复的记录，则记录考勤时间*/
                                        this->storeSignOutRecord2DB(                                                                                                //进行考勤时间的记录
                                                  _userID, _department,
                                                  ((_timer->currentDateTime().time() < this->getNightshiftTime()) ? "员工早退" : "正常签退"),
                                                  _timer
                                        );
                                        return;
                              }
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                                        QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("的") +
                                        QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("重复进行人脸签退\n")
                              );
                    }
                    else {
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +
                                        QString::fromLocal8Bit("]:") + QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("的") +
                                        QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("人脸识别失败!请重新尝试\n")
                              );
                    }
          }
          catch (const InvalidInput&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("当前输入中含有非法字符，请重新输入\n")
                    );
          }
          catch (const EmptyMatrixString&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("查无此人，人脸登陆失败!  请联系系统管理员\n")
                    );
          }
}

/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层输入的新员工注册权限申请
 * @name:QTEmployeeAskPremit
 * @function：在数据库中写入申请信息等待管理员进行决断
 * @param:
 *                  1. 用户ID的输入  const std::string& _userID
 *                  2. 用户姓名的输入    const std::string& _userName
 *                  3. 部门的输入         const std::string & _department
 *                  4.全局时钟系统的输入 QDateTime*& _timer
 *                  5. 输出窗口接口：QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void  Interface::QTEmployeeAskPremit(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QDateTime*& _timer,
          QTextBrowser*& _systemOutput
)
{
          try {
                    if (_userID == "" || _userName == "") {                                                                                                     //ID和名字不可以为空
                              throw  InvalidInput();                                                                                                                       //无效的登录信息输入
                    }
                    this->storeAskPremitRecord2DB(_userID, _userName, _department, _timer);                             //申请访问权限
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("新员工权限申请成功，请等待管理员的审批\n")
                    );
          }
          catch (const InvalidInput&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("当前输入中含有非法字符，请重新输入\n")
                    );
          }
}

/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层输入检查员工的操作权限
 * @name:QTEmployeeCheckPremittion
 * @function：在数据库中写入申请信息等待管理员进行决断
 * @param:
 *                  1. 用户ID的输入  const std::string& _userID
 *                  2. 用户姓名的输入    const std::string& _userName
 *                  3. 部门的输入         const std::string & _department
 *                  4. 注册并录入人脸的开关启用 : QPushButton *&_TrainningSetInput,
 *                  5.全局时钟系统的输入 QDateTime*& _timer
 *                  6 将数据库中的权限信息写入到屏幕上 :QTextBrowser*& _privliedgeStatus
 *                  7. 输出窗口接口：QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTEmployeeCheckPremittion(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QPushButton*& _TrainningSetInput,
          QDateTime*& _timer,
          QTextBrowser*& _privliedgeStatus,
          QTextBrowser*& _systemOutput
)
{
          try {
                    if (_userID == "" || _userName == "") {                                                                                                     //ID和名字不可以为空
                              throw  InvalidInput();                                                                                                                       //无效的登录信息输入
                    }
                    std::string authoritySet = this->checkPremitRecordFromDB(_userID, _userName, _department);         //权限信息
                    if (authoritySet == "" || !authoritySet.size()) {                                                                                           //没有查出申请的权限信息
                              throw EmptyMatrixString();                                                                                                            //无效的申请的权限信息
                    }
                    _privliedgeStatus->insertPlainText(QString::fromLocal8Bit(authoritySet.c_str()));
                    if (authoritySet == "申请通过") {
                              _TrainningSetInput->setEnabled(true);                              //启用注册人脸按钮                                                      
                              _TrainningSetInput->update();                                           //更新注册人脸按钮状态
                    }
                    else {
                              _TrainningSetInput->setDisabled(true);                            //禁用注册并禁用人脸按钮   
                              _TrainningSetInput->update();                                          //更新注册并禁用人脸按钮状态
                    }
          }
          catch (const InvalidInput&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("当前输入中含有非法字符，请重新输入\n")
                    );
          }
          catch (const EmptyMatrixString&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("系统中没有查询到新员工的权限!  请联系系统管理员\n")
                    );
          }
}

/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层管理员账户权限认证系统
 * @name: QTAdminManagementLogin
 * @function：管理员账户权限认证系统
 * @param:
 *                  1. 用户ID的输入  const std::string& _userID
 *                  2. 用户姓名的输入    const std::string& _userName
 *                  3. 部门的输入         const std::string & _department
 *                  4. 当前成功登陆的管理员ID std::string & _currentAdmin,
 *                  5. 登录管理部门系统  QPushButton*& _AdministerLogin,
 *                  6. 访问管理部门系统的开关启用 : QPushButton*& _adminUI
 *                  7. 关闭视频和识别网络的开关启用 : QPushButton*& _closeVideo
 *                  8.全局时钟系统的输入 QDateTime*& _timer
 *                  9. 输出窗口接口：QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTAdminManagementLogin(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          std::string & _currentAdmin,
          QPushButton*& _AdministerLogin,
          QPushButton*& _adminUI,
          QPushButton*& _closeVideo,
          QDateTime*& _timer,
          QTextBrowser*& _systemOutput
)
{
          try {
                    if (_userID == "" || _userName == "") {                                                                                                     //ID和名字不可以为空
                              throw  InvalidInput();                                                                                                                       //无效的登录信息输入
                    }
                    /*将人脸信息从数据库中先提取并验证是否存在*/
                    std::string dbFaceMatrix = this->readFaceRecordFromDB(_userID, _userName, _department);   //输入UserID+UserName+Department得到人脸特征矩阵的字符串
                    if (dbFaceMatrix == "" || !dbFaceMatrix.size()) {                                                                             //返回的人脸矩阵字符串为空
                              throw EmptyMatrixString();                                                                                                    //MatrixString为空的异常
                    }
                    if (this->startFacialRecognize(dbFaceMatrix, _systemOutput)) {                                                   //将数据库dbFaceMatrix和实时人脸进行比对
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                                        QString::fromLocal8Bit("管理员") +QString::fromLocal8Bit(_userName.c_str()) +
                                        QString::fromLocal8Bit("登录成功! 可以访问管理部门系统\n")
                              );
                              _currentAdmin = _userID;                           //更新当前的管理员信息
                              _AdministerLogin->setDisabled(true);         //禁用登录管理部门系统
                              _adminUI->setEnabled(true);                       //登陆成功允许访问管理UI界面
                              _closeVideo->setEnabled(true);                    //允许使用管理员功能
                              _AdministerLogin->update();                       //禁用
                              _adminUI->update();                                     //禁用
                              _closeVideo->update();                                 //禁用
                    }
                    else {
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +QString::fromLocal8Bit("]:") + 
                                        QString::fromLocal8Bit("管理员") + QString::fromLocal8Bit(_userName.c_str()) +
                                        QString::fromLocal8Bit("人脸识别失败!请重新尝试\n")
                              );
                              _adminUI->setDisabled(true);                       //登陆成功允许访问管理UI界面
                              _closeVideo->setDisabled(true);                    //允许使用管理员功能
                              _adminUI->update();                                     //禁用
                              _closeVideo->update();                                 //禁用
                    }
          }
          catch (const InvalidInput&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("当前输入中含有非法字符，请重新输入\n")
                    );
          }
          catch (const EmptyMatrixString&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("系统中没有查询到当前管理员信息! \n")
                    );
          }
}

/*---------------------QTWidget层WorkAttendenceAdmin的操作接口----------------------*/
/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层管理员账户提供的记录查询接口
 * @name: QTAdminStatisticsInterface
 * @function：管理员账户提供的记录查询接口
 * @param:
 *                  1. 用户ID的输入  const std::string& _userID
 *                  2. 用户姓名的输入    const std::string& _userName
 *                  3. 部门的输入         const std::string & _department
 *                  4. 签到按钮: QRadioButton *& _attdenceTable
 *                  5. 签退按钮: QRadioButton*& _signoutTable
 *                  6. 是否选择时间: QCheckBox *& _isTimeEnabled
 *                  7. 左部输入时钟 :   const QDateTime _lefttimer
 *                  8. 右部输入时钟： const QDateTime _righttimer
 *                  9.全局时钟系统的输入 QDateTime*& _timer
 *                  10. 控制Table表格类的封装数据类型 DataDisplay   DataDisplay* m_dataDisplay
 *                  11. 输出窗口接口：QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTAdminStatisticsInterface(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QRadioButton*& _attdenceTable,
          QRadioButton*& _signoutTable,
          QCheckBox*& _isTimeEnabled,
          const QDateTime _lefttimer,
          const QDateTime _righttimer,
          QDateTime*& _timer,
          DataDisplay* _dataDisplay,
          QTextBrowser*& _systemOutput
)
{
          try
          {
                    if (_attdenceTable->isChecked() && _signoutTable->isChecked()) {      //检查单选按钮是否都被勾选
                              throw ThrowInvalidQRadio();
                    }
                    if (!_attdenceTable->isChecked() && !_signoutTable->isChecked()) {      //检查单选按钮是否都没被勾选
                              throw ThrowInvalidQRadio();
                    }
                    if (_isTimeEnabled->isChecked()) {                                                         //是否选择时间
                              if (_lefttimer > _righttimer) {                                                          //左边的日期大于右边的日期
                                        throw   InvalidDateTime();
                              }
                    }
                    //读取记录二元组
                    std::vector<std::vector<std::string>> recordRes = this->readAttendenceRecord(
                              _userID, _userName, _department, _lefttimer, _righttimer,
                              _attdenceTable->isChecked() ? AttendanceTable::ATTENDANCE : AttendanceTable::SIGNOUT,
                              _isTimeEnabled->isChecked()
                    );

                    /*输出给圆饼图的数据*/
                    double _total(0.0f);                                                                                                        //总人数                                                                                  
                    double _late(0.0f);                                                                                                          //迟到上班 / 早退下班人数
                    double _normal (0.0f);                                                                                              // 正常人数

                    this->QTAdminAttendenceRateCalc(                                                                           //计算饼图中的数据
                              _userID, _userName, _department, _lefttimer, _righttimer, _timer,
                              _attdenceTable->isChecked() ? AttendanceTable::ATTENDANCE : AttendanceTable::SIGNOUT,
                              _isTimeEnabled->isChecked(),
                              _total, _late, _normal
                    );
                   this->queryUserNameFromID(recordRes);                                                                             //在二元组集合中根据UserID查询用户姓名
                   _dataDisplay->setAttendenceTableTuple(recordRes);                                                               //加入元组集合显示
                   _dataDisplay->setChartViewData(_attdenceTable->isChecked(), _total, _late, _normal);  //显示当前状态下的饼图
          }
          catch (const ThrowInvalidQRadio&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("当前单选框选择非法，请重新操作\n")
                    );
          }
          catch (const InvalidDateTime&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("当前时间范围输入非法，请重新操作\n")
                    );
          }
          catch (const EmptyMatrixString&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("系统中没有查询到当前用户信息! \n")
                    );
          }
}

/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层管理员账户显示新员工清单
 * @name: QTGetNewEmployeeInterface
 * @param :  控制Table表格类的封装数据类型 DataDisplay   DataDisplay* m_dataDisplay
 * @function：管理员账户提供的记录查询接口
*------------------------------------------------------------------------------------------------------*/
void Interface::QTGetNewEmployeeInterface(DataDisplay* _dataDisplay)
{
          std::vector<std::vector<std::string>> recordRes = this->readNewEmployeeRecord();
          _dataDisplay->setNewEmployeeTableTuple(recordRes);                                                               //加入元组集合显示
}

/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层管理员账户显示新员工清单
 * @name: QTGetNewEmployeeInterface
 * @param :      1.是否通过 bool status
 *                       2.表格视图：QTableView *& _newEmployeeTable
 *                       3.全局时钟系统的输入 QDateTime*& _timer
 *                       4.输出窗口接口：QTextBrowser*& _systemOutput
 *
 * @function：管理员账户提供的记录查询接口
*------------------------------------------------------------------------------------------------------*/
void Interface::QTDeniedAndApprove(
          bool status, 
          QTableView*& _newEmployeeTable,
          QDateTime*& _timer,
          QTextBrowser*& _systemOutput
)
{
          QList<QModelIndex> list = _newEmployeeTable->selectionModel()->selectedIndexes();
          if (list.isEmpty()) {                                                                                                                       //没有选中任何信息
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                               QString::fromLocal8Bit("没有选择需要审批的员工! \n")
                    );
                    return;
          }
          for (auto i : list) {                                                                                                                       //遍历选中行
                    auto UserID = _newEmployeeTable->model()->index(i.row(), 0).data();
                    auto UserName = _newEmployeeTable->model()->index(i.row(), 1).data();
                    this->updateNewEmployeeAuth(
                              UserID.toString().toLocal8Bit().constData(), 
                              UserName.toString().toLocal8Bit().constData(), 
                              status
                    );
          }
}

/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层管理员账户提供的出勤率计算系统
 * @name: QTAdminAttendenceRateCalc
 * @function：管理员账户提供的出勤率计算系统
 * @param: 1. 员工号： const  std::string& employeeNumber
*                  2. 姓名： const  std::string& _name
*                  3. 部门 ：  const std::string& _department
*                  4. 左部时间： QDateTime _leftTimer
*                  5. 右部时间： QDateTime _rightTimer
*                  6.全局时钟系统的输入 QDateTime*& _timer
*                  7. 选择签到签退记录：AttendanceTable tableSelect
*                  8. 是否选择时间: bool _isTimeEnabled
 *                 9. 总人数：double &_total
 *                 10. 迟到上班/早退下班人数：double &_late
 *                 11. 正常人数：double& normal
*------------------------------------------------------------------------------------------------------*/
void Interface::QTAdminAttendenceRateCalc(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QDateTime _leftTimer,
          QDateTime _rightTimer,
          QDateTime*& _timer,
          AttendanceTable tableSelect,
          bool _isTimeEnabled,
          double& _total, double& _late, double& normal
)
{
          if (!_isTimeEnabled) 
          {
                     _leftTimer = getMinimiseTime(tableSelect);               //最早的时间
                     _rightTimer = _timer->currentDateTime();                   //当前最新时间
          }
          auto daysCount = _leftTimer.daysTo(_rightTimer);                //计算时间差(计算天数)当前最新时间 - 最早的时间乘以员工总数
          _total = static_cast<double>(
                    (!daysCount ? 1.0f : daysCount) * this->getEmployeeCount(_userID, _userName, _department)  
           );

          _late = static_cast<double>(this->adminStatisticsOnDutyCount(
                    _userID, _userName, _department,
                    tableSelect, _leftTimer, _rightTimer,
                    tableSelect == AttendanceTable::ATTENDANCE ? "员工迟到" : "员工早退"
          ));
          normal = static_cast<double>(this->adminStatisticsOnDutyCount(
                    _userID, _userName, _department,
                    tableSelect, _leftTimer, _rightTimer,
                    tableSelect == AttendanceTable::ATTENDANCE ? "正常签到" : "正常签退"
          ));
}

/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层管理员账户提供的设置系统参数
 * @name: QTAdminParamSettingInterface
 * @function：管理员账户提供的记录查询接口
 * @param:
 *                  1.训练集数设定：const std::string& _TranningSet,
 *                  2.训练相似度：const std::string& _TranningSimilarity,
 *                  3.迟到时间：const std::string& _LateTimeSet,
 *                  4.早退时间：const std::string& _LeaveEarilyTimeSet
 *                  5.已经登陆的ADMIN ID：const std::string & _loggedUserID
 *                  6.全局时钟系统的输入 QDateTime*& _timer
 *                  7. 输出窗口接口：QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTAdminParamSettingInterface(
          const std::string& _TranningSet,
          const std::string& _TranningSimilarity,
          const std::string& _LateTimeSet,
          const std::string& _LeaveEarilyTimeSet,
          const std::string& _loggedUserID,
          QDateTime*& _timer,
          QTextBrowser*& _systemOutput
)
{
          if (this->updateAdminParam2DB(_TranningSet, _TranningSimilarity, _LateTimeSet, _LeaveEarilyTimeSet, _loggedUserID))
          {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("全局系统参数设定设置成功! \n")
                    );
          }
          else
          {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("全局系统参数设定设置失败! \n")
                    );
          }
}