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

protected:
          /*----------------------WorkAttendanceSys����ϵͳ���ݿ����-------------------------*/
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

private:
          /*------------------DBPROCESS �� MYSQL����ָ��-------------------------*/
          const std::string m_Insert_table_employee = "INSERT INTO employee VALUES(";
          const std::string m_Insert_table_facematrixstorge = "INSERT INTO facematrixstorge  VALUES(";
          const std::string m_SelectTrainningSetting = "SELECT TrainningSetting FROM admintable";             //TrainningSetting���趨
          const std::string m_SelectTrainningSimilarity = "SELECT TrainningSimilarity FROM admintable";   //TrainningSimilarity���趨

private:
          IN std::string m_connSetting;                     //�������Ӳ���
};