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
 * @name: setLabelImage
 * @function：将MAT类型转换为QT的QImage类型并覆盖到QLabel容器中
 * @param 1.输入右值类型的 QImage&
 *                2.输入显示图层的指针_qlabel
*-----------------------------------------------------------------------------------------------------*/
void Interface::setLabelImage(QImage& _qimage, QLabel*& _qlabel)
{
          _qimage = _qimage.scaled(_qlabel->width(), _qlabel->height());                 //适配控件分辨率
          _qlabel->setPixmap(QPixmap::fromImage(_qimage));
}

/*------------------------------------------------------------------------------------------------------
 *  QLCDNumber图层的时限显示函数
 * @name: setLabelImage
 * @function：将QTimer类型作为事件数据类型传入到容器QLCDNumber中
 * @param 1.数据显示数据类型 QLCDNumber*& _qlcd
*-----------------------------------------------------------------------------------------------------*/
void Interface::setLcdTimer(QLCDNumber*& _qlcd)
{
          _qlcd->setDigitCount(10);                                                //设置晶体管控件QLCDNumber能显示的位数
          _qlcd->setMode(QLCDNumber::Dec);                            //设置显示的模式为十进制
          _qlcd->setSegmentStyle(QLCDNumber::Flat);               //设置显示方式
          while(!this->m_videoFlag) 
          {
                    _qlcd->display(QTime::currentTime().toString("hh:mm:ss"));
          }
}

/*-------------------------------QTWidget层对外的操作接口----------------------------------*/
/*------------------------------------------------------------------------------------------------------
 *   Interface类为QTWidget层视频关闭函数
 * @name: QTVideoOutput
 * @function：本质是关闭视频中的循环参数
*-----------------------------------------------------------------------------------------------------*/
void  Interface::videoClose()
{
          m_videoFlag = true;
}

/*------------------------------------------------------------------------------------------------------
 *   Interface类为QTWidget层开启当前帧拍照操作
 * @name: CameraShooting
 * @function：本质是拍摄当前的单帧人脸
*-----------------------------------------------------------------------------------------------------*/
void  Interface::videoCameraShooting()
{
          this->enableCameraShooting();
}

/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层开启当前帧的保存操作
 * @name: videoFrameSavingProcess
 * @function：本质是保存当前的单帧人脸
*-----------------------------------------------------------------------------------------------------*/
void  Interface::videoFrameSavingProcess()
{
          this->enableSavingProcess();
}

/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层开启当前帧的舍弃操作
 * @name: videoFrameIgnoreProcess
 * @function：本质是舍弃当前的单帧人脸
*-----------------------------------------------------------------------------------------------------*/
void  Interface::videoFrameIgnoreProcess()
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
                    this->setLabelImage(this->startVideoDisplay(_systemOutput), _qlabel);                       //适配控件分辨率
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
          if (!this->storeFaceRecord2DB(_userID, _userName, _department, r_faceMatrix))   //将数据存储入数据库
          {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("的") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("人脸信息存储成功!\n")
                    );
          }
}


/*------------------------------------------------------------------------------------------------------
 *  Interface类为QTWidget层输入的人脸登录开关
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
void Interface::QTFacialRecognize(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QTextBrowser*& _systemOutput
)
{
          try{
                    std::string dbFaceMatrix = this->readFaceRecordFromDB(_userID, _userName, _department);   //输入UserID+UserName+Department得到人脸特征矩阵的字符串
                    if (dbFaceMatrix == "" || !dbFaceMatrix.size()) {                                                                             //返回的人脸矩阵字符串为空
                              throw EmptyMatrixString();                                                                                                    //MatrixString为空的异常
                    }
                    if (this->startFacialRecognize(dbFaceMatrix, _systemOutput)) {                                                   //人脸矩阵字符串非空数据库dbFaceMatrix
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("的") +
                                        QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("人脸登陆成功!\n")
                              );
                    }
                    else {
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit(_userName.c_str()) + 
                                        QString::fromLocal8Bit("查无此人，人脸登陆失败!  请联系系统管理员\n")
                              );
                    }
          }
          catch (const EmptyMatrixString &) {

          }
}