#include"FaceDetection.h"

FrontFaceLoader::FrontFaceLoader()
          :m_landMark_Next(m_landMark.get_future()),
          m_faceDetector_Next(m_faceDetector.get_future()) {

}

FrontFaceLoader::~FrontFaceLoader()
{
          for (auto& thread : m_threadPool) {
                    if (thread.joinable()) {
                              thread.join();
                    }
          }
}

/*
 * ��ȡ�������ģ�͵ļ���״̬
 * @name:getLoaderStatus
 * @param
 *                �������ھ��μ���� dlib::frontal_face_detector& _frontFaceDetector
 *                  68��������Ѱ��  dlib::shape_predictor& _shapePredictor
*/
bool FrontFaceLoader::getLoaderStatus(
          dlib::frontal_face_detector& _frontFaceDetector,
          dlib::shape_predictor& _shapePredictor)
{
          m_threadPool.emplace_back(&FrontFaceLoader::LoadLandMark, this, std::ref(_shapePredictor));
          m_threadPool.emplace_back(&FrontFaceLoader::LoadFaceDetector, this, std::ref(_frontFaceDetector));
          m_landMark_Next.wait();
          m_faceDetector_Next.wait();
          return  m_landMark_Next.get() && m_faceDetector_Next.get();
}

/*
 * ����������Landmark
 * @name: LoadLandMark
 * @param 68��������Ѱ��  dlib::shape_predictor& _shapePredictor
*/
void FrontFaceLoader::LoadLandMark(dlib::shape_predictor& _shapePredictor)
{
          dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> _shapePredictor;
          m_mutexLandMark.lock();
          m_landMark.set_value(true);
          m_mutexLandMark.unlock();
}

/*
 * �����������μ����
 * @name:LoadFaceDetector
 * @param  1.�������ھ��μ���� dlib::frontal_face_detector& _frontFaceDetector
*/
void FrontFaceLoader::LoadFaceDetector(dlib::frontal_face_detector& _frontFaceDetector)
{
          _frontFaceDetector = dlib::get_frontal_face_detector();
          m_mutexfaceDetector.lock();
          m_faceDetector.set_value(true);
          m_mutexfaceDetector.unlock();
}