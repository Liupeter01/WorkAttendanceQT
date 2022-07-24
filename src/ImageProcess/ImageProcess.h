#include"../HeaderFile/Common.h"
#include"../FaceDetection/FaceDetection.h"
#include"../TakePicture/TakePicture.h"
#include"../ModelTrain/ModelTrain.h"

#include<QtGui/qimage.h>
#include<QtWidgets/qlabel.h>

/*
 * 控制ImageProcess层的拍照控制状态机
 * @name: enum class CameraSwitch
 * @function: 用于控制人脸训练函数是否需要进行单次拍照的控制
*/
enum class CameraSwitch
{
          NO_INPUT,                               //当前没有输入
          TAKE_PICTURE                      //拍照片
};

/*
 * 控制ImageProcess层的保存控制状态机
 * @name: enum class SavingSwitch
 * @function: 用于控制人脸训练图片是否需要保存
*/
enum class SavingSwitch
{
          DEFAULT,                               //当前没有操作
          SAVING_FACE,                      //保存人脸
          DELETE_FACE                      //删除人脸
};

class ImageProcess :public TakePicture, public FaceDetecion,public ModelTrain {
public:
          ImageProcess() = delete;                           //构造函数默认含参数
          ImageProcess(int TrainningSetting);
          virtual ~ImageProcess();
public:
          /*
           * cv::Mat ----> QImage
           * @name: mat2Qimage
           * @function：将MAT类型转换为QT的QImage类型
           * @param 输入原始图像  const cv::Mat& mat
           * @retValue : 返回位于本类中QImage&引用类型
           * @Correction: 2022-7-21 在类内部引入QImage临时存储结构，可以使用引用加速
           *                        2022-7-21 删除函数中多余的部分，仅仅保留CV8U_C3下的处理方式，并引入内联函数
          */
          inline QImage &mat2Qimage(const cv::Mat& mat);

protected:
          /*
           * 启动人脸的显示线程
           * @name:  startVideoDisplay
           * @function: 实时摄像头图像+人脸检测+人脸识别输出显示接口
           * @retValue: QImage &
          */
          QImage& startVideoDisplay();

          /*
           * 启动人脸训练程序
           * @name:  startVideoRegister
           * @function: 开启当前视频拍摄，启动人脸训练程序
           * @param:  视频开关 std::atomic<bool> &
           * @Correction: 2022-7-24 添加函数参数修复防止线程无法正确的停止运转
          */
          void startVideoRegister(std::atomic<bool>& _videoFlag);

          /*
           * 命令保存功能启动
           * @name:  enableSavingProcess
           * @function: 开启当前视频拍摄，用户根据相关图像进行保存
          */
          void enableSavingProcess();

          /*
           * 命令舍弃功能启动
           * @name:  enableSavingProcess
           * @function: 开启当前视频拍摄，用户根据相关图像进行舍弃
          */
          void enableIgnoreProcess();

          /*
           * 命令摄像头进行拍照
           * @name: enableCameraShooting
           * @function: 开启当前视频拍摄，拍着某一张图像
          */
          void enableCameraShooting();

private:
          /*
           * 初始化图形处理类的内存
           * @name: initImageProcess
          */
          bool initImageProcess();

          /*
           * 释放图形处理类的内存
           * @name:releaseImageProcess;
          */
          void releaseImageProcess();

          /*
           * 实时摄像头图像+人脸检测+人脸识别输出显示接口
           * @name: realTimeFacialDisplay
           * @function：其他的功能调用通过与该程序连接的线程进行操作
           * @retValue: QImage &
          */
          QImage& realTimeFacialDisplay();

          /*
           * 通过摄像头图像进行实时人脸训练功能模块
           * @name: realTimeFacialDisplay
           * @function：和实时摄像头图像协同进行人脸训练功能
           * @param:  视频开关 std::atomic<bool> &
           * @Correction: 2022-7-24 添加函数参数修复防止线程无法正确的停止运转
          */
          void videoSyncFacialTranning(std::atomic<bool> & _videoFlag);

private:
          /*
           * 设置人脸图像就绪信号
           * @name:setImageSyncSignal
           * @function: 用来指示其他线程有关于数据更新的状态
          */
          void setImageSyncSignal();

          /*
           * 获取当前的图片是否被同步
           * @name: getImageSyncStatus
           * @function: 用来指示其他线程有关于数据更新的状态
           * @retValue: 1.true : 当前数据的状态为旧状态
           *                    2.false: 当前数据的状态为新状态
          */
          bool getImageSyncStatus();

          /*
           * 重置人脸图像就绪信号
           * @name:resetImageSyncStatus
           * @function: 用来指示其他线程有关于数据更新的状态
          */
          void resetImageSyncStatus();

          /*
           * 设置人脸位置就绪信号
           * @name:setRectSyncSignal
           * @function: 用来指示其他线程有关于数据更新的状态
          */
          void setRectSyncSignal();

          /*
           * 获取当前人脸位置是否被同步
           * @name: getRectSyncStatus
           * @function: 用来指示其他线程有关于数据更新的状态
           * @retValue: 1.true : 当前数据的状态为旧状态
           *                    2.false: 当前数据的状态为新状态
          */
          bool getRectSyncStatus();

          /*
           * 重置人脸位置就绪信号
           * @name: resetRectSyncStatus;
           * @function: 用来指示其他线程有关于数据更新的状态
          */
          void resetRectSyncStatus();

          /*
           * 重置人脸图像和位置就绪信号
           * @name: resetDataSyncSignal;
           * @function: 用来指示其他线程有关于数据更新的状态
          */
          void resetDataSyncSignal();

          /*
           * 设置视频显示线程的暂停
           * @name: setvideoPause
           * @function: 暂停和继续视频的显示线程
          */
          void setvideoPause();

          /*
           * 设置视频显示线程的继续
           * @name: setvideoContinue
           * @function: 暂停和继续视频的显示线程
          */
          void setvideoContinue();

          /*
           * 视频的显示线程，用于控制暂停和继续
           * @name: isVideoPaused
           * @function: 暂停和继续视频的显示线程
           * @retValue: 1.true : 启动视频的显示
           *                    2.false: 暂停视频的显示
          */
          bool isVideoPaused();

          /*
           * 其他线程判断摄像头是否启动拍照
           * @name: getCameraState
           * @function: 判断摄像头拍照
           * @RetValue:1.true : 允许拍照
           *                    2.false: 不允许拍照
          */
          bool getCameraState();

          /*
           * 其他线程判断保存功能是否启动
           * @name: getImageSavingState
           * @function: 判断摄像头拍照
           * @RetValue:1.true : 得到用户的确认(无论保存或舍弃)
           *                    2.false: 没有得到用户的确认
          */
          bool getImageSavingState();

private:
          /*--------------------------------------------------通用数据结构------------------------------------------------*/
          QImage m_qimageFrameStore;                        //类内部引入QImage存储结构
          std::vector<std::thread> m_threadPool;            //线程池(包含初始化等等操作)
          /*--------------------------------------------------------------------------------------------------------------------*/

          /*--------------------------------------------------画面显示线程------------------------------------------------*/
          std::pair<cv::Mat, std::mutex> m_imageSync;                 //图像更新+图像更新的互斥量mutex
          std::pair<dlib::rectangle, std::mutex> m_faceRectSync;  //人脸位置更新+人脸位置更新的互斥量mutex
       
          std::atomic<bool>  m_imageReadySign;                        //设置人脸就绪信号
          std::atomic<bool> m_faceRectReadySign;                      //设置人脸位置就绪信号

          std::condition_variable m_videoDisplayCtrl;                           //暂停视频线程的条件变量
          /*--------------------------------------------------------------------------------------------------------------------*/

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
          /*--------------------------------------------------------------------------------------------------------------------*/

          /*--------------------------------------------------人脸识别线程------------------------------------------------*/

           /*--------------------------------------------------------------------------------------------------------------------*/
};