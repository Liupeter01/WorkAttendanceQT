#include"ModelTrain.h"

ModelTrain::ModelTrain(int _TranningSetting, double _TrainningSimilarity)
          :TrainningSetting(_TranningSetting),
          TrainningSimilarity(_TrainningSimilarity),
          m_Net(new anet_type),
          m_resetLoader(new ResnetLoader)
{
}

ModelTrain::~ModelTrain() 
{
          this->releaseResnetModel();
}

/*---------------------------------ModelTrain��ͨ�ù��߽ӿ�--------------------------------*/
/*------------------------------------------------------------------------------------------------------
* ��cv::Mat�������ݺ��ʵĲ����ü�Ϊָ����С��dlib�洢���͵�����
* @name: converImageStoreType
* @param 1.����ԭʼͼ��  cv::Mat & _origin
* @           2.������������Ĵ洢�ṹ��dlib::full_object_detection & _shapeInfo
*
* @retValue:  dlib::matrix<dlib::rgb_pixel> ����һ������ѵ�������������
*------------------------------------------------------------------------------------------------------*/
dlib::matrix<dlib::rgb_pixel>
ModelTrain::converImageStoreType(
          cv::Mat& _origin,
          dlib::full_object_detection& _shapeInfo)
{
          dlib::matrix< dlib::rgb_pixel> imageConvert;                                                                  //ת��ΪDLIBר����RGB����
          dlib::assign_image(imageConvert, dlib::cv_image<dlib::rgb_pixel>(_origin));
          dlib::extract_image_chip(imageConvert, dlib::get_face_chip_details(_shapeInfo, 150, 0.25), imageConvert);
          return imageConvert;
}

/*-----------------------------ModelTrain�ĳ�ʼ���ĺ����ӿ�----------------------------*/
/*------------------------------------------------------------------------------------------------------
* ��ʼ������ģ��
* @name: initResnetModel
*------------------------------------------------------------------------------------------------------*/
bool ModelTrain::initResnetModel()
{
          return m_resetLoader->getLoaderStatus(this->m_Net);
}

/*------------------------------------------------------------------------------------------------------
* �ͷ�����ģ��
* @name: releaseResnetModel
*------------------------------------------------------------------------------------------------------*/
void ModelTrain::releaseResnetModel()
{
          delete m_resetLoader;
          delete m_Net;
}

/*--------------------------ModelTrain�����������ѵ�������ӿ�------------------------*/
/*------------------------------------------------------------------------------------------------------
* ������ﵽ�����ѵ��ͼ������ʱ����ѵ��
* @name: externalInput
* @param 1.�ر��ⲿ����Ŀ��ƿ��� std::atomic<bool>& _videoFlag
*                2.����ԭʼͼ��  cv::Mat & _origin
*                3.������������Ĵ洢�ṹ��dlib::full_object_detection & _shapeInfo
* 
* @RetValue : true : ���Լ�������ͼ�� false ����������
* @Correction: 2022-7-30 ����_videoFlag��ʶ�����ڷ�ֹ�����߳��޷��رյ����
*------------------------------------------------------------------------------------------------------*/
bool ModelTrain::externalInput(
          std::atomic<bool>& _videoFlag,
          cv::Mat& _origin,
          dlib::full_object_detection& _shapeInfo
)
{
          if (!_videoFlag) {                                                                                                                             //���ƿ���
                    if (this->m_imageArr.size() != this->TrainningSetting) {
                              this->m_imageArr.push_back(converImageStoreType(_origin, _shapeInfo));          //ѹ������
                              return true;                                                                                                                   //�����Լ�������ͼ��
                    }
          }
          return false;                                                                                                                                      //�Ͻ�ͼ����ٴ�����
}

/*------------------------------------------------------------------------------------------------------
* ����dlib�洢�Ķ�������ģ�ͼ���128D��������������ֵ
* @name:  ResnetTrainning
* @retValue:  ����һ�����α����������128D����������������ƽ��ֵ����
* 
* @Correction: 2022-7-29 �����˶��ڶ�������ѵ��ģ������Ϊ�յ��쳣���
*------------------------------------------------------------------------------------------------------*/
dlib::matrix<float, 0, 1> ModelTrain::resnetTrainning()
{
          try
          {
                    if (!this->m_imageArr.size()) {                                                                               //��������ģ��m_imageArrΪ�����׳��쳣
                              throw EmptyVector();
                    }
                    std::vector < dlib::matrix<float, 0, 1>> faceMatrixArray = (*this->m_Net)(this->m_imageArr);
                    this->m_imageArr.erase(m_imageArr.begin(), m_imageArr.end());           //�������
                    for (int i = 1; i < faceMatrixArray.size(); ++i) {
                              faceMatrixArray[0] += faceMatrixArray[i];
                    }
                    for (dlib::matrix<float, 0, 1>::iterator it = faceMatrixArray[0].begin(); it != faceMatrixArray[0].end(); ++it) {
                              (*it) /= (float)(faceMatrixArray.size());
                    }
                    return  faceMatrixArray.at(0);
          }
          catch (const EmptyVector&) {

          }
          return dlib::matrix<float, 0, 1>();                                                                                      //���ؿյ�128D��������������
}

/*------------------------------------------------------------------------------------------------------
* ����ʵʱ�������Ƶģ����㵱ǰ������Ӧ�ı���
* @name:  resnetEncodingCalc
* @param 1.����ʵʱ���������  dlib::matrix<dlib::rgb_pixel>>& _face
* @retValue:  ����һ������������128D��������������
*------------------------------------------------------------------------------------------------------*/
dlib::matrix<float, 0, 1> 
ModelTrain::resnetEncodingCalc(dlib::matrix<dlib::rgb_pixel>& _face)
{
          return (*this->m_Net)(std::vector< dlib::matrix<dlib::rgb_pixel>> { _face }).at(0);
}

dlib::matrix<float, 0, 1>
ModelTrain::resnetEncodingCalc(dlib::matrix<dlib::rgb_pixel> _face)
{
          return (*this->m_Net)(std::vector< dlib::matrix<dlib::rgb_pixel>> { _face }).at(0);
}

dlib::matrix<float, 0, 1>
ModelTrain::resnetEncodingCalc(cv::Mat& _face, dlib::full_object_detection& m_faceLandmark)
{
          return resnetEncodingCalc(converImageStoreType(_face, m_faceLandmark));            //ת��ΪDLIBר����RGB����
}

/*--------------------------ModelTrain�����ݵ������ȶԺ����ӿ�--------------------*/
/*------------------------------------------------------------------------------------------------------
 * ����ʵʱ�������Ƶģ����㵱ǰ������Ӧ�ı���
 * @name:  compareFaceMatrix
 * @param 1.�������ݿ��е���������   const dlib::matrix<float, 0, 1>& _dbMatrix
 *                2.����ʵʱ�������������  const dlib::matrix<float, 0, 1>& _realTimeMatrix
 *
 * @retValue:  ����һ������������128D��������������
 *------------------------------------------------------------------------------------------------------*/
bool ModelTrain::compareFaceMatrix(
          const dlib::matrix<float, 0, 1>& _dbMatrix,
          const dlib::matrix<float, 0, 1>& _realTimeMatrix
)
{
          return  (dlib::length(_realTimeMatrix - _dbMatrix) < this->TrainningSimilarity ? true : false);
}

/*--------------------------ModelTrain�����ݵ�ת�������ӿ�------------------------*/
/*------------------------------------------------------------------------------------------------------
* ��128D������������ת��Ϊ���ݿ��ַ�����
* @name:   convertMatrixToString
* @param 1.���ݵ�128D��������������const  dlib::matrix<float, 0, 1>& src
*                2.���ݽ������������������ַ����ĵ�ַstd::string& dst
*
* @retValue:  ����һ��ת���Ƿ�ɹ�
* @Correction: 2022-7-29 ����flush����ˢ�£���ֹ���ص���ֵΪ�յ��������
*------------------------------------------------------------------------------------------------------*/
bool ModelTrain::convertMatrixToString(const dlib::matrix<float, 0, 1>& src, std::string& dst)
{
          if (!src.size()) {                                                              //����Ϊ��
                    dst = "";                                                                //�����ַ���Ϊ��
                    return false;
          }
          std::ostringstream outputStream(dst);
          for (auto i : src) {
                    outputStream << i << ",";
          }
          dst = outputStream.str();                                             //����ֵ���¸��ⲿ����
          return true;
}

/*------------------------------------------------------------------------------------------------------
* ���������ݿ��ַ�ת��Ϊ������������128D
* @name:   convertStringToMatrix
* @param 1.���ݽ������������������ַ����ĵ�ַstd::string& src
*                   2.���ݵ�128D��������������  dlib::matrix<float, 0, 1>& dst
*
* @Correction: 2022-7-29 �޸����������޷��ɹ��Ľ�����ȡ���µĳ������
*                        2022-7-31 �޸�std::string����������ʱ����Խ������⣬ʹ��ָ��������
*------------------------------------------------------------------------------------------------------*/
#include<strstream>
void ModelTrain::convertStringToMatrix(std::string& src, dlib::matrix<float, 0, 1>& dst)           
{               
          dlib::matrix<float, 0, 1>matrix_face(128, 1);                                                                          //����������
          dlib::matrix<float, 0, 1>::iterator matrix_face_it = matrix_face.begin();                               //����������
          for (const char* p = src.c_str(); p != src.c_str() + strlen(src.c_str()); ) {
                    char temp[32]{ 0 };
                    std::istrstream inputStream(p);
                    inputStream.getline(temp, 32, ',');                                                                                   //��ȡһ������<������ֵ,����>
                    p += std::strlen(temp) + 1;                                                                                              //�����������ֽ�+����','(1B)
                    *(matrix_face_it++) = std::atof(temp);                                                                              //������������ʱ����������matrix������
          }
          dst = matrix_face;
}