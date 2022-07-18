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
           * @param  1.�������ھ��μ���� dlib::frontal_face_detector*& _frontFaceDetector
           *                 2. 68��������Ѱ��  dlib::shape_predictor*& _shapePredictor
          */
          bool getLoaderStatus(
                    dlib::frontal_face_detector*& _frontFaceDetector,
                    dlib::shape_predictor*& _shapePredictor);
private:
          /*
           * ����������Landmark
           * @name: LoadLandMark
           * @param 68��������Ѱ��  dlib::shape_predictor*& _shapePredictor
          */
          bool LoadLandMark(dlib::shape_predictor*& _shapePredictor);

          /*
           * �����������μ����
           * @name:LoadFaceDetector
           * @param  1.�������ھ��μ���� dlib::frontal_face_detector*& _frontFaceDetector
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
         * ����ר��ͼ��ĻҶ�ͼ(�����㷨����)
         * @name: ConvertBGR2GRAY
         * @function����ͼ��ת��Ϊdlib�ĻҶ�ͼ���������������ٶ�
         * @param ����ԭʼͼ��  cv::Mat  _origin
        */
          inline dlib::cv_image<unsigned char> ConvertBGR2GRAY(cv::Mat _origin);

          /*
           * ��FaceDetecion��ת��ΪRect
           * @name:   operator cv::Rect
          */
          operator cv::Rect();

          /*
          * �ⲿ������ȡ�ڲ�������λ������
          * @name: getFaceRectangle
          * @param : ��ȡ�������ڵ�λ��dlib::rectangle* &rect
          */
          void getFaceRectangle(dlib::rectangle *&rect);

          /*
          * �ⲿ������ȡ�ڲ�����������������
          * @name: ��ȡ���������ڵ�λ��dlib::full_object_detection *& _landmark
          */
          void getFaceLandmark(dlib::full_object_detection *& _landmark);

protected:
          /*
          * ���������ĳ�ʼ������
          * @name: initFaceDetection
          */
          bool initFaceDetection();

          /*
          * ���������ĺ��Ĺ����ͷź���
          * @name: initFaceDetection
          */
          void releaseFaceDetection();

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
private:
          /*
          * �����������(�����㷨����)
          * @name:  cleanRectangle
          */
          inline void cleanRectangle();

          /*
          * ���������(�����㷨����)
          * @name: cleanGrayImage
          */
          inline void cleanLandmark();

public:
          dlib::rectangle *m_facePos = nullptr;                                              //�����ľ�������
          dlib::full_object_detection *m_faceLandmark = nullptr;               //������������
private:
          FrontFaceLoader *m_landmarkLoader = nullptr;
          dlib::frontal_face_detector *m_faceDetector = nullptr;                 //�������ھ��μ����
          dlib::shape_predictor *m_faceFeaturePoint = nullptr;                    //68��������Ѱ����
};