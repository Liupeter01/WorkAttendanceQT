#include"MYSQL.h"

class DBProcess : public MySQL {
public:
          DBProcess();
          virtual ~DBProcess();

protected:
          /*------------------------------------------------------------------------------------------------------
          * 将人脸信息记录数据库
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

          /*
          * 在数据库中搜索人脸记录
          * @name:matchFaceMatrixFromDB
          * @param  当前的实时面部矩阵 : dlib::matrix<float, 0, 1>& _realTimeFace
          * @retValue: 当前的人脸名称，如果为空则没有找到人脸
          */
          std::string matchFaceMatrixFromDB(dlib::matrix<float, 0, 1>& _realTimeFace);

private:
          /*------------------DBPROCESS 的 MYSQL操作指令-------------------------*/
          const std::string m_Insert_table_employee = "INSERT INTO employee VALUES(";
          const std::string m_Insert_table_facematrixstorge = "INSERT INTO facematrixstorge  VALUES(";

private:
          IN std::string m_connSetting;                     //数据连接参数
};