#include"MYSQL.h"

MySQL::MySQL() 
{
          m_mysql = mysql_init(nullptr);
}

MySQL::~MySQL()
{
          mysql_close(this->m_mysql);
}

/*------------------------------------------------------------------------------------------------------
* 数据库的连接函数
* @name:    dbInsert
* @param: 数据库操作const std::string& op
*------------------------------------------------------------------------------------------------------*/
bool MySQL::connect()
{
          return connectionMySQL(m_host, m_user, m_pass, m_dbName);
}

/*------------------------------------------------------------------------------------------------------
* 数据库的插入函数
* @name:    dbInsert
* @param: 数据库操作const std::string op
* @Correction: 2022-7-29 修复了禁用拷贝构造造成参数传递失败，使用值传递
*------------------------------------------------------------------------------------------------------*/
bool MySQL::dbInsert(const std::string op)
{
          if (mysql_query(this->m_mysql, op.c_str())) {
                    return false;
          }
          return true;
}

/*------------------------------------------------------------------------------------------------------
* 数据库的删除函数
* @name:    dbDelete
* @param: 数据库操作const std::string& op
*------------------------------------------------------------------------------------------------------*/
bool MySQL::dbDelete(const std::string& op)
{
          if (mysql_query(m_mysql, op.c_str()))
                    return false;
          return true;
}

/*------------------------------------------------------------------------------------------------------
* 数据库的修改函数
* @name:    dbModify
* @param: 数据库操作const std::string& op
*------------------------------------------------------------------------------------------------------*/
bool MySQL::dbModify(const std::string& op)
{
          if (mysql_query(m_mysql, op.c_str()))
                    return false;
          return true;
}

/*------------------------------------------------------------------------------------------------------
* 数据库的Select函数
* @name:    dbSelect
* @param: 数据库操作const std::string op
* @Correction: 2022-7-30 修复了禁用拷贝构造造成参数传递失败，使用值传递
*------------------------------------------------------------------------------------------------------*/
std::vector<std::vector<std::string>> MySQL::dbSelect(const std::string op)
{
          std::vector<std::vector<std::string>> vRet;
          if (mysql_query(m_mysql, op.c_str())) {
                    std::string vsRet(mysql_error(m_mysql));
                    return vRet;
          }
          MYSQL_RES* res = mysql_store_result(m_mysql);                  //检查完整的数据集到客户端
          if (res == NULL) {
                    return vRet;
          }
          MYSQL_ROW rows;                                                                 //用来保存结果集中行的信息
          int num_fields = mysql_num_fields(res);                                    //结果集中总共多少行数据
          while (rows = mysql_fetch_row(res)) {
                    int i = 0;
                    std::vector<std::string> vItem;
                    vItem.resize(num_fields);
                    for (i = 0; i < num_fields; ++i) {
                              vItem[i] = rows[i];
                    }
                    vRet.push_back(std::move(vItem));
          }
          const char* str = mysql_error(this->m_mysql);
          mysql_free_result(res);
          return vRet;
}

/*------------------------------------------------------------------------------------------------------
* 数据库的统计查询个数函数
* @name:    dbGetCoun
* @param: 数据库操作const std::string& op
*------------------------------------------------------------------------------------------------------*/
size_t MySQL::dbGetCount(const std::string& op)
{
          if (mysql_query(this->m_mysql, op.c_str())) {
                    return 0;
          }
          MYSQL_RES* res = mysql_store_result(this->m_mysql);                   //检索完整的数据集到客户端
          if (res == NULL) {
                    return 0;
          }
          return mysql_num_fields(res);
}

/*------------------------------------------------------------------------------------------------------
* 数据库的连接初始化程序
* @name:   connectionMySQL
* @param: 1.主机std::string host
*                 2.用户名std::string* user
*                 3.密码std::string* passward
*                 4.数据库名std::string* dbName
*                 5.端口号int port = 3306
*------------------------------------------------------------------------------------------------------*/
bool MySQL::connectionMySQL(std::string& host, std::string& user, std::string& passward, std::string& dbName, int port)
{
          if (!mysql_real_connect(this->m_mysql, host.c_str(), user.c_str(), passward.c_str(), dbName.c_str(), port, NULL, 0)) {
                    return false;
          }
          /* 解决mysql中文显示乱码问题 */
          mysql_query(this->m_mysql, "set names 'gbk'");           //解决mysql中文显示乱码问题
          return true;
}