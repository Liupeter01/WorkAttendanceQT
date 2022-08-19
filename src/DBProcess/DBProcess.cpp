#include "DBProcess.h"

DBProcess::DBProcess()
{
          this->connect();
}

DBProcess::~DBProcess()
{
          
}

/*------------------------WorkAttendanceSys考勤系统初始化--------------------------------*/
/*------------------------------------------------------------------------------------------------------
 * @DBProcess考勤系统系统部门设定initDepartmentSetting
 * @name :  initDepartmentSetting
 * @funtion : 初始化系统部门设定DepartmentSetting
 * @RetValue : 返回部门的信息 std::vector<std::string>
 *------------------------------------------------------------------------------------------------------*/
std::vector<std::string> DBProcess::initDepartmentSetting()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(m_SelectDepartment);       //从数据库中获取部门
          std::vector<std::string> _departmentSetting;                                                                       //部门信息数据集
          for (int i = 0; i < dbRes.size(); ++i) {
                    _departmentSetting.push_back(dbRes[i][0].c_str());
          }
          return _departmentSetting;
}

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

/*---------------------WorkAttendanceSys考勤系统考勤信息操作-------------------------*/
/*------------------------------------------------------------------------------------------------------
* 将员工是否在今日存在重复的打卡记录
* @name: readDuplicateRecordFromDB
* @param 1. 员工号： const  std::string& employeeNumber
*                2. 部门 ：  const std::string& _department
*                3. 选择的签到表： const AttendanceTable _table
*                4.全局时钟系统的输入 QDateTime*& _timer
* 
*@retValue: 存在：返回为true 
                     不存在：返回为false
*------------------------------------------------------------------------------------------------------*/
bool  DBProcess::readDuplicateRecordFromDB(
          const  std::string& employeeNumber,
          const std::string& _department,
          const AttendanceTable _table,
          QDateTime*& _timer
)
{
          std::vector<std::vector<std::string>> isExist = this->dbSelect(
                    (_table == AttendanceTable::ATTENDANCE ? this->m_SelectDuplicateAttendence : this->m_SelectDuplicateSignOut) +
                    employeeNumber + " AND Department = " + "\"" + _department + "\"" +
                    " AND MorningshiftTime LIKE " + "\"" + _timer->currentDateTime().toString("yyyy-MM-dd").toLocal8Bit().constData() + " %" + "\""
          );
          return isExist.size() ? true : false;
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

/*------------------------------------------------------------------------------------------------------
* 以二元组的方式将数据库中的数据进行读取
* @name: readAttendenceRecord
* @param 1. 员工号： const  std::string& employeeNumber
*                2. 姓名： const  std::string& _name
*                3. 部门 ：  const std::string& _department
*                4. 左部时间： const QDateTime _leftTimer
*                5. 右部时间： const QDateTime _rightTimer
*                6. 选择签到签退记录：AttendanceTable tableSelect
*                7. 是否选择时间: bool _isTimeEnabled
*
* @retValue:  返回一个二元组的集合 std::vector<std::vector<std::string>>
*------------------------------------------------------------------------------------------------------*/
std::vector<std::vector<std::string>> DBProcess::readAttendenceRecord(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _department,
          const QDateTime _leftTimer,
          const QDateTime _rightTimer,
          AttendanceTable tableSelect,
          bool _isTimeEnabled
)
{
          std::string stringQuery{                                                                                                                                               //初始字符串
                   tableSelect == AttendanceTable::ATTENDANCE ? this->m_SelectStaticsAttendenceData : this->m_SelectStaticsSignoutData + " "
          };
          stringQuery = stringQuery + " Department = " + "\"" + _department + "\"";
          if (employeeNumber.size()) {
                    stringQuery = stringQuery + "  AND UserID = " + employeeNumber;
          }
          if (_name.size()) {
                    stringQuery = stringQuery + " AND UserID = " + "(SELECT UserID FROM employee WHERE UserName = " + "\"" + _name + "\"" + ")";
          }
          if (_isTimeEnabled) {                                                                                                                                                 //是否选择时间
                    stringQuery = stringQuery + " AND " +
                              (tableSelect == AttendanceTable::ATTENDANCE ? "MorningShiftTime" : "NightShiftTime") +
                              " BETWEEN " + "\'" + _leftTimer.toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData() + "\'"+ " AND " +
                              "\'" + _rightTimer.toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData() + "\'" ;

          }
          return this->dbSelect(stringQuery);
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
                    "\"" + "权限申请中..." + "\"" + ","
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
* @Correction: 2022-8-3 修复SELECT查询返回值可能为空的清空
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
          if (!isPriviledgePremit.size()) {                                                                             //申请的信息根本不存在
                    return std::string();                                                                                       
          }
          return std::string(isPriviledgePremit[0][0].c_str());
}

/*---------------------WorkAttendanceSys考勤系统管理员操作---------------------*/
/*------------------------------------------------------------------------------------------------------
* 查询系统管理员的权限
* @name: readAdminPriviledgeFromDB
* @param 1. 员工号： const  std::string& employeeNumber
*                2. 姓名 ：  const std::string& _name
*                3. 部门 ：  const std::string& _department
*
* @retValue : 返回是否查到 bool
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::readAdminPriviledgeFromDB(
          const  std::string& employeeNumber,
          const std::string& _userName,
          const std::string& _department
)
{
          return true;
}