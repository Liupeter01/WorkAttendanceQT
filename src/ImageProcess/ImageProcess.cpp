#include"ImageProcess.h"

ImageProcess::ImageProcess(int TrainningSetting)
          :FaceDetecion(), ModelTrain(TrainningSetting), TakePicture() {
          m_threadPool.emplace_back(&ImageProcess::initImageProcess, this);
}

ImageProcess::~ImageProcess()
{
          //this->releaseImageProcess();
          for (auto& i : m_threadPool) {
                    if (i.joinable()) {
                              i.join();
                    }
          }
}

/*
 * ��ʼ��ͼ�δ�������ڴ�
 * @name: initImageProcess
*/
bool ImageProcess::initImageProcess()
{
          return this->initFaceDetection() && this->initResnetModel();
}

/*
 * �ͷ�ͼ�δ�������ڴ�
 * @name:releaseImageProcess;
*/
void ImageProcess::releaseImageProcess()
{
          this->releaseResnetModel();
          this->releaseFaceDetection();
}

/*
 * cv::Mat ----> QImage
 * @name: mat2Qimage
 * @function����MAT����ת��ΪQT��QImage����
 * @param ����ԭʼͼ��  const cv::Mat& mat
 * @retValue : ����λ�ڱ�����QImage&��������
 * @Correction: 2022-7-21 �����ڲ�����QImage��ʱ�洢�ṹ������ʹ�����ü���
 *                        2022-7-21 ɾ�������ж���Ĳ��֣���������CV8U_C3�µĴ���ʽ����������������
*/
inline QImage& ImageProcess::mat2Qimage(const cv::Mat& mat)
{
          return  (m_qimageFrameStore = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped());
}

/*
 * ʵʱ����ͷͼ��+�������+����ʶ�������ʾ�ӿ�
 * @name: realTimeFacialDisplay
 * @function�������Ĺ��ܵ���ͨ����ó������ӵ��߳̽��в���
 * @param  1. ͼ���޸Ķ�д��  std::mutex& _writeMutex
 *                  2. ����ԭʼͼ��   cv::Mat& mat
 * 
 * @retValue: QImage &
*/
QImage &ImageProcess::realTimeFacialDisplay()
{
          this->m_sharedData.first = this->getImageFrame(this->m_imageRWLock);             //��������ͼ��
          this->m_sharedData.second = this->getFaceRectangle(this->m_sharedData.first);    //��������λ��
          //�ź�����
          this->drawGeometryOnImage(this->m_imageRWLock, this->m_sharedData.second);
          return mat2Qimage(this->m_sharedData.first);
}