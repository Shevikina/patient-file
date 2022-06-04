#include "patientfilewindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    PatientFileWindow w;
    QCoreApplication::setOrganizationName("SHEVIKINA");
    QCoreApplication::setApplicationName("PatientFile");
    w.show();
    return a.exec();
}
