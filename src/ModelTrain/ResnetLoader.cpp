#include"ModelTrain.h"

ResnetLoader::ResnetLoader()
{

}

ResnetLoader::~ResnetLoader()
{

}

/*------------------------------------------------------------------------------------------------------
 * ��ȡ�����в���ģ�͵ļ���״̬
 * @name:getLoaderStatus
 * @param  �в������������ anet_type*& _net
*------------------------------------------------------------------------------------------------------*/
bool ResnetLoader::getLoaderStatus(anet_type*& _net)
{
          m_resnet = std::async(&ResnetLoader::LoadResnetModel, this, std::ref(_net));
          m_resnet.wait();
          return m_resnet.get();
}

/*------------------------------------------------------------------------------------------------------
 * ���������в���ģ��Resnet
 * @name: LoadResnetModel
 * @param �в������������ anet_type*& _net
*------------------------------------------------------------------------------------------------------*/
bool ResnetLoader::LoadResnetModel(anet_type*& _net)
{
          dlib::deserialize("dlib_face_recognition_resnet_model_v1.dat") >> *_net;
          return true;
}