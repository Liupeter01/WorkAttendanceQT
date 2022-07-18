#pragma once
#include <QtWidgets/QDialog>
#include "ui_WorkAttendanceQT.h"
#include<qdebug.h>
#include"../ImageProcess/ImageProcess.h"

class WorkAttendanceQT : public QDialog
{
          Q_OBJECT

public:
          WorkAttendanceQT(QWidget* parent = nullptr);
          ~WorkAttendanceQT();
          //void test() 
          //{
          //          while (1) {
          //                    QImage pp = mat2Qimage(getImageFrame(this->m_cameraMutex));
          //                    pp = pp.scaled(ui.VideoDisplay->width(), ui.VideoDisplay->height());             //×ª»»·Ö±æÂÊ
          //                    ui.VideoDisplay->setPixmap(QPixmap::fromImage(pp));

          //          }
          //}
          
private:
          Ui::WorkAttendanceQTClass ui;
          //std::mutex m_cameraMutex;
          //std::thread th1;
          ImageProcess* p = nullptr;
}; 