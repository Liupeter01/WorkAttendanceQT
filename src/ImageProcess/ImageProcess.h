#include"../HeaderFile/Common.h"
#include"../FaceDetection/FaceDetection.h"
#include"../TakePicture/TakePicture.h"
#include"../ModelTrain/ModelTrain.h"

#include<QtGui/qimage.h>
#include<QtWidgets/qlabel.h>
#include<QtWidgets/qdatetimeedit.h>
#include<QtWidgets/qtextbrowser.h>
#include<QtWidgets/qlcdnumber.h>
#include<QtWidgets/qmessagebox.h>
#include<QtWidgets/qprogressbar.h>
#include<QtWidgets/qpushbutton.h>
#include<QtWidgets/qradiobutton.h>

/*------------------------------------------------------------------------------------------------------
 * 控制ImageProcess层的拍照控制状态机
 * @name: enum class CameraSwitch
 * @function: 用于控制人脸训练函数是否需要进行单次拍照的控制
*------------------------------------------------------------------------------------------------------*/
enum class CameraSwitch
{
          NO_INPUT,                               //当前没有输入
          TAKE_PICTURE                      //拍照片
};

/*------------------------------------------------------------------------------------------------------
 * 控制ImageProcess层的保存控制状态机
 * @name: enum class SavingSwitch
 * @function: 用于控制人脸训练图片是否需要保存
*------------------------------------------------------------------------------------------------------*/
enum class SavingSwitch
{
          DEFAULT,                               //当前没有操作
          SAVING_FACE,                      //保存人脸
          DELETE_FACE                      //删除人脸
};

class ImageProcess :public TakePicture, public FaceDetecion,public ModelTrain {
public:
          ImageProcess() = delete;                           //构造函数默认含参数
          ImageProcess(int _TrainningSetting, double _TrainningSimilarity);
          virtual ~ImageProcess();

protected:  
          /*------------------------------为外部函数提供的ExternalAPI--------------------------------*/
          /*------------------------------------------------------------------------------------------------------
           * 命令保存功能启动
           * @name:  enableSavingProcess
           * @function: 开启当前视频拍摄，用户根据相关图像进行保存
          *------------------------------------------------------------------------------------------------------*/
          void enableSavingProcess();

          /*------------------------------------------------------------------------------------------------------
           * 命令舍弃功能启动
           * @name:  enableSavingProcess
           * @function: 开启当前视频拍摄，用户根据相关图像进行舍弃
          *------------------------------------------------------------------------------------------------------*/
          void enableIgnoreProcess();

          /*------------------------------------------------------------------------------------------------------
           * 命令摄像头进行拍照
           * @name: enableCameraShooting
           * @function: 开启当前视频拍摄，拍着某一张图像
          *------------------------------------------------------------------------------------------------------*/
          void enableCameraShooting();

          /*------------------------------------------------------------------------------------------------------
           * 启动视频的显示线程(与外部GUI连接)
           * @name:  startVideoDisplayThread
           * @function: 实时摄像头图像+人脸检测+人脸识别输出显示接口
           * @param: 输出窗口接口：QTextBrowser*& _systemOutput
           * @retValue: QImage &
          *------------------------------------------------------------------------------------------------------*/
          QImage& startVideoDisplayThread(QTextBrowser*& _systemOutput);

          /*------------------------------------------------------------------------------------------------------
           * 启动人脸的训练集输入函数(与外部GUI连接)
           * @name:  startImageTranningSetInput
           * @function: 开启当前视频拍摄，启动人脸训练程序
           * @param:  1.视频开关 std::atomic<bool> &
           *                  2.输出窗口接口：QTextBrowser*& _systemOutput
           *                  3.进度条的输出接口：QProgressBar* &_processBar
           *                  4.进度数值显示器 : int _displayNumber
           *
           * @Correction: 2022-7-24 添加函数参数修复防止线程无法正确的停止运转
          *------------------------------------------------------------------------------------------------------*/
          void  startImageTranningSetInput(
                    std::atomic<bool>& _videoFlag,
                    QTextBrowser*& _systemOutput,
                    QProgressBar*& _processBar,
                    int _displayNumber
          );

          /*------------------------------------------------------------------------------------------------------
           * 启动人脸注册之后运行训练程序(与外部GUI连接)
           * @name:  startResnetModelTranning
           * @function: 启动人脸注册之后，运行训练程序
           * @param:  输出窗口接口：QTextBrowser*& _systemOutput
           *
           * @retValue: 返回训练好的特征向量字符串  std::string &
          *------------------------------------------------------------------------------------------------------*/
          std::string& startResnetModelTranning(QTextBrowser*& _systemOutput);

          /*------------------------------------------------------------------------------------------------------
           * 启动人脸登录之后运行人脸识别程序(与外部GUI连接)
           * @name:  startFacialRecognize
           * @function: 启动人脸登录之后，运行训练程序
           * @param:  1.人脸特征向量字符串接口： const std::string _dbMatrix
           *                  2.输出窗口接口：QTextBrowser*& _systemOutput
           *
           * @retValue: 返回识别是否成功  bool
          *------------------------------------------------------------------------------------------------------*/
          bool startFacialRecognize(const std::string _dbMatrix, QTextBrowser*& _systemOutput);

private:
          /*-------------------------------为内部函数提供的IxternalAPI------------------------------*/
          /*------------------------------------------------------------------------------------------------------
           * 实时摄像头图像+人脸检测+人脸识别输出显示接口
           * @name: VideoDisplayThread
           * @function：其他的功能调用通过与该程序连接的线程进行操作
           * @param : 输出窗口接口：QTextBrowser*& _systemOutput
           * @retValue: QImage &
          *------------------------------------------------------------------------------------------------------*/
          QImage& VideoDisplayThread(QTextBrowser*& _systemOutput);

          /*------------------------------------------------------------------------------------------------------
           * 启动人脸训练集图片的输入函数
           * @name:  ImageTranningSetInput
           * @function: 开启当前视频拍摄，启动人脸训练集图片的输入
           * @param:  1.视频开关 std::atomic<bool> &
           *                  2.输出窗口接口：QTextBrowser*& _systemOutput
           *                  3.进度条的输出接口：QProgressBar* &_processBar
           *                  4.进度数值显示器 : int _displayNumber
           *
           * @Correction: 2022-7-24 添加函数参数修复防止线程无法正确的停止运转
          *------------------------------------------------------------------------------------------------------*/
          void ImageTranningSetInput(
                    std::atomic<bool>& _videoFlag,
                    QTextBrowser*& _systemOutput,
                    QProgressBar*& _processBar,
                    int _displayNumber
          );

          /*------------------------------------------------------------------------------------------------------
           * 启动人脸注册运行训练程序
           * @name:  ResnetModelTranning
           * @function: 输入的训练集训练开关
           * @param:  输出窗口接口：QTextBrowser*& _systemOutput
           * 
           * @retValue：返回训练好的特征向量字符串  std::string &
          *------------------------------------------------------------------------------------------------------*/
          std::string& ResnetModelTranning(QTextBrowser*& _systemOutput);

          /*------------------------------------------------------------------------------------------------------
           * 启动人脸识别程序
           * @name:   FacialRecognize
           * @function:  启动人脸登录之后，运行训练程序
           * @param:  1.人脸特征向量字符串接口： const std::string _dbMatrix
           *                  2.输出窗口接口：QTextBrowser*& _systemOutput
           *
           * @retValue: 返回识别是否成功  bool 
          *------------------------------------------------------------------------------------------------------*/
          bool FacialRecognize(const std::string _dbMatrix, QTextBrowser*& _systemOutput);

private:
          /*-----------------------------ImageProcess的初始化的函数接口----------------------------*/
          /*-----------------------------------------------------------------------------------------------------
           * 初始化图形处理类的内存
           * @name: initImageProcess
          *------------------------------------------------------------------------------------------------------*/
          bool initImageProcess();

          /*-----------------------------------------------------------------------------------------------------
           * 释放图形处理类的内存
           * @name:releaseImageProcess;
          *------------------------------------------------------------------------------------------------------*/
          void releaseImageProcess();

private:
          /*------------------------ImageProcess的线程信号量通信函数接口-------------------------*/
          /*-----------------------------------------------------------------------------------------------------
           * 设置人脸图像就绪信号
           * @name:setImageSyncSignal
           * @function: 用来指示其他线程有关于数据更新的状态
          *------------------------------------------------------------------------------------------------------*/
          void setImageSyncSignal();

          /*-----------------------------------------------------------------------------------------------------
           * 获取当前的图片是否被同步
           * @name: getImageSyncStatus
           * @function: 用来指示其他线程有关于数据更新的状态
           * @retValue: 1.true : 当前数据的状态为旧状态
           *                    2.false: 当前数据的状态为新状态
          *------------------------------------------------------------------------------------------------------*/
          bool getImageSyncStatus();

          /*-----------------------------------------------------------------------------------------------------
           * 重置人脸图像就绪信号
           * @name:resetImageSyncStatus
           * @function: 用来指示其他线程有关于数据更新的状态
          *------------------------------------------------------------------------------------------------------*/
          void resetImageSyncStatus();

          /*-----------------------------------------------------------------------------------------------------
           * 设置人脸位置就绪信号
           * @name:setRectSyncSignal
           * @function: 用来指示其他线程有关于数据更新的状态
          *------------------------------------------------------------------------------------------------------*/
          void setRectSyncSignal();

          /*-----------------------------------------------------------------------------------------------------
           * 获取当前人脸位置是否被同步
           * @name: getRectSyncStatus
           * @function: 用来指示其他线程有关于数据更新的状态
           * @retValue: 1.true : 当前数据的状态为旧状态
           *                    2.false: 当前数据的状态为新状态
          *------------------------------------------------------------------------------------------------------*/
          bool getRectSyncStatus();

          /*-----------------------------------------------------------------------------------------------------
           * 重置人脸位置就绪信号
           * @name: resetRectSyncStatus;
           * @function: 用来指示其他线程有关于数据更新的状态
          *------------------------------------------------------------------------------------------------------*/
          void resetRectSyncStatus();

          /*-----------------------------------------------------------------------------------------------------
           * 重置人脸图像和位置就绪信号
           * @name: resetDataSyncSignal;
           * @function: 用来指示其他线程有关于数据更新的状态
          *------------------------------------------------------------------------------------------------------*/
          void resetDataSyncSignal();

          /*-----------------------------------------------------------------------------------------------------
           * 其他线程判断保存功能是否启动
           * @name: getImageSavingState
           * @function: 判断摄像头拍照
           * @RetValue:1.true : 得到用户的确认(无论保存或舍弃)
           *                    2.false: 没有得到用户的确认
          *------------------------------------------------------------------------------------------------------*/
          bool getImageSavingState();

          /*-----------------------------------------------------------------------------------------------------
           * 重置保存功能的就绪信号
           * @name: resetImageSavingState
           * @function: 用来指示其他线程有关于图像保存状态
          *------------------------------------------------------------------------------------------------------*/
          void resetImageSavingState();

          /*-----------------------------------------------------------------------------------------------------
           * 设置视频显示线程的暂停
           * @name: setvideoPause
           * @function: 暂停和继续视频的显示线程
          *------------------------------------------------------------------------------------------------------*/
          void setvideoPause();

          /*-----------------------------------------------------------------------------------------------------
           * 设置视频显示线程的继续
           * @name: setvideoContinue
           * @function: 暂停和继续视频的显示线程
          *------------------------------------------------------------------------------------------------------*/
          void setvideoContinue();

          /*-----------------------------------------------------------------------------------------------------
           * 视频的显示线程，用于控制暂停和继续
           * @name: isVideoPaused
           * @function: 暂停和继续视频的显示线程
           * @retValue: 1.true : 启动视频的显示
           *                    2.false: 暂停视频的显示
          *------------------------------------------------------------------------------------------------------*/
          bool isVideoPaused();

          /*-----------------------------------------------------------------------------------------------------
           * 其他线程判断摄像头是否启动拍照
           * @name: getCameraState
           * @function: 判断摄像头拍照
           * @RetValue:1.true : 允许拍照
           *                    2.false: 不允许拍照
           * 
           * @Correction: 2022-7-26 增加对人脸识别图形是否为空检测，否则无法开启拍照程序
          *------------------------------------------------------------------------------------------------------*/
          bool getCameraState();

          /*-----------------------------------------------------------------------------------------------------
           * 其他线程重置摄像头的状态
           * @name: resetCameraState
           * @function: 重置摄像头的拍照状态
          *------------------------------------------------------------------------------------------------------*/
          void resetCameraState();

protected:
          /*----------------------------------ImageProcess的通用工具-----------------------------------*/
          /*------------------------------------------------------------------------------------------------------
           * cv::Mat ----> QImage
           * @name: mat2Qimage
           * @function：将MAT类型转换为QT的QImage类型
           * @param 输入原始图像  const cv::Mat& mat
           * @retValue : 返回位于本类中QImage&引用类型
           * @Correction: 2022-7-21 在类内部引入QImage临时存储结构，可以使用引用加速
           *                        2022-7-21 删除函数中多余的部分，仅仅保留CV8U_C3下的处理方式，并引入内联函数
          *------------------------------------------------------------------------------------------------------*/
          inline QImage& mat2Qimage(const cv::Mat& mat);

          /*------------------------------------------------------------------------------------------------------
           * 将启动日志信息输出到SystemStatusInfo窗口中
           * @name:  printOnTextBroswer
           * @function: 将启动日志信息输出到SystemStatusInfo窗口中
           * @param:  1. 输出窗口接口：QTextBrowser*& _systemOutput
           *                  2. 输出的QString的字符串：const QString _qstring
           * 
           * @Correction: 2022-8-1 将宽字符改为QT专属的QString方便开发
          *------------------------------------------------------------------------------------------------------*/
          inline void printOnTextBroswer(QTextBrowser*& _systemOutput, const QString _qstring);

private:
          /*--------------------------------------------------通用数据结构------------------------------------------------*/
          QImage m_qimageFrameStore;                        //类内部引入QImage存储结构
          std::vector<std::thread> m_threadPool;            //线程池(包含初始化等等操作)
          int TrainningSetting;                                          //训练集的大小设定

          /*--------------------------------------------------画面显示线程------------------------------------------------*/
          std::pair<cv::Mat, std::mutex> m_imageSync;                 //图像更新+图像更新的互斥量mutex
          std::pair<dlib::rectangle, std::mutex> m_faceRectSync;  //人脸位置更新+人脸位置更新的互斥量mutex
       
          std::atomic<bool>  m_imageReadySign;                        //设置人脸就绪信号
          std::atomic<bool> m_faceRectReadySign;                      //设置人脸位置就绪信号

          std::condition_variable m_videoDisplayCtrl;                           //暂停视频线程的条件变量

          /*--------------------------------------------------人脸训练线程------------------------------------------------*/
          cv::Mat m_copiedImage;                                             //拷贝的人脸图像
          dlib::rectangle m_copiedfaceRect;                               //拷贝的人脸位置
          dlib::full_object_detection m_faceLandmark;             //人脸的特征点临时存储
          CameraSwitch m_camera;                                            //拷贝的拍照控制状态机
          SavingSwitch m_saving;                                               //拷贝的保存控制状态机

          std::mutex m_videoDisplaySync;                                      //视频暂停和继续的互斥量mutex
          std::atomic<bool> m_videoDisplaySign;                        //设置视频暂停和继续

          std::pair<CameraSwitch, std::mutex> m_cameraSwitchSync;       //拍照开关系统+拍照开关系统的原子状态
          std::pair<SavingSwitch, std::mutex> m_savingSwitchSync;         //保存开关系统+保存开关系统的原子状态

          std::condition_variable m_cameraCtrl;                                           //拍照开关条件变量
          std::condition_variable m_imageReady;                         //等待图像数据是否准备就绪条件变量
          std::condition_variable m_rectReady;                             //等待人脸数据是否准备就绪条件变量
          std::condition_variable m_savingCtrl;                                            //保存开关条件变量

          /*--------------------------------------------------人脸识别线程------------------------------------------------*/
          dlib::matrix<float, 0, 1> m_copidMatrix;                    //拷贝的128D特征人脸
          std::string m_copiedMatrixString;                               //拷贝的128D特征人脸的字符串
          std::string m_copiedNameString;                                 //拷贝的人脸识别的名字结果

          std::future<std::string&> m_threadMatrixRes;             //接收来自线程的人脸矩阵字符串运算结果
          std::future<bool>  m_threadStatusRes;                        //接收来自线程的识别人脸结果是否成功
};