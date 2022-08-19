#include "DBProcess.h"

DBProcess::DBProcess()
{
          this->connect();
}

DBProcess::~DBProcess()
{
          
}

/*------------------------WorkAttendanceSys����ϵͳ��ʼ��--------------------------------*/
/*------------------------------------------------------------------------------------------------------
 * @DBProcess����ϵͳϵͳ�����趨initDepartmentSetting
 * @name :  initDepartmentSetting
 * @funtion : ��ʼ��ϵͳ�����趨DepartmentSetting
 * @RetValue : ���ز��ŵ���Ϣ std::vector<std::string>
 *------------------------------------------------------------------------------------------------------*/
std::vector<std::string> DBProcess::initDepartmentSetting()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(m_SelectDepartment);       //�����ݿ��л�ȡ����
          std::vector<std::string> _departmentSetting;                                                                       //������Ϣ���ݼ�
          for (int i = 0; i < dbRes.size(); ++i) {
                    _departmentSetting.push_back(dbRes[i][0].c_str());
          }
          return _departmentSetting;
}

/*------------------------------------------------------------------------------------------------------
* @WorkAttendanceSys����ϵͳѵ���������趨TranningSetting
* @name :  initTranningSetting
* @funtion : ��ʼ��ѵ���������趨TranningSetting
* @RetValue : int
*------------------------------------------------------------------------------------------------------*/
int DBProcess::initTranningSetting()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(this->m_SelectTrainningSetting);    //�����ݿ��л�ȡTrainningSetting
          return std::atoi(dbRes[0][0].c_str());                                                                                                  //����TrainningSetting
}

/*------------------------------------------------------------------------------------------------------
* @DBProcess����ϵͳ��������ƥ�����TranningSimilarity
* @name :  initTrainningSimilarity
* @funtion : ��������ƥ�����std::length
* @RetValue : double
*------------------------------------------------------------------------------------------------------*/
double DBProcess::initTrainningSimilarity()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(this->m_SelectTrainningSimilarity);   //�����ݿ��л�ȡTranningSimilarity
          return std::atof(dbRes[0][0].c_str());                                                                                                     //����TranningSimilarity
}

/*------------------------------------------------------------------------------------------------------
* @DBProcess����ϵͳ��ȡ����Ա�趨�����ϳٵ���ʱ��
* @name :  getMorningShiftTime
* @funtion : ��ȡ����Ա�趨�����ϳٵ���ʱ��
*------------------------------------------------------------------------------------------------------*/
QTime DBProcess::getMorningShiftTime()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(this->m_SelectMorningShiftTime);   //�����ݿ��л�ȡMorningShiftTime
          return QTime::fromString(dbRes[0][0].c_str());
}

/*------------------------------------------------------------------------------------------------------
* @DBProcess����ϵͳ��ȡ����Ա�趨���������˵�ʱ��
* @name :  getNightshiftTime
* @funtion : ��ȡ����Ա�趨���������˵�ʱ��
*------------------------------------------------------------------------------------------------------*/
QTime DBProcess::getNightshiftTime()
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(this->m_SelectNightshiftTime);   //�����ݿ��л�ȡNightshiftTime
          return QTime::fromString(dbRes[0][0].c_str());
}

/*--------------------WorkAttendanceSys����ϵͳ�������ݿ����-----------------------*/
/*------------------------------------------------------------------------------------------------------
 * ��������Ϣ�����ݿ�������ȡ����֤�Ƿ����
 * @name : readFaceRecordFromDB
 * @function����ȡԱ����UserID��Ӧ��������Ϣ
 * @param��1. Ա���� �� const  std::string& employeeNumber
 *                   2. Ա������ �� const std::string& _userName
 *                   3. ���� �� const std::string & _department
 *
 * @retValue : ����UserID��Ӧ��������Ϣ
 * @Correction: 2022-7-31 �޸�dbSelect�Ĳ����ַ����Ĵ���
 *------------------------------------------------------------------------------------------------------*/
std::string  DBProcess::readFaceRecordFromDB(
          const  std::string& employeeNumber,
          const std::string& _userName,
          const std::string& _department
)
{
          /*��Ӷ��ڵ�¼�û�����Ϣȷ�ϳ���*/
          std::vector<std::vector<std::string>> isUserExist = this->dbSelect(                                                                                     //�ж��Ƿ���ڵ�ǰ��Ա��
                    this->m_SelectEmployeeString + employeeNumber + " AND Department = " + "\"" + _department + "\""
          );

          if (!isUserExist.size() || std::string(isUserExist[0][0].c_str()) != _userName) {                                                                   //���ڵ�ǰ��Ա��
                    return std::string();                                                                                                                                                       //Ա��Ϊ��
          }
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(this->m_SelectMatrixString + employeeNumber);   //������������
          return std::string(dbRes[0][0].c_str());
}

/*------------------------------------------------------------------------------------------------------
* ��������Ϣ��¼���ݿ�
* @name:storeFaceRecord2DB
* @param 1. Ա���ţ� const  std::string& employeeNumber
*                2. ���� �� const  std::string& _name
*                3. ���� �� const std::string & _department
*                4. �沿���� ��const std::string& _faceMatrix
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::storeFaceRecord2DB(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _department,
          const std::string& _faceMatrix) 
{
          bool employee = this->dbInsert(m_Insert_table_employee + employeeNumber + "," + "\"" + _name + "\"" + "," + "\"" + _department + "\"" + ")");    //��employee����н��в���
          bool matrix = this->dbInsert(m_Insert_table_facematrixstorge + employeeNumber + "," + "\"" + _faceMatrix + "\"" + ")");                                           //��facematrixstorge����в���
         return employee && matrix;
}

/*------------------------------------------------------------------------------------------------------
* ��������Ϣ���������ݿ���
* @name: updateFaceRecord2DB
* @param 1. Ա���ţ� const  std::string& employeeNumber
*                2. ���� �� const  std::string& _name
*                3. �沿���� ��const std::string& _faceMatrix
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::updateFaceRecord2DB(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _faceMatrix
)
{
          return true;
}

/*---------------------WorkAttendanceSys����ϵͳ������Ϣ����-------------------------*/
/*------------------------------------------------------------------------------------------------------
* ��Ա���Ƿ��ڽ��մ����ظ��Ĵ򿨼�¼
* @name: readDuplicateRecordFromDB
* @param 1. Ա���ţ� const  std::string& employeeNumber
*                2. ���� ��  const std::string& _department
*                3. ѡ���ǩ���� const AttendanceTable _table
*                4.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
* 
*@retValue: ���ڣ�����Ϊtrue 
                     �����ڣ�����Ϊfalse
*------------------------------------------------------------------------------------------------------*/
bool  DBProcess::readDuplicateRecordFromDB(
          const  std::string& employeeNumber,
          const std::string& _department,
          const AttendanceTable _table,
          QDateTime*& _timer
)
{
          std::vector<std::vector<std::string>> isExist = this->dbSelect(
                    (_table == AttendanceTable::ATTENDANCE ? this->m_SelectDuplicateAttendence : this->m_SelectDuplicateSignOut) +
                    employeeNumber + " AND Department = " + "\"" + _department + "\"" +
                    " AND MorningshiftTime LIKE " + "\"" + _timer->currentDateTime().toString("yyyy-MM-dd").toLocal8Bit().constData() + " %" + "\""
          );
          return isExist.size() ? true : false;
}

/*------------------------------------------------------------------------------------------------------
* ��Ա���ϰ�򿨵���Ϣ��¼�����ݿ���
* @name: storeAttendanceRecord2DB
* @param 1. Ա���ţ� const  std::string& employeeNumber
*                2. ���� ��  const std::string& _department
*                3. �Ƿ��Ѿ��ٵ���const std::string _islate
*                4.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
*
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::storeAttendanceRecord2DB(
          const  std::string& employeeNumber,
          const std::string& _department,
          const std::string _islate,
          QDateTime*& _timer
)
{
          return this->dbInsert(this->m_Insert_table_attendence + employeeNumber + "," +
                    "\"" + _department + "\"" + "," +
                    "\"" + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData() + "\"" + "," +
                    "\"" + _islate + "\"" + ")"
          );
}

/*------------------------------------------------------------------------------------------------------
* ��Ա���°�ǩ�˵���Ϣ��¼�����ݿ���
* @name: storeSignOutRecord2DB
* @param 1. Ա���ţ� const  std::string& employeeNumber
*                2. ���� ��  const std::string& _department
*                3. �Ƿ��Ѿ��ٵ���const std::string& _islate
*                4.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
*
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::storeSignOutRecord2DB(
          const  std::string& employeeNumber,
          const std::string& _department,
          const std::string& _islate,
          QDateTime*& _timer
)
{
          return this->dbInsert(this->m_Insert_table_signout + employeeNumber + "," +
                    "\"" + _department + "\"" + "," +
                    "\"" + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData() + "\"" + "," +
                    "\"" + _islate + "\"" + ")"
          );
}

/*------------------------------------------------------------------------------------------------------
* �Զ�Ԫ��ķ�ʽ�����ݿ��е����ݽ��ж�ȡ
* @name: readAttendenceRecord
* @param 1. Ա���ţ� const  std::string& employeeNumber
*                2. ������ const  std::string& _name
*                3. ���� ��  const std::string& _department
*                4. ��ʱ�䣺 const QDateTime _leftTimer
*                5. �Ҳ�ʱ�䣺 const QDateTime _rightTimer
*                6. ѡ��ǩ��ǩ�˼�¼��AttendanceTable tableSelect
*                7. �Ƿ�ѡ��ʱ��: bool _isTimeEnabled
*
* @retValue:  ����һ����Ԫ��ļ��� std::vector<std::vector<std::string>>
*------------------------------------------------------------------------------------------------------*/
std::vector<std::vector<std::string>> DBProcess::readAttendenceRecord(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _department,
          const QDateTime _leftTimer,
          const QDateTime _rightTimer,
          AttendanceTable tableSelect,
          bool _isTimeEnabled
)
{
          std::string stringQuery{                                                                                                                                               //��ʼ�ַ���
                   tableSelect == AttendanceTable::ATTENDANCE ? this->m_SelectStaticsAttendenceData : this->m_SelectStaticsSignoutData + " "
          };
          stringQuery = stringQuery + " Department = " + "\"" + _department + "\"";
          if (employeeNumber.size()) {
                    stringQuery = stringQuery + "  AND UserID = " + employeeNumber;
          }
          if (_name.size()) {
                    stringQuery = stringQuery + " AND UserID = " + "(SELECT UserID FROM employee WHERE UserName = " + "\"" + _name + "\"" + ")";
          }
          if (_isTimeEnabled) {                                                                                                                                                 //�Ƿ�ѡ��ʱ��
                    stringQuery = stringQuery + " AND " +
                              (tableSelect == AttendanceTable::ATTENDANCE ? "MorningShiftTime" : "NightShiftTime") +
                              " BETWEEN " + "\'" + _leftTimer.toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData() + "\'"+ " AND " +
                              "\'" + _rightTimer.toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData() + "\'" ;

          }
          return this->dbSelect(stringQuery);
}

/*---------------------WorkAttendanceSys����ϵͳ��Ա���������---------------------*/
/*------------------------------------------------------------------------------------------------------
* ����Ա����ע����Ϣ��ϵͳ����Ա����Ȩ������
* @name: storeAskPremitRecord2DB
* @param 1. Ա���ţ� const  std::string& employeeNumber
*                2. ���� ��  const std::string& _name
*                3. ���� ��  const std::string& _department
*                4.ȫ��ʱ��ϵͳ������ QDateTime*& _timer
*
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::storeAskPremitRecord2DB(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _department,
          QDateTime*& _timer
)
{
          return this->dbInsert(this->m_Insert_table_askpremit + employeeNumber + "," +
                    "\"" + _name + "\"" + "," +
                    "\"" + _department + "\"" + "," +
                    "\"" + "Ȩ��������..." + "\"" + ","
                    "\"" + _timer->currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData() + "\"" + ")"
          );
}

/*------------------------------------------------------------------------------------------------------
* ��ѯ��Ա����ע����Ϣ��ʾ����Ļ��
* @name: checkPremitRecordFromDB
* @param 1. Ա���ţ� const  std::string& employeeNumber
*                2. ���� ��  const std::string& _name
*                3. ���� ��  const std::string& _department
*
* @retValue : �������Ȩ�� std::string
* @Correction: 2022-8-3 �޸�SELECT��ѯ����ֵ����Ϊ�յ����
*------------------------------------------------------------------------------------------------------*/
std::string DBProcess::checkPremitRecordFromDB(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _department
)
{
          //SELECT AuthoritySet FROM askpremit WHERE UserID = 999 AND 
          std::vector<std::vector<std::string>> isPriviledgePremit = this->dbSelect(
                    this->m_SelectAskpremit + employeeNumber +
                    "  AND UserName =  " + "\"" + _name + "\"" +
                    "  AND Department = " + "\"" + _department + "\""
          );
          if (!isPriviledgePremit.size()) {                                                                             //�������Ϣ����������
                    return std::string();                                                                                       
          }
          return std::string(isPriviledgePremit[0][0].c_str());
}

/*---------------------WorkAttendanceSys����ϵͳ����Ա����---------------------*/
/*------------------------------------------------------------------------------------------------------
* ��ѯϵͳ����Ա��Ȩ��
* @name: readAdminPriviledgeFromDB
* @param 1. Ա���ţ� const  std::string& employeeNumber
*                2. ���� ��  const std::string& _name
*                3. ���� ��  const std::string& _department
*
* @retValue : �����Ƿ�鵽 bool
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::readAdminPriviledgeFromDB(
          const  std::string& employeeNumber,
          const std::string& _userName,
          const std::string& _department
)
{
          return true;
}