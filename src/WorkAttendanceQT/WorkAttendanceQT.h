#pragma once
#include "ui_WorkAttendanceQT.h"
#include <QtWidgets/QDialog>
#include"../Interface/Interface.h"

class WorkAttendanceQT : public QDialog,public  Interface{
          Q_OBJECT
public:
          WorkAttendanceQT(QWidget* parent = nullptr);
          ~WorkAttendanceQT();
private slots:
          void videoStreamClose();                          //πÿ±’ ”∆µ¡˜
          void takePictureForTranning();                 //≈ƒ…„’’∆¨
          void savePicture();                                    //±£¥Ê’’∆¨
          void ignorePicture();                                 //∫ˆ¬‘’’∆¨
          void registerEmployee();                           //∆Ù∂Ø—µ¡∑œﬂ≥Ã
private:
          Ui::WorkAttendanceQTClass ui;
          std::vector<std::thread> m_threadPool;
}; 