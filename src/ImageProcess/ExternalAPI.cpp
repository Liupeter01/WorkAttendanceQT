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
 *
 * @Correction: 2022-7-24 ��Ӻ��������޸���ֹ�߳��޷���ȷ��ֹͣ��ת
*------------------------------------------------------------------------------------------------------*/
void  ImageProcess::startVideoRegister(std::atomic<bool>& _videoFlag, QTextBrowser*& _systemOutput)
{
          this->m_threadPool.emplace_back(&ImageProcess::videoSyncFacialTranning, this, std::ref(_videoFlag), std::ref(_systemOutput));
}


/*------------------------------------------------------------------------------------------------------
 * ��������ע��֮������ѵ������(���ⲿGUI����)
 * @name:  startResnetModelTranning
 * @function: ��������ע��֮������ѵ������
 * @param:  1.��Ƶ���� std::atomic<bool> &
 *                  2. �û�ID������  const std::string& _userID
 *                  3. �û�����������    const std::string& _userName
 *                  4. ������ڽӿڣ�QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void ImageProcess::startResnetModelTranning(
          std::atomic<bool>& _videoFlag,
          const std::string& _userID,
          const std::string& _userName,
          QTextBrowser*& _systemOutput
)
{
          this->m_threadPool.emplace_back(
                    &ImageProcess::modelSetTranning, this,
                    std::ref(_videoFlag), std::ref(_userID),
                    std::ref(_userName), std::ref(_systemOutput)
          );
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