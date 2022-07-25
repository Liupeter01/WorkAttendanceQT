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
          for (auto& i : m_threadPool) {
                    if (i.joinable()) {
                              i.join();
                    }
          }
}

/*-----------------------------------------�����ͨ�ù���---------------------------------------*/
/*------------------------------------------------------------------------------------------------------
 * cv::Mat ----> QImage
 * @name: mat2Qimage
 * @function����MAT����ת��ΪQT��QImage����
 * @param ����ԭʼͼ��  const cv::Mat& mat
 * @retValue : ����λ�ڱ�����QImage&��������
 * @Correction: 2022-7-21 �����ڲ�����QImage��ʱ�洢�ṹ������ʹ�����ü���
 *                        2022-7-21 ɾ�������ж���Ĳ��֣���������CV8U_C3�µĴ���ʽ����������������
*------------------------------------------------------------------------------------------------------*/
inline QImage& ImageProcess::mat2Qimage(const cv::Mat& mat)
{
          return  (m_qimageFrameStore = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped());
}

/*------------------------------------------------------------------------------------------------------
 * ��������־��Ϣ�����SystemStatusInfo������
 * @name:  printOnTextBroswer
 * @function: ��������־��Ϣ�����SystemStatusInfo������
 * @param:  1. ������ڽӿڣ�QTextBrowser*& _systemOutput
 *                  2. �����wstring���ַ�����const std::wstring& _wstring
*------------------------------------------------------------------------------------------------------*/
inline void ImageProcess::printOnTextBroswer(
          QTextBrowser*& _systemOutput,
          const std::wstring& _wstring
)
{
          _systemOutput->insertPlainText(QString::fromStdWString(_wstring));
}

/*-------------------------------Ϊ�ڲ������ṩ��IxternalAPI------------------------------*/
/*------------------------------------------------------------------------------------------------------
 * ʵʱ����ͷͼ��+�������+����ʶ�������ʾ�ӿ�
 * @name: realTimeFacialDisplay
 * @function�������Ĺ��ܵ���ͨ����ó������ӵ��߳̽��в���
 * @param : ������ڽӿڣ�QTextBrowser*& _systemOutput
 * @retValue: QImage &
 *------------------------------------------------------------------------------------------------------*/
QImage& ImageProcess::realTimeFacialDisplay(QTextBrowser*& _systemOutput)
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
          this->m_faceRectSync.first = this->getFaceRectangle(this->m_imageSync.first, this->m_faceRectSync.second);    //��������λ��
          this->setRectSyncSignal();                                                                                                                                           //��ǰ������λ���Ѿ�����
          this->drawGeometryOnImage(this->m_imageSync.second, this->m_faceRectSync.first, "");
          return this->mat2Qimage(this->m_imageSync.first);                                                                                                     //��Ҫ�޸ģ�����
}

/*------------------------------------------------------------------------------------------------------
 * ͨ������ͷͼ�����ʵʱ����ѵ������ģ��
 * @name: realTimeFacialDisplay
 * @function����ʵʱ����ͷͼ��Эͬ��������ѵ������
 * @param:  1. ��Ƶ���� std::atomic<bool> &
 *                  2. ������ڽӿڣ�QTextBrowser*& _systemOutput
 *
 * @Correction: 2022-7-24 ��Ӻ��������޸���ֹ�߳��޷���ȷ��ֹͣ��ת
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::videoSyncFacialTranning(std::atomic<bool>& _videoFlag, QTextBrowser*& _systemOutput)
{
          while (!_videoFlag)
          {
                    /*��鵱ǰ�����źţ�ѭ���ȴ��ⲿ������ͷ�Ƿ���������*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_cameraSwitchSync.second);
                              this->m_cameraCtrl.wait(_lckg, [=]() {
                                        return this->getCameraState();
                                        });
                    }();

                    this->setvideoPause();                                                                                                         //��ͣ��ʾ�̵߳��������

                    /*�������ͼ���Ƿ�׼���ã�������������ķ���*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_imageSync.second);
                              this->m_imageReady.wait(_lckg, [=]() {
                                        return this->getImageSyncStatus();
                                        });
                              this->m_copiedImage = this->m_imageSync.first;
                    }();
                    this->resetImageSyncStatus();                                                                                             //ȡ������ͼ��ľ���״̬

                    /*�������λ���Ƿ�׼���ã�������������ķ���*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_faceRectSync.second);
                              this->m_rectReady.wait(_lckg, [=]() {
                                        return this->getRectSyncStatus();
                                        });
                              this->m_copiedfaceRect = this->m_faceRectSync.first;
                    }();
                    this->resetRectSyncStatus();                                                                                              //ȡ������λ�õľ���״̬

                    /*��鵱ǰ�����źţ�ѭ���ȴ��ⲿ�Ǳ����ɾ���ź�*/
                    [=]() {
                              std::unique_lock<std::mutex> _lckg(this->m_savingSwitchSync.second);
                              this->m_saving = this->m_savingSwitchSync.first;                                                 //���浱ǰ����״̬��
                              this->m_savingCtrl.wait(_lckg, [=]() {
                                        return this->getImageSavingState();
                                        });
                    }();

                    this->setvideoContinue();                                                                                                  //�ָ���ʾ�̵߳��������

                    if (this->m_saving == SavingSwitch::SAVING_FACE)
                    {
                              this->m_faceLandmark = this->getFaceLandmark(this->m_copiedImage, this->m_copiedfaceRect);   //��ȡ������
                              if (!externalInput(this->m_copiedImage, this->m_faceLandmark)) {       //����ֵ��Ϊfalse�����ѵ�����������
                                        break;
                              }
                    }
          }
}