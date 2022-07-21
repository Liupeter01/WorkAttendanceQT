#include"ModelTrain.h"

ModelTrain::ModelTrain(int TranningSetting)
          :tranningCount(TranningSetting),
          m_Net(new anet_type),
          m_resetLoader(new ResnetLoader)
{
}

ModelTrain::~ModelTrain() 
{
          this->releaseResnetModel();
}

/*
* ��ʼ������ģ��
* @name: initResnetModel
*/
bool ModelTrain::initResnetModel()
{
          return m_resetLoader->getLoaderStatus(this->m_Net);
}

/*
* �ͷ�����ģ��
* @name: releaseResnetModel
*/
void ModelTrain::releaseResnetModel()
{
          delete m_resetLoader;
          delete m_Net;
}

/*
* ��cv::Mat�������ݺ��ʵĲ����ü�Ϊָ����С��dlib�洢���͵�����
* @name: converImageStoreType
* @param 1.����ԭʼͼ��  cv::Mat & _origin
* @           2.������������Ĵ洢�ṹ��dlib::full_object_detection & _shapeInfo
* @retValue:  dlib::matrix<dlib::rgb_pixel> ����һ������ѵ�������������
*/
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

/*
* ������ﵽ�����ѵ��ͼ������ʱ����ѵ��
* @name: externalInput
* @param 1.����ԭʼͼ��  cv::Mat & _origin
* @           2.������������Ĵ洢�ṹ��dlib::full_object_detection & _shapeInfo
* @RetValue : true : ���Լ�������ͼ�� false ����������
*/
bool ModelTrain::externalInput(cv::Mat& _origin, dlib::full_object_detection& _shapeInfo)
{
          if (this->m_imageArr.size() != this->tranningCount) {
                    this->m_imageArr.push_back(converImageStoreType(_origin, _shapeInfo));                    //ѹ������
                    return true;                                                                                                                             //�����Լ�������ͼ��
          }
          return false;                                                                                                                                     //�Ͻ�ͼ����ٴ�����
}

/*
* ����dlib�洢�Ķ�������ģ�ͼ���128D��������������ֵ
* @name:  ResnetTrainning
* @retValue:  ����һ�����α����������128D����������������ƽ��ֵ����
*/
dlib::matrix<float, 0, 1> ModelTrain::resnetTrainning()
{
          std::vector < dlib::matrix<float, 0, 1>> faceMatrixArray = (*this->m_Net)(this->m_imageArr);
          this->m_imageArr.erase(m_imageArr.begin(), m_imageArr.end());                   //�������
          for (int i = 1; i < faceMatrixArray.size(); ++i) {
                    faceMatrixArray[0] += faceMatrixArray[i];
          }
          for (dlib::matrix<float, 0, 1>::iterator it = faceMatrixArray[0].begin(); it != faceMatrixArray[0].end(); ++it) {
                    (*it) /= (float)(faceMatrixArray.size());
          }
          return  faceMatrixArray.at(0);
}

/*
* ����ʵʱ�������Ƶģ����㵱ǰ������Ӧ�ı���
* @name:  resnetEncodingCalc
* @param 1.����ʵʱ���������  dlib::matrix<dlib::rgb_pixel>>& _face
* @retValue:  ����һ������������128D��������������
*/
dlib::matrix<float, 0, 1> 
ModelTrain::resnetEncodingCalc(dlib::matrix<dlib::rgb_pixel>& _face)
{
          return (*this->m_Net)(std::vector< dlib::matrix<dlib::rgb_pixel>> { _face }).at(0);
}

dlib::matrix<float, 0, 1> 
ModelTrain::resnetEncodingCalc(cv::Mat& _face)
{
          dlib::matrix< dlib::rgb_pixel> imageConvert;                                                                  //ת��ΪDLIBר����RGB����
          dlib::assign_image(imageConvert, dlib::cv_image<dlib::rgb_pixel>(_face));
          return resnetEncodingCalc(imageConvert);
}

/*
* ��128D������������ת��Ϊ���ݿ��ַ�����
* @name:   convertMatrixToString
* @param 1.���ݵ�128D��������������  dlib::matrix<float, 0, 1>& src
*                2.���ݽ������������������ַ����ĵ�ַstd::string& dst
*
* @retValue:  ����һ��ת���Ƿ�ɹ�
*/
bool ModelTrain::convertMatrixToString(dlib::matrix<float, 0, 1>& src, std::string& dst)
{
          if (!src.size()) {                      //����Ϊ��
                    return false;
          }
          std::ostringstream outputStream(dst);
          for (auto i : src) {
                    outputStream << i << ",";
          }
          return true;
}

/*
* ���������ݿ��ַ�ת��Ϊ������������128D
* @name:   convertStringToMatrix
* @param 1.���ݽ������������������ַ����ĵ�ַstd::string& src
*                   2.���ݵ�128D��������������  dlib::matrix<float, 0, 1>& dst
*/
void ModelTrain::convertStringToMatrix(std::string& src, dlib::matrix<float, 0, 1>& dst)
{
          std::istringstream inputStream(src);                                            //������
          dlib::matrix<float, 0, 1>::iterator matrix = dst.begin();             //����������
          for (std::string::iterator it = src.begin(); it != src.end() && matrix < dst.begin()+128 ;) {
                    std::string _args, comma;               //ȡ��������ֵ�Ͷ���
                    inputStream >> _args >> comma;
                    (*matrix++) = std::atof(_args.c_str());
                    it += _args.size() + comma.size();
          }
}