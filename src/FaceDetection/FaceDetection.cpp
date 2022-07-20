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
* 人脸检测类的初始化函数
* @name: initFaceDetection
*/
bool  FaceDetecion::initFaceDetection()
{
          return m_landmarkLoader->getLoaderStatus(this->m_faceDetector, this->m_faceFeaturePoint);
}

/*
* 人脸检测类的核心功能释放函数
* @name: initFaceDetection
*/
void FaceDetecion::releaseFaceDetection()
{
          delete m_faceFeaturePoint;
          delete m_landmarkLoader;
          delete m_faceDetector;
}

/*
 * 生成专属图像的灰度图(用于算法加速)
 * @name: ConvertBGR2GRAY
 * @function：将图像转换为dlib的灰度图像提升人脸检测的速度
 * @param 输入图像fuben  cv::Mat  & _origin
*/
inline dlib::cv_image<unsigned char> FaceDetecion::ConvertBGR2GRAY(cv::Mat &_origin)
{
          cv::cvtColor(_origin, this->m_grayImage, cv::COLOR_BGR2GRAY);                        //转换为灰度图优化识别速度
          return dlib::cv_image<unsigned char>(this->m_grayImage);
}

/*
* 外部函数获取内部的人脸位置数据
* @name: getFaceRectangle
* @param : 输入图像的原始图cv::Mat& _origin
* @retValue：返回分配在栈dlib::rectangle
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
* 外部函数获取内部的人脸特征点数据
* @name:  getFaceLandmark
* @param: 1.输入图像的原始图cv::Mat& _origin
*                 2. 人脸所在图像的位置dlib::rectangle& _rect
*
* @retValue : 返回分配在栈dlib::full_object_detection
*/
dlib::full_object_detection  FaceDetecion::getFaceLandmark(cv::Mat& _origin, dlib::rectangle& _rect)
{
          return dlib::full_object_detection((*this->m_faceFeaturePoint)(ConvertBGR2GRAY(_origin), _rect));
}
