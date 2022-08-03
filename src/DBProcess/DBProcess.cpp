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

/*------------------------------------------------------------------------------------------------------
* @DBProcess考勤系统获取管理员设定的早上迟到的时间
* @name :  getMorningShiftTime
* @funtion : 获取管理员设定的早上迟到的时间
*------------------------------------------------------------------------------------------------------*/
QTime DBProcess::getMorningShiftTime()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(this->m_SelectMorningShiftTime);   //从数据库中获取MorningShiftTime
          return QTime::fromString(dbRes[0][0].c_str());
}

/*------------------------------------------------------------------------------------------------------
* @DBProcess考勤系统获取管理员设定的下午早退的时间
* @name :  getNightshiftTime
* @funtion : 获取管理员设定的下午早退的时间
*------------------------------------------------------------------------------------------------------*/
QTime DBProcess::getNightshiftTime()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(this->m_SelectNightshiftTime);   //从数据库中获取NightshiftTime
          return QTime::fromString(dbRes[0][0].c_str());
}

/*--------------------WorkAttendanceSys考勤系统人脸数据库操作-----------------------*/
/*------------------------------------------------------------------------------------------------------
 * 将人脸信息从数据库中先提取并验证是否存在
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

          if (!isUserExist.size() || std::string(isUserExist[0][0].c_str()) != _userName) {                                                                   //存在当前的员工
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

/*------------------------------------------------------------------------------------------------------
* 将员工上班打卡的信息记录在数据库中
* @name: storeAttendanceRecord2DB
* @param 1. 员工号： const  std::string& employeeNumber
*                2. 部门 ：  const std::string& _department
*                3. 是否已经迟到：const std::string _islate
*                4.全局时钟系统的输入 QDateTime*& _timer
*
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::storeAttendanceRecord2DB(
          const  std::string& employeeNumber,
          const std::string& _department,
          const std::string _islate,
          QDateTime*& _timer
)
{
          return this->dbInsert(this->m_Insert_table_attendence + employeeNumber + "," +
                    "\"" + _department + "\"" + "," +
                    "\"" + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData() + "\"" + "," +
                    "\"" + _islate + "\"" + ")"
          );
}

/*------------------------------------------------------------------------------------------------------
* 将员工下班签退的信息记录在数据库中
* @name: storeSignOutRecord2DB
* @param 1. 员工号： const  std::string& employeeNumber
*                2. 部门 ：  const std::string& _department
*                3. 是否已经迟到：const std::string& _islate
*                4.全局时钟系统的输入 QDateTime*& _timer
*
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::storeSignOutRecord2DB(
          const  std::string& employeeNumber,
          const std::string& _department,
          const std::string& _islate,
          QDateTime*& _timer
)
{
          return this->dbInsert(this->m_Insert_table_signout + employeeNumber + "," +
                    "\"" + _department + "\"" + "," +
                    "\"" + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData() + "\"" + "," +
                    "\"" + _islate + "\"" + ")"
          );
}

/*---------------------WorkAttendanceSys考勤系统新员工申请操作---------------------*/
/*------------------------------------------------------------------------------------------------------
* 将新员工的注册信息向系统管理员进行权限申请
* @name: storeAskPremitRecord2DB
* @param 1. 员工号： const  std::string& employeeNumber
*                2. 姓名 ：  const std::string& _name
*                3. 部门 ：  const std::string& _department
*                4.全局时钟系统的输入 QDateTime*& _timer
*
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::storeAskPremitRecord2DB(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _department,
          QDateTime*& _timer
)
{
          return this->dbInsert(this->m_Insert_table_askpremit + employeeNumber + "," +
                    "\"" + _name + "\"" + "," +
                    "\"" + _department + "\"" + "," +
                    "\"" + "申请权限" + "\"" + ","
                    "\"" + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData() + "\"" + ")"
          );
}

/*------------------------------------------------------------------------------------------------------
* 查询新员工的注册信息显示在屏幕上
* @name: checkPremitRecordFromDB
* @param 1. 员工号： const  std::string& employeeNumber
*                2. 姓名 ：  const std::string& _name
*                3. 部门 ：  const std::string& _department
*
* @retValue : 返回许可权限 std::string
*------------------------------------------------------------------------------------------------------*/
std::string DBProcess::checkPremitRecordFromDB(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _department
)
{
          //SELECT AuthoritySet FROM askpremit WHERE UserID = 999 AND 
          std::vector<std::vector<std::string>> isPriviledgePremit = this->dbSelect(
                    this->m_SelectAskpremit + employeeNumber +
                    "  AND UserName =  " + "\"" + _name + "\"" +
                    "  AND Department = " + "\"" + _department + "\""
          );
          return std::string(isPriviledgePremit[0][0].c_str());
}