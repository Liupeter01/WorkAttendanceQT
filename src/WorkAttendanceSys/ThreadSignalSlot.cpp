//#include"WorkAttendanceSys.h"
//
//int ThreadSignalSlot::m_processValue = 1;         //人脸系统的进度条
//
///*---------------------WorkAttendanceSys考勤系统申明信号函数--------------------------*/
// /*------------------------------------------------------------------------------------------------------
// * 涨进度条的信号函数
// * @name : processIncrementSignal
// * @funtion : 用于执行涨进度条的信号函数
// *------------------------------------------------------------------------------------------------------*/
//void ThreadSignalSlot::processIncrementSignal()
//{
//          //this->ui_sys->progressBar->setValue(this->m_processValue);
//          this->m_processValue += 1;                                                                //进度增长
//}
//
///*------------------------------------------------------------------------------------------------------
//* 重置进度条的信号函数
//* @name : processResetSignal
//* @funtion : 用于执行重置进度条的信号函数
//*------------------------------------------------------------------------------------------------------*/
//void ThreadSignalSlot::processResetSignal()
//{
//          this->m_processValue = 1;
//          //this->ui_sys->progressBar->setValue(this->m_processValue);
//}
//
///*-----------------WorkAttendanceSys考勤系统申明和定义槽函数-----------------------*/
// /*------------------------------------------------------------------------------------------------------
// * 涨进度条的申明和定义槽函数Emit Signal
// * @name : callProcessIncreEmitSignal
// * @funtion : 用于产生emit增长进度条的槽函数
// *------------------------------------------------------------------------------------------------------*/
//void  ThreadSignalSlot::callProcessIncreEmitSignal()
//{
//          emit processIncrementSignal();          //产生增长进度条的信号函数
//}
//
///*------------------------------------------------------------------------------------------------------
//* 重置进度条的申明和定义槽函数Emit Signal
//* @name :callProcessResetEmitSignal
//* @funtion : 用于产生emit重置进度条的槽函数
//*------------------------------------------------------------------------------------------------------*/
//void ThreadSignalSlot::callProcessResetEmitSignal()
//{
//          emit processResetSignal();               //产生重置进度条的信号函数
//}