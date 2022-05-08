#include "patientfilewindow.h"
#include "ui_patientfilewindow.h"

PatientFileWindow::PatientFileWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PatientFileWindow)
{
    ui->setupUi(this);
    ui->table_patient->setRowCount(20);//устанавливает количество строк в таблице
    ui->table_patient->setColumnCount(3);//устанавливает количество столбцов
    ui->table_patient->setHorizontalHeaderLabels(QStringList()<<"Фамилия"<<"Имя"<<"Отчество");//добавление названия столбцам
    ui->horizontalLayout->setStretchFactor(ui->table_patient,2);
    ui->horizontalLayout->setStretchFactor(ui->verticalLayout_3,1);
    QString str2="";
   for(int q=j;q<ui->table_patient->rowCount();q++)
   {
       for(int i=0;i<ui->table_patient->columnCount();i++){
       itm=  new QTableWidgetItem(str2);
       ui->table_patient->setItem(q,i,itm);}
   }
}

PatientFileWindow::~PatientFileWindow()
{
    QFile save_table("D:/QTprojects/patient-file/table.csv");
    if(save_table.open(QIODevice::WriteOnly)){
        QTextStream st(&save_table);
    for(int k=0; k< ui->table_patient->rowCount(); k++)
    {
       for(int q = 0; q< ui->table_patient->columnCount(); q++)
       {
           if(ui->table_patient->item(k,q)->text()!="")
            st << ui->table_patient->item(k,q)->text() << ';';
       }
       if(ui->table_patient->item(k,0)->text()!="")
        st << '\n';
    }
    save_table.close();
    }
    else qDebug()<<"Ошибка открытия файла для записи.";
    delete ui;
}

void PatientFileWindow::clear()
{
    ui->name->clear();
    ui->surname->clear();
    ui->patronymic->clear();
}


void PatientFileWindow::on_add_note_clicked()
{
    if(ui->name->text()!=""&&ui->surname->text()!=""&&ui->patronymic->text()!=""){
       QString str;
       for(int i=0;i<ui->table_patient->columnCount();i++)
       {
           if(i==0)
               str=ui->surname->text();
           else if(i==1)
               str=ui->name->text();
           else
               str=ui->patronymic->text();
           itm=  new QTableWidgetItem(str);
           ui->table_patient->setItem(j,i,itm);}
       j++;
    }
    else{

        QMessageBox msg;
        msg.setWindowTitle("Пустое поле");
        msg.setText("Внимание!\nВы не заполнили все поля. Добавление невозможно.");
        msg.exec();
}
    clear();
}


void PatientFileWindow::on_change_note_clicked()
{
    if(ui->name->text()!=""&&ui->surname->text()!=""&&ui->patronymic->text()!=""){
        int row=ui->table_patient->currentRow();
        QString str;
        for(int i=0;i<ui->table_patient->columnCount();i++)
        {
            if(i==0)
                str=ui->surname->text();
            else if(i==1)
                str=ui->name->text();
            else
                str=ui->patronymic->text();
            itm=  new QTableWidgetItem(str);
            ui->table_patient->setItem(row,i,itm);}
        clear();
    }
}


void PatientFileWindow::on_table_patient_cellClicked(int row)
{
    if(ui->table_patient->item(row, 0)->text()!=""){
    ui->surname->setText(ui->table_patient->item(row, 0)->text());
    ui->name->setText(ui->table_patient->item(row, 1)->text());
    ui->patronymic->setText(ui->table_patient->item(row, 2)->text());
    }
}


void PatientFileWindow::on_delete_note_clicked()
{
    int row=ui->table_patient->currentRow();
    if(ui->name->text()!=""&&ui->surname->text()!=""&&ui->patronymic->text()!=""){
        dia.setModal(true);
        dia.exec();
        if(dia.result()==Dialog::Accepted){
            QString str="";
            for(int i=0;i<ui->table_patient->columnCount();i++)
            {
                itm=  new QTableWidgetItem(str);
                ui->table_patient->setItem(row,i,itm);}
            clear();
            for(int q=row, row_count = ui->table_patient->rowCount()-1;q<row_count;++q){
            for(int i=0;i<ui->table_patient->columnCount();i++)
            {
                str=ui->table_patient->item(q+1,i)->text();
                itm=  new QTableWidgetItem(str);
                ui->table_patient->setItem(q,i,itm);
            }
            }
            if (j>0) j--;
            else j=0;
        }
    }
}

void PatientFileWindow::on_upload_clicked()
{

}

