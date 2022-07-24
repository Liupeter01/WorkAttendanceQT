#include"ImageProcess.h"

ImageProcess::ImageProcess(int TrainningSetting)
          :FaceDetecion(), ModelTrain(TrainningSetting), TakePicture() 
{
          m_threadPool.emplace_back(&ImageProcess::initImageProcess, this);
          this->setvideoContinue();                                                                            //��Ƶ����������
          this->m_cameraSwitchSync.first = CameraSwitch::NO_INPUT;              //��������--��ǰû������
          this->m_savingSwitchSync.first = SavingSwitch::DEFAULT;                  //��������--��ǰû�в���
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
 * �������������ź�
 * @name:setImageSyncSignal
 * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
*/
void ImageProcess::setImageSyncSignal()
{
          this->m_imageReadySign = true;
}

/*
 * ��������λ�þ����ź�
 * @name:setRectSyncSignal
 * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
*/
void ImageProcess::setRectSyncSignal()
{
          this->m_faceRectReadySign = true;
}

/*
 * ��ȡ��ǰ��ͼƬ�Ƿ�ͬ��
 * @name: getImageSyncStatus
 * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
 * @retValue: 1.true : ��ǰ���ݵ�״̬Ϊ��״̬
 *                    2.false: ��ǰ���ݵ�״̬Ϊ��״̬
*/
bool  ImageProcess::getImageSyncStatus()
{
          return    this->m_imageReadySign;
}

/*
 * ��ȡ��ǰ����λ���Ƿ�ͬ��
 * @name: getRectSyncStatus
 * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
 * @retValue: 1.true : ��ǰ���ݵ�״̬Ϊ��״̬
 *                    2.false: ��ǰ���ݵ�״̬Ϊ��״̬
*/
bool  ImageProcess::getRectSyncStatus()
{
          return   this->m_faceRectReadySign;
}

/*
 * ��������ͼ������ź�
 * @name:resetImageSyncStatus
 * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
*/
void  ImageProcess::resetImageSyncStatus()
{
          this->m_imageReadySign = false;
}

/*
 * ��������λ�þ����ź�
 * @name: resetRectSyncStatus;
 * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
*/
void  ImageProcess::resetRectSyncStatus()
{
          this->m_faceRectReadySign = false;
}

/*
 * ��������ͼ���λ�þ����ź�
 * @name: resetDataSyncSignal;
 * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
*/
void  ImageProcess::resetDataSyncSignal()
{
          this->resetImageSyncStatus();
          this->resetRectSyncStatus();
}

/*
 * ������Ƶ��ʾ�̵߳���ͣ
 * @name: setvideoPause
 * @function: ��ͣ�ͼ�����Ƶ����ʾ�߳�
*/
void ImageProcess::setvideoPause()
{
          this->m_videoDisplaySign = false;
}

/*
 * ������Ƶ��ʾ�̵߳ļ���
 * @name: setvideoContinue
 * @function: ��ͣ�ͼ�����Ƶ����ʾ�߳�
*/
void ImageProcess::setvideoContinue()
{
          this->m_videoDisplaySign = true;
}

/*
* ��Ƶ����ʾ�̣߳����ڿ�����ͣ�ͼ���
* @name: isVideoPaused
* @function: ��ͣ�ͼ�����Ƶ����ʾ�߳�
* @retValue: 1.true : ������Ƶ����ʾ
*/
bool ImageProcess::isVideoPaused()
{
          return m_videoDisplaySign;
}

/*
 * ��������ͷ�����չ���
 * @name: enableCameraPhotography
 * @function: ������ǰ֡����Ƶ���㹦��
*/
void ImageProcess::enableCameraShooting()
{
          //������ǰ��Ƶ�����չ���
          std::lock_guard<std::mutex> _lckg(this->m_cameraSwitchSync.second);
          this->m_cameraSwitchSync.first = CameraSwitch::TAKE_PICTURE;
}

/*
 * ��ȡ����ͷ�Ƿ��Ѿ�������
 * @name: readCameraStatus
 * @function: ��ȡ�������յ�����
 * @RetValue:1.true : ��������
 *                    2.false: ����������
*/
bool ImageProcess::readCameraStatus()
{
          bool _retValue(false);
          {      //��ȡ��ǰ����ͷ��Ƶ����
                    std::lock_guard<std::mutex> _lckg(this->m_cameraSwitchSync.second);
                    _retValue = static_cast<bool>(this->m_cameraSwitchSync.first);                  //��ȡ֮ǰ������
                    this->m_cameraSwitchSync.first = CameraSwitch::NO_INPUT;                     //����֮ǰ�������״̬
          }
          return _retValue;
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
          [=]() {               //�����������ã�������Ҫ�ȴ�����ģ����źţ��ſ��Լ�������ִ��
                    std::unique_lock<std::mutex> _lckg(this->m_videoDisplaySync);
                    this->m_videoDisplayCtrl.wait(_lckg, [=]() 
                    {
                              return this->isVideoPaused();                               //��Ƶ�Ƿ�����ͣ״̬
                    });
          }();                     
          this->resetDataSyncSignal();                                                                                                                                        //�������ݾ����ź�
          this->m_imageSync.first = this->getImageFrame(this->m_imageSync.second);                                                        //��������ͼ��
          this->setImageSyncSignal();                                                                                                                                         //��ǰ������ͼ���Ѿ�����
          this->m_faceRectSync.first= this->getFaceRectangle(this->m_imageSync.first, this->m_faceRectSync.second);    //��������λ��
          this->setRectSyncSignal();                                                                                                                                           //��ǰ������λ���Ѿ�����
          this->drawGeometryOnImage(this->m_imageSync.second, this->m_faceRectSync.first, "");
          return mat2Qimage(this->m_imageSync.first);
}

/*
 * ͨ������ͷͼ�����ʵʱ����ѵ������ģ��
 * @name: realTimeFacialDisplay
 * @function����ʵʱ����ͷͼ��Эͬ��������ѵ������
*/
void ImageProcess::videoSyncFacialTranning()
{
          while (true) {
                    /*��鵱ǰ�����źţ�ѭ���ȴ��ⲿ��������������*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_cameraSwitchSync.second);
                              this->m_cameraCtrl.wait(_lckg, [=]() {return true; });
                    }();

                    //��ͣ��ʾ�̵߳��������

                    /*�������ͼ���Ƿ�׼���ã�������������ķ���*/
                    [=]() {            
                              std::unique_lock<std::mutex> _lckg(this->m_imageSync.second);
                              this->m_imageReady.wait(_lckg, [=]() {return this->getImageSyncStatus(); });
                              this->m_copiedImage = this->m_imageSync.first;
                    }();
                    this->resetImageSyncStatus();                                                                                             //ȡ������ͼ��ľ���״̬

                    /*�������λ���Ƿ�׼���ã�������������ķ���*/
                    [=]() {              
                              std::unique_lock<std::mutex> _lckg(this->m_faceRectSync.second);
                              this->m_rectReady.wait(_lckg, [=]() {return this->getRectSyncStatus(); });
                              this->m_copiedfaceRect = this->m_faceRectSync.first;
                    }();
                    this->resetRectSyncStatus();                                                                                              //ȡ������λ�õľ���״̬

                    /*��鵱ǰ�����źţ�ѭ���ȴ��ⲿ�Ǳ����ɾ���ź�*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_savingSwitchSync.second);
                              this->m_savingCtrl.wait(_lckg, [=]() {return true; });
                    }();

                    //�ָ���ʾ�̵߳��������
                    if (false)
                    {
                              this->m_faceLandmark = this->getFaceLandmark(this->m_copiedImage, this->m_copiedfaceRect);   //��ȡ������
                              if (!externalInput(this->m_copiedImage, this->m_faceLandmark)) {       //����ֵ��Ϊfalse�����ѵ�����������
                                        break;
                              }
                    }
          }
}