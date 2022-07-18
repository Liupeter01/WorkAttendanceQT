#pragma once

#include <QtWidgets/QDialog>
#include "ui_WorkAttendanceQT.h"

class WorkAttendanceQT : public QDialog
{
    Q_OBJECT

public:
    WorkAttendanceQT(QWidget *parent = nullptr);
    ~WorkAttendanceQT();

private:
    Ui::WorkAttendanceQTClass ui;
};
