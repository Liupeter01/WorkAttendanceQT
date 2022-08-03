#include<QtCore/qdatetime.h>
#include"MYSQL.h"

class DBProcess : public MySQL {
public:
          DBProcess();
          virtual ~DBProcess();
          
protected:
          /*------------------------WorkAttendanceSys����ϵͳ��ʼ��-----------------------------*/
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
          *------------------------------------------------------------------------------------------------------*/
          std::string checkPremitRecordFromDB(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department
          );

private:
          /*------------------DBPROCESS �� MYSQL����ָ��-------------------------*/
          /*-------------------------------MYSQL INSERTָ��-----------------------------*/
          const std::string m_Insert_table_employee = "INSERT INTO employee VALUES(";
          const std::string m_Insert_table_facematrixstorge = "INSERT INTO facematrixstorge  VALUES(";
          const std::string m_Insert_table_attendence = "INSERT INTO attendence  VALUES(";
          const std::string m_Insert_table_signout = "INSERT INTO signout  VALUES(";
          const std::string m_Insert_table_askpremit = "INSERT INTO askpremit VALUES(";

          /*-------------------------------MYSQL SELECTָ��-----------------------------*/
          const std::string m_SelectTrainningSetting = "SELECT TrainningSetting FROM admintable";             //TrainningSetting���趨
          const std::string m_SelectTrainningSimilarity = "SELECT TrainningSimilarity FROM admintable";   //TrainningSimilarity���趨
          const std::string m_SelectMorningShiftTime = "SELECT MorningShiftTime FROM admintable";         //MorningShiftTime���趨
          const std::string m_SelectNightshiftTime = "SELECT NightshiftTime FROM admintable";               //NightshiftTime���趨

          const std::string m_SelectMatrixString = "SELECT faceMatrix FROM facematrixstorge WHERE UserID = "; //������������
          const std::string m_SelectEmployeeString = "SELECT UserName FROM employee WHERE UserID = ";        //������������

          const std::string m_SelectAskpremit = "SELECT AuthoritySet FROM askpremit WHERE UserID = ";              //����������Ȩ��

private:
          IN std::string m_connSetting;                     //�������Ӳ���
};