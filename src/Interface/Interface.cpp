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

/*----------------------QTWidget��WorkAttendenceSys�Ĳ����ӿ�-------------------------*/
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
*                   4.��Ա������Ȩ����� QPushButton *&_pushButton
*                   5.������ֵ��ʾ�� : int _displayNumber
*
*@Correction : 2022 - 7 - 24 ��Ӻ��������޸���ֹ�߳��޷���ȷ��ֹͣ��ת
*-----------------------------------------------------------------------------------------------------*/
void Interface::QTtranningSetInput(
          std::atomic<bool>& _videoFlag,
          QTextBrowser*& _systemOutput,
          QProgressBar*& _processBar,
          QPushButton*& _pushButton,
          int _displayNumber
)
{
          _pushButton->setDisabled(true);
          this->startImageTranningSetInput(_videoFlag, _systemOutput, _processBar, _displayNumber);
          _pushButton->setEnabled(true);
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
 *                  4. ע�Ტ¼������ QPushButton *&signin
 *                  5.������ڽӿڣ�QTextBrowser*& _systemOutput
 *
*------------------------------------------------------------------------------------------------------*/
void Interface::QTResnetTranning(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QPushButton*& signin,
          QTextBrowser*& _systemOutput
)
{
          signin->setDisabled(true);
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
          signin->setEnabled(true);
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
                              /*����Ƿ�����ظ��Ŀ��ڼ�¼*/
                              if (!this->readDuplicateRecordFromDB(_userID, _department, AttendanceTable::ATTENDANCE, _timer))
                              {
                                        /*����������ظ��ļ�¼�����¼����ʱ��*/
                                        this->storeAttendanceRecord2DB(                                                                                         //���п���ʱ��ļ�¼
                                                  _userID, _department,
                                                  ((_timer->currentDateTime().time() > this->getMorningShiftTime()) ? "Ա���ٵ�" : "����ǩ��"),
                                                  _timer
                                        );
                                        return;
                              }
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                                        QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("��") +
                                        QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("�ظ���������ǩ��\n")
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
                    /*��������Ϣ�����ݿ�������ȡ����֤�Ƿ����*/
                    std::string dbFaceMatrix = this->readFaceRecordFromDB(_userID, _userName, _department);   //����UserID+UserName+Department�õ���������������ַ���
                    if (dbFaceMatrix == "" || !dbFaceMatrix.size()) {                                                                             //���ص����������ַ���Ϊ��
                              throw EmptyMatrixString();                                                                                                    //MatrixStringΪ�յ��쳣
                    }
                    if (this->startFacialRecognize(dbFaceMatrix, _systemOutput)) {                                                   //�����ݿ�dbFaceMatrix��ʵʱ�������бȶ�
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                                        QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("��") +
                                        QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("����ǩ�˳ɹ�!\n")
                              );
                              /*����Ƿ�����ظ��Ŀ��ڼ�¼*/
                              if (!this->readDuplicateRecordFromDB(_userID, _department, AttendanceTable::SIGNOUT, _timer))
                              {
                                        /*����������ظ��ļ�¼�����¼����ʱ��*/
                                        this->storeSignOutRecord2DB(                                                                                                //���п���ʱ��ļ�¼
                                                  _userID, _department,
                                                  ((_timer->currentDateTime().time() < this->getNightshiftTime()) ? "Ա������" : "����ǩ��"),
                                                  _timer
                                        );
                                        return;
                              }
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                                        QString::fromLocal8Bit(_department.c_str()) + QString::fromLocal8Bit("��") +
                                        QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("�ظ���������ǩ��\n")
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
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("��Ա��Ȩ������ɹ�����ȴ�����Ա������\n")
                    );
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
                    if (authoritySet == "" || !authoritySet.size()) {                                                                                           //û�в�������Ȩ����Ϣ
                              throw EmptyMatrixString();                                                                                                            //��Ч�������Ȩ����Ϣ
                    }
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
          catch (const EmptyMatrixString&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("ϵͳ��û�в�ѯ����Ա����Ȩ��!  ����ϵϵͳ����Ա\n")
                    );
          }
}

/*------------------------------------------------------------------------------------------------------
 *  Interface��ΪQTWidget�����Ա�˻�Ȩ����֤ϵͳ
 * @name: QTAdminManagementLogin
 * @function������Ա�˻�Ȩ����֤ϵͳ
 * @param:
 *                  1. �û�ID������  const std::string& _userID
 *                  2. �û�����������    const std::string& _userName
 *                  3. ���ŵ�����         const std::string & _department
 *                  4. ��ǰ�ɹ���½�Ĺ���ԱID std::string & _currentAdmin,
 *                  5. ��¼������ϵͳ  QPushButton*& _AdministerLogin,
 *                  6. ���ʹ�����ϵͳ�Ŀ������� : QPushButton*& _adminUI
 *                  7. �ر���Ƶ��ʶ������Ŀ������� : QPushButton*& _closeVideo
 *                  8.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
 *                  9. ������ڽӿڣ�QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTAdminManagementLogin(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          std::string & _currentAdmin,
          QPushButton*& _AdministerLogin,
          QPushButton*& _adminUI,
          QPushButton*& _closeVideo,
          QDateTime*& _timer,
          QTextBrowser*& _systemOutput
)
{
          try {
                    if (_userID == "" || _userName == "") {                                                                                                     //ID�����ֲ�����Ϊ��
                              throw  InvalidInput();                                                                                                                       //��Ч�ĵ�¼��Ϣ����
                    }
                    /*��������Ϣ�����ݿ�������ȡ����֤�Ƿ����*/
                    std::string dbFaceMatrix = this->readFaceRecordFromDB(_userID, _userName, _department);   //����UserID+UserName+Department�õ���������������ַ���
                    if (dbFaceMatrix == "" || !dbFaceMatrix.size()) {                                                                             //���ص����������ַ���Ϊ��
                              throw EmptyMatrixString();                                                                                                    //MatrixStringΪ�յ��쳣
                    }
                    if (this->startFacialRecognize(dbFaceMatrix, _systemOutput)) {                                                   //�����ݿ�dbFaceMatrix��ʵʱ�������бȶ�
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                                        QString::fromLocal8Bit("����Ա") +QString::fromLocal8Bit(_userName.c_str()) +
                                        QString::fromLocal8Bit("��¼�ɹ�! ���Է��ʹ�����ϵͳ\n")
                              );
                              _currentAdmin = _userID;                           //���µ�ǰ�Ĺ���Ա��Ϣ
                              _AdministerLogin->setDisabled(true);         //���õ�¼������ϵͳ
                              _adminUI->setEnabled(true);                       //��½�ɹ�������ʹ���UI����
                              _closeVideo->setEnabled(true);                    //����ʹ�ù���Ա����
                              _AdministerLogin->update();                       //����
                              _adminUI->update();                                     //����
                              _closeVideo->update();                                 //����
                    }
                    else {
                              _systemOutput->insertPlainText(
                                        QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +QString::fromLocal8Bit("]:") + 
                                        QString::fromLocal8Bit("����Ա") + QString::fromLocal8Bit(_userName.c_str()) +
                                        QString::fromLocal8Bit("����ʶ��ʧ��!�����³���\n")
                              );
                              _adminUI->setDisabled(true);                       //��½�ɹ�������ʹ���UI����
                              _closeVideo->setDisabled(true);                    //����ʹ�ù���Ա����
                              _adminUI->update();                                     //����
                              _closeVideo->update();                                 //����
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
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("ϵͳ��û�в�ѯ����ǰ����Ա��Ϣ! \n")
                    );
          }
}

/*---------------------QTWidget��WorkAttendenceAdmin�Ĳ����ӿ�----------------------*/
/*------------------------------------------------------------------------------------------------------
 *  Interface��ΪQTWidget�����Ա�˻��ṩ�ļ�¼��ѯ�ӿ�
 * @name: QTAdminStatisticsInterface
 * @function������Ա�˻��ṩ�ļ�¼��ѯ�ӿ�
 * @param:
 *                  1. �û�ID������  const std::string& _userID
 *                  2. �û�����������    const std::string& _userName
 *                  3. ���ŵ�����         const std::string & _department
 *                  4. ǩ����ť: QRadioButton *& _attdenceTable
 *                  5. ǩ�˰�ť: QRadioButton*& _signoutTable
 *                  6. �Ƿ�ѡ��ʱ��: QCheckBox *& _isTimeEnabled
 *                  7. ������ʱ�� :   const QDateTime _lefttimer
 *                  8. �Ҳ�����ʱ�ӣ� const QDateTime _righttimer
 *                  9.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
 *                  10. ����Table�����ķ�װ�������� DataDisplay   DataDisplay* m_dataDisplay
 *                  11. ������ڽӿڣ�QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTAdminStatisticsInterface(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QRadioButton*& _attdenceTable,
          QRadioButton*& _signoutTable,
          QCheckBox*& _isTimeEnabled,
          const QDateTime _lefttimer,
          const QDateTime _righttimer,
          QDateTime*& _timer,
          DataDisplay* _dataDisplay,
          QTextBrowser*& _systemOutput
)
{
          try
          {
                    if (_attdenceTable->isChecked() && _signoutTable->isChecked()) {      //��鵥ѡ��ť�Ƿ񶼱���ѡ
                              throw ThrowInvalidQRadio();
                    }
                    if (!_attdenceTable->isChecked() && !_signoutTable->isChecked()) {      //��鵥ѡ��ť�Ƿ�û����ѡ
                              throw ThrowInvalidQRadio();
                    }
                    if (_isTimeEnabled->isChecked()) {                                                         //�Ƿ�ѡ��ʱ��
                              if (_lefttimer > _righttimer) {                                                          //��ߵ����ڴ����ұߵ�����
                                        throw   InvalidDateTime();
                              }
                    }
                    //��ȡ��¼��Ԫ��
                    std::vector<std::vector<std::string>> recordRes = this->readAttendenceRecord(
                              _userID, _userName, _department, _lefttimer, _righttimer,
                              _attdenceTable->isChecked() ? AttendanceTable::ATTENDANCE : AttendanceTable::SIGNOUT,
                              _isTimeEnabled->isChecked()
                    );

                    /*�����Բ��ͼ������*/
                    double _total(0.0f);                                                                                                        //������                                                                                  
                    double _late(0.0f);                                                                                                          //�ٵ��ϰ� / �����°�����
                    double _normal (0.0f);                                                                                              // ��������

                    this->QTAdminAttendenceRateCalc(                                                                           //�����ͼ�е�����
                              _userID, _userName, _department, _lefttimer, _righttimer, _timer,
                              _attdenceTable->isChecked() ? AttendanceTable::ATTENDANCE : AttendanceTable::SIGNOUT,
                              _isTimeEnabled->isChecked(),
                              _total, _late, _normal
                    );
                   this->queryUserNameFromID(recordRes);                                                                             //�ڶ�Ԫ�鼯���и���UserID��ѯ�û�����
                   _dataDisplay->setAttendenceTableTuple(recordRes);                                                               //����Ԫ�鼯����ʾ
                   _dataDisplay->setChartViewData(_attdenceTable->isChecked(), _total, _late, _normal);  //��ʾ��ǰ״̬�µı�ͼ
          }
          catch (const ThrowInvalidQRadio&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("��ǰ��ѡ��ѡ��Ƿ��������²���\n")
                    );
          }
          catch (const InvalidDateTime&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("��ǰʱ�䷶Χ����Ƿ��������²���\n")
                    );
          }
          catch (const EmptyMatrixString&) {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit(_userName.c_str()) + QString::fromLocal8Bit("ϵͳ��û�в�ѯ����ǰ�û���Ϣ! \n")
                    );
          }
}

/*------------------------------------------------------------------------------------------------------
 *  Interface��ΪQTWidget�����Ա�˻���ʾ��Ա���嵥
 * @name: QTGetNewEmployeeInterface
 * @param :  ����Table�����ķ�װ�������� DataDisplay   DataDisplay* m_dataDisplay
 * @function������Ա�˻��ṩ�ļ�¼��ѯ�ӿ�
*------------------------------------------------------------------------------------------------------*/
void Interface::QTGetNewEmployeeInterface(DataDisplay* _dataDisplay)
{
          std::vector<std::vector<std::string>> recordRes = this->readNewEmployeeRecord();
          _dataDisplay->setNewEmployeeTableTuple(recordRes);                                                               //����Ԫ�鼯����ʾ
}

/*------------------------------------------------------------------------------------------------------
 *  Interface��ΪQTWidget�����Ա�˻���ʾ��Ա���嵥
 * @name: QTGetNewEmployeeInterface
 * @param :      1.�Ƿ�ͨ�� bool status
 *                       2.�����ͼ��QTableView *& _newEmployeeTable
 *                       3.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
 *                       4.������ڽӿڣ�QTextBrowser*& _systemOutput
 *
 * @function������Ա�˻��ṩ�ļ�¼��ѯ�ӿ�
*------------------------------------------------------------------------------------------------------*/
void Interface::QTDeniedAndApprove(
          bool status, 
          QTableView*& _newEmployeeTable,
          QDateTime*& _timer,
          QTextBrowser*& _systemOutput
)
{
          QList<QModelIndex> list = _newEmployeeTable->selectionModel()->selectedIndexes();
          if (list.isEmpty()) {                                                                                                                       //û��ѡ���κ���Ϣ
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                               QString::fromLocal8Bit("û��ѡ����Ҫ������Ա��! \n")
                    );
                    return;
          }
          for (auto i : list) {                                                                                                                       //����ѡ����
                    auto UserID = _newEmployeeTable->model()->index(i.row(), 0).data();
                    auto UserName = _newEmployeeTable->model()->index(i.row(), 1).data();
                    this->updateNewEmployeeAuth(
                              UserID.toString().toLocal8Bit().constData(), 
                              UserName.toString().toLocal8Bit().constData(), 
                              status
                    );
          }
}

/*------------------------------------------------------------------------------------------------------
 *  Interface��ΪQTWidget�����Ա�˻��ṩ�ĳ����ʼ���ϵͳ
 * @name: QTAdminAttendenceRateCalc
 * @function������Ա�˻��ṩ�ĳ����ʼ���ϵͳ
 * @param: 1. Ա���ţ� const  std::string& employeeNumber
*                  2. ������ const  std::string& _name
*                  3. ���� ��  const std::string& _department
*                  4. ��ʱ�䣺 QDateTime _leftTimer
*                  5. �Ҳ�ʱ�䣺 QDateTime _rightTimer
*                  6.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
*                  7. ѡ��ǩ��ǩ�˼�¼��AttendanceTable tableSelect
*                  8. �Ƿ�ѡ��ʱ��: bool _isTimeEnabled
 *                 9. ��������double &_total
 *                 10. �ٵ��ϰ�/�����°�������double &_late
 *                 11. ����������double& normal
*------------------------------------------------------------------------------------------------------*/
void Interface::QTAdminAttendenceRateCalc(
          const std::string& _userID,
          const std::string& _userName,
          const std::string& _department,
          QDateTime _leftTimer,
          QDateTime _rightTimer,
          QDateTime*& _timer,
          AttendanceTable tableSelect,
          bool _isTimeEnabled,
          double& _total, double& _late, double& normal
)
{
          if (!_isTimeEnabled) 
          {
                     _leftTimer = getMinimiseTime(tableSelect);               //�����ʱ��
                     _rightTimer = _timer->currentDateTime();                   //��ǰ����ʱ��
          }
          auto daysCount = _leftTimer.daysTo(_rightTimer);                //����ʱ���(��������)��ǰ����ʱ�� - �����ʱ�����Ա������
          _total = static_cast<double>(
                    (!daysCount ? 1.0f : daysCount) * this->getEmployeeCount(_userID, _userName, _department)  
           );

          _late = static_cast<double>(this->adminStatisticsOnDutyCount(
                    _userID, _userName, _department,
                    tableSelect, _leftTimer, _rightTimer,
                    tableSelect == AttendanceTable::ATTENDANCE ? "Ա���ٵ�" : "Ա������"
          ));
          normal = static_cast<double>(this->adminStatisticsOnDutyCount(
                    _userID, _userName, _department,
                    tableSelect, _leftTimer, _rightTimer,
                    tableSelect == AttendanceTable::ATTENDANCE ? "����ǩ��" : "����ǩ��"
          ));
}

/*------------------------------------------------------------------------------------------------------
 *  Interface��ΪQTWidget�����Ա�˻��ṩ������ϵͳ����
 * @name: QTAdminParamSettingInterface
 * @function������Ա�˻��ṩ�ļ�¼��ѯ�ӿ�
 * @param:
 *                  1.ѵ�������趨��const std::string& _TranningSet,
 *                  2.ѵ�����ƶȣ�const std::string& _TranningSimilarity,
 *                  3.�ٵ�ʱ�䣺const std::string& _LateTimeSet,
 *                  4.����ʱ�䣺const std::string& _LeaveEarilyTimeSet
 *                  5.�Ѿ���½��ADMIN ID��const std::string & _loggedUserID
 *                  6.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
 *                  7. ������ڽӿڣ�QTextBrowser*& _systemOutput
*------------------------------------------------------------------------------------------------------*/
void Interface::QTAdminParamSettingInterface(
          const std::string& _TranningSet,
          const std::string& _TranningSimilarity,
          const std::string& _LateTimeSet,
          const std::string& _LeaveEarilyTimeSet,
          const std::string& _loggedUserID,
          QDateTime*& _timer,
          QTextBrowser*& _systemOutput
)
{
          if (this->updateAdminParam2DB(_TranningSet, _TranningSimilarity, _LateTimeSet, _LeaveEarilyTimeSet, _loggedUserID))
          {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("ȫ��ϵͳ�����趨���óɹ�! \n")
                    );
          }
          else
          {
                    _systemOutput->insertPlainText(
                              QString::fromLocal8Bit("[") + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit("]:") +
                              QString::fromLocal8Bit("ȫ��ϵͳ�����趨����ʧ��! \n")
                    );
          }
}