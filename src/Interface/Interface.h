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
           * @param 1.数据显示数据类型 QLCDNumber*& _qlcd
           *                2.全局时钟系统的输入 QTime*& _timer
           * 
           *  @Correction: 2022-8-2 引入全局的时钟系统
          *-----------------------------------------------------------------------------------------------------*/
          void QTsetLcdTimer(QLCDNumber*& _qlcd, QTime*& _timer);

          /*-------------------------------QTWidget层对外的操作接口----------------------------------*/
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
          *                   4.进度数值显示器 : int _displayNumber
          *
          *@Correction : 2022 - 7 - 24 添加函数参数修复防止线程无法正确的停止运转
          *-----------------------------------------------------------------------------------------------------*/
          void QTtranningSetInput(
                    std::atomic<bool>& _videoFlag,
                    QTextBrowser*& _systemOutput,
                    QProgressBar*& _processBar,
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
           *                  3. 输出窗口接口：QTextBrowser*& _systemOutput
           * 
          *------------------------------------------------------------------------------------------------------*/
          void QTResnetTranning(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
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
           *                  4.全局时钟系统的输入 QTime*& _timer
           *                  5. 输出窗口接口：QTextBrowser*& _systemOutput
          *------------------------------------------------------------------------------------------------------*/
          void QTEmployeeAttendance(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QTime *& _timer,
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
           *                  4.全局时钟系统的输入 QTime*& _timer
           *                  5. 输出窗口接口：QTextBrowser*& _systemOutput
          *------------------------------------------------------------------------------------------------------*/
          void QTEmployeeSignOut(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QTime*& _timer,
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
           *                  3. 输出窗口接口：QTextBrowser*& _systemOutput
          *------------------------------------------------------------------------------------------------------*/
          void QTEmployeeAskPremit(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QTextBrowser*& _systemOutput
          );

protected:
          std::atomic<bool> m_videoFlag = false;                      //视频开关
};