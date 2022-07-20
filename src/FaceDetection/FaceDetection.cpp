#include"FaceDetection.h"

FaceDetecion::FaceDetecion()
          :m_faceFeaturePoint(new dlib::shape_predictor),
          m_landmarkLoader(new FrontFaceLoader),
          m_faceDetector(new dlib::frontal_face_detector)
{
}

FaceDetecion::~FaceDetecion() 
{
          this->releaseFaceDetection();
}

/*
* ���������ĳ�ʼ������
* @name: initFaceDetection
*/
bool  FaceDetecion::initFaceDetection()
{
          return m_landmarkLoader->getLoaderStatus(this->m_faceDetector, this->m_faceFeaturePoint);
}

/*
* ���������ĺ��Ĺ����ͷź���
* @name: initFaceDetection
*/
void FaceDetecion::releaseFaceDetection()
{
          delete m_faceFeaturePoint;
          delete m_landmarkLoader;
          delete m_faceDetector;
}

/*
 * ����ר��ͼ��ĻҶ�ͼ(�����㷨����)
 * @name: ConvertBGR2GRAY
 * @function����ͼ��ת��Ϊdlib�ĻҶ�ͼ���������������ٶ�
 * @param ����ͼ��fuben  cv::Mat  & _origin
*/
inline dlib::cv_image<unsigned char> FaceDetecion::ConvertBGR2GRAY(cv::Mat &_origin)
{
          cv::cvtColor(_origin, this->m_grayImage, cv::COLOR_BGR2GRAY);                        //ת��Ϊ�Ҷ�ͼ�Ż�ʶ���ٶ�
          return dlib::cv_image<unsigned char>(this->m_grayImage);
}

/*
* �ⲿ������ȡ�ڲ�������λ������
* @name: getFaceRectangle
* @param : ����ͼ���ԭʼͼcv::Mat& _origin
* @retValue�����ط�����ջdlib::rectangle
*/
dlib::rectangle FaceDetecion::getFaceRectangle(cv::Mat& _origin)
{
          std::vector<dlib::rectangle> temp = (*this->m_faceDetector)(ConvertBGR2GRAY(_origin));
          if (temp.size() != 1) {
                    return dlib::rectangle();
          }
          return  dlib::rectangle(temp.at(0));
}

/*
* �ⲿ������ȡ�ڲ�����������������
* @name:  getFaceLandmark
* @param: 1.����ͼ���ԭʼͼcv::Mat& _origin
*                 2. ��������ͼ���λ��dlib::rectangle& _rect
*
* @retValue : ���ط�����ջdlib::full_object_detection
*/
dlib::full_object_detection  FaceDetecion::getFaceLandmark(cv::Mat& _origin, dlib::rectangle& _rect)
{
          return dlib::full_object_detection((*this->m_faceFeaturePoint)(ConvertBGR2GRAY(_origin), _rect));
}
