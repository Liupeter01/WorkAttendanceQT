#include"../DataDisplay/TableViewDisplay.h"
#include"../ImageProcess/ImageProcess.h"
#include"../DBProcess/DBProcess.h"

/*------------------------------------------------------------------------------------------------------
 * �յ�MatrixString������
 * @name:class EmptyMatrixString
* @function��String�������ǿյ�
*------------------------------------------------------------------------------------------------------*/
class EmptyMatrixString {
public:
          /*------------------------------------------------------------------------------------------------------
          * ���ص�ǰ��String������Ϊ��
          * @name: what
          * @RetValue:  std::string�����ַ���
          *------------------------------------------------------------------------------------------------------*/
          std::string what() {
                    return std::string("��ǰ��String������Ϊ��");
          }
};

/*------------------------------------------------------------------------------------------------------
 * ��Ч�ĵ�¼��Ϣ����
 * @name:class InvalidInput
* @function��String�������ǿյ�
*------------------------------------------------------------------------------------------------------*/
class InvalidInput {
public:
          /*------------------------------------------------------------------------------------------------------
          * ��Ч�ĵ�¼��Ϣ����
          * @name: what
          * @RetValue:  std::string�����ַ���
          *------------------------------------------------------------------------------------------------------*/
          std::string what() {
                    return std::string("��ǰ��String������������");
          }
};

/*------------------------------------------------------------------------------------------------------
 * ��Ч��QRadio������߶���QRadio������
 * @name:class ThrowInvalidQRadio
* @function����Ч��QRadio�����QRadio����
*------------------------------------------------------------------------------------------------------*/
class ThrowInvalidQRadio{
public:
          /*------------------------------------------------------------------------------------------------------
          * ��Ч��QRadio�����QRadio����
          * @name: what
          * @RetValue:  std::string�����ַ���
          *------------------------------------------------------------------------------------------------------*/
          std::string what() {
                    return std::string("��Ч��QRadio�����QRadio����");
          }
};

/*------------------------------------------------------------------------------------------------------
 * �����DateTime��Ч
 * @name:class InvalidDateTime 
* @function����ߵ����ڴ����ұߵ�����
*------------------------------------------------------------------------------------------------------*/
class InvalidDateTime {
public:
          /*------------------------------------------------------------------------------------------------------
          * ��Ч��QRadio�����QRadio����
          * @name: what
          * @RetValue:  std::string�����ַ���
          *------------------------------------------------------------------------------------------------------*/
          std::string what() {
                    return std::string("��ߵ����ڴ����ұߵ�����");
          }
};

class Interface :public DBProcess, public ImageProcess {
public:
          Interface();
          virtual ~Interface();

public:
          /*-------------------------------QTWidget�����ķ�װ�ӿ�----------------------------------*/
          /*------------------------------------------------------------------------------------------------------
           *  QLabelͼ�㸲д����
           * @name: QTsetLabelImage
           * @function����MAT����ת��ΪQT��QImage���Ͳ����ǵ�QLabel������
           * @param 1.������ֵ���͵� QImage&
           *                2.������ʾͼ���ָ��_qlabel
          *-----------------------------------------------------------------------------------------------------*/
          void QTsetLabelImage(QImage& _qimage, QLabel*& _qlabel);

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
          void QTsetLcdTimer(
                    QLCDNumber*& _date,
                    QLCDNumber*& _realTimeClock,
                    QDateTime*& _timer
          );

          /*-----------------------QTWidget��WorkAttendenceSys�Ĳ����ӿ�------------------------*/
          /*------------------------------------------------------------------------------------------------------
           *  Interface��ΪQTWidget���ṩ����Ƶ�رպ���
           * @name: QTcloseVideo
           * @function�������ǹر���Ƶ�е�ѭ������
          *-----------------------------------------------------------------------------------------------------*/
          void  QTcloseVideo(); 

          /*------------------------------------------------------------------------------------------------------
           *   Interface��ΪQTWidget�㿪����ǰ֡���ղ���
           * @name: QTsavePicture
           * @function�����������㵱ǰ�ĵ�֡����
          *-----------------------------------------------------------------------------------------------------*/
          void QTtakePicture();

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
          void QTtranningSetInput(
                    std::atomic<bool>& _videoFlag,
                    QTextBrowser*& _systemOutput,
                    QProgressBar*& _processBar,
                    QPushButton*& _pushButton,
                    int _displayNumber
          );

          /*------------------------------------------------------------------------------------------------------
           *   Interface��ΪQTWidget�㿪����ǰ֡�ı������
           * @name: videoFrameSavingProcess
           * @function�������Ǳ��浱ǰ�ĵ�֡����
          *-----------------------------------------------------------------------------------------------------*/
          void QTsaveImage();

          /*------------------------------------------------------------------------------------------------------
           *   Interface��ΪQTWidget�㿪����ǰ֡����������
           * @name: videoFrameIgnoreProcess
           * @function��������������ǰ�ĵ�֡����
          *-----------------------------------------------------------------------------------------------------*/
          void QTignoreImage();

          /*------------------------------------------------------------------------------------------------------
           *   Interface��ΪQTWidget����Ƶ�����������
           * @name: QTVideoOutput
           * @function������������ʾͼ������ʾ��ͼ��
           * @param: 1. ��Ƶ���pixmap : QLabel*& _qlabel
           *                 2. ������ڽӿڣ�QTextBrowser*& _systemOutput
          *-----------------------------------------------------------------------------------------------------*/
          void QTVideoOutput(QLabel*& _qlabel, QTextBrowser*& _systemOutput);

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
          void QTResnetTranning(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QPushButton*& signin,
                    QTextBrowser*& _systemOutput
          );

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
          void QTEmployeeAttendance(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QDateTime*& _timer,
                    QTextBrowser*& _systemOutput
          );

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
          void QTEmployeeSignOut(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QDateTime*& _timer,
                    QTextBrowser*& _systemOutput
          );

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
          void QTEmployeeAskPremit(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QDateTime*& _timer,
                    QTextBrowser*& _systemOutput
          );

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
          void QTEmployeeCheckPremittion(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QPushButton *&_TrainningSetInput,
                    QDateTime*& _timer,
                    QTextBrowser*& _privliedgeStatus,
                    QTextBrowser*& _systemOutput
          );

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
          void QTAdminManagementLogin(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    std::string& _currentAdmin,
                    QPushButton*& _AdministerLogin,
                    QPushButton*& _adminUI,
                    QPushButton*& _closeVideo,
                    QDateTime*& _timer,
                    QTextBrowser*& _systemOutput
          );

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
          void QTAdminStatisticsInterface(
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
          );

          /*------------------------------------------------------------------------------------------------------
           *  Interface��ΪQTWidget�����Ա�˻���ʾ��Ա���嵥
           * @name: QTGetNewEmployeeInterface
           * @param :  ����Table�����ķ�װ�������� DataDisplay   DataDisplay* m_dataDisplay
           * @function������Ա�˻��ṩ�ļ�¼��ѯ�ӿ�
          *------------------------------------------------------------------------------------------------------*/
          void QTGetNewEmployeeInterface(DataDisplay* _dataDisplay);

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
          void QTDeniedAndApprove(
                    bool status,
                    QTableView*& _newEmployeeTable,
                    QDateTime*& _timer,
                    QTextBrowser*& _systemOutput
          );

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
          void QTAdminAttendenceRateCalc(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QDateTime _leftTimer,
                    QDateTime _rightTimer,
                    QDateTime*& _timer,
                    AttendanceTable tableSelect,
                    bool _isTimeEnabled,
                    double& _total, double& _late, double& normal
          );

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
          void QTAdminParamSettingInterface(
                    const std::string& _TranningSet,
                    const std::string& _TranningSimilarity,
                    const std::string& _LateTimeSet,
                    const std::string& _LeaveEarilyTimeSet,
                    const std::string& _loggedUserID,
                    QDateTime*& _timer,
                    QTextBrowser*& _systemOutput
          );

protected:
          std::atomic<bool> m_videoFlag = false;                      //��Ƶ����
};