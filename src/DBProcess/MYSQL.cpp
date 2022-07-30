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
* ���ݿ�����Ӻ���
* @name:    dbInsert
* @param: ���ݿ����const std::string& op
*------------------------------------------------------------------------------------------------------*/
bool MySQL::connect()
{
          return connectionMySQL(m_host, m_user, m_pass, m_dbName);
}

/*------------------------------------------------------------------------------------------------------
* ���ݿ�Ĳ��뺯��
* @name:    dbInsert
* @param: ���ݿ����const std::string op
* @Correction: 2022-7-29 �޸��˽��ÿ���������ɲ�������ʧ�ܣ�ʹ��ֵ����
*------------------------------------------------------------------------------------------------------*/
bool MySQL::dbInsert(const std::string op)
{
          if (mysql_query(this->m_mysql, op.c_str())) {
                    return false;
          }
          return true;
}

/*------------------------------------------------------------------------------------------------------
* ���ݿ��ɾ������
* @name:    dbDelete
* @param: ���ݿ����const std::string& op
*------------------------------------------------------------------------------------------------------*/
bool MySQL::dbDelete(const std::string& op)
{
          if (mysql_query(m_mysql, op.c_str()))
                    return false;
          return true;
}

/*------------------------------------------------------------------------------------------------------
* ���ݿ���޸ĺ���
* @name:    dbModify
* @param: ���ݿ����const std::string& op
*------------------------------------------------------------------------------------------------------*/
bool MySQL::dbModify(const std::string& op)
{
          if (mysql_query(m_mysql, op.c_str()))
                    return false;
          return true;
}

/*------------------------------------------------------------------------------------------------------
* ���ݿ��Select����
* @name:    dbSelect
* @param: ���ݿ����const std::string op
* @Correction: 2022-7-30 �޸��˽��ÿ���������ɲ�������ʧ�ܣ�ʹ��ֵ����
*------------------------------------------------------------------------------------------------------*/
std::vector<std::vector<std::string>> MySQL::dbSelect(const std::string op)
{
          std::vector<std::vector<std::string>> vRet;
          if (mysql_query(m_mysql, op.c_str())) {
                    std::string vsRet(mysql_error(m_mysql));
                    return vRet;
          }
          MYSQL_RES* res = mysql_store_result(m_mysql);                  //������������ݼ����ͻ���
          if (res == NULL) {
                    return vRet;
          }
          MYSQL_ROW rows;                                                                 //���������������е���Ϣ
          int num_fields = mysql_num_fields(res);                                    //��������ܹ�����������
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
* ���ݿ��ͳ�Ʋ�ѯ��������
* @name:    dbGetCoun
* @param: ���ݿ����const std::string& op
*------------------------------------------------------------------------------------------------------*/
size_t MySQL::dbGetCount(const std::string& op)
{
          if (mysql_query(this->m_mysql, op.c_str())) {
                    return 0;
          }
          MYSQL_RES* res = mysql_store_result(this->m_mysql);                   //�������������ݼ����ͻ���
          if (res == NULL) {
                    return 0;
          }
          return mysql_num_fields(res);
}

/*------------------------------------------------------------------------------------------------------
* ���ݿ�����ӳ�ʼ������
* @name:   connectionMySQL
* @param: 1.����std::string host
*                 2.�û���std::string* user
*                 3.����std::string* passward
*                 4.���ݿ���std::string* dbName
*                 5.�˿ں�int port = 3306
*------------------------------------------------------------------------------------------------------*/
bool MySQL::connectionMySQL(std::string& host, std::string& user, std::string& passward, std::string& dbName, int port)
{
          if (!mysql_real_connect(this->m_mysql, host.c_str(), user.c_str(), passward.c_str(), dbName.c_str(), port, NULL, 0)) {
                    return false;
          }
          /* ���mysql������ʾ�������� */
          mysql_query(this->m_mysql, "set names 'gbk'");           //���mysql������ʾ��������
          return true;
}