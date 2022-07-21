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
           * ������ʶ���ƥ��������ϻ������ͼ��
           * @name: drawGeometryOnImage
           * @param  1.д����������: std::mutex&_writeMutex
           *                 2.����λ��  dlib::rectangle& m_facePos 
           *                 3. ʶ��������ʾ  std::string m_targetName = ""
          */
          void drawGeometryOnImage(
                    std::mutex& _writeMutex,
                    dlib::rectangle& m_facePos,
                    std::string m_targetName = "");

private:
          cv::Mat m_imageFrameStore;                 //��ǰ��Ƶ��Ⱦ֡��ʱ�洢��
          cv::VideoCapture m_videoCapture;         //����ͷ��
};