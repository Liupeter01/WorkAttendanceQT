#include"TableViewDisplay.h"

DataDisplay::DataDisplay()
          :m_newEmployee(nullptr),
          m_attendenceRecord(nullptr),
          m_attendenceRecordModel(nullptr) ,
          m_newEmployeeModel(nullptr){

}

DataDisplay::DataDisplay(
          QObject* parent, 
          QTableView*& _attendenceRecord, 
          QTableView*& _newEmployee)
          : m_attendenceRecord(_attendenceRecord),
          m_newEmployee(_newEmployee),
          m_attendenceRecordModel(new QStandardItemModel(parent)),
          m_newEmployeeModel(new QStandardItemModel(parent))
{
          this->setDataDisplayModel();
          this->initTableViewRowName();
}

DataDisplay ::~DataDisplay() {
          delete m_newEmployee;
          delete m_attendenceRecord;
          delete  m_attendenceRecordModel;
          delete m_newEmployeeModel;
}

/*--------------------------------------DataDisplay初始化-------------------------------------*/
/*------------------------------------------------------------------------------------------------------
* @DataDisplay初始化模型信息
* @name : setDataDisplayModel
* @funtion : 初始化模型信息
*------------------------------------------------------------------------------------------------------*/
void DataDisplay::setDataDisplayModel()
{
          this->m_attendenceRecord->setModel(this->m_attendenceRecordModel);
          this->m_newEmployee->setModel(this->m_newEmployeeModel);
}

/*------------------------------------------------------------------------------------------------------
* @DataDisplay初始化TableView的列明信息
* @name :   initTableViewRowName
* @funtion :初始化管理系统的打卡记录列名
*------------------------------------------------------------------------------------------------------*/
void DataDisplay::initTableViewRowName()
{
          int RecordRowCount(0);
          this->m_attendenceRecordModel->setColumnCount(static_cast<int>(this->m_attendenceRecordRow.size()));       //设置列数
          this->m_newEmployeeModel->setColumnCount(static_cast<int>(this->m_newEmployeeRow.size()));                  //设置列数
          for (auto i : this->m_attendenceRecordRow) {
                    this->m_attendenceRecordModel->setHeaderData(RecordRowCount++, Qt::Horizontal, QString::fromLocal8Bit(i.c_str()));
          }
          RecordRowCount = 0;                                                                                                                                                  //重置数值
          for (auto i : this->m_newEmployeeRow) {
                    this->m_newEmployeeModel->setHeaderData(RecordRowCount++, Qt::Horizontal, QString::fromLocal8Bit(i.c_str()));
          }
}