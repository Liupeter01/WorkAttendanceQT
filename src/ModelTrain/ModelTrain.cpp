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
* 初始化人脸模型
* @name: initResnetModel
*/
bool ModelTrain::initResnetModel()
{
          return m_resetLoader->getLoaderStatus(this->m_Net);
}

/*
* 释放人脸模型
* @name: releaseResnetModel
*/
void ModelTrain::releaseResnetModel()
{
          delete m_resetLoader;
          delete m_Net;
}

/*
* 将cv::Mat人脸根据合适的参数裁剪为指定大小的dlib存储类型的人脸
* @name: converImageStoreType
* @param 1.输入原始图像  cv::Mat & _origin
* @           2.人脸的特征点的存储结构：dlib::full_object_detection & _shapeInfo
* @retValue:  dlib::matrix<dlib::rgb_pixel> 返回一个用于训练神经网络的人脸
*/
dlib::matrix<dlib::rgb_pixel> 
ModelTrain::converImageStoreType(
          cv::Mat& _origin, 
          dlib::full_object_detection& _shapeInfo)
{
          dlib::matrix< dlib::rgb_pixel> imageConvert;                                                                  //转换为DLIB专属的RGB矩阵
          dlib::assign_image(imageConvert, dlib::cv_image<dlib::rgb_pixel>(_origin));
          dlib::extract_image_chip(imageConvert, dlib::get_face_chip_details(_shapeInfo, 150, 0.25), imageConvert);
          return imageConvert;
}

/*
* 当输入达到需求的训练图像数量时开启训练
* @name: externalInput
* @param 1.输入原始图像  cv::Mat & _origin
* @           2.人脸的特征点的存储结构：dlib::full_object_detection & _shapeInfo
* @RetValue : true : 可以继续输入图像 false 不可以输入
*/
bool ModelTrain::externalInput(cv::Mat& _origin, dlib::full_object_detection& _shapeInfo)
{
          if (this->m_imageArr.size() != this->tranningCount) {
                    this->m_imageArr.push_back(converImageStoreType(_origin, _shapeInfo));                    //压入容器
                    return true;                                                                                                                             //还可以继续输入图像
          }
          return false;                                                                                                                                     //严禁图像的再次输入
}

/*
* 根据dlib存储的多张人脸模型计算128D人脸特征向量均值
* @name:  ResnetTrainning
* @retValue:  返回一个初次保存的人脸的128D的人脸特征向量的平均值用于
*/
dlib::matrix<float, 0, 1> ModelTrain::resnetTrainning()
{
          std::vector < dlib::matrix<float, 0, 1>> faceMatrixArray = (*this->m_Net)(this->m_imageArr);
          this->m_imageArr.erase(m_imageArr.begin(), m_imageArr.end());                   //清空容器
          for (int i = 1; i < faceMatrixArray.size(); ++i) {
                    faceMatrixArray[0] += faceMatrixArray[i];
          }
          for (dlib::matrix<float, 0, 1>::iterator it = faceMatrixArray[0].begin(); it != faceMatrixArray[0].end(); ++it) {
                    (*it) /= (float)(faceMatrixArray.size());
          }
          return  faceMatrixArray.at(0);
}

/*
* 根据实时输入的视频模块计算当前人脸对应的编码
* @name:  resnetEncodingCalc
* @param 1.传递实时输入的人脸  dlib::matrix<dlib::rgb_pixel>>& _face
* @retValue:  返回一个描述人脸的128D的人脸特征向量
*/
dlib::matrix<float, 0, 1> 
ModelTrain::resnetEncodingCalc(dlib::matrix<dlib::rgb_pixel>& _face)
{
          return (*this->m_Net)(std::vector< dlib::matrix<dlib::rgb_pixel>> { _face }).at(0);
}

dlib::matrix<float, 0, 1> 
ModelTrain::resnetEncodingCalc(cv::Mat& _face)
{
          dlib::matrix< dlib::rgb_pixel> imageConvert;                                                                  //转换为DLIB专属的RGB矩阵
          dlib::assign_image(imageConvert, dlib::cv_image<dlib::rgb_pixel>(_face));
          return resnetEncodingCalc(imageConvert);
}

/*
* 将128D人脸特征向量转换为数据库字符类型
* @name:   convertMatrixToString
* @param 1.传递的128D的人脸特征向量  dlib::matrix<float, 0, 1>& src
*                2.传递接收人脸特征向量的字符串的地址std::string& dst
*
* @retValue:  返回一个转换是否成功
*/
bool ModelTrain::convertMatrixToString(dlib::matrix<float, 0, 1>& src, std::string& dst)
{
          if (!src.size()) {                      //数据为空
                    return false;
          }
          std::ostringstream outputStream(dst);
          for (auto i : src) {
                    outputStream << i << ",";
          }
          return true;
}

/*
* 将人脸数据库字符转换为特征向量类型128D
* @name:   convertStringToMatrix
* @param 1.传递接收人脸特征向量的字符串的地址std::string& src
*                   2.传递的128D的人脸特征向量  dlib::matrix<float, 0, 1>& dst
*/
void ModelTrain::convertStringToMatrix(std::string& src, dlib::matrix<float, 0, 1>& dst)
{
          std::istringstream inputStream(src);                                            //输入流
          dlib::matrix<float, 0, 1>::iterator matrix = dst.begin();             //矩阵输入流
          for (std::string::iterator it = src.begin(); it != src.end() && matrix < dst.begin()+128 ;) {
                    std::string _args, comma;               //取出参数数值和逗号
                    inputStream >> _args >> comma;
                    (*matrix++) = std::atof(_args.c_str());
                    it += _args.size() + comma.size();
          }
}