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

          void separateRowName(const QString& qs); //分离表的列名
          void setTableRowName();     //设置表的列名
          void setTupleDisplayData(const QString& qs); //设置表单行元组显式的数据

private:

          QTableView* m_attendenceRecord = nullptr;                               //打卡记录TableView
          QTableView* m_newEmployee = nullptr;                                    ///新员工审批窗口TableView
          QStandardItemModel* m_attendenceRecordModel = nullptr;     //打卡记录QStandItemModel
          QStandardItemModel* m_newEmployeeModel = nullptr;           //新员工审批窗口QStandItemModel
          const std::vector<std::string> m_attendenceRecordRow{ "UserID","员工姓名","部门信息","时间信息","员工状态" };       //显示打卡记录
          const std::vector<std::string> m_newEmployeeRow{ "UserID","员工姓名","部门信息","员工申请状态" };                         //显示申请状态
};