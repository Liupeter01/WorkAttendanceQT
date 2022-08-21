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
          /*------------------------------DataDisplay的TableView行设置-------------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * @DataDisplay设置签到记录TableView的元组信息
          * @name :  setAttendenceTableTuple
          * @funtion :设置签到记录TableView的元组信息
          * @param: 二元组的集合 const std::vector<std::vector<std::string>>& _matrix
          *------------------------------------------------------------------------------------------------------*/
          void setAttendenceTableTuple(const std::vector<std::vector<std::string>>& _matrix);

          /*------------------------------------------------------------------------------------------------------
          * @DataDisplay设置签退记录TableView的元组信息
          * @name :  setNewEmployeeTableTuple
          * @funtion :设置签退记录TableView的元组信息
          * @param: 二元组的集合 const std::vector<std::vector<std::string>>& _matrix
          *------------------------------------------------------------------------------------------------------*/
          void setNewEmployeeTableTuple(const std::vector<std::vector<std::string>>& _matrix);

public:
          /*--------------------------------------DataDisplay饼图显示-------------------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * @DataDisplay设置圆饼图QChartView的范围内的所有员工到场信息
          * @name : setChartViewData
          * @funtion :设置圆饼图QChartView的范围内的所有员工到场信息
          *@param: 1. 当前是否为Attendence签到表：bool status;
          *                2. 总人数：double _total
          *                3. 迟到上班 / 早退下班人数：double _late
          *                4. 正常人数 double _normal
          *
          *---------------------------------------------------------------------------------------------------------*/
          void setChartViewData(bool status, double _total, double _late, double _normal);

private:
          /*--------------------------------------DataDisplay初始化-------------------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * @DataDisplay初始化模型信息
          * @name : setDataDisplayModel
          * @funtion : 初始化模型信息
          *------------------------------------------------------------------------------------------------------*/
          void setDataDisplayModel(); 

          /*------------------------------------------------------------------------------------------------------
          * @DataDisplay初始化TableView的列明信息
          * @name :   initTableViewRowName
          * @funtion :初始化管理系统的打卡记录列名
          *------------------------------------------------------------------------------------------------------*/
          void initTableViewRowName();

private:
          /*--------------------------------------DataDisplay析构-------------------------------------*/
          /*------------------------------------------------------------------------------------------------------
          * @DataDisplay将圆饼图和图表类进行解绑
          * @name :   unboundSeriesFromChart
          * @funtion :初始化管理系统的打卡记录列名
          * @Correction: 2022-8-20 该函数用于在析构时解绑圆饼图和图表类防止程序出错
          *------------------------------------------------------------------------------------------------------*/
          void unboundSeriesFromChart();

private:
          /*----------------------------DataDisplay的表格视图QTableView-----------------------------*/
          QTableView* m_attendenceRecord = nullptr;                               //打卡记录TableView
          QTableView* m_newEmployee = nullptr;                                    //新员工审批窗口TableView
          QStandardItemModel* m_attendenceRecordModel = nullptr;     //打卡记录QStandItemModel
          QStandardItemModel* m_newEmployeeModel = nullptr;           //新员工审批窗口QStandItemModel
          const std::vector<std::string> m_attendenceRecordRow{ "UserID","员工姓名","部门信息","时间信息","员工状态" };       //显示打卡记录
          const std::vector<std::string> m_newEmployeeRow{ "UserID","员工姓名","部门信息","员工申请状态","时间信息"};                         //显示申请状态

          /*----------------------------DataDisplay的饼图视图QChartView-----------------------------*/
          QChartView* m_chartViewWidgets = nullptr;                            //QChartView结构,接收界面传回的经过提升的接口
          QPieSeries* m_pieChartSeries = nullptr;                                     //QChartView结构显示的饼图结构

private:
          int m_attendenceLineCount = 0;                                                                       //打卡记录TableView行数指针
          int m_attendenceRowCount = 0;                                                                       //打卡记录TableView列数指针
          int m_newEmployeeLineCount = 0;                                                                 //打卡记录TableView行数指针
          int m_newEmployeeRowCount = 0;                                                                 //打卡记录TableView列数指针
};