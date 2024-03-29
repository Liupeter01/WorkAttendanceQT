#include"../HeaderFile/Common.h"

//定义Resnet网络
//并从dnn_imagenet_ex.cpp示例粘贴，但我们替换了损失
//使用损耗度量进行分层，使网络变得更小。去读导论吧
//dlib dnn示例了解所有这些内容的含义。
//另外，dnn_metric_learning_on_images_ex.cpp示例显示了如何训练此网络。
//本例使用的dlib_face_recognition_resnet_model_v1模型是使用\
//基本上是dnn_metric_learning_on_images_ex.cpp中显示的代码，除了
//小批量大（35x15而不是5x5），迭代没有进展
//设置为10000，训练数据集由大约300万个图像组成，而不是
//55。此外，输入层被锁定为150大小的图像。
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

/*------------------------------------------------------------------------------------------------------
 *空的Vector的容器
 * @name:class EmptyVector 
* @function：Vector的容器是空的
*------------------------------------------------------------------------------------------------------*/
class EmptyVector {
public:
          /*------------------------------------------------------------------------------------------------------
          * 返回当前的Vector的容器为空
          * @name: what
          * @RetValue:  std::string返回字符串
          *------------------------------------------------------------------------------------------------------*/
          std::string what() {
                    return std::string("当前的Vector的容器为空");
          }
};

/*------------------------------------------------------------------------------------------------------
 *残差神经网络加载类
 * @name:class ResnetLoader
* @function：加速残差神经网络模型加载速度
*------------------------------------------------------------------------------------------------------*/
class ResnetLoader {             
public:
          ResnetLoader();
          virtual ~ResnetLoader();

          /*------------------------------------------------------------------------------------------------------
           * 获取人脸残差神经模型的加载状态
           * @name:getLoaderStatus
           * @param  残差神经网络操作类 anet_type*& _net
          *------------------------------------------------------------------------------------------------------*/
          bool getLoaderStatus(anet_type*& _net);

private:
          /*------------------------------------------------------------------------------------------------------
           * 加载人脸残差神经模型Resnet
           * @name: LoadResnetModel
           * @param 残差神经网络操作类 anet_type*& _net
          *------------------------------------------------------------------------------------------------------*/
          bool LoadResnetModel(anet_type*& _net);

private:
          std::future<bool>m_resnet;
};

class ModelTrain {
public:
          ModelTrain(int _TranningSetting, double _TrainningSimilarity);
          virtual ~ModelTrain();

public:
          /*---------------------------------ModelTrain的通用工具接口--------------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * 将cv::Mat人脸根据合适的参数裁剪为指定大小的dlib存储类型的人脸
          * @name: converImageStoreType
          * @param 1.输入原始图像  cv::Mat & _origin
          * @           2.人脸的特征点的存储结构：dlib::full_object_detection & _shapeInfo
          * @retValue:  dlib::matrix<dlib::rgb_pixel> 返回一个用于训练神经网络的人脸
          *------------------------------------------------------------------------------------------------------*/
          static dlib::matrix<dlib::rgb_pixel> converImageStoreType(cv::Mat& _origin, dlib::full_object_detection& _shapeInfo);

protected:
          /*-----------------------------ModelTrain的初始化的函数接口----------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * 初始化人脸模型
          * @name: initResnetModel
          *------------------------------------------------------------------------------------------------------*/
          bool initResnetModel();

          /*------------------------------------------------------------------------------------------------------
          * 释放人脸模型
          * @name: releaseResnetModel
          *------------------------------------------------------------------------------------------------------*/ 
          void  releaseResnetModel();

protected:
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
          bool externalInput(
                    std::atomic<bool>& _videoFlag,
                    cv::Mat& _origin,
                    dlib::full_object_detection& _shapeInfo
          );

          /*------------------------------------------------------------------------------------------------------
          * 根据dlib存储的多张人脸模型计算128D人脸特征向量均值
          * @name:  ResnetTrainning
          * @retValue:  返回一个初次保存的人脸的128D的人脸特征向量的平均值用于
          *
          * @Correction: 2022-7-29 增加了对于多张人脸训练模型容器为空的异常检测
          *------------------------------------------------------------------------------------------------------*/
          OUT dlib::matrix<float, 0, 1> resnetTrainning();

          /*------------------------------------------------------------------------------------------------------
          * 根据实时输入的视频模块计算当前人脸对应的编码
          * @name:  resnetEncodingCalc
          * @param 1.传递实时输入的人脸  dlib::matrix<dlib::rgb_pixel>>& _face
          * @retValue:  返回一个描述人脸的128D的人脸特征向量
          *------------------------------------------------------------------------------------------------------*/
          OUT dlib::matrix<float, 0, 1> resnetEncodingCalc(dlib::matrix<dlib::rgb_pixel>& _face);
          OUT dlib::matrix<float, 0, 1> resnetEncodingCalc(cv::Mat & _face);

          /*------------------------------------------------------------------------------------------------------
          * 将128D人脸特征向量转换为数据库字符类型
          * @name:   convertMatrixToString
          * @param 1.传递的128D的人脸特征向量 const dlib::matrix<float, 0, 1>& src
          *                2.传递接收人脸特征向量的字符串的地址std::string& dst
          *
          * @retValue:  返回一个转换是否成功
          * @Correction: 2022-7-29 加上flush用于刷新，防止返回的数值为空的情况发生
          *------------------------------------------------------------------------------------------------------*/
          bool convertMatrixToString(const dlib::matrix<float, 0, 1>& src, std::string& dst);

          /*------------------------------------------------------------------------------------------------------
          * 将人脸数据库字符转换为特征向量类型128D
          * @name:   convertStringToMatrix
          * @param 1.传递接收人脸特征向量的字符串的地址std::string& src
          *                   2.传递的128D的人脸特征向量  dlib::matrix<float, 0, 1>& dst
          *
          * @Correction: 2022-7-29 修复数据类型无法成功的进行提取导致的程序出错
          *------------------------------------------------------------------------------------------------------*/
          void convertStringToMatrix(std::string& src, dlib::matrix<float, 0, 1>& dst);

private:
          int TrainningSetting;                                                                         //初始化时训练的张数确定
          double TrainningSimilarity;                                                              //初始化时训练的特征矩阵匹配参数
          anet_type *m_Net = nullptr;                                                             //残差神经网络操作类
          ResnetLoader *m_resetLoader = nullptr;                                         //残差神经网络加载类
          std::vector < dlib::matrix<dlib::rgb_pixel>>m_imageArr;              //残差神经网络输入的dlib风格的图像数组
          std::shared_future< dlib::matrix<float, 0, 1>> m_threadres;           //神经网络中的线程优化
};