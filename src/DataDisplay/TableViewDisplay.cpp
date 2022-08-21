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

/*--------------------------------------DataDisplay��ʼ��-------------------------------------*/
/*------------------------------------------------------------------------------------------------------
* @DataDisplay��ʼ��ģ����Ϣ
* @name : setDataDisplayModel
* @funtion : ��ʼ��ģ����Ϣ
*------------------------------------------------------------------------------------------------------*/
void DataDisplay::setDataDisplayModel()
{
          this->m_attendenceRecord->setModel(this->m_attendenceRecordModel);
          this->m_newEmployee->setModel(this->m_newEmployeeModel);
}

/*------------------------------------------------------------------------------------------------------
* @DataDisplay��ʼ��TableView��������Ϣ
* @name :   initTableViewRowName
* @funtion :��ʼ������ϵͳ�Ĵ򿨼�¼����
*------------------------------------------------------------------------------------------------------*/
void DataDisplay::initTableViewRowName()
{
          int RecordRowCount(0);
          this->m_attendenceRecordModel->setColumnCount(static_cast<int>(this->m_attendenceRecordRow.size()));       //��������
          this->m_newEmployeeModel->setColumnCount(static_cast<int>(this->m_newEmployeeRow.size()));                  //��������
          for (auto i : this->m_attendenceRecordRow) {
                    this->m_attendenceRecordModel->setHeaderData(RecordRowCount++, Qt::Horizontal, QString::fromLocal8Bit(i.c_str()));
          }
          RecordRowCount = 0;                                                                                                                                                  //������ֵ
          for (auto i : this->m_newEmployeeRow) {
                    this->m_newEmployeeModel->setHeaderData(RecordRowCount++, Qt::Horizontal, QString::fromLocal8Bit(i.c_str()));
          }
}

/*--------------------------------------DataDisplay����-------------------------------------*/
/*------------------------------------------------------------------------------------------------------
* @DataDisplay��Բ��ͼ��ͼ������н��
* @name :   unboundSeriesFromChart
* @funtion :��ʼ������ϵͳ�Ĵ򿨼�¼����
* @Correction: 2022-8-20 �ú�������������ʱ���Բ��ͼ��ͼ�����ֹ�������
*------------------------------------------------------------------------------------------------------*/
void DataDisplay::unboundSeriesFromChart()
{
          m_chartViewWidgets->chart()->removeAllSeries();             //���
}

/*------------------------------DataDisplay��TableView������-------------------------------*/
/*------------------------------------------------------------------------------------------------------
* @DataDisplay����ǩ����¼TableView��Ԫ����Ϣ
* @name :  setAttendenceTableTuple
* @funtion :����ǩ����¼TableView��Ԫ����Ϣ
* @param: ��Ԫ��ļ��� const std::vector<std::vector<std::string>>& _matrix
*------------------------------------------------------------------------------------------------------*/
void DataDisplay::setAttendenceTableTuple(const std::vector<std::vector<std::string>>& _matrix)
{
          this->m_attendenceRecordModel->clear();
          this->m_attendenceRecordModel->setColumnCount(static_cast<int>(this->m_attendenceRecordRow.size()));       //��������
          for (auto i : this->m_attendenceRecordRow) {
                    this->m_attendenceRecordModel->setHeaderData(m_attendenceRowCount++, Qt::Horizontal, QString::fromLocal8Bit(i.c_str()));
          }
          this->m_attendenceLineCount = 0;                                //�����е�ѭ������
          this->m_attendenceRowCount = 0;                                //�����е�ѭ������
          for (int i = 0; i < _matrix.size(); ++i) {                            //��ѭ��
                    for (auto j : _matrix.at(i)) {                                    //��ѭ��
                              this->m_attendenceRecordModel->setItem(
                                        this->m_attendenceLineCount,       //�е�ѭ����������
                                        this->m_attendenceRowCount++,
                                        new QStandardItem(QString::fromLocal8Bit(j.c_str()))
                              );
                    }
                    this->m_attendenceLineCount++;
                    this->m_attendenceRowCount = 0;                     //�����е�ѭ������
          }
          this->m_attendenceLineCount = 0;                               //�����е�ѭ������
          this->m_attendenceRowCount = 0;                               //�����е�ѭ������
}

/*------------------------------------------------------------------------------------------------------
* @DataDisplay����ǩ�˼�¼TableView��Ԫ����Ϣ
* @name :  setNewEmployeeTableTuple
* @funtion :����ǩ�˼�¼TableView��Ԫ����Ϣ
* @param: ��Ԫ��ļ��� const std::vector<std::vector<std::string>>& _matrix
*------------------------------------------------------------------------------------------------------*/
void DataDisplay::setNewEmployeeTableTuple(const std::vector<std::vector<std::string>>& _matrix)
{
          this->m_newEmployeeModel->clear();
          int RecordRowCount(0);
          this->m_newEmployeeModel->setColumnCount(static_cast<int>(this->m_newEmployeeRow.size()));                  //��������
          for (auto i : this->m_newEmployeeRow) {
                    this->m_newEmployeeModel->setHeaderData(RecordRowCount++, Qt::Horizontal, QString::fromLocal8Bit(i.c_str()));
          }
          this->m_newEmployeeLineCount = 0;
          this->m_newEmployeeRowCount = 0;                     //�����е�ѭ������
          for (int i = 0; i < _matrix.size(); ++i) {                            //��ѭ��
                    for (auto j : _matrix.at(i)) {                                    //��ѭ��
                              this->m_newEmployeeModel->setItem(
                                        this->m_newEmployeeLineCount,       //�е�ѭ����������
                                        this->m_newEmployeeRowCount++,
                                        new QStandardItem(QString::fromLocal8Bit(j.c_str()))
                              );
                    }
                    this->m_newEmployeeLineCount++;
                    this->m_newEmployeeRowCount = 0;                     //�����е�ѭ������
          }
          this->m_newEmployeeLineCount = 0;                               //�����е�ѭ������
          this->m_newEmployeeRowCount = 0;                               //�����е�ѭ������
}

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
void DataDisplay::setChartViewData(bool status, double _total, double _late, double _normal)
{
          m_pieChartSeries->clear();
          m_chartViewWidgets->chart()->setTitle(status ?                                              // ���ñ�ͼ����
                    QString::fromLocal8Bit("��ǰɸѡ�����µ�ǩ�����ͳ��") :
                    QString::fromLocal8Bit("��ǰɸѡ�����µ�ǩ�����ͳ��")
          );
          m_chartViewWidgets->setRenderHint(QPainter::Antialiasing);                      // ���ÿ����
          m_chartViewWidgets->chart()->setTheme(QChart::ChartThemeQt);             // ���ñ�ͼ����
          if (_total <= 0.000000001 && _total >= -0.000000001)                                  // _total��Ϊ0
          {
                    double res = 0.0f;
                    /*---------------------------------------��ͼ�������-------------------------------------*/
                    m_pieChartSeries->append( status ? QString::fromLocal8Bit("�ٵ��ϰ����") : QString::fromLocal8Bit("�����°����"),res);
                    m_pieChartSeries->append(status ? QString::fromLocal8Bit("�����ϰ����") : QString::fromLocal8Bit("�����°����"), res);
                    m_pieChartSeries->append(QString::fromLocal8Bit("δ�򿨱���"), res);
          }
          else
          {
                    /*---------------------------------------��ͼ�������-------------------------------------*/
                    m_pieChartSeries->append(
                              status ? QString::fromLocal8Bit("�ٵ��ϰ����") : QString::fromLocal8Bit("�����°����"),
                              static_cast<float>(_late / _total)
                    );
                    m_pieChartSeries->append(
                              status ? QString::fromLocal8Bit("�����ϰ����") : QString::fromLocal8Bit("�����°����"),
                              static_cast <float>(static_cast<float>(_normal / _total))
                    );
                    m_pieChartSeries->append(
                              QString::fromLocal8Bit("δ�򿨱���"),
                              static_cast<float>(1.0f - static_cast<float>(_normal / _total) - static_cast<float>(_late / _total))
                    );
                    /*---------------------------------------��ͼ���ÿ��ǩ-------------------------------------*/
                    for (int sNum = 0; sNum < m_pieChartSeries->slices().count(); sNum++)
                    {
                              QPieSlice* slice = m_pieChartSeries->slices().at(sNum);
                              QString tagText = QString("%1%2%3%4")
                                        .arg(slice->label())
                                        .arg(QStringLiteral("ռ��"))
                                        .arg(QString::number(slice->percentage() * 100))
                                        .arg(QStringLiteral("%"));
                              slice->setLabel(tagText);
                    }
          }
          m_pieChartSeries->setLabelsPosition(QPieSlice::LabelPosition::LabelOutside);                        // ���ñ�ǩ��ʾλ��
          m_pieChartSeries->setLabelsVisible(true);                                                                               // ��ʾ��ǩ
          m_pieChartSeries->slices().at(m_pieChartSeries->slices().count() - 1)->setExploded(true); // �������һ���ֿ�ͻ��
          m_pieChartSeries->setHoleSize(0.5);                                                                                     // �������Ŀ���Բ��С
          m_chartViewWidgets->chart()->addSeries(m_pieChartSeries);                                             // ��ʾ��ͼ
}