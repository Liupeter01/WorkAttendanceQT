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
         * @param ����ԭʼͼ��  cv::Mat  &_origin
        */
          inline dlib::cv_image<unsigned char> ConvertBGR2GRAY(cv::Mat &_origin);

protected:

          /*
          * �ⲿ������ȡ�ڲ�������λ������
          * @name: getFaceRectangle
          * @param :  1.����ͼ���ԭʼͼcv::Mat& _origin
          *                   2.����λ�ø�����std::mutex & _writeMutex
          * 
          * @retValue�����ط���dlib::rectangle������
          */
          dlib::rectangle& getFaceRectangle(cv::Mat& _origin,std::mutex & _writeMutex);

          /*
          * �ⲿ������ȡ�ڲ�����������������
          * @name:  getFaceLandmark
          * @param: 1.����ͼ���ԭʼͼcv::Mat& _origin
          *                 2. ��������ͼ���λ��dlib::rectangle& _rect
          *
          * @retValue : ���ط�����ջdlib::full_object_detection
          */
          dlib::full_object_detection getFaceLandmark(cv::Mat& _origin, dlib::rectangle& _rect);

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
          * @Correct: 2022-7-18 �޸�����߼�����
          *                   2022-7-19 ��ģ����������еķ���ֵ�����ڵ�ǰ���л���
          */
          bool findRectFromImage(cv::Mat& _origin) = delete;

          /*
          * ������ͼ���л�ȡ����ͼ�������������ڲ����ݽṹ
          * @name: findLandmarkFromImage
          * @param ����ԭʼͼ��  cv::Mat & _origin
          * @retValue: true = ʶ��һ������;false =ʶ�𵽶��ŷǷ�����
          * @Correct: 2022-7-19 ��ģ����������еķ���ֵ�����ڵ�ǰ���л���
          */
          void findLandmarkFromImage(cv::Mat& _origin) =  delete;

public:
          cv::Mat m_grayImage;                                                                    //�洢�Ҷ�ͼ
private:
          dlib::rectangle m_imageFaceScale;                                                 //��ǰ��Ƶ��������
          FrontFaceLoader *m_landmarkLoader = nullptr;
          dlib::frontal_face_detector *m_faceDetector = nullptr;                 //�������ھ��μ����
          dlib::shape_predictor *m_faceFeaturePoint = nullptr;                    //68��������Ѱ����
};