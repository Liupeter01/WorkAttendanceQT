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

/*---------------------------------ModelTrain的通用工具接口--------------------------------*/
/*------------------------------------------------------------------------------------------------------
* 将cv::Mat人脸根据合适的参数裁剪为指定大小的dlib存储类型的人脸
* @name: converImageStoreType
* @param 1.输入原始图像  cv::Mat & _origin
* @           2.人脸的特征点的存储结构：dlib::full_object_detection & _shapeInfo
*
* @retValue:  dlib::matrix<dlib::rgb_pixel> 返回一个用于训练神经网络的人脸
*------------------------------------------------------------------------------------------------------*/
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

/*-----------------------------ModelTrain的初始化的函数接口----------------------------*/
/*------------------------------------------------------------------------------------------------------
* 初始化人脸模型
* @name: initResnetModel
*------------------------------------------------------------------------------------------------------*/
bool ModelTrain::initResnetModel()
{
          return m_resetLoader->getLoaderStatus(this->m_Net);
}

/*------------------------------------------------------------------------------------------------------
* 释放人脸模型
* @name: releaseResnetModel
*------------------------------------------------------------------------------------------------------*/
void ModelTrain::releaseResnetModel()
{
          delete m_resetLoader;
          delete m_Net;
}

/*--------------------------ModelTrain的数据输入和训练函数接口------------------------*/
/*------------------------------------------------------------------------------------------------------
* 当输入达到需求的训练图像数量时开启训练
* @name: externalInput
* @param 1.关闭外部输入的控制开关 std::atomic<bool>& _videoFlag
*                2.输入原始图像  cv::Mat & _origin
*                3.人脸的特征点的存储结构：dlib::full_object_detection & _shapeInfo
* 
* @RetValue : true : 可以继续输入图像 false 不可以输入
* @Correction: 2022-7-30 引入_videoFlag标识符用于防止输入线程无法关闭的情况
*------------------------------------------------------------------------------------------------------*/
bool ModelTrain::externalInput(
          std::atomic<bool>& _videoFlag,
          cv::Mat& _origin,
          dlib::full_object_detection& _shapeInfo
)
{
          if (!_videoFlag) {                                                                                                                             //控制开关
                    if (this->m_imageArr.size() != this->TrainningSetting) {
                              this->m_imageArr.push_back(converImageStoreType(_origin, _shapeInfo));          //压入容器
                              return true;                                                                                                                   //还可以继续输入图像
                    }
          }
          return false;                                                                                                                                      //严禁图像的再次输入
}

/*------------------------------------------------------------------------------------------------------
* 根据dlib存储的多张人脸模型计算128D人脸特征向量均值
* @name:  ResnetTrainning
* @retValue:  返回一个初次保存的人脸的128D的人脸特征向量的平均值用于
* 
* @Correction: 2022-7-29 增加了对于多张人脸训练模型容器为空的异常检测
*------------------------------------------------------------------------------------------------------*/
dlib::matrix<float, 0, 1> ModelTrain::resnetTrainning()
{
          try
          {
                    if (!this->m_imageArr.size()) {                                                                               //多张人脸模型m_imageArr为空则抛出异常
                              throw EmptyVector();
                    }
                    std::vector < dlib::matrix<float, 0, 1>> faceMatrixArray = (*this->m_Net)(this->m_imageArr);
                    this->m_imageArr.erase(m_imageArr.begin(), m_imageArr.end());           //清空容器
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
          return dlib::matrix<float, 0, 1>();                                                                                      //返回空的128D的人脸特征向量
}

/*------------------------------------------------------------------------------------------------------
* 根据实时输入的视频模块计算当前人脸对应的编码
* @name:  resnetEncodingCalc
* @param 1.传递实时输入的人脸  dlib::matrix<dlib::rgb_pixel>>& _face
* @retValue:  返回一个描述人脸的128D的人脸特征向量
*------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------
* 将128D人脸特征向量转换为数据库字符类型
* @name:   convertMatrixToString
* @param 1.传递的128D的人脸特征向量const  dlib::matrix<float, 0, 1>& src
*                2.传递接收人脸特征向量的字符串的地址std::string& dst
*
* @retValue:  返回一个转换是否成功
* @Correction: 2022-7-29 加上flush用于刷新，防止返回的数值为空的情况发生
*------------------------------------------------------------------------------------------------------*/
bool ModelTrain::convertMatrixToString(const dlib::matrix<float, 0, 1>& src, std::string& dst)
{
          if (!src.size()) {                                                              //数据为空
                    dst = "";                                                                //设置字符串为空
                    return false;
          }
          std::ostringstream outputStream(dst);
          for (auto i : src) {
                    outputStream << i << ",";
          }
          dst = outputStream.str();                                             //将数值更新给外部变量
          return true;
}

/*------------------------------------------------------------------------------------------------------
* 将人脸数据库字符转换为特征向量类型128D
* @name:   convertStringToMatrix
* @param 1.传递接收人脸特征向量的字符串的地址std::string& src
*                   2.传递的128D的人脸特征向量  dlib::matrix<float, 0, 1>& dst
*
* @Correction: 2022-7-29 修复数据类型无法成功的进行提取导致的程序出错
*------------------------------------------------------------------------------------------------------*/
void ModelTrain::convertStringToMatrix(std::string& src, dlib::matrix<float, 0, 1>& dst)
{
          std::istringstream inputStream(src);                                                                                           //输入流
          dlib::matrix<float, 0, 1>::iterator matrix = dst.begin();                                                             //矩阵输入流
          for (std::string::iterator it = src.begin(); it != src.end() && matrix < dst.begin()+128 ;) {       //默认128D的向量
                    double _args;                                                                                                                    //取出参数数值
                    char  comma;                                                                                                                    //取出逗号
                    inputStream >> _args >> comma;                                                                                    //提取一对数据<参数数值,逗号>
                    std::ostringstream dbArgs;
                    dbArgs << _args;                                                                                                             //将浮点数转换为字符串
                    (*matrix++) = _args;
                    it += dbArgs.str().size() + 1;                                                                                           //浮点数若干字节+逗号1B
          }
}