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
           * @param 1.������ʾ�������� QLCDNumber*& _qlcd
           *                2.ȫ��ʱ��ϵͳ������ QTime*& _timer
           * 
           *  @Correction: 2022-8-2 ����ȫ�ֵ�ʱ��ϵͳ
          *-----------------------------------------------------------------------------------------------------*/
          void QTsetLcdTimer(QLCDNumber*& _qlcd, QTime*& _timer);

          /*-------------------------------QTWidget�����Ĳ����ӿ�----------------------------------*/
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
          *                   4.������ֵ��ʾ�� : int _displayNumber
          *
          *@Correction : 2022 - 7 - 24 ��Ӻ��������޸���ֹ�߳��޷���ȷ��ֹͣ��ת
          *-----------------------------------------------------------------------------------------------------*/
          void QTtranningSetInput(
                    std::atomic<bool>& _videoFlag,
                    QTextBrowser*& _systemOutput,
                    QProgressBar*& _processBar,
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
           *                  3. ������ڽӿڣ�QTextBrowser*& _systemOutput
           * 
          *------------------------------------------------------------------------------------------------------*/
          void QTResnetTranning(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
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
           *                  4.ȫ��ʱ��ϵͳ������ QTime*& _timer
           *                  5. ������ڽӿڣ�QTextBrowser*& _systemOutput
          *------------------------------------------------------------------------------------------------------*/
          void QTEmployeeAttendance(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QTime *& _timer,
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
           *                  4.ȫ��ʱ��ϵͳ������ QTime*& _timer
           *                  5. ������ڽӿڣ�QTextBrowser*& _systemOutput
          *------------------------------------------------------------------------------------------------------*/
          void QTEmployeeSignOut(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QTime*& _timer,
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
           *                  3. ������ڽӿڣ�QTextBrowser*& _systemOutput
          *------------------------------------------------------------------------------------------------------*/
          void QTEmployeeAskPremit(
                    const std::string& _userID,
                    const std::string& _userName,
                    const std::string& _department,
                    QTextBrowser*& _systemOutput
          );

protected:
          std::atomic<bool> m_videoFlag = false;                      //��Ƶ����
};