#ifndef PATIENTFILE_H
#define PATIENTFILE_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QStringList>//для добавление названия столбцам
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStandardItem>
#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class PatientFileWindow;
}
QT_END_NAMESPACE

class PatientFileWindow : public QMainWindow {
        Q_OBJECT

    public:
        PatientFileWindow(QWidget *parent = nullptr);
        ~PatientFileWindow();
        Dialog dia;
        void clear();
        QStandardItemModel *csvModel;
        QTableWidgetItem *createItem(QString text);
        void getNullMessage();
        bool isSemicolon();

    private slots:
        void on_add_note_clicked();

        void on_change_note_clicked();

        void on_table_patient_cellClicked(int row);

        void on_delete_note_clicked();


    private:
        Ui::PatientFileWindow *ui;
        QTableWidgetItem *itm;

};
#endif // PATIENTFILE_H
