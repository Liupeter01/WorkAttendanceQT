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

/*-------------------------------QTWidget层对外的操作接口----------------------------------*/
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
*                   4.进度数值显示器 : int _displayNumber
*
*@Correction : 2022 - 7 - 24 添加函数参数修复防止线程无法正确的停止运转
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTtranningSetInput(
          std::atomic<bool>& _videoFlag,
          QTextBrowser*& _systemOutput,
          QProgressBar*& _processBar,
          int _displayNumber
)
{
          this->startImageTranningSetInput(_videoFlag, _systemOutput, _processBar, _displayNumber);
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
 *                  3. 输出窗口接口：QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTResnetTranning(
          const std::string & _userID,
           const std::string & _userName,
          const std::string & _department,
          QTextBrowser * &_systemOutput
)
{
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
                    else
                    {
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
                                        this->storeAttendanceRecord2DB(                                                                                         //进行考勤时间的记录
                                                  _userID, _department,
                                                  ((_timer->currentDateTime().time() > this->getMorningShiftTime()) ? "员工迟到" : "正常签到"),
                                                  _timer
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
                    else
                    {
                              /*将人脸信息从数据库中先提取并验证是否存在*/
                              std::string dbFaceMatrix = this->readFaceRecordFromDB(_userID, _userName, _department);   //输入UserID+UserName+Department得到人脸特征矩阵的字符串
                              if (dbFaceMatrix == "" || !dbFaceMatrix.size()) {                                                                             //返回的人脸矩阵字符串为空
                                        throw EmptyMatrixString();                                                                                                    //MatrixString为空的异常
                              }
                              if (this->startFacialRecognize(dbFaceMatrix, _systemOutput)) {                                                   //将数据库dbFaceMatrix和实时人脸进行比对
                                        _systemOutput->insertPlainText(
                                                  QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss")  + QString::fromLocal8Bit("]:") +
                                                  QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("的") +
                                                  QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("人脸登陆成功!\n")
                                        );
                                        this->storeSignOutRecord2DB(                                                                                                //进行考勤时间的记录
                                                  _userID, _department,
                                                  ((_timer->currentDateTime().time() < this->getNightshiftTime()) ? "员工早退" : "正常签退"),
                                                  _timer
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
}