#include "patientfile.h"
#include "ui_patientfile.h"
#include <QDebug>


PatientFile::PatientFile(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PatientFile)
{
    ui->setupUi(this);
    ui->table_patient->setRowCount(20);//устанавливает количество строк в таблице
    ui->table_patient->setColumnCount(3);//устанавливает количество столбцов
    ui->table_patient->setHorizontalHeaderLabels(QStringList()<<"Фамилия"<<"Имя"<<"Отчество");//добавление названия столбцам
    ui->horizontalLayout->setStretchFactor(ui->table_patient,2);
    ui->horizontalLayout->setStretchFactor(ui->verticalLayout_3,1);
}

PatientFile::~PatientFile()
{
    delete ui;
}


void PatientFile::on_add_note_clicked()
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
          // connect(itm, SIGNAL(cellClicked()), this, SLOT(on_cell_clicked()));
           ui->table_patient->setItem(j,i,itm);}
       j++;
    }
    else{

        QMessageBox msg;
        msg.setWindowTitle("Пустое поле");
        msg.setText("Внимание!\nВы не заполнили все поля. Добавление невозможно.");
        msg.exec();
}
    ui->name->clear();
    ui->surname->clear();
    ui->patronymic->clear();
}


void PatientFile::on_change_note_clicked()
{
    if(ui->name->text()!=""&&ui->surname->text()!=""&&ui->patronymic->text()!=""){

    }
}

void PatientFile::on_cell_clicked(){
    qDebug()<<'kek';
}

void PatientFile::on_table_patient_cellClicked(int row, int column)
{
    qDebug()<<row<<column;
}

