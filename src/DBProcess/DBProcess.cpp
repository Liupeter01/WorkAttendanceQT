#include "DBProcess.h"

DBProcess::DBProcess()
{
          this->connect();
}

DBProcess::~DBProcess()
{
          
}

/*------------------------------------------------------------------------------------------------------
* ��������Ϣ��¼���ݿ�
* @name:storeFaceRecord2DB
* @param 1. Ա���ţ� const int employeeNumber
*                2. ���� �� const  std::string& _name
*                3. ���� �� const std::string & _department
*                4. �沿���� ��const std::string& _faceMatrix
*------------------------------------------------------------------------------------------------------*/
void DBProcess::storeFaceRecord2DB(
          const int employeeNumber,
          const  std::string& _name,
          const std::string& _department,
          const std::string& _faceMatrix) 
{

          //this->dbInsert();
}
