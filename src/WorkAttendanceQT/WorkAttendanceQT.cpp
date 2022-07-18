#include "WorkAttendanceQT.h"

WorkAttendanceQT::WorkAttendanceQT(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    //th1 = std::thread(&WorkAttendanceQT::test, this);
    p = new ImageProcess(10);
}

WorkAttendanceQT::~WorkAttendanceQT()
{

}
