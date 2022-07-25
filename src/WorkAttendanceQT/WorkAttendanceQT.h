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
          void videoStreamClose();                          //�ر���Ƶ��
          void takePictureForTranning();                 //������Ƭ
          void savePicture();                                    //������Ƭ
          void ignorePicture();                                 //������Ƭ
          void registerEmployee();                           //����ѵ���߳�
private:
          Ui::WorkAttendanceQTClass ui;
          std::vector<std::thread> m_threadPool;
}; 