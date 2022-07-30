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

protected:
          /*----------------------WorkAttendanceSys考勤系统数据库操作-------------------------*/
         /*------------------------------------------------------------------------------------------------------
          * 将人脸信息从数据库中进行提取
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

private:
          /*------------------DBPROCESS 的 MYSQL操作指令-------------------------*/
          /*-------------------------------MYSQL INSERT指令-----------------------------*/
          const std::string m_Insert_table_employee = "INSERT INTO employee VALUES(";
          const std::string m_Insert_table_facematrixstorge = "INSERT INTO facematrixstorge  VALUES(";

          /*-------------------------------MYSQL SELECT指令-----------------------------*/
          const std::string m_SelectTrainningSetting = "SELECT TrainningSetting FROM admintable";             //TrainningSetting的设定
          const std::string m_SelectTrainningSimilarity = "SELECT TrainningSimilarity FROM admintable";   //TrainningSimilarity的设定
          const std::string m_SelectMatrixString = "SELECT faceMatrix FROM facematrixstorge WHERE UserID = "; //搜索人脸矩阵
          const std::string m_SelectEmployeeString = "SELECT UserName FROM employee WHERE UserID = ";        //搜索人名矩阵

private:
          IN std::string m_connSetting;                     //数据连接参数
};