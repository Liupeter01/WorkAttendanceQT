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

private:
          QImage m_qimageFrameStore;                        //类内部引入QImage存储结构
          std::mutex m_imageRWLock;                           //图像更新写入锁
          std::pair<cv::Mat, dlib::rectangle> m_sharedData;  //线程共享人脸及其位置
          std::vector<std::thread> m_threadPool;            //线程池(包含初始化等等操作)
};