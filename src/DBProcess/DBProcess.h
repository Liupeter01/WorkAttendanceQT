#include"MYSQL.h"

class DBProcess {
public:
          /*
          * ���ݿ�ĳ�ʼ������
          * @name:   DBProcess
          * @retValue:  ���س�ʼ���Ƿ�ɹ�
          */
          DBProcess();
          virtual ~DBProcess();
public:
          /*
          * ���ݿ�����ӳ�ʼ������
          * @name:   DBProcess
          * @retValue:  ���س�ʼ���Ƿ�ɹ�
          */
          bool connect();

public://�ò�����QT UI����в���


private:
          /*
          * ��������Ϣ��¼���ݿ�
          * @name:storeFaceRecord2DB
          * @param 1. Ա����
          *                2. ����
          *                3. ����
          *                4. �沿����
          */
          void storeFaceRecord2DB(
                    const int employeeNumber, 
                    const  std::string& _name, 
                    const std::string & _department,
                    const std::string& _faceMatrix
          );

          /*
          * �����ݿ�������������¼
          * @name:matchFaceMatrixFromDB
          * @param  ��ǰ��ʵʱ�沿���� : dlib::matrix<float, 0, 1>& _realTimeFace
          * @retValue: ��ǰ���������ƣ����Ϊ����û���ҵ�����
          */
          std::string matchFaceMatrixFromDB(dlib::matrix<float, 0, 1>& _realTimeFace);

          /*
          * ���ݿ�����ӹرճ���
          * @name:closeDataBas
          */
          void closeDataBase();

private:
          IN std::string m_connSetting;                     //�������Ӳ���
          IN MySQL* m_db;                                        //����������      
                   //���ݿ�������
};