#include"ModelTrain.h"

ResnetLoader::ResnetLoader()
{

}

ResnetLoader::~ResnetLoader()
{

}

/*------------------------------------------------------------------------------------------------------
 * 获取人脸残差神经模型的加载状态
 * @name:getLoaderStatus
 * @param  残差神经网络操作类 anet_type*& _net
*------------------------------------------------------------------------------------------------------*/
bool ResnetLoader::getLoaderStatus(anet_type*& _net)
{
          m_resnet = std::async(&ResnetLoader::LoadResnetModel, this, std::ref(_net));
          m_resnet.wait();
          return m_resnet.get();
}

/*------------------------------------------------------------------------------------------------------
 * 加载人脸残差神经模型Resnet
 * @name: LoadResnetModel
 * @param 残差神经网络操作类 anet_type*& _net
*------------------------------------------------------------------------------------------------------*/
bool ResnetLoader::LoadResnetModel(anet_type*& _net)
{
          dlib::deserialize("dlib_face_recognition_resnet_model_v1.dat") >> *_net;
          return true;
}