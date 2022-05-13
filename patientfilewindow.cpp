#include "patientfilewindow.h"
#include "ui_patientfilewindow.h"

PatientFileWindow::PatientFileWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PatientFileWindow)
{
    ui->setupUi(this);
    ui->table_patient->setColumnCount(3);//устанавливает количество столбцов
    ui->table_patient->setHorizontalHeaderLabels(QStringList()<<"Фамилия"<<"Имя"<<"Отчество");//добавление названия столбцам
    //Загружаем сохраненные данные
    QFile upload_table("/table.csv");
    if(upload_table.open(QIODevice::ReadOnly)){
        QTextStream ut(&upload_table);
        int c=0;//итератор по строкам
        while(!ut.atEnd()){
            QString line = ut.readLine();//считываем построчно(а не считываем ли всегда одну и ту же строку?)
            int l=0;//итератор по столбцам
            ui->table_patient->setRowCount(c+1);
            for (QString item : line.split(";")){
                  itm=  createItem(item);
                  ui->table_patient->setItem(c,l,itm);
                  l++;
            }
            c++;
        }
    upload_table.close();
    }
    else qDebug()<<"Ошибка открытия файла для чтения.";
    ui->horizontalLayout->setStretchFactor(ui->table_patient,2);
    ui->horizontalLayout->setStretchFactor(ui->verticalLayout_3,1);
}

PatientFileWindow::~PatientFileWindow()
{
    QFile save_table("/table.csv");
    if(save_table.open(QIODevice::WriteOnly)){
        QTextStream st(&save_table);
        bool stop_writing=false;
    for(int k=0; k< ui->table_patient->rowCount(); k++)
    {
       for(int q = 0; q< ui->table_patient->columnCount(); q++)
       {
           QTableWidgetItem* item=ui->table_patient->item(k,q);
           if(!item){
               stop_writing=true;
               break;
           }
           if(item->text()!="")
            st << ui->table_patient->item(k,q)->text() << ';';
       }
       if(stop_writing) break;
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

QTableWidgetItem *PatientFileWindow::createItem(QString text)
{
    QTableWidgetItem* itm=  new QTableWidgetItem(text);
    Qt::ItemFlags flags=itm->flags();
    flags.setFlag(Qt::ItemIsEditable,false);
    itm->setFlags(flags);
    return itm;
}

void PatientFileWindow::nullMessage()
{
    QMessageBox msg;
    msg.setWindowTitle("Пустое поле");
    msg.setText("Внимание!\nВы не заполнили все поля. Добавление или изменение невозможно.");
    msg.exec();
}


void PatientFileWindow::on_add_note_clicked()
{
    if(ui->name->text()!=""&&ui->surname->text()!=""&&ui->patronymic->text()!=""){
       QString str;
       int last_row=ui->table_patient->rowCount()+1;
       ui->table_patient->setRowCount(last_row);
       for(int i=0;i<ui->table_patient->columnCount();i++)
       {
           if(i==0)
               str=ui->surname->text();
           else if(i==1)
               str=ui->name->text();
           else
               str=ui->patronymic->text();
           itm=  createItem(str);
           ui->table_patient->setItem(last_row-1,i,itm);}
       j++;
       clear();
    }
    else
    {
        nullMessage();
    }
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
            itm=  createItem(str);
            ui->table_patient->setItem(row,i,itm);}
        clear();
    }
    else
        nullMessage();
}


void PatientFileWindow::on_table_patient_cellClicked(int row)
{
    QTableWidgetItem* item=ui->table_patient->item(row, 0);
    if(!item)
    {
        clear();
        return;}
    if(item->text()!=""){
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
                itm=  createItem(str);
                ui->table_patient->setItem(row,i,itm);}
            clear();
            for(int q=row, row_count = ui->table_patient->rowCount()-1;q<row_count;++q){
            for(int i=0;i<ui->table_patient->columnCount();i++)
            {
                str=ui->table_patient->item(q+1,i)->text();
                itm=  createItem(str);
                ui->table_patient->setItem(q,i,itm);
            }
            }
            int lastRow=ui->table_patient->rowCount()-1;
            for(int i=0;i<ui->table_patient->columnCount();i++)
            {
                ui->table_patient->setItem(lastRow,i,0);
            }
            ui->table_patient->setRowCount(lastRow);
            if (j>0) j--;
            else j=0;
        }
    }
}



