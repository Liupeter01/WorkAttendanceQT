#include"ImageProcess.h"

/*------------------------------Ϊ�ⲿ�����ṩ��ExternalAPI--------------------------------*/
/*------------------------------------------------------------------------------------------------------
 * ������Ƶ����ʾ�߳�(���ⲿGUI����)
 * @name:  startVideoDisplay
 * @function: ʵʱ����ͷͼ��+�������+����ʶ�������ʾ�ӿ�
 * @param: ������ڽӿڣ�QTextBrowser*& _systemOutput
 * @retValue: QImage &
 *------------------------------------------------------------------------------------------------------*/
QImage& ImageProcess::startVideoDisplay(QTextBrowser*& _systemOutput)
{
          return this->realTimeFacialDisplay(_systemOutput);
}

/*------------------------------------------------------------------------------------------------------
 * ��������ע�ắ��(���ⲿGUI����)
 * @name:  startVideoRegister
 * @function: ������ǰ��Ƶ���㣬��������ѵ������
 * @param:  1.��Ƶ���� std::atomic<bool> &
 *                  2.������ڽӿڣ�QTextBrowser*& _systemOutput
 *                  3.������������ӿڣ�QProgressBar* &_processBar
 *                  4.������ֵ��ʾ�� : int _displayNumber
 *
 * @Correction: 2022-7-24 ��Ӻ��������޸���ֹ�߳��޷���ȷ��ֹͣ��ת
*------------------------------------------------------------------------------------------------------*/
void  ImageProcess::startVideoRegister(
          std::atomic<bool>& _videoFlag,
          QTextBrowser*& _systemOutput,
          QProgressBar*& _processBar,
          int _displayNumber
)
{
          this->m_threadPool.emplace_back(
                    &ImageProcess::videoSyncFacialTranning, this, 
                    std::ref(_videoFlag), std::ref(_systemOutput),
                    std::ref(_processBar),_displayNumber
          );
}


/*------------------------------------------------------------------------------------------------------
 * ��������ע��֮������ѵ������(���ⲿGUI����)
 * @name:  startResnetModelTranning
 * @function: ��������ע��֮������ѵ������
 * @param:  ������ڽӿڣ�QTextBrowser*& _systemOutput
 * @retValue: ����ѵ���õ����������ַ���  std::string &
*------------------------------------------------------------------------------------------------------*/
std::string& ImageProcess::startResnetModelTranning(QTextBrowser*& _systemOutput)
{
          this->m_threadRes = std::async(&ImageProcess::modelSetTranning, this, std::ref(_systemOutput));
          this->m_threadRes.wait();
          return   this->m_threadRes.get();
}

/*------------------------------------------------------------------------------------------------------
 * ����湦������
 * @name:  enableSavingProcess
 * @function: ������ǰ��Ƶ���㣬�û��������ͼ����б���
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::enableSavingProcess()
{
          //������ǰͼ��ı��湦��
          std::lock_guard<std::mutex> _lckg(this->m_savingSwitchSync.second);
          this->m_savingSwitchSync.first = SavingSwitch::SAVING_FACE;
          this->m_savingCtrl.notify_all();                       //֪ͨ�߳������������Թ���(���ܴ���Bug)
}

/*------------------------------------------------------------------------------------------------------
 * ����������������
 * @name:  enableSavingProcess
 * @function: ������ǰ��Ƶ���㣬�û��������ͼ���������
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::enableIgnoreProcess()
{
          //������ǰͼ�����������
          std::lock_guard<std::mutex> _lckg(this->m_savingSwitchSync.second);
          this->m_savingSwitchSync.first = SavingSwitch::DELETE_FACE;
          this->m_savingCtrl.notify_all();                       //֪ͨ�߳������������Թ���(���ܴ���Bug)
}

/*------------------------------------------------------------------------------------------------------
 * ��������ͷ��������
 * @name: enableCameraShooting
 * @function: ������ǰ��Ƶ���㣬����ĳһ��ͼ��
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::enableCameraShooting()
{
          //������ǰ��Ƶ�����չ���
          std::lock_guard<std::mutex> _lckg(this->m_cameraSwitchSync.second);
          this->m_cameraSwitchSync.first = CameraSwitch::TAKE_PICTURE;
          this->m_cameraCtrl.notify_all();                       //֪ͨ�߳������������Թ���(���ܴ���Bug)
}