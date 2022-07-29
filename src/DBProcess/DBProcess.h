#include"MYSQL.h"

class DBProcess : public MySQL {
public:
          DBProcess();
          virtual ~DBProcess();

protected:
          /*------------------------------------------------------------------------------------------------------
          * ��������Ϣ��¼���ݿ�
          * @name:storeFaceRecord2DB
          * @param 1. Ա���ţ� const  std::string& employeeNumber
          *                2. ���� �� const  std::string& _name
          *                3. ���� �� const std::string & _department
          *                4. �沿���� ��const std::string& _faceMatrix
          *------------------------------------------------------------------------------------------------------*/
          bool storeFaceRecord2DB(
                    const  std::string& employeeNumber,
                    const  std::string& _name,
                    const std::string& _department,
                    const std::string& _faceMatrix
          );

          /*
          * �����ݿ�������������¼
          * @name:matchFaceMatrixFromDB
          * @param  ��ǰ��ʵʱ�沿���� : dlib::matrix<float, 0, 1>& _realTimeFace
          * @retValue: ��ǰ���������ƣ����Ϊ����û���ҵ�����
          */
          std::string matchFaceMatrixFromDB(dlib::matrix<float, 0, 1>& _realTimeFace);

private:
          /*------------------DBPROCESS �� MYSQL����ָ��-------------------------*/
          const std::string m_Insert_table_employee = "INSERT INTO employee VALUES(";
          const std::string m_Insert_table_facematrixstorge = "INSERT INTO facematrixstorge  VALUES(";

private:
          IN std::string m_connSetting;                     //�������Ӳ���
};