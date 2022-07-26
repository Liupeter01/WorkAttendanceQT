#include "WorkAttendanceSys.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WorkAttendanceSys w;
    w.show();
    return a.exec();
}
