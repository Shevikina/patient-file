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
                                 "surname TEXT,"
                         "name TEXT,"
                         "patronymic TEXT)");
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
        ui->table_patient_db->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->table_patient_db->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}


PatientFileWindow::~PatientFileWindow() {
    delete ui;
}


void PatientFileWindow::clearInputFields() {
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

bool PatientFileWindow::isSemicolon() {
    bool is_semicolon = false;

    QString name = ui->name->text();
    QString surname = ui->surname->text();
    QString patronymic = ui->patronymic->text();

    QString name_filter=ui->name_for_filter->text();
    QString surname_filter=ui->surname_for_filter->text();
    QString patronymic_filter=ui->patronymic_for_filter->text();

    QMessageBox msg;
    msg.setWindowTitle("Недопустимый символ");
    msg.setText("Внимание! "
                "\nВ одном из полей ввода присутсвует недопустимый символ \";\""
                "\nДобавление, изменение или сохраннение поля невозможно.");

    if(name!=""&&surname!=""&patronymic!=""){
    if (name.contains(";") || surname.contains(";") || patronymic.contains(";")) {
        is_semicolon = true;
        msg.exec();
    }
    }
    else
    {
        if(name_filter!=""||surname_filter!=""||patronymic_filter!="")
            if(name_filter.contains(";")||surname_filter.contains(";")||patronymic_filter.contains(";")){
                is_semicolon=true;
                msg.exec();
            }
    }
    return is_semicolon;
}

void PatientFileWindow::on_add_note_clicked() {
    if(!isSemicolon()){
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
            clearInputFields();
        }
        else
            getNullMessage();
    }
}


void PatientFileWindow::on_change_note_clicked() {
    if(!isSemicolon()){
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
            clearInputFields();
        }
        else
            getNullMessage();
    }
}


void PatientFileWindow::on_delete_note_clicked() {
        dia.setModal(true);
        dia.exec();
        if (dia.result() == Dialog::Accepted) {
            model->removeRow(ui->table_patient_db->currentIndex().row());
            model->submitAll();
            clearInputFields();
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


void PatientFileWindow::on_filter_notes_clicked()
{
    if(!isSemicolon()){
    QString name=ui->name_for_filter->text();
    QString surname=ui->surname_for_filter->text();
    QString patronymic=ui->patronymic_for_filter->text();
    if(name!=""||surname!=""||patronymic!=""){
        if (name!=""&&surname!=""&&patronymic!="")
            model->setFilter(QString("name='%1' AND surname='%2' AND patronymic='%3'").arg(name).arg(surname).arg(patronymic));
        else if(name==""&&surname!=""&&patronymic!="")
            model->setFilter(QString("patronymic='%1' AND surname='%2'").arg(patronymic).arg(surname));
        else if(name!=""&&surname==""&&patronymic!="")
            model->setFilter(QString("name='%1' AND patronymic='%2'").arg(name).arg(patronymic));
        else if(name!=""&&surname!=""&&patronymic=="")
            model->setFilter(QString("name='%1' AND surname='%2'").arg(name).arg(surname));
        else if(name!=""&&surname==""&&patronymic=="")
            model->setFilter(QString("name='%1'").arg(name));
        else if(name==""&&surname!=""&&patronymic=="")
            model->setFilter(QString("surname='%1'").arg(surname));
        else
            model->setFilter(QString("patronymic='%1'").arg(patronymic));
    }
    else
        model->setFilter(QString());
    qDebug()<<model->filter();
    }
}


void PatientFileWindow::on_reset_filter_notes_clicked()
{
       model->setFilter(QString());
       ui->name_for_filter->clear();
       ui->surname_for_filter->clear();
       ui->patronymic_for_filter->clear();
}


void PatientFileWindow::on_save_table_clicked()
{
    if(!isSemicolon()){//добавлена проверка на запрещенные символы на случай, если кнопка фильтр не была нажатаю, а сохранение таблицы произошло
    QFile save_table("/table.csv");
        if(save_table.open(QIODevice::WriteOnly)){
            QTextStream st(&save_table);
            bool stop_writing=false;
        for(int row_iterator=0; row_iterator< model->rowCount(); row_iterator++)
        {
           for(int column_iterator = 0; column_iterator< model->columnCount(); column_iterator++)
           {
               QVariant item=model->data(model->index(row_iterator,column_iterator));
               if(item.isNull()){
                   stop_writing=true;
                   break;
               }
               if(item.toString()!="")
                st <<item.toString() << ';';
           }
           if(stop_writing) break;
            st << '\n';
        }
        save_table.close();
        qDebug()<<"Запись таблицы прошла успешно";
        }
        else qDebug()<<"Ошибка открытия файла для записи.";
    QFile save_filter("/filter.txt");
    if(save_filter.open(QIODevice::WriteOnly)){
        QTextStream sf(&save_filter);
        QString name=ui->name_for_filter->text();
        QString surname=ui->surname_for_filter->text();
        QString patronymic=ui->patronymic_for_filter->text();
        if(name!=""||surname!=""||patronymic!="")
            sf<<surname<<';'<<name<<';'<<patronymic;
        save_filter.close();
        qDebug()<<"Запись фильтра прошла успешно";
    }
    else
        qDebug()<<"Ошибка открытия файла для записи.";
    }
}


void PatientFileWindow::on_download_filter_clicked()
{
    QFile upload_filter("/filter.txt");
        if(upload_filter.open(QIODevice::ReadOnly)){
            QTextStream uf(&upload_filter);

            while(!uf.atEnd()){
                QString line = uf.readLine();//считываем строку
                int word_iterator=0;
                for (QString item : line.split(";")){
                    switch (word_iterator) {
                    case 0:{
                        ui->surname_for_filter->setText(item);
                        break;
                    }
                    case 1:{
                        ui->name_for_filter->setText(item);
                        break;
                    }
                    case 2:{
                        ui->patronymic_for_filter->setText(item);
                        break;
                    }
                    }
                      word_iterator++;
                }
            }
        upload_filter.close();
        }
        else qDebug()<<"Ошибка открытия файла для чтения.";
        on_filter_notes_clicked();
}

