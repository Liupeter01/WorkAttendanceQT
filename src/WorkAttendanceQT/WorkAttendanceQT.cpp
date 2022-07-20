#include "WorkAttendanceQT.h"

WorkAttendanceQT::WorkAttendanceQT(QWidget *parent)
    : QDialog(parent), Interface(10)
{
    ui.setupUi(this);
    m_test = std::thread(&Interface::QTVideoOutput, this, std::ref(this->ui.VideoDisplay));
    //th1 = std::thread(&WorkAttendanceQT::test, this);
}

WorkAttendanceQT::~WorkAttendanceQT()
{
          m_test.join();
}
