#include"ModelTrain.h"

ResnetLoader::ResnetLoader()
          :m_resnet_Next(m_resnet.get_future()) {

}

void ResnetLoader::LoadResnetModel(anet_type& _net)
{
          dlib::deserialize("dlib_face_recognition_resnet_model_v1.dat") >> _net;
          m_resnet.set_value(true);
}

bool ResnetLoader::getLoaderStatus(anet_type& _net)
{
          m_loadThread = std::thread(&ResnetLoader::LoadResnetModel, this, std::ref(_net));
          m_resnet_Next.wait();
          return m_resnet_Next.get();
}

ResnetLoader::~ResnetLoader()
{
          if (m_loadThread.joinable()) {
                    m_loadThread.join();
          }
}