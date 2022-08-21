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
 * @DBProcess����ϵͳϵͳͨ���û�ID��ȡ�û���
 * @name :   getUserNameFromUserID
 * @funtion : ͨ���û�ID��ȡ�û���
 * @RetValue : �����û�������Ϣ std::string
 *------------------------------------------------------------------------------------------------------*/
std::string DBProcess::getUserNameFromUserID(const std::string& _userID)
{
          std::vector<std::vector<std::string>> dbRes = this->dbSelect(this->m_SelectUserName + _userID);
          return std::string(dbRes[0][0].c_str());
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

/*------------------------WorkAttendanceSys����ϵͳ����Ա����-------------------------*/
/*------------------------------------------------------------------------------------------------------
* ��admim UI�н�ϵͳ����������
* @name: updateAdminParam2DB
* @param 1.ѵ�������趨��const std::string& _TranningSet,
*                2.ѵ�����ƶȣ�const std::string& _TranningSimilarity,
*                3.�ٵ�ʱ�䣺const std::string& _LateTimeSet,
*                4.����ʱ�䣺const std::string& _LeaveEarilyTimeSet
*                5.�Ѿ���½��ADMIN ID��const std::string & _loggedUserID
* 
*@Correction: 2022-8-20 �޸�ʱ����Ҫʹ�õ����Ž��б�ʶ��bug
*------------------------------------------------------------------------------------------------------*/
bool DBProcess::updateAdminParam2DB(
          const std::string& _TranningSet,
          const std::string& _TranningSimilarity,
          const std::string& _LateTimeSet,
          const std::string& _LeaveEarilyTimeSet,
          const std::string & _loggedUserID
)
{
          return this->dbModify(
                    this->m_UpdateAdminParam + " TrainningSetting = " + _TranningSet + "," +
                    " TrainningSimilarity = " + _TranningSimilarity +"," +
                    " MorningshiftTime = " + "\'" + _LateTimeSet + "\'" + "," +
                    " NightshiftTime = " + "\'" + _LeaveEarilyTimeSet + "\'" + " WHERE UserID = " + _loggedUserID
          );
}

/*------------------------------------------------------------------------------------------------------
 *  �����ڶ�Ԫ�鼯���и���UserID��ѯ�û�����
 * @name: queryUserNameFromID
 * @function������Ա����UserID��ѯ�û�������ԭ��һ��Ԫ����ĸ�Ԫ����չΪ���Ԫ��
 * @param:
 *                  1.��Ԫ�鼯�ϣ�  std::vector<std::vector<std::string>>
*------------------------------------------------------------------------------------------------------*/
void DBProcess::queryUserNameFromID(std::vector<std::vector<std::string>>& _matrix)
{
          for (auto& i : _matrix) {                                                                                                                   //�ڶ�Ԫ���в���һ����������
                    i.insert(i.begin() + 1, this->getUserNameFromUserID(i.at(0)));                                           //��������
          }
}

/*------------------------WorkAttendanceSys����ϵͳ����Աͳ�ƹ���-------------------------*/
/*------------------------------------------------------------------------------------------------------
 *  �����ݿ�������Ա������
 * @name: getEmployeeCount
 * @function�������ݿ�������Ա������
 * @param:
 *                  1. Ա���ţ� const  std::string& employeeNumber
*                   2. ���� ��  const std::string& _name
*                   3. ���� ��  const std::string& _department
*------------------------------------------------------------------------------------------------------*/
int DBProcess::getEmployeeCount(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _department
)
{
          std::string stringQuery = this->m_SelectEmployeeCount +  " WHERE Department = " + "\"" + _department + "\"";
          if (employeeNumber.size()) {
                    stringQuery = stringQuery + "  AND UserID = " + employeeNumber;
          }
          if (_name.size()) {
                    stringQuery = stringQuery + " AND UserID = " + "(SELECT UserID FROM employee WHERE UserName = " + "\"" + _name + "\"" + ")";
          }

          return this->dbSelect(stringQuery).size();
}

/*------------------------------------------------------------------------------------------------------
 *  �����ݿ���������ǰ�����ʱ���¼
 * @name: getMinimiseTime
 * @function��������ǰ�����ʱ���¼
 * @param:.ѡ��ǩ��ǩ�˼�¼����AttendanceTable _tableSelect
*------------------------------------------------------------------------------------------------------*/
QDateTime DBProcess::getMinimiseTime(AttendanceTable _tableSelect)
{
          std::vector<std::vector<std::string>> retRes = this->dbSelect(
                    _tableSelect == AttendanceTable::ATTENDANCE ?
                    this->m_SelectAttendenceMinTime : m_SelectSignOutMinTime
          );
          return QDateTime::fromString(QString::fromLocal8Bit(retRes[0][0].c_str()), QString::fromLocal8Bit("yyyy-MM-dd hh:mm:ss"));
}

/*------------------------------------------------------------------------------------------------------
 *  �����ݿ���������ǰ����е��ڵ���Ա���������������γ�ӳ��
 * @name: adminStatisticsOnDutyCount
 * @function������Ա����UserID��ѯ�û�������ԭ��һ��Ԫ����ĸ�Ԫ����չΪ���Ԫ��
 * @param:
 *                  1. Ա���ţ� const  std::string& employeeNumber
*                   2. ���� ��  const std::string& _name
*                   3. ���� ��  const std::string& _department
 *                  4.ѡ��ǩ��ǩ�˼�¼����AttendanceTable _tableSelect
 *                  5. ��ʱ�䣺 const QDateTime _leftTimer
*                   6. �Ҳ�ʱ�䣺 const QDateTime _rightTimer
*                   7. Ա��״̬��const std::string _employeeStatus
*------------------------------------------------------------------------------------------------------*/
int DBProcess::adminStatisticsOnDutyCount(
          const  std::string& employeeNumber,
          const  std::string& _name,
          const std::string& _department,
          AttendanceTable _tableSelect,
          const QDateTime _leftTimer,
          const QDateTime _rightTimer,
          const std::string _employeeStatus
)
{
          std::string stringQuery = (_tableSelect == AttendanceTable::ATTENDANCE ?
                    this->m_SelectAttendenceOnDutyCount : this->m_SelectSignOutOnDutyCount) +
                    " BETWEEN " + "\'" + _leftTimer.toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData() + "\'" +
                    "   AND " + "\'" + _rightTimer.toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData() + "\'" + "  AND  " +
                    (_tableSelect == AttendanceTable::ATTENDANCE ? " islate = " : " isearly = ") +
                    "\"" + _employeeStatus + "\"";

          stringQuery = stringQuery + (_tableSelect == AttendanceTable::ATTENDANCE ? " AND attendence.Department = " : " AND signout.Department = ") + "\"" + _department + "\"";
          if (employeeNumber.size()) {
                    stringQuery = stringQuery + (_tableSelect == AttendanceTable::ATTENDANCE ? " AND attendence.UserID = " : " AND signout.UserID = ") + employeeNumber;
          }
          if (_name.size()) {
                    stringQuery = stringQuery +
                    (_tableSelect == AttendanceTable::ATTENDANCE ? " AND attendence.UserID = " : " AND signout.UserID = ")+
                   "(SELECT UserID FROM employee WHERE UserName = " + "\"" + _name + "\"" + ")";
          }
          return this->dbSelect(stringQuery).size();
}

/*-----------------------WorkAttendanceSys����ϵͳ����Ա��Ա������------------------------*/
/*------------------------------------------------------------------------------------------------------
 * �Զ�Ԫ��ķ�ʽ�����ݿ�����Ա�����ݽ��ж�ȡ
 * @name: readAttendenceRecord
 * @retValue:  ����һ����Ԫ��ļ��� std::vector<std::vector<std::string>>
 *------------------------------------------------------------------------------------------------------*/
std::vector<std::vector<std::string>> DBProcess::readNewEmployeeRecord()
{
          return this->dbSelect(this->m_SelectNewEmployee);
}

/*------------------------------------------------------------------------------------------------------
 * ���¶�Ԫ����ָ��Ա��������״̬
 * @name: updateNewEmployeeAuth
 * @param : 1. Ա���ţ� const  std::string& employeeNumber
*                   2. ���� ��  const std::string& _name
*                   3. �Ƿ�ͨ�� bool status
*
 * @retValue:  bool
 *------------------------------------------------------------------------------------------------------*/
bool DBProcess::updateNewEmployeeAuth(
          const  std::string& employeeNumber,
          const  std::string& _name,
          bool status
)
{
          return this->dbModify(
                    this->m_updateNewEmployee + "\"" + (status ? "����ͨ��" : "���벵��") + "\"" +
                    " WHERE UserID = " + employeeNumber + " AND UserName = " + "\"" + _name + "\""
          );
}