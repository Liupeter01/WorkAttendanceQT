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
cv::Mat &TakePicture::getImageFrame(std::mutex& _writeMutex)
{
          std::lock_guard<std::mutex> m_lock(_writeMutex);
          m_videoCapture >> m_imageFrameStore;
          return m_imageFrameStore;
}

/*
 * ������ʶ���ƥ��������ϻ������ͼ��
 * @name: drawGeometryOnImage
 * @param  1.д����������: std::mutex&_writeMutex
 *                 2.����λ��  dlib::rectangle& m_facePos
 *                 3. ʶ��������ʾ  std::string m_targetName = ""
*/
void TakePicture::drawGeometryOnImage(std::mutex& _writeMutex, dlib::rectangle& m_facePos, std::string m_targetName ) {
          cv::Rect face(
                    cv::Point(m_facePos.left(), m_facePos.top()),
                    cv::Point(m_facePos.right(), m_facePos.bottom())
          );
          std::unique_lock<std::mutex> _lckg(_writeMutex);                                                                                                                                                                            //д��ͼ��ʱΪͼ�����
          cv::putText(this->m_imageFrameStore, m_targetName, cv::Point(face.x, face.y + face.height + 20), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 255), 1, 8);
          cv::rectangle(this->m_imageFrameStore, cv::Point(face.x, face.y), cv::Point(face.x + face.width, face.y + face.height), cv::Scalar(0, 255, 0), 2, 8);                   //���Ͻ����½�
}