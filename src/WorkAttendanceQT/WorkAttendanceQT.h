#pragma once
#include <QtWidgets/QDialog>
#include "ui_WorkAttendanceQT.h"
#include<qdebug.h>
#include"../Interface/Interface.h"

class WorkAttendanceQT : public QDialog,public  Interface
{
          Q_OBJECT

public:
          WorkAttendanceQT(QWidget* parent = nullptr);
          ~WorkAttendanceQT();
          
private slots:
         void videoStreamClose() {
                    this->videoClose();
          }
         void takePictureForTranning() {
                   this->videoCameraShooting();
          }

         void registerEmployee()
         {

         }

private:
          Ui::WorkAttendanceQTClass ui;
          std::thread m_test;
}; 