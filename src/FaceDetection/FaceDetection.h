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
         * @param 输入原始图像  cv::Mat  &_origin
        */
          inline dlib::cv_image<unsigned char> ConvertBGR2GRAY(cv::Mat &_origin);

protected:

          /*
          * 外部函数获取内部的人脸位置数据
          * @name: getFaceRectangle
          * @param :  1.输入图像的原始图cv::Mat& _origin
          *                   2.人脸位置更新锁std::mutex & _writeMutex
          * 
          * @retValue：返回分配dlib::rectangle的引用
          */
          dlib::rectangle& getFaceRectangle(cv::Mat& _origin,std::mutex & _writeMutex);

          /*
          * 外部函数获取内部的人脸特征点数据
          * @name:  getFaceLandmark
          * @param: 1.输入图像的原始图cv::Mat& _origin
          *                 2. 人脸所在图像的位置dlib::rectangle& _rect
          *
          * @retValue : 返回分配在栈dlib::full_object_detection
          */
          dlib::full_object_detection getFaceLandmark(cv::Mat& _origin, dlib::rectangle& _rect);

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
          * @Correct: 2022-7-18 修复检测逻辑错误
          *                   2022-7-19 此模块废弃，所有的返回值无需在当前类中缓存
          */
          bool findRectFromImage(cv::Mat& _origin) = delete;

          /*
          * 从输入图像中获取人脸图像的特征点存于内部数据结构
          * @name: findLandmarkFromImage
          * @param 输入原始图像  cv::Mat & _origin
          * @retValue: true = 识别到一张人脸;false =识别到多张非法人脸
          * @Correct: 2022-7-19 此模块废弃，所有的返回值无需在当前类中缓存
          */
          void findLandmarkFromImage(cv::Mat& _origin) =  delete;

public:
          cv::Mat m_grayImage;                                                                    //存储灰度图
private:
          dlib::rectangle m_imageFaceScale;                                                 //当前视频人脸坐标
          FrontFaceLoader *m_landmarkLoader = nullptr;
          dlib::frontal_face_detector *m_faceDetector = nullptr;                 //人脸所在矩形检测器
          dlib::shape_predictor *m_faceFeaturePoint = nullptr;                    //68个特征点寻找器
};