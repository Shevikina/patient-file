#include "patientfile.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PatientFile w;
    w.show();
    return a.exec();
}
