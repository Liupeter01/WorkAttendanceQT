#include"../HeaderFile/Common.h"

extern "C" {
#include<mysql.h> 
}

class MySQL {
public:
          /*
          * 数据库的初始化程序
          * @name:   MySQL
          */
          MySQL();
          /*
          * 数据库的释放程序
          * @name:   ~MySQL
          */
          ~MySQL();

           /*
          * 数据库的连接函数
          * @name:    dbInsert
          * @param: 数据库操作const std::string& op
          */
          bool connect();

          /*
          * 数据库的插入函数
          * @name:    dbInsert
          * @param: 数据库操作const std::string& op
          */
          bool dbInsert(const std::string& op);

           /*
          * 数据库的删除函数
          * @name:    dbDelete
          * @param: 数据库操作const std::string& op
          */
          bool dbDelete(const std::string& op);

          /*
         * 数据库的修改函数
         * @name:    dbModify
         * @param: 数据库操作const std::string& op
         */
          bool dbModify(const std::string& op);

          /*
          * 数据库的Select函数
          * @name:    dbSelect
          * @param: 数据库操作const std::string& op
          */
          std::vector<std::vector<std::string>> dbSelect(const std::string& op);

           /*
          * 数据库的统计查询个数函数
          * @name:    dbGetCoun
          * @param: 数据库操作const std::string& op
          */
          size_t dbGetCount(const std::string& op);

private:
          /*
          * 数据库的连接初始化程序
          * @name:   connectionMySQL
          * @param: 1.主机std::string host
          *                 2.用户名std::string* user
          *                 3.密码std::string* passward
          *                 4.数据库名std::string* dbName
          *                 5.端口号int port = 3306
          */
          bool connectionMySQL(std::string& host, std::string& user, std::string& passward, std::string& dbName, int port = 3306);

private:
          MYSQL* m_mysql;                                                       //C语言风格的代码
          std::string m_host = "127.0.0.1";                                    //
          std::string m_user = "root";
          std::string m_pass = "Askherout@FTY#ESF";
          std::string m_dbName = "facerecognize";
          std::vector<std::string> m_tables;
};