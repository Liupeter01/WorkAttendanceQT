#include "WorkAttendanceQT.h"

WorkAttendanceQT::WorkAttendanceQT(QWidget* parent)
          : QDialog(parent), Interface(10)
{
          ui.setupUi(this);
          QObject::connect(ui.Exit, SIGNAL(clicked()), this, SLOT(videoStreamClose()));
          QObject::connect(ui.TakePicture, SIGNAL(clicked()), this, SLOT(takePictureForTranning()));
          QObject::connect(ui.Register, SIGNAL(clicked()), this, SLOT(registerEmployee()));
          m_test = std::thread(&Interface::QTVideoOutput, this, std::ref(this->ui.VideoDisplay));
}

WorkAttendanceQT::~WorkAttendanceQT()
{
          this->videoClose();
          m_test.join();
}
