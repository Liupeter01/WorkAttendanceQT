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
          this->m_imageReady.notify_all();                        //֪ͨ�߳������������Թ���
}

/*
 * ��������λ�þ����ź�
 * @name:setRectSyncSignal
 * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
*/
void ImageProcess::setRectSyncSignal()
{
          this->m_faceRectReadySign = true;
          this->m_rectReady.notify_all();                        //֪ͨ�߳������������Թ���
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
          m_videoDisplayCtrl.notify_all();                        //֪ͨ�߳������������Թ���(���ܴ���Bug)
}

/*
 * ������Ƶ��ʾ�̵߳ļ���
 * @name: setvideoContinue
 * @function: ��ͣ�ͼ�����Ƶ����ʾ�߳�
*/
void ImageProcess::setvideoContinue()
{
          this->m_videoDisplaySign = true;
          m_videoDisplayCtrl.notify_all();                        //֪ͨ�߳������������Թ���(���ܴ���Bug)
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
 * ��������ͷ��������
 * @name: enableCameraShooting
 * @function: ������ǰ��Ƶ���㣬����ĳһ��ͼ��
*/
void ImageProcess::enableCameraShooting()
{
          //������ǰ��Ƶ�����չ���
          std::lock_guard<std::mutex> _lckg(this->m_cameraSwitchSync.second);
          this->m_cameraSwitchSync.first = CameraSwitch::TAKE_PICTURE;
          this->m_cameraCtrl.notify_all();                       //֪ͨ�߳������������Թ���(���ܴ���Bug)
}

/*
 * �����߳��ж�����ͷ�Ƿ���������
 * @name: getCameraState
 * @function: �ж�����ͷ����
 * @RetValue:1.true : ��������
 *                    2.false: ����������
 * @Correction: 2022-7-24 
*/
bool ImageProcess::getCameraState()
{
          bool _retValue(false);
          {      //��ȡ��ǰ����ͷ��Ƶ����
                    //std::lock_guard<std::mutex> _lckg(this->m_cameraSwitchSync.second);
                    _retValue = static_cast<bool>(this->m_cameraSwitchSync.first);                  //��ȡ֮ǰ������
                    this->m_cameraSwitchSync.first = CameraSwitch::NO_INPUT;                     //����֮ǰ�������״̬
          }
          return _retValue;
}

/*
 * ����湦������
 * @name:  enableSavingProcess
 * @function: ������ǰ��Ƶ���㣬�û��������ͼ����б���
*/
void ImageProcess::enableSavingProcess()
{
          //������ǰͼ��ı��湦��
          std::lock_guard<std::mutex> _lckg(this->m_savingSwitchSync.second);
          this->m_savingSwitchSync.first = SavingSwitch::SAVING_FACE;
          this->m_savingCtrl.notify_all();                       //֪ͨ�߳������������Թ���(���ܴ���Bug)
}

/*
 * ����������������
 * @name:  enableSavingProcess
 * @function: ������ǰ��Ƶ���㣬�û��������ͼ���������
*/
void ImageProcess::enableIgnoreProcess()
{
          //������ǰͼ�����������
          std::lock_guard<std::mutex> _lckg(this->m_savingSwitchSync.second);
          this->m_savingSwitchSync.first = SavingSwitch::DELETE_FACE;
          this->m_savingCtrl.notify_all();                       //֪ͨ�߳������������Թ���(���ܴ���Bug)
}

/*
 * �����߳��жϱ��湦���Ƿ�����
 * @name: getImageSavingState
 * @function: �ж�����ͷ����
 * @RetValue:1.true : �õ��û���ȷ��(���۱��������)
 *                    2.false: û�еõ��û���ȷ��
*/
bool ImageProcess::getImageSavingState()
{
          bool _retValue(false);
          {      //��ȡ��ǰ����ͷ��Ƶ����
                    //std::lock_guard<std::mutex> _lckg(this->m_savingSwitchSync.second);
                    _retValue = static_cast<bool>(this->m_savingSwitchSync.first);                  //��ȡ֮ǰ������
                    this->m_savingSwitchSync.first = SavingSwitch::DEFAULT;                     //����֮ǰ�ı���״̬
          }
          return _retValue;
}