#include"../HeaderFile/Common.h"

class TakePicture{
public:
          TakePicture(int videoType = 0);
          virtual ~TakePicture();
          /*
           * ����ͷ��֡��ȡ����
           * @name:getImageFrame
           * @param ���������: std::mutex&_cameraMutex  
          */
          cv::Mat& getImageFrame(std::mutex& _cameraMutex);

          /*
           * ���������������ʾ����
           * @name: displayImage
           * @param imshow�����Ļ�����: std::mutex&_cameraMutex
          */
          void displayImage(std::mutex &_imshowMutex) =  delete;

private:
          cv::Mat m_image;                                    //��ǰ����Ƶ��Ⱦ֡
          cv::VideoCapture m_videoCapture;         //����ͷ��
};