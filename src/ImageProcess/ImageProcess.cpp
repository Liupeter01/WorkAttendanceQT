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
 * ���������Ѿ����߳�ģ��ͬ��
 * @name:setDataSyncSignal;
 * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
*/
void  ImageProcess::setDataSyncSignal()
{
          this->m_imageSync.second = true;
          this->m_faceRectSync.second = true;
}

/*
 * �������ݵ�ǰ����������ģ�����ݹ�ʱ
 * @name: resetDataSyncSignal;
 * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
*/
void  ImageProcess::resetDataSyncSignal()
{
          this->m_imageSync.second = false;
          this->m_faceRectSync.second = false;
}

/*
 * ��ȡ��ǰ�����Ƿ�Ϊ����״̬
 * @name: resetDataSyncSignal;
 * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
 * @retValue: 1.true : ��ǰ���ݵ�״̬Ϊ��״̬
 *                    2.false: ��ǰ���ݵ�״̬Ϊ��״̬
*/
bool  ImageProcess::getDataSyncStatus()
{
          return  this->m_imageSync.second && this->m_faceRectSync.second;
}

/*
 * ��ͣ��Ļ��ʾ�̵߳Ĺ������ȴ��û�����ͼ���ȷ�Ϻ���
 * @name: getUserConfirmStatus
 * @function: ��ѵ���߳���������Ҫ�ȴ��û����ͼ��ȷ��֮��ſ��Լ�����ʾ
 * @retValue: 1.true : �����������һ֡����ʾ
 *                    2.false: ��������һ֡����ʾ
*/
bool  ImageProcess::getUserConfirmStatus()
{
          return true;
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
          //�����������ã�������Ҫ�ȴ�����ģ����źţ��ſ��Լ�������ִ��
          {
                    std::unique_lock<std::mutex> _lckg(this->m_CofirmUserInptutEvent);
                    this->m_UserInput.wait(_lckg, [=]() {
                              return getUserConfirmStatus();
                     });
          }
          this->resetDataSyncSignal();                                                                                                                                 //�趨��ǰ����Ϊ����״̬
          this->m_imageSync.first = this->getImageFrame(this->m_imageRWLock);                                                        //��������ͼ��
          this->m_faceRectSync.first= this->getFaceRectangle(this->m_imageSync.first, this->m_faceRectRWLock);    //��������λ��

           //�ź����ã����������Ѿ�׼����ϣ������߳�ģ����Է���
          this->setDataSyncSignal();                                                                                                                                      //�趨��ǰ����Ϊ����״̬
          this->drawGeometryOnImage(this->m_imageRWLock, this->m_faceRectSync.first, "");
          return mat2Qimage(this->m_imageSync.first);
}

/*
 * ͨ������ͷͼ�����ʵʱ����ѵ������ģ��
 * @name: realTimeFacialDisplay
 * @function����ʵʱ����ͷͼ��Эͬ��������ѵ������
*/
void ImageProcess::videoSyncFacialTranning()
{
          //do
          //{
          //          if (true) {
          //                    continue;
          //          }
          //} while (externalInput());
}