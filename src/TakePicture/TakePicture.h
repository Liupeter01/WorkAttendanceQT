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
           * 将经过识别和匹配的人脸上绘制相关图形
           * @name: drawGeometryOnImage
           * @param  1.写入锁互斥量: std::mutex&_writeMutex
           *                 2.人脸位置  dlib::rectangle& m_facePos 
           *                 3. 识别人名显示  std::string m_targetName = ""
          */
          void drawGeometryOnImage(
                    std::mutex& _writeMutex,
                    dlib::rectangle& m_facePos,
                    std::string m_targetName = "");

private:
          cv::Mat m_imageFrameStore;                 //当前视频渲染帧临时存储器
          cv::VideoCapture m_videoCapture;         //摄像头类
};