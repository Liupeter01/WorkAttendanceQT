#include<QtCore/qdatetime.h>
#include"MYSQL.h"

enum class AttendanceTable{
          ATTENDANCE,                             //ǩ����
          SIGNOUT                                      //ǩ�˱�
};

class DBProcess : public MySQL {
public:
          DBProcess();
          virtual ~DBProcess();
          
protected:
          /*------------------------WorkAttendanceSys����ϵͳ��ʼ��-----------------------------*/
         /*------------------------------------------------------------------------------------------------------
          * @DBProcess����ϵͳϵͳ�����趨initDepartmentSetting
          * @name :  initDepartmentSetting
          * @funtion : ��ʼ��ϵͳ�����趨DepartmentSetting
          * @RetValue : ���ز��ŵ���Ϣ std::vector<std::string>
          *------------------------------------------------------------------------------------------------------*/
          std::vector<std::string> initDepartmentSetting();

          /*------------------------------------------------------------------------------------------------------
          * @DBProcess����ϵͳѵ���������趨TranningSetting
          * @name :  initTranningSetting
          * @funtion : ��ʼ��ѵ���������趨TranningSetting
          * @RetValue : int
          *------------------------------------------------------------------------------------------------------*/
          int initTranningSetting();

          /*------------------------------------------------------------------------------------------------------
          * @DBProcess����ϵͳ��������ƥ�����TranningSimilarity
          * @name :  initTrainningSimilarity
          * @funtion : ��������ƥ�����std::length 
          * @RetValue : double
          *------------------------------------------------------------------------------------------------------*/
          double initTrainningSimilarity();

          /*------------------------------------------------------------------------------------------------------
          * @DBProcess����ϵͳ��ȡ����Ա�趨�����ϳٵ���ʱ��
          * @name :  getMorningShiftTime
          * @funtion : ��ȡ����Ա�趨�����ϳٵ���ʱ��
          *------------------------------------------------------------------------------------------------------*/
          QTime getMorningShiftTime();

          /*------------------------------------------------------------------------------------------------------
          * @DBProcess����ϵͳ��ȡ����Ա�趨���������˵�ʱ��
          * @name :  getNightshiftTime
          * @funtion : ��ȡ����Ա�趨���������˵�ʱ��
          *------------------------------------------------------------------------------------------------------*/
          QTime getNightshiftTime();

protected:
          /*--------------------WorkAttendanceSys����ϵͳ�������ݿ����-----------------------*/
         /*------------------------------------------------------------------------------------------------------
          * ��������Ϣ�����ݿ�������ȡ����֤�Ƿ����
          * @name : readFaceRecordFromDB
          * @function����ȡԱ����UserID��Ӧ��������Ϣ
          * @param��1. Ա���� �� const  std::string& employeeNumber
          *                   2. Ա������ �� const std::string& _userName
          *                    3. ���� �� const std::string & _department
          * 
          * @retValue : ����UserID��Ӧ��������Ϣ
          *------------------------------------------------------------------------------------------------------*/
          std::string readFaceRecordFromDB(
                    const  std::string& employeeNumber, 
                    const std::string& _userName,
                    const std::string& _department
          );

          /*------------------------------------------------------------------------------------------------------
          * ��������Ϣ��¼�����ݿ���
          * @name:storeFaceRecord2DB
          * @param 1. Ա���ţ� const  std::string& employeeNumber
          *                2. ���� �� const  std::string& _name
          *                3. ���� �� const std::string & _department
          *                4. �沿���� ��const std::string& _faceMatrix
          *------------------------------------------------------------------------------------------------------*/
          bool storeFaceRecord2DB(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department,
                    const std::string& _faceMatrix
          );

          /*------------------------------------------------------------------------------------------------------
          * ��������Ϣ���������ݿ���
          * @name: updateFaceRecord2DB
          * @param 1. Ա���ţ� const  std::string& employeeNumber
          *                2. ���� �� const  std::string& _name
          *                3. �沿���� ��const std::string& _faceMatrix
          *------------------------------------------------------------------------------------------------------*/
          bool updateFaceRecord2DB(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _faceMatrix
          );

          /*---------------------WorkAttendanceSys����ϵͳ������Ϣ����-------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * ��Ա���Ƿ��ڽ��մ����ظ��Ĵ򿨼�¼
          * @name: readDuplicateRecordFromDB
          * @param 1. Ա���ţ� const  std::string& employeeNumber
          *                2. ���� ��  const std::string& _department
          *                3. ѡ���ǩ���� const AttendanceTable _table
          *                4.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
          *
          *------------------------------------------------------------------------------------------------------*/
          bool  readDuplicateRecordFromDB(
                    const  std::string& employeeNumber,
                    const std::string& _department,
                    const AttendanceTable _table,
                    QDateTime*& _timer
          );

          /*------------------------------------------------------------------------------------------------------
          * ��Ա���ϰ�򿨵���Ϣ��¼�����ݿ���
          * @name: storeAttendanceRecord2DB
          * @param 1. Ա���ţ� const  std::string& employeeNumber
          *                2. ���� ��  const std::string& _department
          *                3. �Ƿ��Ѿ��ٵ���const std::string _islate
          *                4.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
          * 
          *------------------------------------------------------------------------------------------------------*/
          bool storeAttendanceRecord2DB(
                    const  std::string& employeeNumber,
                    const std::string& _department,
                    const std::string _islate,
                    QDateTime*& _timer
          );

          /*------------------------------------------------------------------------------------------------------
          * ��Ա���°�ǩ�˵���Ϣ��¼�����ݿ���
          * @name: storeSignOutRecord2DB
          * @param 1. Ա���ţ� const  std::string& employeeNumber
          *                2. ���� ��  const std::string& _department
          *                3. �Ƿ��Ѿ��ٵ���const std::string& _islate
          *                4.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
          * 
          *------------------------------------------------------------------------------------------------------*/
          bool storeSignOutRecord2DB(
                    const  std::string& employeeNumber,
                    const std::string& _department,
                    const std::string& _islate,
                    QDateTime*& _timer
          );

          /*------------------------------------------------------------------------------------------------------
          * �Զ�Ԫ��ķ�ʽ�����ݿ��е����ݽ��ж�ȡ
          * @name: readAttendenceRecord
          * @param 1. Ա���ţ� const  std::string& employeeNumber
          *                2. ������ const  std::string& _name
          *                3. ���� ��  const std::string& _department
          *                4. ��ʱ�䣺 const QDateTime _leftTimer
          *                5. �Ҳ�ʱ�䣺 const QDateTime _rightTimer
          *                6. ѡ��ǩ��ǩ�˼�¼��AttendanceTable tableSelect
          *                7. �Ƿ�ѡ��ʱ��: bool _isTimeEnabled
          *
          * @retValue:  ����һ����Ԫ��ļ��� std::vector<std::vector<std::string>>
          *------------------------------------------------------------------------------------------------------*/
          std::vector<std::vector<std::string>> readAttendenceRecord(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department,
                    const QDateTime _leftTimer,
                    const QDateTime _rightTimer,
                    AttendanceTable tableSelect,
                    bool _isTimeEnabled
          );

          /*---------------------WorkAttendanceSys����ϵͳ��Ա���������---------------------*/
          /*------------------------------------------------------------------------------------------------------
          * ����Ա����ע����Ϣ��ϵͳ����Ա����Ȩ������
          * @name: storeAskPremitRecord2DB
          * @param 1. Ա���ţ� const  std::string& employeeNumber
          *                2. ���� ��  const std::string& _name
          *                3. ���� ��  const std::string& _department
          *                4.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
          *
          *------------------------------------------------------------------------------------------------------*/
          bool storeAskPremitRecord2DB(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department,
                    QDateTime*& _timer
          );

          /*------------------------------------------------------------------------------------------------------
          * ��ѯ��Ա����ע����Ϣ��ʾ����Ļ��
          * @name: checkPremitRecordFromDB
          * @param 1. Ա���ţ� const  std::string& employeeNumber
          *                2. ���� ��  const std::string& _name
          *                3. ���� ��  const std::string& _department
          *
          * @retValue : �������Ȩ�� std::string
          * @Correction: 2022-8-3 �޸�SELECT��ѯ����ֵ����Ϊ�յ����
          *------------------------------------------------------------------------------------------------------*/
          std::string checkPremitRecordFromDB(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department
          );

          /*---------------------WorkAttendanceSys����ϵͳ����Ա����---------------------*/
          /*------------------------------------------------------------------------------------------------------
          * ��ѯϵͳ����Ա��Ȩ��
          * @name: readAdminPriviledgeFromDB
          * @param 1. Ա���ţ� const  std::string& employeeNumber
          *                2. ���� ��  const std::string& _name
          *                3. ���� ��  const std::string& _department
          *
          * @retValue : �����Ƿ�鵽 bool
          *------------------------------------------------------------------------------------------------------*/
          bool readAdminPriviledgeFromDB(
                    const  std::string& employeeNumber,
                    const std::string& _userName,
                    const std::string& _department
          );

private:
          /*-------------------------------MYSQL INSERTָ��-----------------------------*/
          const std::string m_Insert_table_employee = "INSERT INTO employee VALUES(";
          const std::string m_Insert_table_facematrixstorge = "INSERT INTO facematrixstorge  VALUES(";
          const std::string m_Insert_table_attendence = "INSERT INTO attendence  VALUES(";
          const std::string m_Insert_table_signout = "INSERT INTO signout  VALUES(";
          const std::string m_Insert_table_askpremit = "INSERT INTO askpremit VALUES(";

          /*-------------------------------MYSQL SELECTָ��-----------------------------*/
          const std::string m_SelectDepartment = "SELECT Department FROM departmenttable";                    //��ȡ����ϵͳ�Ĳ����趨
          const std::string m_SelectTrainningSetting = "SELECT TrainningSetting FROM admintable";             //TrainningSetting���趨
          const std::string m_SelectTrainningSimilarity = "SELECT TrainningSimilarity FROM admintable";   //TrainningSimilarity���趨
          const std::string m_SelectMorningShiftTime = "SELECT MorningShiftTime FROM admintable";         //MorningShiftTime���趨
          const std::string m_SelectNightshiftTime = "SELECT NightshiftTime FROM admintable";               //NightshiftTime���趨

          const std::string m_SelectMatrixString = "SELECT faceMatrix FROM facematrixstorge WHERE UserID = "; //������������
          const std::string m_SelectEmployeeString = "SELECT UserName FROM employee WHERE UserID = ";        //������������

          const std::string m_SelectAskpremit = "SELECT AuthoritySet FROM askpremit WHERE UserID = ";              //����������Ȩ��

          const std::string m_SelectDuplicateAttendence = "SELECT * FROM  attendence WHERE UserID = ";            //�������յ��ظ�ǩ��
          const std::string m_SelectDuplicateSignOut = "SELECT * FROM signout WHERE UserID = ";                       //�������յ��ظ�ǩ��

          const std::string m_SelectStaticsAttendenceData = "SELECT * FROM attendence WHERE ";                           //����ǩ�����е�����
          const std::string m_SelectStaticsSignoutData = "SELECT * FROM signout WHERE ";                                      //����ǩ�˱��е�����
          //const std::string m_SelectAdminTablePrmittion = "SELECT UserID FROM admintable WHERE "

private:
          IN std::string m_connSetting;                     //�������Ӳ���
};