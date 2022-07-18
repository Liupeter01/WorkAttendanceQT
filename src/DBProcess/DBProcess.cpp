#include "DBProcess.h"

/*
* ���ݿ�ĳ�ʼ������
* @name:   DBProcess
* @retValue:  ���س�ʼ���Ƿ�ɹ�
*/
DBProcess::DBProcess()
          :m_db(new MySQL)
{

}

/*
*���ݿ�����ӳ�ʼ������
* @name:   DBProcess
* @retValue : ���س�ʼ���Ƿ�ɹ�
*/
bool DBProcess::connect()
{
          return  m_db->connect();
}

DBProcess::~DBProcess()
{
          this->closeDataBase();
}

/*
* ��������Ϣ��¼���ݿ�
* @name:storeFaceRecord2DB
* @param 1. Ա����
*                2. ����
*                3. ����
*                4. �沿����
*/
void DBProcess::storeFaceRecord2DB(
          const int employeeNumber,
          const  std::string& _name,
          const std::string& _department,
          const std::string& _faceMatrix) 
{

}

/*
* ���ݿ�����ӹرճ���
* @name:closeDataBas
* @retValue:  ����0Ϊ�ر�ʧ�ܻ�ر�û�д��������ݿ⣬����1Ϊ�ɹ�
*/
void DBProcess::closeDataBase()
{
          delete m_db;
}