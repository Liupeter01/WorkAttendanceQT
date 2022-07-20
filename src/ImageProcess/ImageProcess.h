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
          */
          static QImage mat2Qimage(const cv::Mat& mat);

protected:
          /*
           * 在默认没有摁键操作的情况下，默认检测人脸
           * @name: detectFaceScaleOnly
           * @function：用户在没有操作的情况下默认显示的界面
           * @param  1. 图像修改读写锁  std::mutex& _writeMutex
           *                  2. 输入原始图像   cv::Mat& mat
          */
          void detectFaceScaleOnly(std::mutex& _writeMutex, cv::Mat& image);

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
          //std::mutex m_cameraMutex;               //摄像机获取互斥量
          std::vector<std::thread> m_threadPool;
};