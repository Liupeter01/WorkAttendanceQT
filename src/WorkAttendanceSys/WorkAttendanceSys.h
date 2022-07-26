#pragma once
#include "ui_WorkAttendanceSys.h"
#include <QtWidgets/QDialog>
#include"../Interface/Interface.h"

class WorkAttendanceSys : public QDialog, public Interface
{
    Q_OBJECT

public:
          WorkAttendanceSys(QWidget* parent = nullptr);
          ~WorkAttendanceSys();

private slots:
          /*------------------------------------------------------------------------------------------------------
          * 视频流的关闭开关
          * @name : videoStreamClose
          * @funtion : 提供操作槽SLOT供给signal信号调用
          *------------------------------------------------------------------------------------------------------*/
          void videoStreamClose();

          /*------------------------------------------------------------------------------------------------------
           * 在训练模型时作为训练集的拍照开关
           * @name : takePictureForTranning
           * @funtion : 提供操作槽SLOT供给signal信号调用
           *------------------------------------------------------------------------------------------------------*/
          void takePictureForTranning();

          /*------------------------------------------------------------------------------------------------------
           * 开始使用训练模型进行训练集的训练
           * @name : initModelTranning
           * @funtion : 提供操作槽SLOT供给signal信号调用
           *------------------------------------------------------------------------------------------------------*/
          void initModelTranning();

          /*------------------------------------------------------------------------------------------------------
           * 保存当前的照片图像
           * @name : savePicture
           * @funtion : 提供操作槽SLOT供给signal信号调用
           *------------------------------------------------------------------------------------------------------*/
          void savePicture();

          /*------------------------------------------------------------------------------------------------------
           * 舍弃当前的照片图像
           * @name : ignorePicture
           * @funtion : 提供操作槽SLOT供给signal信号调用
           *------------------------------------------------------------------------------------------------------*/
          void ignorePicture();

          /*------------------------------------------------------------------------------------------------------
           * 摁下当前开关启动人脸的训练程序
           * @name : registerEmployee
           * @funtion : 提供操作槽SLOT供给signal信号调用
           *------------------------------------------------------------------------------------------------------*/
          void registerEmployee();

private:
          Ui::WorkAttendanceSys ui;
          std::vector<std::thread> m_threadPool;
};
