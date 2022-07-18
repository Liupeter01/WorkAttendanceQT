#include"../HeaderFile/Common.h"

class TakePicture{
public:
          TakePicture(int videoType = 0);
          virtual ~TakePicture();
          /*
           * 摄像头单帧获取函数
           * @name:getImageFrame
           * @param 成员变量写入锁互斥量: std::mutex&_writeMutex
          */
          cv::Mat& getImageFrame(std::mutex& _writeMutex);

          /*
           * 经过处理的人脸显示函数
           * @name: displayImage
           * @param 成员变量写入锁互斥量: std::mutex&_writeMutex
          */
          void displayImage(std::mutex & _writeMutex) =  delete;

private:
          cv::Mat m_image;                                    //当前的视频渲染帧
          cv::VideoCapture m_videoCapture;         //摄像头类
};