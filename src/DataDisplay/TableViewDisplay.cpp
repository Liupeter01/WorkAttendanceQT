#include"TableViewDisplay.h"

DataDisplay::DataDisplay()
          :m_newEmployee(nullptr),
          m_attendenceRecord(nullptr),
          m_attendenceRecordModel(nullptr) ,
          m_newEmployeeModel(nullptr),
          m_chartViewWidgets(nullptr),
          m_pieChartSeries(nullptr){

}

DataDisplay::DataDisplay(
          QObject* parent,
          QTableView*& _attendenceRecord,
          QTableView*& _newEmployee,
          QChartView*& _rateStatistics
)
          : m_attendenceRecord(_attendenceRecord),
          m_newEmployee(_newEmployee),
          m_chartViewWidgets(_rateStatistics),
          m_attendenceRecordModel(new QStandardItemModel(parent)),
          m_newEmployeeModel(new QStandardItemModel(parent)),
          m_pieChartSeries(new QPieSeries())
{
          QT_CHARTS_USE_NAMESPACE
          this->setDataDisplayModel();
          this->initTableViewRowName();
}

DataDisplay ::~DataDisplay() {
          this->unboundSeriesFromChart();
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

/*--------------------------------------DataDisplay析构-------------------------------------*/
/*------------------------------------------------------------------------------------------------------
* @DataDisplay将圆饼图和图表类进行解绑
* @name :   unboundSeriesFromChart
* @funtion :初始化管理系统的打卡记录列名
* @Correction: 2022-8-20 该函数用于在析构时解绑圆饼图和图表类防止程序出错
*------------------------------------------------------------------------------------------------------*/
void DataDisplay::unboundSeriesFromChart()
{
          m_chartViewWidgets->chart()->removeAllSeries();             //解绑
}

/*------------------------------DataDisplay的TableView行设置-------------------------------*/
/*------------------------------------------------------------------------------------------------------
* @DataDisplay设置签到记录TableView的元组信息
* @name :  setAttendenceTableTuple
* @funtion :设置签到记录TableView的元组信息
* @param: 二元组的集合 const std::vector<std::vector<std::string>>& _matrix
*------------------------------------------------------------------------------------------------------*/
void DataDisplay::setAttendenceTableTuple(const std::vector<std::vector<std::string>>& _matrix)
{
          this->m_attendenceRecordModel->clear();
          this->m_attendenceRecordModel->setColumnCount(static_cast<int>(this->m_attendenceRecordRow.size()));       //设置列数
          for (auto i : this->m_attendenceRecordRow) {
                    this->m_attendenceRecordModel->setHeaderData(m_attendenceRowCount++, Qt::Horizontal, QString::fromLocal8Bit(i.c_str()));
          }
          this->m_attendenceLineCount = 0;                                //重置行的循环计数
          this->m_attendenceRowCount = 0;                                //重置列的循环计数
          for (int i = 0; i < _matrix.size(); ++i) {                            //行循环
                    for (auto j : _matrix.at(i)) {                                    //列循环
                              this->m_attendenceRecordModel->setItem(
                                        this->m_attendenceLineCount,       //行的循环计数不变
                                        this->m_attendenceRowCount++,
                                        new QStandardItem(QString::fromLocal8Bit(j.c_str()))
                              );
                    }
                    this->m_attendenceLineCount++;
                    this->m_attendenceRowCount = 0;                     //重置列的循环计数
          }
          this->m_attendenceLineCount = 0;                               //重置行的循环计数
          this->m_attendenceRowCount = 0;                               //重置列的循环计数
}

/*------------------------------------------------------------------------------------------------------
* @DataDisplay设置签退记录TableView的元组信息
* @name :  setNewEmployeeTableTuple
* @funtion :设置签退记录TableView的元组信息
* @param: 二元组的集合 const std::vector<std::vector<std::string>>& _matrix
*------------------------------------------------------------------------------------------------------*/
void DataDisplay::setNewEmployeeTableTuple(const std::vector<std::vector<std::string>>& _matrix)
{
          this->m_newEmployeeModel->clear();
          int RecordRowCount(0);
          this->m_newEmployeeModel->setColumnCount(static_cast<int>(this->m_newEmployeeRow.size()));                  //设置列数
          for (auto i : this->m_newEmployeeRow) {
                    this->m_newEmployeeModel->setHeaderData(RecordRowCount++, Qt::Horizontal, QString::fromLocal8Bit(i.c_str()));
          }
          this->m_newEmployeeLineCount = 0;
          this->m_newEmployeeRowCount = 0;                     //重置列的循环计数
          for (int i = 0; i < _matrix.size(); ++i) {                            //行循环
                    for (auto j : _matrix.at(i)) {                                    //列循环
                              this->m_newEmployeeModel->setItem(
                                        this->m_newEmployeeLineCount,       //行的循环计数不变
                                        this->m_newEmployeeRowCount++,
                                        new QStandardItem(QString::fromLocal8Bit(j.c_str()))
                              );
                    }
                    this->m_newEmployeeLineCount++;
                    this->m_newEmployeeRowCount = 0;                     //重置列的循环计数
          }
          this->m_newEmployeeLineCount = 0;                               //重置行的循环计数
          this->m_newEmployeeRowCount = 0;                               //重置列的循环计数
}

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
void DataDisplay::setChartViewData(bool status, double _total, double _late, double _normal)
{
          m_pieChartSeries->clear();
          m_chartViewWidgets->chart()->setTitle(status ?                                              // 设置饼图名称
                    QString::fromLocal8Bit("当前筛选条件下的签到情况统计") :
                    QString::fromLocal8Bit("当前筛选条件下的签退情况统计")
          );
          m_chartViewWidgets->setRenderHint(QPainter::Antialiasing);                      // 设置抗锯齿
          m_chartViewWidgets->chart()->setTheme(QChart::ChartThemeQt);             // 设置饼图主题
          if (_total <= 0.000000001 && _total >= -0.000000001)                                  // _total不为0
          {
                    double res = 0.0f;
                    /*---------------------------------------饼图添加数据-------------------------------------*/
                    m_pieChartSeries->append( status ? QString::fromLocal8Bit("迟到上班比例") : QString::fromLocal8Bit("早退下班比例"),res);
                    m_pieChartSeries->append(status ? QString::fromLocal8Bit("正常上班比例") : QString::fromLocal8Bit("正常下班比例"), res);
                    m_pieChartSeries->append(QString::fromLocal8Bit("未打卡比例"), res);
          }
          else
          {
                    /*---------------------------------------饼图添加数据-------------------------------------*/
                    m_pieChartSeries->append(
                              status ? QString::fromLocal8Bit("迟到上班比例") : QString::fromLocal8Bit("早退下班比例"),
                              static_cast<float>(_late / _total)
                    );
                    m_pieChartSeries->append(
                              status ? QString::fromLocal8Bit("正常上班比例") : QString::fromLocal8Bit("正常下班比例"),
                              static_cast <float>(static_cast<float>(_normal / _total))
                    );
                    m_pieChartSeries->append(
                              QString::fromLocal8Bit("未打卡比例"),
                              static_cast<float>(1.0f - static_cast<float>(_normal / _total) - static_cast<float>(_late / _total))
                    );
                    /*---------------------------------------饼图设置块标签-------------------------------------*/
                    for (int sNum = 0; sNum < m_pieChartSeries->slices().count(); sNum++)
                    {
                              QPieSlice* slice = m_pieChartSeries->slices().at(sNum);
                              QString tagText = QString("%1%2%3%4")
                                        .arg(slice->label())
                                        .arg(QStringLiteral("占比"))
                                        .arg(QString::number(slice->percentage() * 100))
                                        .arg(QStringLiteral("%"));
                              slice->setLabel(tagText);
                    }
          }
          m_pieChartSeries->setLabelsPosition(QPieSlice::LabelPosition::LabelOutside);                        // 设置标签显示位置
          m_pieChartSeries->setLabelsVisible(true);                                                                               // 显示标签
          m_pieChartSeries->slices().at(m_pieChartSeries->slices().count() - 1)->setExploded(true); // 设置最后一个分块突出
          m_pieChartSeries->setHoleSize(0.5);                                                                                     // 设置中心空心圆大小
          m_chartViewWidgets->chart()->addSeries(m_pieChartSeries);                                             // 显示饼图
}