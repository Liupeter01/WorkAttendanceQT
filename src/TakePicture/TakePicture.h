#include"../HeaderFile/Common.h"

class TakePicture{
public:
          TakePicture(int videoType = 0);
          virtual ~TakePicture();
          /*
           * ����ͷ��֡��ȡ����
           * @name:getImageFrame
           * @param ��Ա����д����������: std::mutex&_writeMutex
          */
          cv::Mat& getImageFrame(std::mutex& _writeMutex);

          /*
           * ���������������ʾ����
           * @name: displayImage
           * @param ��Ա����д����������: std::mutex&_writeMutex
          */
          void displayImage(std::mutex & _writeMutex) =  delete;

private:
          cv::Mat m_image;                                    //��ǰ����Ƶ��Ⱦ֡
          cv::VideoCapture m_videoCapture;         //����ͷ��
};