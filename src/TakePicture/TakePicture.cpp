#include"TakePicture.h"

TakePicture::TakePicture(int videoType)
          :m_videoCapture(videoType) {
          
}

TakePicture::~TakePicture() 
{
          try {
                    m_videoCapture.release();
          }
          catch (const cv::Exception& e) {
                    std::cout << e.what() << std::endl;
          }
}

/*
 * 摄像头单帧获取函数
 * @name:getImageFrame
 * @param: 
 *     相机互斥量: std::mutex&_cameraMutex
*/
cv::Mat &TakePicture::getImageFrame(std::mutex& _cameraMutex)
{
          {
                    std::lock_guard<std::mutex> m_lock(_cameraMutex);
                    m_videoCapture >> m_image;
          }
          return m_image;
}

/*
 * 经过处理的人脸显示函数
 * @name: displayImage
 * @param imshow函数的互斥量: std::mutex&_cameraMutex
*/
//void TakePicture::displayImage(std::mutex& _imshowMutex) 