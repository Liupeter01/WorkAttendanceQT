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
          this->cleanLandmark();
          this->cleanRectangle();
}

/*
 * ����ר��ͼ��ĻҶ�ͼ(�����㷨����)
 * @name: ConvertBGR2GRAY
 * @function����ͼ��ת��Ϊdlib�ĻҶ�ͼ���������������ٶ�
 * @param ����ͼ��fuben  cv::Mat  _origin
*/
inline dlib::cv_image<unsigned char> FaceDetecion::ConvertBGR2GRAY(cv::Mat _origin)
{
          cv::cvtColor(_origin, _origin, cv::COLOR_BGR2GRAY);                        //ת��Ϊ�Ҷ�ͼ�Ż�ʶ���ٶ�
          return dlib::cv_image<unsigned char>(_origin);
}

/*
 * ��FaceDetecion��ת��ΪRect
 * @name:   operator cv::Rect
*/
FaceDetecion::operator cv::Rect() 
{
          return cv::Rect(
                    cv::Point(this->m_facePos->left(), this->m_facePos->top()), 
                    cv::Point(this->m_facePos->right(), this->m_facePos->bottom())
          );
}

/*
*�ⲿ������ȡ�ڲ�������λ������
* @name: getFaceRectangle
* @param : ��ȡ�������ڵ�λ��dlib::rectangle*& rect
*/
void FaceDetecion::getFaceRectangle(dlib::rectangle*& rect)
{
          if (rect != nullptr) {
                    delete rect;
                    rect = nullptr;
          }
          rect = new dlib::rectangle(*this->m_facePos);
          this->cleanRectangle();
}

/*
* �ⲿ������ȡ�ڲ�����������������
* @name: ��ȡ���������ڵ�λ��dlib::full_object_detection & _landmark
*/
void FaceDetecion::getFaceLandmark(dlib::full_object_detection*& _landmark)
{
          if (_landmark != nullptr) {
                    delete _landmark;
                    _landmark = nullptr;
          }
          _landmark = new dlib::full_object_detection(*this->m_faceLandmark);
          this->cleanLandmark();
}

/*
* ������ͼ���л�ȡ����������������ڲ����ݽṹ
* @name: findRectFromImage
* @param ����ԭʼͼ��  cv::Mat & _origin
* @retValue: true = ʶ��һ������;false =ʶ�𵽶��ŷǷ�����
* @Correct: 2022-7-18 �޸�����߼�����
*/
bool FaceDetecion::findRectFromImage(cv::Mat& _origin)
{
          std::vector<dlib::rectangle> temp = (*this->m_faceDetector)(ConvertBGR2GRAY(_origin));
          if (temp.size() != 1) {
                    return false;
          }
          this->cleanRectangle();
          this->m_facePos = new dlib::rectangle(temp.at(0));
          return true;
}

/*
* ������ͼ���л�ȡ����ͼ�������������ڲ����ݽṹ
* @name: findLandmarkFromImage
* @cautions: �˺�������֮ǰ�Ͻ�ʹ��cleanRectangle()�ͷ��ڴ�
* @param ����ԭʼͼ��  cv::Mat & _origin
*/
void FaceDetecion::findLandmarkFromImage(cv::Mat& _origin)
{
          this->cleanLandmark();
          this->m_faceLandmark = new dlib::full_object_detection(
                    (*this->m_faceFeaturePoint)(ConvertBGR2GRAY(_origin), *this->m_facePos)
          );
}

/*
* �����������(�����㷨����)
* @name:  cleanRectangle
*/
inline void FaceDetecion::cleanRectangle()
{
          if (this->m_facePos != nullptr) {
                    delete this->m_facePos;
                    this->m_facePos = nullptr;
          }
}

/*
* ���������(�����㷨����)
* @name: cleanGrayImage
*/
inline void FaceDetecion::cleanLandmark()
{
          if (this->m_faceLandmark != nullptr) {
                    delete this->m_faceLandmark;
                    this->m_faceLandmark = nullptr;
          }
}