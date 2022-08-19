#pragma once
#include<QtWidgets/qtablewidget.h>
#include<QtWidgets/qtableview.h>
#include<QtGui/qstandarditemmodel.h>

class DataDisplay {
public:
          DataDisplay();
          DataDisplay(QObject* parent, QTableView*& _attendenceRecord, QTableView*& _newEmployee);
          virtual ~DataDisplay();

private:
          /*--------------------------------------DataDisplay��ʼ��-------------------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * @DataDisplay��ʼ��ģ����Ϣ
          * @name : setDataDisplayModel
          * @funtion : ��ʼ��ģ����Ϣ
          *------------------------------------------------------------------------------------------------------*/
          void setDataDisplayModel(); 

          /*------------------------------------------------------------------------------------------------------
          * @DataDisplay��ʼ��TableView��������Ϣ
          * @name :   initTableViewRowName
          * @funtion :��ʼ������ϵͳ�Ĵ򿨼�¼����
          *------------------------------------------------------------------------------------------------------*/
          void initTableViewRowName();

          void separateRowName(const QString& qs); //����������
          void setTableRowName();     //���ñ������
          void setTupleDisplayData(const QString& qs); //���ñ���Ԫ����ʽ������

private:

          QTableView* m_attendenceRecord = nullptr;                               //�򿨼�¼TableView
          QTableView* m_newEmployee = nullptr;                                    ///��Ա����������TableView
          QStandardItemModel* m_attendenceRecordModel = nullptr;     //�򿨼�¼QStandItemModel
          QStandardItemModel* m_newEmployeeModel = nullptr;           //��Ա����������QStandItemModel
          const std::vector<std::string> m_attendenceRecordRow{ "UserID","Ա������","������Ϣ","ʱ����Ϣ","Ա��״̬" };       //��ʾ�򿨼�¼
          const std::vector<std::string> m_newEmployeeRow{ "UserID","Ա������","������Ϣ","Ա������״̬" };                         //��ʾ����״̬
};