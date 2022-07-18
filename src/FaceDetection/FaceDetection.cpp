#include"FaceDetection.h"

FaceDetecion::FaceDetecion()
{
          
}

FaceDetecion::~FaceDetecion() 
{

}

/*
* ���������ĳ�ʼ������
* @name: initFaceDetection
*/
bool  FaceDetecion::initFaceDetection()
{
          return m_landmarkLoader.getLoaderStatus(this->m_faceDetector, this->m_faceFeaturePoint);
}

/*
 * ����ר��ͼ��ĻҶ�ͼ(�����㷨����)
 * @name: ConvertBGR2GRAY
 * @function����ͼ��ת��Ϊdlib�ĻҶ�ͼ���������������ٶ�
 * @param ����ԭʼͼ��  cv::Mat & _origin
*/
inline dlib::cv_image<unsigned char> FaceDetecion::ConvertBGR2GRAY(cv::Mat& _origin)
{
          cv::Mat backup;
          cv::cvtColor(_origin, backup, cv::COLOR_BGR2GRAY);                        //ת��Ϊ�Ҷ�ͼ�Ż�ʶ���ٶ�
          return dlib::cv_image<unsigned char>(backup);
}

/*
 * ��FaceDetecion��ת��ΪRect
 * @name:   operator cv::Rect
*/
FaceDetecion::operator cv::Rect() 
{
          return cv::Rect(cv::Point(this->m_facePos.left(), this->m_facePos.top()), cv::Point(this->m_facePos.right(), this->m_facePos.bottom()));
}

/*
* �ⲿ������ȡ�ڲ�������λ������
* @name: getFaceRectangle
*/
dlib::rectangle& FaceDetecion::getFaceRectangle()
{
          return this->m_facePos;
}

/*
* �ⲿ������ȡ�ڲ�����������������
* @name: getFaceLandmark
*/
dlib::full_object_detection& FaceDetecion::getFaceLandmark()
{
          return this->m_faceLandmark;
}

/*
* ������ͼ���л�ȡ����������������ڲ����ݽṹ
* @name: findRectFromImage
* @param ����ԭʼͼ��  cv::Mat & _origin
* @retValue: true = ʶ��һ������;false =ʶ�𵽶��ŷǷ�����
*/
bool FaceDetecion::findRectFromImage(cv::Mat& _origin)
{
          this->m_imageGray = ConvertBGR2GRAY(_origin);
          std::vector<dlib::rectangle> temp = this->m_faceDetector(this->m_imageGray);
          if (temp.size() > 1) {
                    return false;
          }
          this->m_facePos = temp.at(0);
          return true;
}

/*
* ������ͼ���л�ȡ����ͼ�������������ڲ����ݽṹ
* @name: findLandmarkFromImage
* @param ����ԭʼͼ��  cv::Mat & _origin
*/
void FaceDetecion::findLandmarkFromImage(cv::Mat& _origin)
{
          this->m_faceLandmark = this->m_faceFeaturePoint(this->m_imageGray, this->m_facePos);
}