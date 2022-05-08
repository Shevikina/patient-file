#ifndef PATIENTFILE_H
#define PATIENTFILE_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class PatientFile; }
QT_END_NAMESPACE

class PatientFile : public QMainWindow
{
    Q_OBJECT

public:
    PatientFile(QWidget *parent = nullptr);
    ~PatientFile();

private slots:
    void on_add_note_clicked();

    void on_change_note_clicked();

    void on_cell_clicked();

    void on_table_patient_cellClicked(int row, int column);

private:
    Ui::PatientFile *ui;
    int j=0;
    QTableWidgetItem* itm;

};
#endif // PATIENTFILE_H
