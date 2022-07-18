#include"ModelTrain.h"

ResnetLoader::ResnetLoader()
{

}

bool ResnetLoader::LoadResnetModel(anet_type*& _net)
{
          dlib::deserialize("dlib_face_recognition_resnet_model_v1.dat") >> *_net;
          return true;

}

bool ResnetLoader::getLoaderStatus(anet_type*& _net)
{
          m_resnet = std::async(&ResnetLoader::LoadResnetModel, this, std::ref(_net));
          m_resnet.wait();
          return m_resnet.get();
}

ResnetLoader::~ResnetLoader()
{

}