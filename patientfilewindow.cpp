#include "patientfilewindow.h"
#include "ui_patientfilewindow.h"

PatientFileWindow::PatientFileWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PatientFileWindow) {
    ui->setupUi(this);
    patient_bd=QSqlDatabase::addDatabase("QSQLITE");
    patient_bd.setDatabaseName("patient_file");
    if(!patient_bd.open()){
        qDebug()<<"Ошибка открытия базы данных "<<patient_bd.lastError().text();
    }
    else {
        QSqlQuery create_patient("CREATE TABLE patient("
                         "name VARCHAR(20),"
                         "surname VARCHAR(30),"
                         "patronymic VARCHAR(30))");
        model=new QSqlTableModel(this, patient_bd);
        model->setTable("patient");
        model->select();
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);//сохранение происходит при выполнении submitAll()
        model->setHeaderData(0, Qt::Horizontal, tr("Фамилия"));
        model->setHeaderData(1, Qt::Horizontal, tr("Имя"));
        model->setHeaderData(2, Qt::Horizontal, tr("Отчество"));
        ui->table_patient_db->setModel(model);
        ui->horizontalLayout->setStretchFactor(ui->table_patient_db, 2);
        ui->horizontalLayout->setStretchFactor(ui->verticalLayout_3, 1);
        ui->table_patient->hide();
        ui->table_patient_db->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->table_patient_db->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}


PatientFileWindow::~PatientFileWindow() {
    delete ui;
}


void PatientFileWindow::clear() {
    ui->name->clear();
    ui->surname->clear();
    ui->patronymic->clear();
}


void PatientFileWindow::getNullMessage() {
    QMessageBox msg;
    msg.setWindowTitle("Пустое поле");
    msg.setText("Внимание!\nВы не заполнили все поля. Добавление или изменение невозможно.");
    msg.exec();
}


void PatientFileWindow::on_add_note_clicked() {
        if (ui->name->text() != "" && ui->surname->text() != "" && ui->patronymic->text() != "") {
            int last_row_db=model->rowCount();
            model->insertRow(last_row_db);
            QString str="";
            for(int column_db_iterator=0;column_db_iterator<model->columnCount();column_db_iterator++){
                if (column_db_iterator == 0)
                    str=ui->surname->text();
                else if (column_db_iterator == 1)
                    str=ui->name->text();
                else
                    str=ui->patronymic->text();
                model->setData(model->index(last_row_db,column_db_iterator),str);
            }
            if(!model->submitAll())
                qDebug()<<"Ошибка ввода записей";
            clear();
        }
        else
            getNullMessage();
}


void PatientFileWindow::on_change_note_clicked() {
        if (ui->name->text() != "" && ui->surname->text() != "" && ui->patronymic->text() != "") {
            int row_db=ui->table_patient_db->currentIndex().row();
            qDebug()<<row_db;
            QString str="";
            for(int column_db_iterator=0;column_db_iterator<model->columnCount();column_db_iterator++){
                if (column_db_iterator == 0)
                    str=ui->surname->text();
                else if (column_db_iterator == 1)
                    str=ui->name->text();
                else
                    str=ui->patronymic->text();
                model->setData(model->index(row_db,column_db_iterator),str);
            }
            if(!model->submitAll())
                qDebug()<<"Ошибка ввода записей";
            clear();
        }
        else
            getNullMessage();
}


void PatientFileWindow::on_delete_note_clicked() {
        dia.setModal(true);
        dia.exec();
        if (dia.result() == Dialog::Accepted) {
            model->removeRow(ui->table_patient_db->currentIndex().row());
            model->submitAll();
            clear();
    }
}


void PatientFileWindow::on_table_patient_db_clicked(const QModelIndex &index)
{
    QSqlRecord row=model->record(index.row());
    QVariant value;
    for(int column_iterator=0;column_iterator<model->columnCount();column_iterator++){
    value=row.value(column_iterator);
    if(column_iterator==0)
        ui->surname->setText(value.toString());
    else if(column_iterator==1)
        ui->name->setText(value.toString());
    else
        ui->patronymic->setText(value.toString());
    }
}

