#include"MYSQL.h"

class DBProcess {
public:
          /*
          * 数据库的初始化程序
          * @name:   DBProcess
          * @retValue:  返回初始化是否成功
          */
          DBProcess();
          virtual ~DBProcess();
public:
          /*
          * 数据库的连接初始化程序
          * @name:   DBProcess
          * @retValue:  返回初始化是否成功
          */
          bool connect();

public://该部分由QT UI层进行操作


private:
          /*
          * 将人脸信息记录数据库
          * @name:storeFaceRecord2DB
          * @param 1. 员工号
          *                2. 姓名
          *                3. 部门
          *                4. 面部矩阵
          */
          void storeFaceRecord2DB(
                    const int employeeNumber, 
                    const  std::string& _name, 
                    const std::string & _department,
                    const std::string& _faceMatrix
          );

          /*
          * 在数据库中搜索人脸记录
          * @name:matchFaceMatrixFromDB
          * @param  当前的实时面部矩阵 : dlib::matrix<float, 0, 1>& _realTimeFace
          * @retValue: 当前的人脸名称，如果为空则没有找到人脸
          */
          std::string matchFaceMatrixFromDB(dlib::matrix<float, 0, 1>& _realTimeFace);

          /*
          * 数据库的连接关闭程序
          * @name:closeDataBas
          */
          void closeDataBase();

private:
          IN std::string m_connSetting;                     //数据连接参数
          IN MySQL* m_db;                                        //数据连接类      
                   //数据库输出结果
};