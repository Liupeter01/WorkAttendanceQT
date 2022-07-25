#include "DBProcess.h"

DBProcess::DBProcess()
{
          this->connect();
}

DBProcess::~DBProcess()
{
          
}

/*------------------------------------------------------------------------------------------------------
* 将人脸信息记录数据库
* @name:storeFaceRecord2DB
* @param 1. 员工号： const int employeeNumber
*                2. 姓名 ： const  std::string& _name
*                3. 部门 ： const std::string & _department
*                4. 面部矩阵 ：const std::string& _faceMatrix
*------------------------------------------------------------------------------------------------------*/
void DBProcess::storeFaceRecord2DB(
          const int employeeNumber,
          const  std::string& _name,
          const std::string& _department,
          const std::string& _faceMatrix) 
{

          //this->dbInsert();
}
