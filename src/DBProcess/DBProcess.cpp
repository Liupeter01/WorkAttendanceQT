#include "DBProcess.h"

DBProcess::DBProcess()
{
          this->connect();
}

DBProcess::~DBProcess()
{
          
}

/*------------------------WorkAttendanceSys考勤系统初始化-----------------------------*/
/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys考勤系统训练集数量设定TranningSetting
* @name :  initTranningSetting
* @funtion : 初始化训练集数量设定TranningSetting
* @RetValue : int
*------------------------------------------------------------------------------------------------------*/
int DBProcess::initTranningSetting()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(this->m_SelectTrainningSetting);    //从数据库中获取TrainningSetting
          return std::atoi(dbRes[0][0].c_str());                                                                                                  //返回TrainningSetting
}

/*------------------------------------------------------------------------------------------------------
* @DBProcess考勤系统特征矩阵匹配参数TranningSimilarity
* @name :  initTrainningSimilarity
* @funtion : 特征矩阵匹配参数std::length
* @RetValue : double
*------------------------------------------------------------------------------------------------------*/
double DBProcess::initTrainningSimilarity()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(this->m_SelectTrainningSimilarity);   //从数据库中获取TranningSimilarity
          return std::atof(dbRes[0][0].c_str());                                                                                                     //返回TranningSimilarity
}

/*----------------------WorkAttendanceSys考勤系统数据库操作-------------------------*/
/*------------------------------------------------------------------------------------------------------
 * 将人脸信息从数据库中进行提取
 * @name : readFaceRecordFromDB
 * @function：获取员工号UserID对应的人脸信息
 * @param：1. 员工号 ： const  std::string& employeeNumber
 *                   2. 员工名字 ： const std::string& _userName
 *                   3. 部门 ： const std::string & _department
 *
 * @retValue : 返回UserID对应的人脸信息
 * @Correction: 2022-7-31 修复dbSelect的部门字符串的错误
 *------------------------------------------------------------------------------------------------------*/
std::string  DBProcess::readFaceRecordFromDB(
          const  std::string& employeeNumber,
          const std::string& _userName,
          const std::string& _department
)
{
          /*添加对于登录用户的信息确认程序*/
          std::vector<std::vector<std::string>> isUserExist = this->dbSelect(                                                                                     //判断是否存在当前的员工
                    this->m_SelectEmployeeString + employeeNumber + " AND Department = " + "\"" + _department + "\""
          );

          if (std::string(isUserExist[0][0].c_str()) != _userName) {                                                                                                    //存在当前的员工
                    return std::string();                                                                                                                                                       //员工为空
          }
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(this->m_SelectMatrixString + employeeNumber);   //搜索人脸矩阵
          return std::string(dbRes[0][0].c_str());
}

/*------------------------------------------------------------------------------------------------------
* 将人脸信息记录数据库
* @name:storeFaceRecord2DB
* @param 1. 员工号： const  std::string& employeeNumber
*                2. 姓名 ： const  std::string& _name
*                3. 部门 ： const std::string & _department
*                4. 面部矩阵 ：const std::string& _faceMatrix
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::storeFaceRecord2DB(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _department,
          const std::string& _faceMatrix) 
{
          bool employee = this->dbInsert(m_Insert_table_employee + employeeNumber + "," + "\"" + _name + "\"" + "," + "\"" + _department + "\"" + ")");    //在employee表格中进行插入
          bool matrix = this->dbInsert(m_Insert_table_facematrixstorge + employeeNumber + "," + "\"" + _faceMatrix + "\"" + ")");                                           //在facematrixstorge表格中插入
         return employee && matrix;
}

/*------------------------------------------------------------------------------------------------------
* 将人脸信息更新在数据库中
* @name: updateFaceRecord2DB
* @param 1. 员工号： const  std::string& employeeNumber
*                2. 姓名 ： const  std::string& _name
*                3. 面部矩阵 ：const std::string& _faceMatrix
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::updateFaceRecord2DB(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _faceMatrix
)
{
          return true;
}