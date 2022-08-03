#include"Interface.h"

Interface::Interface()
          :DBProcess(),
          ImageProcess(this->initTranningSetting(), this->initTrainningSimilarity())
{
          //�ȳ�ʼ��DBProcess���ݿ�����ٳ�ʼ��ImageProcess
          //��ʱ��ImageProcess���Ѿ����ڲ������������ٴγ�ʼ��!!!
}

Interface::~Interface()
{
}

/*-------------------------------QTWidget�����ķ�װ�ӿ�----------------------------------*/
/*------------------------------------------------------------------------------------------------------
 *  QLabelͼ�㸲д����
 * @name: QTsetLabelImage
 * @function����MAT����ת��ΪQT��QImage���Ͳ����ǵ�QLabel������
 * @param 1.������ֵ���͵� QImage&
 *                2.������ʾͼ���ָ��_qlabel
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTsetLabelImage(QImage& _qimage, QLabel*& _qlabel)
{
          _qimage = _qimage.scaled(_qlabel->width(), _qlabel->height());                 //����ؼ��ֱ���
          _qlabel->setPixmap(QPixmap::fromImage(_qimage));
}

/*------------------------------------------------------------------------------------------------------
 *  QLCDNumberͼ���ʱ����ʾ����
 * @name: QTsetLabelImage
 * @function����QTimer������Ϊ�¼��������ʹ��뵽����QLCDNumber��
 * @param 1.date�������� QLCDNumber*& _date
 *                2.ʱ���������� QLCDNumber*& _realTimeClock
 *                3.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
 *
 *  @Correction: 2022-8-2 ����ȫ�ֵ�ʱ��ϵͳ
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTsetLcdTimer(
          QLCDNumber*& _date,
          QLCDNumber*& _realTimeClock,
          QDateTime*& _timer
)
{
          _date->setDigitCount(10);                                                //���þ���ܿؼ�QLCDNumber����ʾ��λ��
          _date->setMode(QLCDNumber::Dec);                            //������ʾ��ģʽΪʮ����
          _date->setSegmentStyle(QLCDNumber::Flat);               //������ʾ��ʽ
          _realTimeClock->setDigitCount(10);                                                //���þ���ܿؼ�QLCDNumber����ʾ��λ��
          _realTimeClock->setMode(QLCDNumber::Dec);                            //������ʾ��ģʽΪʮ����
          _realTimeClock->setSegmentStyle(QLCDNumber::Flat);               //������ʾ��ʽ
          while (!this->m_videoFlag)
          {
                    _date->display(_timer->currentDateTime().toString("yyyy-MM-dd"));
                    _realTimeClock->display(_timer->currentDateTime().toString("hh:mm:ss"));
          }
}

/*-------------------------------QTWidget�����Ĳ����ӿ�----------------------------------*/
/*------------------------------------------------------------------------------------------------------
 *  Interface��ΪQTWidget���ṩ����Ƶ�رպ���
 * @name: QTcloseVideo
 * @function�������ǹر���Ƶ�е�ѭ������
*-----------------------------------------------------------------------------------------------------*/
void  Interface::QTcloseVideo()
{
          m_videoFlag = true;
}

/*------------------------------------------------------------------------------------------------------
 *   Interface��ΪQTWidget�㿪����ǰ֡���ղ���
 * @name: QTsavePicture
 * @function�����������㵱ǰ�ĵ�֡����
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTtakePicture()
{
          this->enableCameraShooting();
}

/*------------------------------------------------------------------------------------------------------
 *   Interface��ΪQTWidget��������߳��л�ȡͼƬ������ѵ����
 * @name: QTtranningSetInput
 * @function������������ʵʱ��������ѵ����������
*@param:    1.��Ƶ���� std::atomic<bool> &
*                   2.������ڽӿڣ�QTextBrowser * &_systemOutput
*                   3.������������ӿڣ�QProgressBar * &_processBar
*                   4.������ֵ��ʾ�� : int _displayNumber
*
*@Correction : 2022 - 7 - 24 ��Ӻ��������޸���ֹ�߳��޷���ȷ��ֹͣ��ת
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTtranningSetInput(
          std::atomic<bool>& _videoFlag,
          QTextBrowser*& _systemOutput,
          QProgressBar*& _processBar,
          int _displayNumber
)
{
          this->startImageTranningSetInput(_videoFlag, _systemOutput, _processBar, _displayNumber);
}

/*------------------------------------------------------------------------------------------------------
 *   Interface��ΪQTWidget�㿪����ǰ֡�ı������
 * @name: videoFrameSavingProcess
 * @function�������Ǳ��浱ǰ�ĵ�֡����
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTsaveImage()
{
          this->enableSavingProcess();
}

/*------------------------------------------------------------------------------------------------------
 *   Interface��ΪQTWidget�㿪����ǰ֡����������
 * @name: videoFrameIgnoreProcess
 * @function��������������ǰ�ĵ�֡����
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTignoreImage()
{
          this->enableIgnoreProcess();
}

/*------------------------------------------------------------------------------------------------------
 *   Interface��ΪQTWidget����Ƶ�����������
 * @name: QTVideoOutput
 * @function������������ʾͼ������ʾ��ͼ��
 * @param: 1. ��Ƶ���pixmap : QLabel*& _qlabel
 *                 2. ������ڽӿڣ�QTextBrowser*& _systemOutput
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTVideoOutput(QLabel*& _qlabel, QTextBrowser*& _systemOutput)
{
          while (!m_videoFlag) {
                    this->QTsetLabelImage(this->startVideoDisplayThread(_systemOutput), _qlabel);                       //����ؼ��ֱ���
          }
          _qlabel->clear();
}

/*------------------------------------------------------------------------------------------------------
 *   Interface��ΪQTWidget�������ѵ����ѵ������
 * @name: QTResnetTranning
 * @function�������ѵ����ѵ�����أ������⽫��Ϣд�����ݿ�
 * @param:
 *                  1. �û�ID������  const std::string& _userID
 *                  2. �û�����������    const std::string& _userName
 *                  3. ���ŵ�����         const std::string & _department
 *                  3. ������ڽӿڣ�QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTResnetTranning(
          const std::string & _userID,
           const std::string & _userName,
          const std::string & _department,
          QTextBrowser * &_systemOutput
)
{
          std::string& r_faceMatrix(this->startResnetModelTranning(_systemOutput));       //��ȡ�����������ֵ
          if (this->storeFaceRecord2DB(_userID, _userName, _department, r_faceMatrix)){   //�����ݴ洢�����ݿ�
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("��") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("������Ϣ�洢�ɹ�!\n")
                    );
          }
          else {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("��") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("������Ϣ�洢ʧ��!\n")
                    );
          }
}


/*------------------------------------------------------------------------------------------------------
 *  Interface��ΪQTWidget�����������ǩ������
 * @name: QTEmployeeAttendance
 * @function�������ݿ����ҵ���Ӧ���������������ַ��������ݵ�ǰ��ʵʱ�������жԱ�
 *                      ���������������Ϣ���뵽ʵʱ����ͷ����ʾ��
 *
 * @param:
 *                  1. �û�ID������  const std::string& _userID
 *                  2. �û�����������    const std::string& _userName
 *                  3. ���ŵ�����         const std::string & _department
 *                  4.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
 *                  5. ������ڽӿڣ�QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTEmployeeAttendance(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QDateTime*& _timer,
          QTextBrowser*& _systemOutput
)
{
          try{
                    if (_userID == "" || _userName == "") {                                                                                                     //ID�����ֲ�����Ϊ��
                              throw  InvalidInput();                                                                                                                       //��Ч�ĵ�¼��Ϣ����
                    }
                    else
                    {
                              /*��������Ϣ�����ݿ�������ȡ����֤�Ƿ����*/
                              std::string dbFaceMatrix = this->readFaceRecordFromDB(_userID, _userName, _department);   //����UserID+UserName+Department�õ���������������ַ���
                              if (dbFaceMatrix == "" || !dbFaceMatrix.size()) {                                                                             //���ص����������ַ���Ϊ��
                                        throw EmptyMatrixString();                                                                                                    //MatrixStringΪ�յ��쳣
                              }
                              if (this->startFacialRecognize(dbFaceMatrix, _systemOutput)) {                                                   //�����ݿ�dbFaceMatrix��ʵʱ�������бȶ�
                                        _systemOutput->insertPlainText(
                                                  QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                                                  QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("��") +
                                                  QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("������½�ɹ�!\n")
                                        );
                                        this->storeAttendanceRecord2DB(                                                                                         //���п���ʱ��ļ�¼
                                                  _userID, _department,
                                                  ((_timer->currentDateTime().time() > this->getMorningShiftTime()) ? "Ա���ٵ�" : "����ǩ��"),
                                                  _timer
                                        );
                              }
                              else {
                                        _systemOutput->insertPlainText(
                                                  QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                                                  QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("��") +
                                                  QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("����ʶ��ʧ��!�����³���\n")
                                        );
                              }
                    }
          }
          catch (const InvalidInput&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("��ǰ�����к��зǷ��ַ�������������\n")
                    );
          }
          catch (const EmptyMatrixString &) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("���޴��ˣ�������½ʧ��!  ����ϵϵͳ����Ա\n")
                    );
          }
}

/*------------------------------------------------------------------------------------------------------
 *  Interface��ΪQTWidget������������ǳ�����
 * @name: QTEmployeeSignOut
 * @function�������ݿ����ҵ���Ӧ���������������ַ��������ݵ�ǰ��ʵʱ�������жԱ�
 *                      ���������������Ϣ���뵽ʵʱ����ͷ����ʾ��
 *
 * @param:
 *                  1. �û�ID������  const std::string& _userID
 *                  2. �û�����������    const std::string& _userName
 *                  3. ���ŵ�����         const std::string & _department
 *                  4.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
 *                  5. ������ڽӿڣ�QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTEmployeeSignOut(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QDateTime*& _timer,
          QTextBrowser*& _systemOutput
)
{
          try {
                    if (_userID == "" || _userName == "") {                                                                                                     //ID�����ֲ�����Ϊ��
                              throw  InvalidInput();                                                                                                                       //��Ч�ĵ�¼��Ϣ����
                    }
                    else
                    {
                              /*��������Ϣ�����ݿ�������ȡ����֤�Ƿ����*/
                              std::string dbFaceMatrix = this->readFaceRecordFromDB(_userID, _userName, _department);   //����UserID+UserName+Department�õ���������������ַ���
                              if (dbFaceMatrix == "" || !dbFaceMatrix.size()) {                                                                             //���ص����������ַ���Ϊ��
                                        throw EmptyMatrixString();                                                                                                    //MatrixStringΪ�յ��쳣
                              }
                              if (this->startFacialRecognize(dbFaceMatrix, _systemOutput)) {                                                   //�����ݿ�dbFaceMatrix��ʵʱ�������бȶ�
                                        _systemOutput->insertPlainText(
                                                  QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss")  + QString::fromLocal8Bit("]:") +
                                                  QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("��") +
                                                  QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("������½�ɹ�!\n")
                                        );
                                        this->storeSignOutRecord2DB(                                                                                                //���п���ʱ��ļ�¼
                                                  _userID, _department,
                                                  ((_timer->currentDateTime().time() < this->getNightshiftTime()) ? "Ա������" : "����ǩ��"),
                                                  _timer
                                        );
                              }
                              else {
                                        _systemOutput->insertPlainText(
                                                  QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +
                                                  QString::fromLocal8Bit("]:") + QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("��") +
                                                  QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("����ʶ��ʧ��!�����³���\n")
                                        );
                              }
                    }
          }
          catch (const InvalidInput&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("��ǰ�����к��зǷ��ַ�������������\n")
                    );
          }
          catch (const EmptyMatrixString&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("���޴��ˣ�������½ʧ��!  ����ϵϵͳ����Ա\n")
                    );
          }
}

/*------------------------------------------------------------------------------------------------------
 *  Interface��ΪQTWidget���������Ա��ע��Ȩ������
 * @name:QTEmployeeAskPremit
 * @function�������ݿ���д��������Ϣ�ȴ�����Ա���о���
 * @param:
 *                  1. �û�ID������  const std::string& _userID
 *                  2. �û�����������    const std::string& _userName
 *                  3. ���ŵ�����         const std::string & _department
 *                  4.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
 *                  5. ������ڽӿڣ�QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void  Interface::QTEmployeeAskPremit(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QDateTime*& _timer,
          QTextBrowser*& _systemOutput
)
{
          try {
                    if (_userID == "" || _userName == "") {                                                                                                     //ID�����ֲ�����Ϊ��
                              throw  InvalidInput();                                                                                                                       //��Ч�ĵ�¼��Ϣ����
                    }
                    this->storeAskPremitRecord2DB(_userID, _userName, _department, _timer);                             //�������Ȩ��
          }
          catch (const InvalidInput&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("��ǰ�����к��зǷ��ַ�������������\n")
                    );
          }
}

/*------------------------------------------------------------------------------------------------------
 *  Interface��ΪQTWidget��������Ա���Ĳ���Ȩ��
 * @name:QTEmployeeCheckPremittion
 * @function�������ݿ���д��������Ϣ�ȴ�����Ա���о���
 * @param:
 *                  1. �û�ID������  const std::string& _userID
 *                  2. �û�����������    const std::string& _userName
 *                  3. ���ŵ�����         const std::string & _department
 *                  4. ע�Ტ¼�������Ŀ������� : QPushButton *&_TrainningSetInput,
 *                  5.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
 *                  6 �����ݿ��е�Ȩ����Ϣд�뵽��Ļ�� :QTextBrowser*& _privliedgeStatus
 *                  7. ������ڽӿڣ�QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTEmployeeCheckPremittion(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QPushButton*& _TrainningSetInput,
          QDateTime*& _timer,
          QTextBrowser*& _privliedgeStatus,
          QTextBrowser*& _systemOutput
)
{
          try {
                    if (_userID == "" || _userName == "") {                                                                                                     //ID�����ֲ�����Ϊ��
                              throw  InvalidInput();                                                                                                                       //��Ч�ĵ�¼��Ϣ����
                    }
                    std::string authoritySet = this->checkPremitRecordFromDB(_userID, _userName, _department);         //Ȩ����Ϣ
                    _privliedgeStatus->insertPlainText(QString::fromLocal8Bit(authoritySet.c_str()));
                    if (authoritySet == "����ͨ��") {
                              _TrainningSetInput->setEnabled(true);                              //����ע��������ť                                                      
                              _TrainningSetInput->update();                                           //����ע��������ť״̬
                    }
                    else {
                              _TrainningSetInput->setDisabled(true);                            //����ע�Ტ����������ť   
                              _TrainningSetInput->update();                                          //����ע�Ტ����������ť״̬
                    }
          }
          catch (const InvalidInput&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("��ǰ�����к��зǷ��ַ�������������\n")
                    );
          }
}