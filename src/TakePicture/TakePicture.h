#include"../HeaderFile/Common.h"

class TakePicture{
public:
          TakePicture(int videoType = 0);
          virtual ~TakePicture();
          /*
           * 摄像头单帧获取函数
           * @name:getImageFrame
           * @param 相机互斥量: std::mutex&_cameraMutex  
          */
          cv::Mat& getImageFrame(std::mutex& _cameraMutex);

          /*
           * 经过处理的人脸显示函数
           * @name: displayImage
           * @param imshow函数的互斥量: std::mutex&_cameraMutex
          */
          void displayImage(std::mutex &_imshowMutex) =  delete;

private:
          cv::Mat m_image;                                    //当前的视频渲染帧
          cv::VideoCapture m_videoCapture;         //摄像头类
};