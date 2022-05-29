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
        QString getLastRowFile();


    private slots:
        void on_add_note_clicked();

        void on_change_note_clicked();

        void on_delete_note_clicked();

        void on_table_patient_db_clicked(const QModelIndex &index);

        void on_filter_notes_clicked();

        void on_reset_filter_notes_clicked();

        void on_save_table_clicked();

        void on_download_filter_clicked();

        void on_download_save_table_clicked();

private:
        Ui::PatientFileWindow *ui;
        QTableWidgetItem *itm;
        QSqlDatabase patient_bd;
        QSqlTableModel *model;

};
#endif // PATIENTFILE_H
