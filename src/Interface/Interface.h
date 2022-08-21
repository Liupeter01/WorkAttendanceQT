#include"../DataDisplay/TableViewDisplay.h"
#include"../ImageProcess/ImageProcess.h"
#include"../DBProcess/DBProcess.h"

/*------------------------------------------------------------------------------------------------------
 * 空的MatrixString的容器
 * @name:class EmptyMatrixString
* @function：String的容器是空的
*------------------------------------------------------------------------------------------------------*/
class EmptyMatrixString {
public:
          /*------------------------------------------------------------------------------------------------------
          * 返回当前的String的容器为空
          * @name: what
          * @RetValue:  std::string返回字符串
          *------------------------------------------------------------------------------------------------------*/
          std::string what() {
                    return std::string("当前的String的容器为空");
          }
};

/*------------------------------------------------------------------------------------------------------
 * 无效的登录信息输入
 * @name:class InvalidInput
* @function：String的容器是空的
*------------------------------------------------------------------------------------------------------*/
class InvalidInput {
public:
          /*------------------------------------------------------------------------------------------------------
          * 无效的登录信息输入
          * @name: what
          * @RetValue:  std::string返回字符串
          *------------------------------------------------------------------------------------------------------*/
          std::string what() {
                    return std::string("当前的String的容器无输入");
          }
};

/*------------------------------------------------------------------------------------------------------
 * 无效的QRadio输入或者多重QRadio的输入
 * @name:class ThrowInvalidQRadio
* @function：无效的QRadio或多重QRadio输入
*------------------------------------------------------------------------------------------------------*/
class ThrowInvalidQRadio{
public:
          /*------------------------------------------------------------------------------------------------------
          * 无效的QRadio或多重QRadio输入
          * @name: what
          * @RetValue:  std::string返回字符串
          *------------------------------------------------------------------------------------------------------*/
          std::string what() {
                    return std::string("无效的QRadio或多重QRadio输入");
          }
};

/*------------------------------------------------------------------------------------------------------
 * 输入的DateTime无效
 * @name:class InvalidDateTime 
* @function：左边的日期大于右边的日期
*------------------------------------------------------------------------------------------------------*/
class InvalidDateTime {
public:
          /*------------------------------------------------------------------------------------------------------
          * 无效的QRadio或多重QRadio输入
          * @name: what
          * @RetValue:  std::string返回字符串
          *------------------------------------------------------------------------------------------------------*/
          std::string what() {
                    return std::string("左边的日期大于右边的日期");
          }
};

class Interface :public DBProcess, public ImageProcess {
public:
          Interface();
          virtual ~Interface();

public:
          /*-------------------------------QTWidget层对外的封装接口----------------------------------*/
          /*------------------------------------------------------------------------------------------------------
           *  QLabel图层覆写函数
           * @name: QTsetLabelImage
           * @function：将MAT类型转换为QT的QImage类型并覆盖到QLabel容器中
           * @param 1.输入右值类型的 QImage&
           *                2.输入显示图层的指针_qlabel
          *-----------------------------------------------------------------------------------------------------*/
          void QTsetLabelImage(QImage& _qimage, QLabel*& _qlabel);

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
          void QTsetLcdTimer(
                    QLCDNumber*& _date,
                    QLCDNumber*& _realTimeClock,
                    QDateTime*& _timer
          );

          /*-----------------------QTWidget层WorkAttendenceSys的操作接口------------------------*/
          /*------------------------------------------------------------------------------------------------------
           *  Interface类为QTWidget层提供的视频关闭函数
           * @name: QTcloseVideo
           * @function：本质是关闭视频中的循环参数
          *-----------------------------------------------------------------------------------------------------*/
          void  QTcloseVideo(); 

          /*------------------------------------------------------------------------------------------------------
           *   Interface类为QTWidget层开启当前帧拍照操作
           * @name: QTsavePicture
           * @function：本质是拍摄当前的单帧人脸
          *-----------------------------------------------------------------------------------------------------*/
          void QTtakePicture();

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
          void QTtranningSetInput(
                    std::atomic<bool>& _videoFlag,
                    QTextBrowser*& _systemOutput,
                    QProgressBar*& _processBar,
                    QPushButton*& _pushButton,
                    int _displayNumber
          );

          /*------------------------------------------------------------------------------------------------------
           *   Interface类为QTWidget层开启当前帧的保存操作
           * @name: videoFrameSavingProcess
           * @function：本质是保存当前的单帧人脸
          *-----------------------------------------------------------------------------------------------------*/
          void QTsaveImage();

          /*------------------------------------------------------------------------------------------------------
           *   Interface类为QTWidget层开启当前帧的舍弃操作
           * @name: videoFrameIgnoreProcess
           * @function：本质是舍弃当前的单帧人脸
          *-----------------------------------------------------------------------------------------------------*/
          void QTignoreImage();

          /*------------------------------------------------------------------------------------------------------
           *   Interface类为QTWidget层视频连续输出函数
           * @name: QTVideoOutput
           * @function：连续更新显示图层所显示的图像
           * @param: 1. 视频输出pixmap : QLabel*& _qlabel
           *                 2. 输出窗口接口：QTextBrowser*& _systemOutput
          *-----------------------------------------------------------------------------------------------------*/
          void QTVideoOutput(QLabel*& _qlabel, QTextBrowser*& _systemOutput);

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
          void QTResnetTranning(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QPushButton*& signin,
                    QTextBrowser*& _systemOutput
          );

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
          void QTEmployeeAttendance(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QDateTime*& _timer,
                    QTextBrowser*& _systemOutput
          );

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
          void QTEmployeeSignOut(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QDateTime*& _timer,
                    QTextBrowser*& _systemOutput
          );

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
          void QTEmployeeAskPremit(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QDateTime*& _timer,
                    QTextBrowser*& _systemOutput
          );

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
          void QTEmployeeCheckPremittion(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QPushButton *&_TrainningSetInput,
                    QDateTime*& _timer,
                    QTextBrowser*& _privliedgeStatus,
                    QTextBrowser*& _systemOutput
          );

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
          void QTAdminManagementLogin(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    std::string& _currentAdmin,
                    QPushButton*& _AdministerLogin,
                    QPushButton*& _adminUI,
                    QPushButton*& _closeVideo,
                    QDateTime*& _timer,
                    QTextBrowser*& _systemOutput
          );

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
          void QTAdminStatisticsInterface(
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
          );

          /*------------------------------------------------------------------------------------------------------
           *  Interface类为QTWidget层管理员账户显示新员工清单
           * @name: QTGetNewEmployeeInterface
           * @param :  控制Table表格类的封装数据类型 DataDisplay   DataDisplay* m_dataDisplay
           * @function：管理员账户提供的记录查询接口
          *------------------------------------------------------------------------------------------------------*/
          void QTGetNewEmployeeInterface(DataDisplay* _dataDisplay);

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
          void QTDeniedAndApprove(
                    bool status,
                    QTableView*& _newEmployeeTable,
                    QDateTime*& _timer,
                    QTextBrowser*& _systemOutput
          );

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
          void QTAdminAttendenceRateCalc(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QDateTime _leftTimer,
                    QDateTime _rightTimer,
                    QDateTime*& _timer,
                    AttendanceTable tableSelect,
                    bool _isTimeEnabled,
                    double& _total, double& _late, double& normal
          );

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
          void QTAdminParamSettingInterface(
                    const std::string& _TranningSet,
                    const std::string& _TranningSimilarity,
                    const std::string& _LateTimeSet,
                    const std::string& _LeaveEarilyTimeSet,
                    const std::string& _loggedUserID,
                    QDateTime*& _timer,
                    QTextBrowser*& _systemOutput
          );

protected:
          std::atomic<bool> m_videoFlag = false;                      //视频开关
};