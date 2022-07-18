#include"../HeaderFile/Common.h"

//����Resnet����
//����dnn_imagenet_ex.cppʾ��ճ�����������滻����ʧ
//ʹ����Ķ������зֲ㣬ʹ�����ø�С��ȥ�����۰�
//dlib dnnʾ���˽�������Щ���ݵĺ��塣
//���⣬dnn_metric_learning_on_images_ex.cppʾ����ʾ�����ѵ�������硣
//����ʹ�õ�dlib_face_recognition_resnet_model_v1ģ����ʹ��\
//��������dnn_metric_learning_on_images_ex.cpp����ʾ�Ĵ��룬����
//С������35x15������5x5��������û�н�չ
//����Ϊ10000��ѵ�����ݼ��ɴ�Լ300���ͼ����ɣ�������
//55�����⣬����㱻����Ϊ150��С��ͼ��
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = dlib::add_prev1<block<N, BN, 1, dlib::tag1<SUBNET>>>;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = dlib::add_prev2< dlib::avg_pool<2, 2, 2, 2, dlib::skip1< dlib::tag2<block<N, BN, 2, dlib::tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN< dlib::con<N, 3, 3, 1, 1, dlib::relu<BN< dlib::con<N, 3, 3, stride, stride, SUBNET>>>>>;

template <int N, typename SUBNET> using ares = dlib::relu<residual<block, N, dlib::affine, SUBNET>>;
template <int N, typename SUBNET> using ares_down = dlib::relu<residual_down<block, N, dlib::affine, SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;

using anet_type = dlib::loss_metric< dlib::fc_no_bias<128, dlib::avg_pool_everything<
          alevel0< alevel1<alevel2< alevel3<alevel4<
          dlib::max_pool<3, 3, 2, 2, dlib::relu<dlib::affine<dlib::con<32, 7, 7, 2, 2, dlib::input_rgb_image_sized<150>>>>>>>>>>>>>;

/*
 *�в������������
 * @name:class FrontFaceLoader
* @function����������ģ�͵ļ����ٶ�
*/
class ResnetLoader {                //��������������ģ�ͺ��Լ��������ģ��
public:
          ResnetLoader();
          virtual ~ResnetLoader();
          bool getLoaderStatus(anet_type*& _net);
private:
          bool LoadResnetModel(anet_type*& _net);
private:
          std::future<bool>m_resnet;
};

class ModelTrain {
public:
          ModelTrain(int TranningSetting);
          virtual ~ModelTrain();
public:
          /*
          * ��ʼ������ģ��
          * @name: initResnetModel
          */
          bool initResnetModel();

          /*
          * �ͷ�����ģ��
          * @name: releaseResnetModel
          */
          void  releaseResnetModel();

          /*
          * ��cv::Mat�������ݺ��ʵĲ����ü�Ϊָ����С��dlib�洢���͵�����
          * @name: converImageStoreType
          * @param 1.����ԭʼͼ��  cv::Mat & _origin
          * @           2.������������Ĵ洢�ṹ��dlib::full_object_detection & _shapeInfo
          * @retValue:  dlib::matrix<dlib::rgb_pixel> ����һ������ѵ�������������
          */
          static dlib::matrix<dlib::rgb_pixel> converImageStoreType(cv::Mat &_origin, dlib::full_object_detection& _shapeInfo);

           /*
          * ������ﵽ�����ѵ��ͼ������ʱ����ѵ��
          * @name: externalInput
          * @param 1.����ԭʼͼ��  cv::Mat & _origin
          * @           2.������������Ĵ洢�ṹ��dlib::full_object_detection & _shapeInfo
          * @RetValue : true : ���Լ�������ͼ�� false ����������
          */
           bool externalInput(cv::Mat& _origin, dlib::full_object_detection& _shapeInfo);

private:
          /*
          * ����dlib�洢�Ķ�������ģ�ͼ���128D��������������ֵ
          * @name:  ResnetTrainning
          * @param 1.����Ԥ���趨��ѵ��������ʼ������  std::vector < dlib::matrix<dlib::rgb_pixel>>& _faces
          * @retValue:  ����һ�����α����������128D����������������ƽ��ֵ����
          */
          OUT dlib::matrix<float, 0, 1> resnetTrainning(std::vector < dlib::matrix<dlib::rgb_pixel>>& _faces);

          /*
          * ����ʵʱ�������Ƶģ����㵱ǰ������Ӧ�ı���
          * @name:  resnetEncodingCalc
          * @param 1.����ʵʱ���������  dlib::matrix<dlib::rgb_pixel>>& _face
          * @retValue:  ����һ������������128D��������������
          */
          OUT dlib::matrix<float, 0, 1> resnetEncodingCalc(dlib::matrix<dlib::rgb_pixel>& _face);

          /*
          * ��128D������������ת��Ϊ���ݿ��ַ�����
          * @name:   convertMatrixToString
          * @param 1.���ݵ�128D��������������  dlib::matrix<float, 0, 1>& src
          *                2.���ݽ������������������ַ����ĵ�ַstd::string& dst
          * 
          * @retValue:  ����һ��ת���Ƿ�ɹ�
          */
          bool convertMatrixToString(dlib::matrix<float, 0, 1>& src, std::string& dst);

          /*
          * ���������ݿ��ַ�ת��Ϊ������������128D
          * @name:   convertStringToMatrix
          * @param 1.���ݽ������������������ַ����ĵ�ַstd::string& src
          *                   2.���ݵ�128D��������������  dlib::matrix<float, 0, 1>& dst
          *
          * @retValue:  ����һ��ת���Ƿ�ɹ�
          */
          void convertStringToMatrix(std::string& src, dlib::matrix<float, 0, 1>& dst);

private:
          int tranningCount;                                                                              //��ʼ��ʱѵ��������ȷ��
          anet_type *m_Net = nullptr;                                                                               //�в������������
          ResnetLoader *m_resetLoader = nullptr;                                                            //�в������������
          std::vector < dlib::matrix<dlib::rgb_pixel>>m_imageArr;                //�в������������dlib����ͼ������
          dlib::matrix<float, 0, 1> m_faceEncoding;                                       //�в��������������128D����
          std::shared_future< dlib::matrix<float, 0, 1>> m_threadres;                 //�������е��߳��Ż�
};