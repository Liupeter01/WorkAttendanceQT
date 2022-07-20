#include"../HeaderFile/Common.h"

extern "C" {
#include<mysql.h> 
}

class MySQL {
public:
          /*
          * ���ݿ�ĳ�ʼ������
          * @name:   MySQL
          */
          MySQL();
          /*
          * ���ݿ���ͷų���
          * @name:   ~MySQL
          */
          ~MySQL();

           /*
          * ���ݿ�����Ӻ���
          * @name:    dbInsert
          * @param: ���ݿ����const std::string& op
          */
          bool connect();

          /*
          * ���ݿ�Ĳ��뺯��
          * @name:    dbInsert
          * @param: ���ݿ����const std::string& op
          */
          bool dbInsert(const std::string& op);

           /*
          * ���ݿ��ɾ������
          * @name:    dbDelete
          * @param: ���ݿ����const std::string& op
          */
          bool dbDelete(const std::string& op);

          /*
         * ���ݿ���޸ĺ���
         * @name:    dbModify
         * @param: ���ݿ����const std::string& op
         */
          bool dbModify(const std::string& op);

          /*
          * ���ݿ��Select����
          * @name:    dbSelect
          * @param: ���ݿ����const std::string& op
          */
          std::vector<std::vector<std::string>> dbSelect(const std::string& op);

           /*
          * ���ݿ��ͳ�Ʋ�ѯ��������
          * @name:    dbGetCoun
          * @param: ���ݿ����const std::string& op
          */
          size_t dbGetCount(const std::string& op);

private:
          /*
          * ���ݿ�����ӳ�ʼ������
          * @name:   connectionMySQL
          * @param: 1.����std::string host
          *                 2.�û���std::string* user
          *                 3.����std::string* passward
          *                 4.���ݿ���std::string* dbName
          *                 5.�˿ں�int port = 3306
          */
          bool connectionMySQL(std::string& host, std::string& user, std::string& passward, std::string& dbName, int port = 3306);

private:
          MYSQL* m_mysql;                                                       //C���Է��Ĵ���
          std::string m_host = "127.0.0.1";                                    //
          std::string m_user = "root";
          std::string m_pass = "Askherout@FTY#ESF";
          std::string m_dbName = "facerecognize";
          std::vector<std::string> m_tables;
};