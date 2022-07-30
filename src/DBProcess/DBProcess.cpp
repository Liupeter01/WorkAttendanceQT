#include "DBProcess.h"

DBProcess::DBProcess()
{
          this->connect();
}

DBProcess::~DBProcess()
{
          
}

/*------------------------WorkAttendanceSys����ϵͳ��ʼ��-----------------------------*/
/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys����ϵͳѵ���������趨TranningSetting
* @name :  initTranningSetting
* @funtion : ��ʼ��ѵ���������趨TranningSetting
* @RetValue : int
*------------------------------------------------------------------------------------------------------*/
int DBProcess::initTranningSetting()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(this->m_SelectTrainningSetting);    //�����ݿ��л�ȡTrainningSetting
          return std::atoi(dbRes[0][0].c_str());                                                                                                  //����TrainningSetting
}

/*------------------------------------------------------------------------------------------------------
* @DBProcess����ϵͳ��������ƥ�����TranningSimilarity
* @name :  initTrainningSimilarity
* @funtion : ��������ƥ�����std::length
* @RetValue : double
*------------------------------------------------------------------------------------------------------*/
double DBProcess::initTrainningSimilarity()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(this->m_SelectTrainningSimilarity);   //�����ݿ��л�ȡTranningSimilarity
          return std::atof(dbRes[0][0].c_str());                                                                                                     //����TranningSimilarity
}

/*----------------------WorkAttendanceSys����ϵͳ���ݿ����-------------------------*/
/*------------------------------------------------------------------------------------------------------
* ��������Ϣ��¼���ݿ�
* @name:storeFaceRecord2DB
* @param 1. Ա���ţ� const  std::string& employeeNumber
*                2. ���� �� const  std::string& _name
*                3. ���� �� const std::string & _department
*                4. �沿���� ��const std::string& _faceMatrix
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::storeFaceRecord2DB(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _department,
          const std::string& _faceMatrix) 
{
          bool employee = this->dbInsert(m_Insert_table_employee + employeeNumber + "," + "\"" + _name + "\"" + "," + "\"" + _department + "\"" + ")");    //��employee����н��в���
          bool matrix = this->dbInsert(m_Insert_table_facematrixstorge + employeeNumber + "," + "\"" + _faceMatrix + "\"" + ")");                                           //��facematrixstorge����в���
         return employee && matrix;
}

/*------------------------------------------------------------------------------------------------------
* ��������Ϣ���������ݿ���
* @name: updateFaceRecord2DB
* @param 1. Ա���ţ� const  std::string& employeeNumber
*                2. ���� �� const  std::string& _name
*                3. �沿���� ��const std::string& _faceMatrix
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::updateFaceRecord2DB(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _faceMatrix
)
{
          return true;
}