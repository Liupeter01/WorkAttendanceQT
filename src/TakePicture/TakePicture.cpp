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
 * ����ͷ��֡��ȡ����
 * @name:getImageFrame
 * @param: 
 *     ���������: std::mutex&_cameraMutex
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
 * ���������������ʾ����
 * @name: displayImage
 * @param imshow�����Ļ�����: std::mutex&_cameraMutex
*/
//void TakePicture::displayImage(std::mutex& _imshowMutex) 