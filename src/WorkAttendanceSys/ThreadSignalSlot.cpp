//#include"WorkAttendanceSys.h"
//
//int ThreadSignalSlot::m_processValue = 1;         //����ϵͳ�Ľ�����
//
///*---------------------WorkAttendanceSys����ϵͳ�����źź���--------------------------*/
// /*------------------------------------------------------------------------------------------------------
// * �ǽ��������źź���
// * @name : processIncrementSignal
// * @funtion : ����ִ���ǽ��������źź���
// *------------------------------------------------------------------------------------------------------*/
//void ThreadSignalSlot::processIncrementSignal()
//{
//          //this->ui_sys->progressBar->setValue(this->m_processValue);
//          this->m_processValue += 1;                                                                //��������
//}
//
///*------------------------------------------------------------------------------------------------------
//* ���ý��������źź���
//* @name : processResetSignal
//* @funtion : ����ִ�����ý��������źź���
//*------------------------------------------------------------------------------------------------------*/
//void ThreadSignalSlot::processResetSignal()
//{
//          this->m_processValue = 1;
//          //this->ui_sys->progressBar->setValue(this->m_processValue);
//}
//
///*-----------------WorkAttendanceSys����ϵͳ�����Ͷ���ۺ���-----------------------*/
// /*------------------------------------------------------------------------------------------------------
// * �ǽ������������Ͷ���ۺ���Emit Signal
// * @name : callProcessIncreEmitSignal
// * @funtion : ���ڲ���emit�����������Ĳۺ���
// *------------------------------------------------------------------------------------------------------*/
//void  ThreadSignalSlot::callProcessIncreEmitSignal()
//{
//          emit processIncrementSignal();          //�����������������źź���
//}
//
///*------------------------------------------------------------------------------------------------------
//* ���ý������������Ͷ���ۺ���Emit Signal
//* @name :callProcessResetEmitSignal
//* @funtion : ���ڲ���emit���ý������Ĳۺ���
//*------------------------------------------------------------------------------------------------------*/
//void ThreadSignalSlot::callProcessResetEmitSignal()
//{
//          emit processResetSignal();               //�������ý��������źź���
//}