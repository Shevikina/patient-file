#include "patientfilewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PatientFileWindow w;
    w.show();
    return a.exec();
}
