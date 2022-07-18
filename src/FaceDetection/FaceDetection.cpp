#include"FaceDetection.h"

FaceDetecion::FaceDetecion()
{
          
}

FaceDetecion::~FaceDetecion() 
{

}

/*
* 人脸检测类的初始化函数
* @name: initFaceDetection
*/
bool  FaceDetecion::initFaceDetection()
{
          return m_landmarkLoader.getLoaderStatus(this->m_faceDetector, this->m_faceFeaturePoint);
}

/*
 * 生成专属图像的灰度图(用于算法加速)
 * @name: ConvertBGR2GRAY
 * @function：将图像转换为dlib的灰度图像提升人脸检测的速度
 * @param 输入原始图像  cv::Mat & _origin
*/
inline dlib::cv_image<unsigned char> FaceDetecion::ConvertBGR2GRAY(cv::Mat& _origin)
{
          cv::Mat backup;
          cv::cvtColor(_origin, backup, cv::COLOR_BGR2GRAY);                        //转换为灰度图优化识别速度
          return dlib::cv_image<unsigned char>(backup);
}

/*
 * 将FaceDetecion类转换为Rect
 * @name:   operator cv::Rect
*/
FaceDetecion::operator cv::Rect() 
{
          return cv::Rect(cv::Point(this->m_facePos.left(), this->m_facePos.top()), cv::Point(this->m_facePos.right(), this->m_facePos.bottom()));
}

/*
* 外部函数获取内部的人脸位置数据
* @name: getFaceRectangle
*/
dlib::rectangle& FaceDetecion::getFaceRectangle()
{
          return this->m_facePos;
}

/*
* 外部函数获取内部的人脸特征点数据
* @name: getFaceLandmark
*/
dlib::full_object_detection& FaceDetecion::getFaceLandmark()
{
          return this->m_faceLandmark;
}

/*
* 从输入图像中获取人脸所在坐标存于内部数据结构
* @name: findRectFromImage
* @param 输入原始图像  cv::Mat & _origin
* @retValue: true = 识别到一张人脸;false =识别到多张非法人脸
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
* 从输入图像中获取人脸图像的特征点存于内部数据结构
* @name: findLandmarkFromImage
* @param 输入原始图像  cv::Mat & _origin
*/
void FaceDetecion::findLandmarkFromImage(cv::Mat& _origin)
{
          this->m_faceLandmark = this->m_faceFeaturePoint(this->m_imageGray, this->m_facePos);
}