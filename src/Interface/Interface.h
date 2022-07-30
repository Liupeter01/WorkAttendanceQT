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

class Interface :public DBProcess, public ImageProcess {
public:
          Interface();
          virtual ~Interface();

public:
          /*-------------------------------QTWidget层对外的封装接口----------------------------------*/
          /*------------------------------------------------------------------------------------------------------
           *  QLabel图层覆写函数
           * @name: setLabelImage
           * @function：将MAT类型转换为QT的QImage类型并覆盖到QLabel容器中
           * @param 1.输入右值类型的 QImage&
           *                2.输入显示图层的指针_qlabel
          *-----------------------------------------------------------------------------------------------------*/
          void setLabelImage(QImage& _qimage, QLabel*& _qlabel);

          /*------------------------------------------------------------------------------------------------------
           *  QLCDNumber图层的时限显示函数
           * @name: setLabelImage
           * @function：将QTimer类型作为事件数据类型传入到容器QLCDNumber中
           * @param 1.数据显示数据类型 QLCDNumber*& _qlcd
          *-----------------------------------------------------------------------------------------------------*/
          void setLcdTimer(QLCDNumber*& _qlcd);

          /*-------------------------------QTWidget层对外的操作接口----------------------------------*/
          /*------------------------------------------------------------------------------------------------------
           *  QTWidget层视频关闭函数
           * @name: QTVideoOutput
           * @function：本质是关闭视频中的循环参数
          *-----------------------------------------------------------------------------------------------------*/
          void  videoClose(); 

          /*------------------------------------------------------------------------------------------------------
           *  QTWidget层开启当前帧拍照操作
           * @name: CameraShooting
           * @function：本质是拍摄当前的单帧人脸
          *-----------------------------------------------------------------------------------------------------*/
          void  videoCameraShooting();

          /*------------------------------------------------------------------------------------------------------
           *  QTWidget层开启当前帧的保存操作
           * @name: videoFrameSavingProcess
           * @function：本质是保存当前的单帧人脸
          *-----------------------------------------------------------------------------------------------------*/
          void  videoFrameSavingProcess();

          /*------------------------------------------------------------------------------------------------------
           *  QTWidget层开启当前帧的舍弃操作
           * @name: videoFrameIgnoreProcess
           * @function：本质是舍弃当前的单帧人脸
          *-----------------------------------------------------------------------------------------------------*/
          void  videoFrameIgnoreProcess();

          /*------------------------------------------------------------------------------------------------------
           *  QTWidget层视频连续输出函数
           * @name: QTVideoOutput
           * @function：连续更新显示图层所显示的图像
           * @param: 1. 视频输出pixmap : QLabel*& _qlabel
           *                 2. 输出窗口接口：QTextBrowser*& _systemOutput
          *-----------------------------------------------------------------------------------------------------*/
          void QTVideoOutput(QLabel*& _qlabel, QTextBrowser*& _systemOutput);

          /*------------------------------------------------------------------------------------------------------
           *  QTWidget层输入的训练集训练开关
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
           *  QTWidget层输入的人脸登录开关
           * @name: QTFacialRecognize
           * @function：从数据库中找到对应的人脸特征向量字符串并根据当前的实时人脸进行对比
           *                      如果满足条件则将信息输入到实时摄像头的显示层
           * 
           * @param:
           *                  1. 用户ID的输入  const std::string& _userID
           *                  2. 用户姓名的输入    const std::string& _userName
           *                  3. 部门的输入         const std::string & _department
           *                  3. 输出窗口接口：QTextBrowser*& _systemOutput
          *------------------------------------------------------------------------------------------------------*/
          void QTFacialRecognize(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QTextBrowser*& _systemOutput
          );

protected:
          std::atomic<bool> m_videoFlag = false;                      //视频开关
};