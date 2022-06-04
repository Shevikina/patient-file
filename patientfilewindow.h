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
#include <QtSql>
#include "dialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QObject>
#include <QLineEdit>
#include <QMessageBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QSettings>

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
        void clearInputFields();
        QStandardItemModel *csvModel;
        QTableWidgetItem *createItem(QString text);
        void getNullMessage();
        bool isSemicolon();
        bool isOverlapTable(QString line);


    private slots:
        void on_add_note_clicked();

        void on_change_note_clicked();

        void on_delete_note_clicked();

        void on_table_patient_db_clicked(const QModelIndex &index);

        void on_export_cvs_triggered();

        void blockedButton_textChanget();

        void on_import_cvs_triggered();

        void filter_notes_textChanget();

        void on_reset_filter_note_clicked();

    private:
        Ui::PatientFileWindow *ui;
        QTableWidgetItem *itm;
        QSqlDatabase patient_bd;
        QSqlTableModel *model;

};
#endif // PATIENTFILE_H
