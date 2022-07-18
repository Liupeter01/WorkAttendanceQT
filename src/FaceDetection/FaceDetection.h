#include"../HeaderFile/Common.h"

/*
 *��������������ģ�ͼ�����
 * @name:class FrontFaceLoader
* @function����������ģ�͵ļ����ٶ�
*/
class FrontFaceLoader {             
public:
          FrontFaceLoader();
          virtual ~FrontFaceLoader(); 
          /*
           * ��ȡ�������ģ�͵ļ���״̬
           * @name:getLoaderStatus
           * @param  1.�������ھ��μ���� dlib::frontal_face_detector& _frontFaceDetector
           *                 2. 68��������Ѱ��  dlib::shape_predictor& _shapePredictor
          */
          bool getLoaderStatus(
                    dlib::frontal_face_detector& _frontFaceDetector,
                    dlib::shape_predictor& _shapePredictor);
private:
          /*
           * ����������Landmark
           * @name: LoadLandMark
           * @param 68��������Ѱ��  dlib::shape_predictor& _shapePredictor
          */
          void LoadLandMark(dlib::shape_predictor& _shapePredictor); 

          /*
           * �����������μ����
           * @name:LoadFaceDetector
           * @param  1.�������ھ��μ���� dlib::frontal_face_detector& _frontFaceDetector
          */
          void LoadFaceDetector(dlib::frontal_face_detector& _frontFaceDetector);

private:
          std::promise<bool>m_landMark;
          std::promise<bool>m_faceDetector;
          std::future<bool>m_landMark_Next;
          std::future<bool>m_faceDetector_Next;
          std::mutex m_mutexLandMark;
          std::mutex m_mutexfaceDetector;
          std::vector<std::thread> m_threadPool;
};

class FaceDetecion {
public:
          FaceDetecion();
          virtual ~FaceDetecion();
public:
          /*
          * ���������ĳ�ʼ������
          * @name: initFaceDetection
          */
          bool initFaceDetection();

          /*
           * ����ר��ͼ��ĻҶ�ͼ(�����㷨����)
           * @name: ConvertBGR2GRAY
           * @function����ͼ��ת��Ϊdlib�ĻҶ�ͼ���������������ٶ�
           * @param ����ԭʼͼ��  cv::Mat & _origin
          */
          static inline dlib::cv_image<unsigned char> ConvertBGR2GRAY(cv::Mat& _origin);

          /*
           * ��FaceDetecion��ת��ΪRect
           * @name:   operator cv::Rect
          */
          operator cv::Rect();           

          /*
          * �ⲿ������ȡ�ڲ�������λ������
          * @name: getFaceRectangle
          */
          dlib::rectangle& getFaceRectangle();

          /*
          * �ⲿ������ȡ�ڲ�����������������
          * @name: getFaceLandmark
          */
          dlib::full_object_detection& getFaceLandmark();
private:
          /*
          * ������ͼ���л�ȡ����������������ڲ����ݽṹ
          * @name: findRectFromImage
          * @param ����ԭʼͼ��  cv::Mat & _origin
          * @retValue: true = ʶ��һ������;false =ʶ�𵽶��ŷǷ�����          
          */
          bool findRectFromImage(cv::Mat& _origin);

          /*
          * ������ͼ���л�ȡ����ͼ�������������ڲ����ݽṹ
          * @name: findLandmarkFromImage
          * @param ����ԭʼͼ��  cv::Mat & _origin
          * @retValue: true = ʶ��һ������;false =ʶ�𵽶��ŷǷ�����
          */
          void findLandmarkFromImage(cv::Mat& _origin);

public:
          dlib::rectangle m_facePos;                                            //�����ľ�������
          dlib::full_object_detection m_faceLandmark;             //������������
private:
          dlib::cv_image< unsigned char>m_imageGray;                   //�Ҷ�ͼ
          FrontFaceLoader m_landmarkLoader;
          dlib::frontal_face_detector m_faceDetector;                 //�������ھ��μ����
          dlib::shape_predictor m_faceFeaturePoint;                   //68��������Ѱ����
};