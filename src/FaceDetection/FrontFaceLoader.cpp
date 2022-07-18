#include"FaceDetection.h"

FrontFaceLoader::FrontFaceLoader()
{

}

FrontFaceLoader::~FrontFaceLoader()
{

}

/*
 * ��ȡ�������ģ�͵ļ���״̬
 * @name:getLoaderStatus
 * @param
 *                �������ھ��μ���� dlib::frontal_face_detector*& _frontFaceDetector
 *                  68��������Ѱ��  dlib::shape_predictor*& _shapePredictor
*/
bool FrontFaceLoader::getLoaderStatus(
          dlib::frontal_face_detector*& _frontFaceDetector,
          dlib::shape_predictor*& _shapePredictor)
{
          this->m_landMark = std::async(&FrontFaceLoader::LoadLandMark, this, std::ref(_shapePredictor));
          this->m_faceDetector = std::async(&FrontFaceLoader::LoadFaceDetector, this, std::ref(_frontFaceDetector));
          this->m_landMark.wait();
          this->m_faceDetector.wait();
          return   this->m_faceDetector.get() && this->m_landMark.get();
}

/*
 * ����������Landmark
 * @name: LoadLandMark
 * @param 68��������Ѱ��  dlib::shape_predictor*& _shapePredictor
*/
bool  FrontFaceLoader::LoadLandMark(dlib::shape_predictor*& _shapePredictor)
{
          dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> *_shapePredictor;
          return true;
}

/*
 * �����������μ����
 * @name:LoadFaceDetector
 * @param  1.�������ھ��μ���� dlib::frontal_face_detector*& _frontFaceDetector
*/
bool  FrontFaceLoader::LoadFaceDetector(dlib::frontal_face_detector*& _frontFaceDetector)
{
          *_frontFaceDetector = dlib::get_frontal_face_detector();
          return true;
}