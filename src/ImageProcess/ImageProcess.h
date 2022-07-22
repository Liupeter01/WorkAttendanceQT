#include"../HeaderFile/Common.h"
#include"../FaceDetection/FaceDetection.h"
#include"../TakePicture/TakePicture.h"
#include"../ModelTrain/ModelTrain.h"

#include<QtGui/qimage.h>
#include<QtWidgets/qlabel.h>

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
           * 设置数据已经被线程模块同步
           * @name:setDataSyncSignal;
           * @function: 用来指示其他线程有关于数据更新的状态
          */
          void setDataSyncSignal();

          /*
           * 设置数据当前的输入数据模块数据过时
           * @name: resetDataSyncSignal;
           * @function: 用来指示其他线程有关于数据更新的状态
          */
          void resetDataSyncSignal();

          /*
           * 获取当前数据是否为最新状态
           * @name: resetDataSyncSignal;
           * @function: 用来指示其他线程有关于数据更新的状态
           * @retValue: 1.true : 当前数据的状态为旧状态
           *                    2.false: 当前数据的状态为新状态
          */
          bool getDataSyncStatus();

          /*
           * 暂停屏幕显示线程的工作，等待用户对于图像的确认函数
           * @name: getUserConfirmStatus
           * @function: 当训练线程启动后，需要等待用户完成图像确认之后才可以继续显示
           * @retValue: 1.true : 允许继续的下一帧的显示
           *                    2.false: 不允许下一帧的显示
          */
          bool getUserConfirmStatus();

protected:
          /*
           * 实时摄像头图像+人脸检测+人脸识别输出显示接口
           * @name: realTimeFacialDisplay
           * @function：其他的功能调用通过与该程序连接的线程进行操作
           * @param  1. 图像修改读写锁  std::mutex& _writeMutex
           *                  2. 输入原始图像   cv::Mat& mat
           *
           * @retValue: QImage &
          */
          QImage& realTimeFacialDisplay();

          /*
           * 通过摄像头图像进行实时人脸训练功能模块
           * @name: realTimeFacialDisplay
           * @function：和实时摄像头图像协同进行人脸训练功能
          */
          void videoSyncFacialTranning();

private:
          /*通用数据结构*/
          QImage m_qimageFrameStore;                        //类内部引入QImage存储结构
          std::vector<std::thread> m_threadPool;            //线程池(包含初始化等等操作)

          std::mutex m_imageRWLock;                           //图像更新写入锁
          std::mutex m_faceRectRWLock;                      //人脸位置更新写入锁
          std::pair<cv::Mat, std::atomic<bool>> m_imageSync;             //图像更新+图像更新的原子状态
          std::pair<dlib::rectangle, std::atomic<bool>> m_faceRectSync;  //人脸位置更新+人脸位置更新的原子状态

          /*画面显示线程*/
          std::mutex m_CofirmUserInptutEvent;                        //等待用户在界面上的摁键操作事件
          std::condition_variable m_UserInput;                         //等待用户在界面上的摁键操作条件变量

          /*人脸训练线程*/

          /*人脸识别线程*/
};