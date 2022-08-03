#include<QtCore/qdatetime.h>
#include"MYSQL.h"

class DBProcess : public MySQL {
public:
          DBProcess();
          virtual ~DBProcess();
          
protected:
          /*------------------------WorkAttendanceSys考勤系统初始化-----------------------------*/
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
          *------------------------------------------------------------------------------------------------------*/
          std::string checkPremitRecordFromDB(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department
          );

private:
          /*------------------DBPROCESS 的 MYSQL操作指令-------------------------*/
          /*-------------------------------MYSQL INSERT指令-----------------------------*/
          const std::string m_Insert_table_employee = "INSERT INTO employee VALUES(";
          const std::string m_Insert_table_facematrixstorge = "INSERT INTO facematrixstorge  VALUES(";
          const std::string m_Insert_table_attendence = "INSERT INTO attendence  VALUES(";
          const std::string m_Insert_table_signout = "INSERT INTO signout  VALUES(";
          const std::string m_Insert_table_askpremit = "INSERT INTO askpremit VALUES(";

          /*-------------------------------MYSQL SELECT指令-----------------------------*/
          const std::string m_SelectTrainningSetting = "SELECT TrainningSetting FROM admintable";             //TrainningSetting的设定
          const std::string m_SelectTrainningSimilarity = "SELECT TrainningSimilarity FROM admintable";   //TrainningSimilarity的设定
          const std::string m_SelectMorningShiftTime = "SELECT MorningShiftTime FROM admintable";         //MorningShiftTime的设定
          const std::string m_SelectNightshiftTime = "SELECT NightshiftTime FROM admintable";               //NightshiftTime的设定

          const std::string m_SelectMatrixString = "SELECT faceMatrix FROM facematrixstorge WHERE UserID = "; //搜索人脸矩阵
          const std::string m_SelectEmployeeString = "SELECT UserName FROM employee WHERE UserID = ";        //搜索人名矩阵

          const std::string m_SelectAskpremit = "SELECT AuthoritySet FROM askpremit WHERE UserID = ";              //搜索审批的权限

private:
          IN std::string m_connSetting;                     //数据连接参数
};