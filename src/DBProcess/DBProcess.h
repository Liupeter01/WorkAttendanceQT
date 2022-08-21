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
           * @DBProcess����ϵͳϵͳͨ���û�ID��ȡ�û���
           * @name :   getUserNameFromUserID
           * @funtion : ͨ���û�ID��ȡ�û���
           * @RetValue : �����û�������Ϣ std::string
           *------------------------------------------------------------------------------------------------------*/
          std::string getUserNameFromUserID(const std::string &_userID);

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

          /*------------------------WorkAttendanceSys����ϵͳ����Ա����-------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * ��admim UI�н�ϵͳ����������
          * @name: updateAdminParam2DB
          * @param 1.ѵ�������趨��const std::string& _TranningSet,
          *                2.ѵ�����ƶȣ�const std::string& _TranningSimilarity,
          *                3.�ٵ�ʱ�䣺const std::string& _LateTimeSet,
          *                4.����ʱ�䣺const std::string& _LeaveEarilyTimeSet
          *                5.�Ѿ���½��ADMIN ID��const std::string & _loggedUserID
          *
          *@Correction: 2022-8-20 �޸�ʱ����Ҫʹ�õ����Ž��б�ʶ��bug
          *------------------------------------------------------------------------------------------------------*/
          bool updateAdminParam2DB(
                    const std::string& _TranningSet,
                    const std::string& _TranningSimilarity,
                    const std::string& _LateTimeSet,
                    const std::string& _LeaveEarilyTimeSet,
                    const std::string& _loggedUserID
          );

          /*------------------------------------------------------------------------------------------------------
           *  �����ڶ�Ԫ�鼯���и���UserID��ѯ�û�����
           * @name: queryUserNameFromID
           * @function������Ա����UserID��ѯ�û�������ԭ��һ��Ԫ����ĸ�Ԫ����չΪ���Ԫ��
           * @param:
           *                  1.��Ԫ�鼯�ϣ�  std::vector<std::vector<std::string>>
          *------------------------------------------------------------------------------------------------------*/
          void queryUserNameFromID(std::vector<std::vector<std::string>> & _matrix);

          /*------------------------WorkAttendanceSys����ϵͳ����Աͳ�ƹ���-------------------------*/
          /*------------------------------------------------------------------------------------------------------
           *  �����ݿ�������Ա������
           * @name: getEmployeeCount
           * @function�������ݿ�������Ա������
           * @param:
           *                  1. Ա���ţ� const  std::string& employeeNumber
          *                   2. ���� ��  const std::string& _name
          *                   3. ���� ��  const std::string& _department
          *------------------------------------------------------------------------------------------------------*/
          int getEmployeeCount(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department
          );

          /*------------------------------------------------------------------------------------------------------
           *  �����ݿ���������ǰ�����ʱ���¼
           * @name: getMinimiseTime
           * @function��������ǰ�����ʱ���¼
           * @param:.ѡ��ǩ��ǩ�˼�¼����AttendanceTable _tableSelect
          *------------------------------------------------------------------------------------------------------*/
          QDateTime getMinimiseTime(AttendanceTable _tableSelect);

          /*------------------------------------------------------------------------------------------------------
           *  �����ݿ���������ǰ����е��ڵ���Ա���������������γ�ӳ��
           * @name: adminStatisticsOnDutyCount
           * @function������Ա����UserID��ѯ�û�������ԭ��һ��Ԫ����ĸ�Ԫ����չΪ���Ԫ��
           * @param:
           *                  1. Ա���ţ� const  std::string& employeeNumber
          *                   2. ���� ��  const std::string& _name
          *                   3. ���� ��  const std::string& _department
           *                  4.ѡ��ǩ��ǩ�˼�¼����AttendanceTable _tableSelect
           *                  5. ��ʱ�䣺 const QDateTime _leftTimer
          *                   6. �Ҳ�ʱ�䣺 const QDateTime _rightTimer
          *                   7. Ա��״̬��const std::string _employeeStatus
          *------------------------------------------------------------------------------------------------------*/
          int adminStatisticsOnDutyCount(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department,
                    AttendanceTable _tableSelect,
                    const QDateTime _leftTimer,
                    const QDateTime _rightTimer,
                    const std::string _employeeStatus
          );

          /*-----------------------WorkAttendanceSys����ϵͳ����Ա��Ա������------------------------*/
         /*------------------------------------------------------------------------------------------------------
          * �Զ�Ԫ��ķ�ʽ�����ݿ�����Ա�����ݽ��ж�ȡ
          * @name: readAttendenceRecord
          * @retValue:  ����һ����Ԫ��ļ��� std::vector<std::vector<std::string>>
          *------------------------------------------------------------------------------------------------------*/
          std::vector<std::vector<std::string>> readNewEmployeeRecord();

          /*------------------------------------------------------------------------------------------------------
           * ���¶�Ԫ����ָ��Ա��������״̬
           * @name: updateNewEmployeeAuth
           * @param : 1. Ա���ţ� const  std::string& employeeNumber
          *                   2. ���� ��  const std::string& _name
          *                   3. �Ƿ�ͨ�� bool status
          * 
           * @retValue:  bool
           *------------------------------------------------------------------------------------------------------*/
          bool updateNewEmployeeAuth(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    bool status
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
          const std::string m_SelectUserName = "SELECT UserName FROM employee WHERE UserID = ";     //UserName���趨

          const std::string m_SelectMatrixString = "SELECT faceMatrix FROM facematrixstorge WHERE UserID = "; //������������
          const std::string m_SelectEmployeeString = "SELECT UserName FROM employee WHERE UserID = ";        //������������

          const std::string m_SelectAskpremit = "SELECT AuthoritySet FROM askpremit WHERE UserID = ";              //����������Ȩ��

          const std::string m_SelectDuplicateAttendence = "SELECT * FROM  attendence WHERE UserID = ";            //�������յ��ظ�ǩ��
          const std::string m_SelectDuplicateSignOut = "SELECT * FROM signout WHERE UserID = ";                       //�������յ��ظ�ǩ��

          const std::string m_SelectStaticsAttendenceData = "SELECT * FROM attendence WHERE ";                           //����ǩ�����е�����
          const std::string m_SelectStaticsSignoutData = "SELECT * FROM signout WHERE ";                                      //����ǩ�˱��е�����
          //const std::string m_SelectAdminTablePrmittion = "SELECT UserID FROM admintable WHERE "

                   /*-------------------------------MYSQL UPDATEָ��-----------------------------*/
          const std::string m_UpdateAdminParam = "UPDATE admintable SET ";

          /*------------------------------------------MYSQL ������Ա������-----------------------------------------*/
          const std::string m_SelectNewEmployee = "SELECT * FROM askpremit WHERE AuthoritySet = \"Ȩ��������...\"";
          const std::string m_updateNewEmployee = "UPDATE askpremit SET AuthoritySet = ";

          /*------------------------------------------MYSQL ����ͳ������-----------------------------------------*/
          const std::string m_SelectEmployeeCount = "SELECT * FROM employee";                                                    //����Ա������
          const std::string m_SelectAttendenceMinTime = "SELECT MIN(MorningShiftTime) FROM attendence";    //����ǩ��������ϵ�ʱ��
          const std::string m_SelectSignOutMinTime = "SELECT MIN(NightShiftTime) FROM signout";                    //����ǩ�˱�����ϵ�ʱ��

          const std::string m_SelectAttendenceOnDutyCount =                                                                                       //����ǩ������������
                    "SELECT attendence.UserID FROM  employee, attendence WHERE employee.UserID = attendence.UserID AND attendence.MorningshiftTime ";

          const std::string m_SelectSignOutOnDutyCount =                                                                                            //����ǩ�˱����������
                    "SELECT signout.UserID FROM  employee, signout WHERE employee.UserID = signout.UserID AND signout.NightshiftTime ";

private:
          IN std::string m_connSetting;                     //�������Ӳ���
};