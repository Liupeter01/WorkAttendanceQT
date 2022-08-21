#pragma once
#include<QtWidgets/qtablewidget.h>
#include<QtWidgets/qtableview.h>
#include<QtGui/qstandarditemmodel.h>
#include <QtCharts/QtCharts>
QT_CHARTS_USE_NAMESPACE

class DataDisplay {
public:
          DataDisplay();
          DataDisplay(
                    QObject* parent, 
                    QTableView*& _attendenceRecord, 
                    QTableView*& _newEmployee,
                    QChartView*&_rateStatistics
          );
          virtual ~DataDisplay();

public:
          /*------------------------------DataDisplay��TableView������-------------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * @DataDisplay����ǩ����¼TableView��Ԫ����Ϣ
          * @name :  setAttendenceTableTuple
          * @funtion :����ǩ����¼TableView��Ԫ����Ϣ
          * @param: ��Ԫ��ļ��� const std::vector<std::vector<std::string>>& _matrix
          *------------------------------------------------------------------------------------------------------*/
          void setAttendenceTableTuple(const std::vector<std::vector<std::string>>& _matrix);

          /*------------------------------------------------------------------------------------------------------
          * @DataDisplay����ǩ�˼�¼TableView��Ԫ����Ϣ
          * @name :  setNewEmployeeTableTuple
          * @funtion :����ǩ�˼�¼TableView��Ԫ����Ϣ
          * @param: ��Ԫ��ļ��� const std::vector<std::vector<std::string>>& _matrix
          *------------------------------------------------------------------------------------------------------*/
          void setNewEmployeeTableTuple(const std::vector<std::vector<std::string>>& _matrix);

public:
          /*--------------------------------------DataDisplay��ͼ��ʾ-------------------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * @DataDisplay����Բ��ͼQChartView�ķ�Χ�ڵ�����Ա��������Ϣ
          * @name : setChartViewData
          * @funtion :����Բ��ͼQChartView�ķ�Χ�ڵ�����Ա��������Ϣ
          *@param: 1. ��ǰ�Ƿ�ΪAttendenceǩ����bool status;
          *                2. ��������double _total
          *                3. �ٵ��ϰ� / �����°�������double _late
          *                4. �������� double _normal
          *
          *---------------------------------------------------------------------------------------------------------*/
          void setChartViewData(bool status, double _total, double _late, double _normal);

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

private:
          /*--------------------------------------DataDisplay����-------------------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * @DataDisplay��Բ��ͼ��ͼ������н��
          * @name :   unboundSeriesFromChart
          * @funtion :��ʼ������ϵͳ�Ĵ򿨼�¼����
          * @Correction: 2022-8-20 �ú�������������ʱ���Բ��ͼ��ͼ�����ֹ�������
          *------------------------------------------------------------------------------------------------------*/
          void unboundSeriesFromChart();

private:
          /*----------------------------DataDisplay�ı����ͼQTableView-----------------------------*/
          QTableView* m_attendenceRecord = nullptr;                               //�򿨼�¼TableView
          QTableView* m_newEmployee = nullptr;                                    //��Ա����������TableView
          QStandardItemModel* m_attendenceRecordModel = nullptr;     //�򿨼�¼QStandItemModel
          QStandardItemModel* m_newEmployeeModel = nullptr;           //��Ա����������QStandItemModel
          const std::vector<std::string> m_attendenceRecordRow{ "UserID","Ա������","������Ϣ","ʱ����Ϣ","Ա��״̬" };       //��ʾ�򿨼�¼
          const std::vector<std::string> m_newEmployeeRow{ "UserID","Ա������","������Ϣ","Ա������״̬","ʱ����Ϣ"};                         //��ʾ����״̬

          /*----------------------------DataDisplay�ı�ͼ��ͼQChartView-----------------------------*/
          QChartView* m_chartViewWidgets = nullptr;                            //QChartView�ṹ,���ս��洫�صľ��������Ľӿ�
          QPieSeries* m_pieChartSeries = nullptr;                                     //QChartView�ṹ��ʾ�ı�ͼ�ṹ

private:
          int m_attendenceLineCount = 0;                                                                       //�򿨼�¼TableView����ָ��
          int m_attendenceRowCount = 0;                                                                       //�򿨼�¼TableView����ָ��
          int m_newEmployeeLineCount = 0;                                                                 //�򿨼�¼TableView����ָ��
          int m_newEmployeeRowCount = 0;                                                                 //�򿨼�¼TableView����ָ��
};