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
          void slot1() {
                    this->Videoclose();
          }

private:
          Ui::WorkAttendanceQTClass ui;
          std::thread m_test;
}; 