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
          this->cleanLandmark();
          this->cleanRectangle();
}

/*
 * 生成专属图像的灰度图(用于算法加速)
 * @name: ConvertBGR2GRAY
 * @function：将图像转换为dlib的灰度图像提升人脸检测的速度
 * @param 输入图像fuben  cv::Mat  _origin
*/
inline dlib::cv_image<unsigned char> FaceDetecion::ConvertBGR2GRAY(cv::Mat _origin)
{
          cv::cvtColor(_origin, _origin, cv::COLOR_BGR2GRAY);                        //转换为灰度图优化识别速度
          return dlib::cv_image<unsigned char>(_origin);
}

/*
 * 将FaceDetecion类转换为Rect
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
*外部函数获取内部的人脸位置数据
* @name: getFaceRectangle
* @param : 获取人脸所在的位置dlib::rectangle*& rect
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
* 外部函数获取内部的人脸特征点数据
* @name: 获取特征点所在的位置dlib::full_object_detection & _landmark
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
* 从输入图像中获取人脸所在坐标存于内部数据结构
* @name: findRectFromImage
* @param 输入原始图像  cv::Mat & _origin
* @retValue: true = 识别到一张人脸;false =识别到多张非法人脸
* @Correct: 2022-7-18 修复检测逻辑错误
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
* 从输入图像中获取人脸图像的特征点存于内部数据结构
* @name: findLandmarkFromImage
* @cautions: 此函数调用之前严禁使用cleanRectangle()释放内存
* @param 输入原始图像  cv::Mat & _origin
*/
void FaceDetecion::findLandmarkFromImage(cv::Mat& _origin)
{
          this->cleanLandmark();
          this->m_faceLandmark = new dlib::full_object_detection(
                    (*this->m_faceFeaturePoint)(ConvertBGR2GRAY(_origin), *this->m_facePos)
          );
}

/*
* 清除矩形坐标(用于算法加速)
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
* 清除特征点(用于算法加速)
* @name: cleanGrayImage
*/
inline void FaceDetecion::cleanLandmark()
{
          if (this->m_faceLandmark != nullptr) {
                    delete this->m_faceLandmark;
                    this->m_faceLandmark = nullptr;
          }
}