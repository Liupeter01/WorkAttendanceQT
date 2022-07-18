#include"../HeaderFile/Common.h"

/*
 *特征点和人脸检测模型加载类
 * @name:class FrontFaceLoader
* @function：加速人脸模型的加载速度
*/
class FrontFaceLoader {             
public:
          FrontFaceLoader();
          virtual ~FrontFaceLoader(); 
          /*
           * 获取人脸检测模型的加载状态
           * @name:getLoaderStatus
           * @param  1.人脸所在矩形检测器 dlib::frontal_face_detector*& _frontFaceDetector
           *                 2. 68个特征点寻找  dlib::shape_predictor*& _shapePredictor
          */
          bool getLoaderStatus(
                    dlib::frontal_face_detector*& _frontFaceDetector,
                    dlib::shape_predictor*& _shapePredictor);
private:
          /*
           * 加载特征点Landmark
           * @name: LoadLandMark
           * @param 68个特征点寻找  dlib::shape_predictor*& _shapePredictor
          */
          bool LoadLandMark(dlib::shape_predictor*& _shapePredictor);

          /*
           * 加载人脸矩形检测器
           * @name:LoadFaceDetector
           * @param  1.人脸所在矩形检测器 dlib::frontal_face_detector*& _frontFaceDetector
          */
          bool LoadFaceDetector(dlib::frontal_face_detector*& _frontFaceDetector);

private:
          std::future<bool>m_landMark;
          std::future<bool>m_faceDetector;
};

class FaceDetecion {
public:
          FaceDetecion();
          virtual ~FaceDetecion();
public:
          /*
         * 生成专属图像的灰度图(用于算法加速)
         * @name: ConvertBGR2GRAY
         * @function：将图像转换为dlib的灰度图像提升人脸检测的速度
         * @param 输入原始图像  cv::Mat  _origin
        */
          inline dlib::cv_image<unsigned char> ConvertBGR2GRAY(cv::Mat _origin);

          /*
           * 将FaceDetecion类转换为Rect
           * @name:   operator cv::Rect
          */
          operator cv::Rect();

          /*
          * 外部函数获取内部的人脸位置数据
          * @name: getFaceRectangle
          * @param : 获取人脸所在的位置dlib::rectangle* &rect
          */
          void getFaceRectangle(dlib::rectangle *&rect);

          /*
          * 外部函数获取内部的人脸特征点数据
          * @name: 获取特征点所在的位置dlib::full_object_detection *& _landmark
          */
          void getFaceLandmark(dlib::full_object_detection *& _landmark);

protected:
          /*
          * 人脸检测类的初始化函数
          * @name: initFaceDetection
          */
          bool initFaceDetection();

          /*
          * 人脸检测类的核心功能释放函数
          * @name: initFaceDetection
          */
          void releaseFaceDetection();

          /*
          * 从输入图像中获取人脸所在坐标存于内部数据结构
          * @name: findRectFromImage
          * @param 输入原始图像  cv::Mat & _origin
          * @retValue: true = 识别到一张人脸;false =识别到多张非法人脸          
          */
          bool findRectFromImage(cv::Mat& _origin);

          /*
          * 从输入图像中获取人脸图像的特征点存于内部数据结构
          * @name: findLandmarkFromImage
          * @param 输入原始图像  cv::Mat & _origin
          * @retValue: true = 识别到一张人脸;false =识别到多张非法人脸
          */
          void findLandmarkFromImage(cv::Mat& _origin);
private:
          /*
          * 清除矩形坐标(用于算法加速)
          * @name:  cleanRectangle
          */
          inline void cleanRectangle();

          /*
          * 清除特征点(用于算法加速)
          * @name: cleanGrayImage
          */
          inline void cleanLandmark();

public:
          dlib::rectangle *m_facePos = nullptr;                                              //人脸的矩形坐标
          dlib::full_object_detection *m_faceLandmark = nullptr;               //人脸的特征点
private:
          FrontFaceLoader *m_landmarkLoader = nullptr;
          dlib::frontal_face_detector *m_faceDetector = nullptr;                 //人脸所在矩形检测器
          dlib::shape_predictor *m_faceFeaturePoint = nullptr;                    //68个特征点寻找器
};