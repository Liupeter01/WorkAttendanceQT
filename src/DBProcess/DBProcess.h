#include<QtCore/qdatetime.h>
#include"MYSQL.h"

enum class AttendanceTable{
          ATTENDANCE,                             //签到表
          SIGNOUT                                      //签退表
};

class DBProcess : public MySQL {
public:
          DBProcess();
          virtual ~DBProcess();
          
protected:
          /*------------------------WorkAttendanceSys考勤系统初始化-----------------------------*/
         /*------------------------------------------------------------------------------------------------------
          * @DBProcess考勤系统系统部门设定initDepartmentSetting
          * @name :  initDepartmentSetting
          * @funtion : 初始化系统部门设定DepartmentSetting
          * @RetValue : 返回部门的信息 std::vector<std::string>
          *------------------------------------------------------------------------------------------------------*/
          std::vector<std::string> initDepartmentSetting();

          /*------------------------------------------------------------------------------------------------------
           * @DBProcess考勤系统系统通过用户ID读取用户名
           * @name :   getUserNameFromUserID
           * @funtion : 通过用户ID读取用户名
           * @RetValue : 返回用户名的信息 std::string
           *------------------------------------------------------------------------------------------------------*/
          std::string getUserNameFromUserID(const std::string &_userID);

          /*------------------------------------------------------------------------------------------------------
          * @DBProcess考勤系统训练集数量设定TranningSetting
          * @name :  initTranningSetting
          * @funtion : 初始化训练集数量设定TranningSetting
          * @RetValue : int
          *------------------------------------------------------------------------------------------------------*/
          int initTranningSetting();

          /*------------------------------------------------------------------------------------------------------
          * @DBProcess考勤系统特征矩阵匹配参数TranningSimilarity
          * @name :  initTrainningSimilarity
          * @funtion : 特征矩阵匹配参数std::length 
          * @RetValue : double
          *------------------------------------------------------------------------------------------------------*/
          double initTrainningSimilarity();

          /*------------------------------------------------------------------------------------------------------
          * @DBProcess考勤系统获取管理员设定的早上迟到的时间
          * @name :  getMorningShiftTime
          * @funtion : 获取管理员设定的早上迟到的时间
          *------------------------------------------------------------------------------------------------------*/
          QTime getMorningShiftTime();

          /*------------------------------------------------------------------------------------------------------
          * @DBProcess考勤系统获取管理员设定的下午早退的时间
          * @name :  getNightshiftTime
          * @funtion : 获取管理员设定的下午早退的时间
          *------------------------------------------------------------------------------------------------------*/
          QTime getNightshiftTime();

protected:
          /*--------------------WorkAttendanceSys考勤系统人脸数据库操作-----------------------*/
         /*------------------------------------------------------------------------------------------------------
          * 将人脸信息从数据库中先提取并验证是否存在
          * @name : readFaceRecordFromDB
          * @function：获取员工号UserID对应的人脸信息
          * @param：1. 员工号 ： const  std::string& employeeNumber
          *                   2. 员工名字 ： const std::string& _userName
          *                    3. 部门 ： const std::string & _department
          * 
          * @retValue : 返回UserID对应的人脸信息
          *------------------------------------------------------------------------------------------------------*/
          std::string readFaceRecordFromDB(
                    const  std::string& employeeNumber, 
                    const std::string& _userName,
                    const std::string& _department
          );

          /*------------------------------------------------------------------------------------------------------
          * 将人脸信息记录在数据库中
          * @name:storeFaceRecord2DB
          * @param 1. 员工号： const  std::string& employeeNumber
          *                2. 姓名 ： const  std::string& _name
          *                3. 部门 ： const std::string & _department
          *                4. 面部矩阵 ：const std::string& _faceMatrix
          *------------------------------------------------------------------------------------------------------*/
          bool storeFaceRecord2DB(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department,
                    const std::string& _faceMatrix
          );

          /*------------------------------------------------------------------------------------------------------
          * 将人脸信息更新在数据库中
          * @name: updateFaceRecord2DB
          * @param 1. 员工号： const  std::string& employeeNumber
          *                2. 姓名 ： const  std::string& _name
          *                3. 面部矩阵 ：const std::string& _faceMatrix
          *------------------------------------------------------------------------------------------------------*/
          bool updateFaceRecord2DB(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _faceMatrix
          );

          /*---------------------WorkAttendanceSys考勤系统考勤信息操作-------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * 将员工是否在今日存在重复的打卡记录
          * @name: readDuplicateRecordFromDB
          * @param 1. 员工号： const  std::string& employeeNumber
          *                2. 部门 ：  const std::string& _department
          *                3. 选择的签到表： const AttendanceTable _table
          *                4.全局时钟系统的输入 QDateTime*& _timer
          *
          *------------------------------------------------------------------------------------------------------*/
          bool  readDuplicateRecordFromDB(
                    const  std::string& employeeNumber,
                    const std::string& _department,
                    const AttendanceTable _table,
                    QDateTime*& _timer
          );

          /*------------------------------------------------------------------------------------------------------
          * 将员工上班打卡的信息记录在数据库中
          * @name: storeAttendanceRecord2DB
          * @param 1. 员工号： const  std::string& employeeNumber
          *                2. 部门 ：  const std::string& _department
          *                3. 是否已经迟到：const std::string _islate
          *                4.全局时钟系统的输入 QDateTime*& _timer
          * 
          *------------------------------------------------------------------------------------------------------*/
          bool storeAttendanceRecord2DB(
                    const  std::string& employeeNumber,
                    const std::string& _department,
                    const std::string _islate,
                    QDateTime*& _timer
          );

          /*------------------------------------------------------------------------------------------------------
          * 将员工下班签退的信息记录在数据库中
          * @name: storeSignOutRecord2DB
          * @param 1. 员工号： const  std::string& employeeNumber
          *                2. 部门 ：  const std::string& _department
          *                3. 是否已经迟到：const std::string& _islate
          *                4.全局时钟系统的输入 QDateTime*& _timer
          * 
          *------------------------------------------------------------------------------------------------------*/
          bool storeSignOutRecord2DB(
                    const  std::string& employeeNumber,
                    const std::string& _department,
                    const std::string& _islate,
                    QDateTime*& _timer
          );

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
          std::vector<std::vector<std::string>> readAttendenceRecord(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department,
                    const QDateTime _leftTimer,
                    const QDateTime _rightTimer,
                    AttendanceTable tableSelect,
                    bool _isTimeEnabled
          );

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
          bool storeAskPremitRecord2DB(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department,
                    QDateTime*& _timer
          );

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
          std::string checkPremitRecordFromDB(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department
          );

          /*------------------------WorkAttendanceSys考勤系统管理员操作-------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * 在admim UI中将系统参数更新在
          * @name: updateAdminParam2DB
          * @param 1.训练集数设定：const std::string& _TranningSet,
          *                2.训练相似度：const std::string& _TranningSimilarity,
          *                3.迟到时间：const std::string& _LateTimeSet,
          *                4.早退时间：const std::string& _LeaveEarilyTimeSet
          *                5.已经登陆的ADMIN ID：const std::string & _loggedUserID
          *
          *@Correction: 2022-8-20 修复时间需要使用单引号进行标识的bug
          *------------------------------------------------------------------------------------------------------*/
          bool updateAdminParam2DB(
                    const std::string& _TranningSet,
                    const std::string& _TranningSimilarity,
                    const std::string& _LateTimeSet,
                    const std::string& _LeaveEarilyTimeSet,
                    const std::string& _loggedUserID
          );

          /*------------------------------------------------------------------------------------------------------
           *  用于在二元组集合中根据UserID查询用户姓名
           * @name: queryUserNameFromID
           * @function：管理员根据UserID查询用户名并将原本一个元组从四个元素扩展为五个元素
           * @param:
           *                  1.二元组集合：  std::vector<std::vector<std::string>>
          *------------------------------------------------------------------------------------------------------*/
          void queryUserNameFromID(std::vector<std::vector<std::string>> & _matrix);

          /*------------------------WorkAttendanceSys考勤系统管理员统计工具-------------------------*/
          /*------------------------------------------------------------------------------------------------------
           *  在数据库中搜索员工总数
           * @name: getEmployeeCount
           * @function：在数据库中搜索员工总数
           * @param:
           *                  1. 员工号： const  std::string& employeeNumber
          *                   2. 姓名 ：  const std::string& _name
          *                   3. 部门 ：  const std::string& _department
          *------------------------------------------------------------------------------------------------------*/
          int getEmployeeCount(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department
          );

          /*------------------------------------------------------------------------------------------------------
           *  在数据库中搜索当前最早的时间记录
           * @name: getMinimiseTime
           * @function：搜索当前最早的时间记录
           * @param:.选择签到签退记录：：AttendanceTable _tableSelect
          *------------------------------------------------------------------------------------------------------*/
          QDateTime getMinimiseTime(AttendanceTable _tableSelect);

          /*------------------------------------------------------------------------------------------------------
           *  在数据库中搜索当前表格中到岗的人员数量，并与日期形成映射
           * @name: adminStatisticsOnDutyCount
           * @function：管理员根据UserID查询用户名并将原本一个元组从四个元素扩展为五个元素
           * @param:
           *                  1. 员工号： const  std::string& employeeNumber
          *                   2. 姓名 ：  const std::string& _name
          *                   3. 部门 ：  const std::string& _department
           *                  4.选择签到签退记录：：AttendanceTable _tableSelect
           *                  5. 左部时间： const QDateTime _leftTimer
          *                   6. 右部时间： const QDateTime _rightTimer
          *                   7. 员工状态：const std::string _employeeStatus
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

          /*-----------------------WorkAttendanceSys考勤系统管理员新员工工具------------------------*/
         /*------------------------------------------------------------------------------------------------------
          * 以二元组的方式将数据库中新员工数据进行读取
          * @name: readAttendenceRecord
          * @retValue:  返回一个二元组的集合 std::vector<std::vector<std::string>>
          *------------------------------------------------------------------------------------------------------*/
          std::vector<std::vector<std::string>> readNewEmployeeRecord();

          /*------------------------------------------------------------------------------------------------------
           * 更新二元组中指定员工的审批状态
           * @name: updateNewEmployeeAuth
           * @param : 1. 员工号： const  std::string& employeeNumber
          *                   2. 姓名 ：  const std::string& _name
          *                   3. 是否通过 bool status
          * 
           * @retValue:  bool
           *------------------------------------------------------------------------------------------------------*/
          bool updateNewEmployeeAuth(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    bool status
          );

private:

          /*-------------------------------MYSQL INSERT指令-----------------------------*/
          const std::string m_Insert_table_employee = "INSERT INTO employee VALUES(";
          const std::string m_Insert_table_facematrixstorge = "INSERT INTO facematrixstorge  VALUES(";
          const std::string m_Insert_table_attendence = "INSERT INTO attendence  VALUES(";
          const std::string m_Insert_table_signout = "INSERT INTO signout  VALUES(";
          const std::string m_Insert_table_askpremit = "INSERT INTO askpremit VALUES(";

          /*-------------------------------MYSQL SELECT指令-----------------------------*/
          const std::string m_SelectDepartment = "SELECT Department FROM departmenttable";                    //读取考勤系统的部门设定
          const std::string m_SelectTrainningSetting = "SELECT TrainningSetting FROM admintable";             //TrainningSetting的设定
          const std::string m_SelectTrainningSimilarity = "SELECT TrainningSimilarity FROM admintable";   //TrainningSimilarity的设定
          const std::string m_SelectMorningShiftTime = "SELECT MorningShiftTime FROM admintable";         //MorningShiftTime的设定
          const std::string m_SelectNightshiftTime = "SELECT NightshiftTime FROM admintable";               //NightshiftTime的设定
          const std::string m_SelectUserName = "SELECT UserName FROM employee WHERE UserID = ";     //UserName的设定

          const std::string m_SelectMatrixString = "SELECT faceMatrix FROM facematrixstorge WHERE UserID = "; //搜索人脸矩阵
          const std::string m_SelectEmployeeString = "SELECT UserName FROM employee WHERE UserID = ";        //搜索人名矩阵

          const std::string m_SelectAskpremit = "SELECT AuthoritySet FROM askpremit WHERE UserID = ";              //搜索审批的权限

          const std::string m_SelectDuplicateAttendence = "SELECT * FROM  attendence WHERE UserID = ";            //搜索今日的重复签到
          const std::string m_SelectDuplicateSignOut = "SELECT * FROM signout WHERE UserID = ";                       //搜索今日的重复签退

          const std::string m_SelectStaticsAttendenceData = "SELECT * FROM attendence WHERE ";                           //搜索签到表中的数据
          const std::string m_SelectStaticsSignoutData = "SELECT * FROM signout WHERE ";                                      //搜索签退表中的数据
          //const std::string m_SelectAdminTablePrmittion = "SELECT UserID FROM admintable WHERE "

                   /*-------------------------------MYSQL UPDATE指令-----------------------------*/
          const std::string m_UpdateAdminParam = "UPDATE admintable SET ";

          /*------------------------------------------MYSQL 用于新员工操作-----------------------------------------*/
          const std::string m_SelectNewEmployee = "SELECT * FROM askpremit WHERE AuthoritySet = \"权限申请中...\"";
          const std::string m_updateNewEmployee = "UPDATE askpremit SET AuthoritySet = ";

          /*------------------------------------------MYSQL 用于统计数据-----------------------------------------*/
          const std::string m_SelectEmployeeCount = "SELECT * FROM employee";                                                    //搜索员工总数
          const std::string m_SelectAttendenceMinTime = "SELECT MIN(MorningShiftTime) FROM attendence";    //搜索签到表最古老的时间
          const std::string m_SelectSignOutMinTime = "SELECT MIN(NightShiftTime) FROM signout";                    //搜索签退表最古老的时间

          const std::string m_SelectAttendenceOnDutyCount =                                                                                       //搜索签到表到岗总人数
                    "SELECT attendence.UserID FROM  employee, attendence WHERE employee.UserID = attendence.UserID AND attendence.MorningshiftTime ";

          const std::string m_SelectSignOutOnDutyCount =                                                                                            //搜索签退表离岗总人数
                    "SELECT signout.UserID FROM  employee, signout WHERE employee.UserID = signout.UserID AND signout.NightshiftTime ";

private:
          IN std::string m_connSetting;                     //数据连接参数
};