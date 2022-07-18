#include "DBProcess.h"

/*
* 数据库的初始化程序
* @name:   DBProcess
* @retValue:  返回初始化是否成功
*/
DBProcess::DBProcess()
          :m_db(new MySQL)
{

}

/*
*数据库的连接初始化程序
* @name:   DBProcess
* @retValue : 返回初始化是否成功
*/
bool DBProcess::connect()
{
          return  m_db->connect();
}

DBProcess::~DBProcess()
{
          this->closeDataBase();
}

/*
* 将人脸信息记录数据库
* @name:storeFaceRecord2DB
* @param 1. 员工号
*                2. 姓名
*                3. 部门
*                4. 面部矩阵
*/
void DBProcess::storeFaceRecord2DB(
          const int employeeNumber,
          const  std::string& _name,
          const std::string& _department,
          const std::string& _faceMatrix) 
{

}

/*
* 数据库的连接关闭程序
* @name:closeDataBas
* @retValue:  返回0为关闭失败或关闭没有创建的数据库，返回1为成功
*/
void DBProcess::closeDataBase()
{
          delete m_db;
}