#include "patientfilewindow.h"
#include "ui_patientfilewindow.h"

PatientFileWindow::PatientFileWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PatientFileWindow) {
    ui->setupUi(this);
    ui->table_patient->setColumnCount(3);//устанавливает количество столбцов
    ui->table_patient->setHorizontalHeaderLabels(QStringList() << "Фамилия" << "Имя" <<
                                                 "Отчество"); //добавление названия столбцам
    //Загружаем сохраненные данные
    QFile upload_table("/table.csv");
    if (upload_table.exists()) {
        if (upload_table.open(QIODevice::ReadOnly)) {
            QTextStream ut(&upload_table);//upload table
            int row_iterator = 0;
            while (!ut.atEnd()) {
                QString line = ut.readLine();//считываем построчно
                int column_iterator = 0;
                ui->table_patient->setRowCount(row_iterator + 1);
                for (QString item : line.split(";")) {
                    itm =  createItem(item);
                    ui->table_patient->setItem(row_iterator, column_iterator, itm);
                    column_iterator++;
                }
                row_iterator++;
            }
            upload_table.close();
        }
        else qDebug() << "Ошибка открытия файла для чтения.";
    }
    ui->horizontalLayout->setStretchFactor(ui->table_patient, 2);
    ui->horizontalLayout->setStretchFactor(ui->verticalLayout_3, 1);
    ui->table_patient->setSelectionBehavior(
        QAbstractItemView::SelectRows);//выделение строки при нажатии на ячейку
}

PatientFileWindow::~PatientFileWindow() {
    QFile save_table("/table.csv");
    if (save_table.open(QIODevice::WriteOnly)) {
        QTextStream st(&save_table);//save table
        bool stop_writing = false;
        for (int row_iterator = 0; row_iterator < ui->table_patient->rowCount(); row_iterator++) {
            for (int column_iterator = 0; column_iterator < ui->table_patient->columnCount(); column_iterator++) {
                QTableWidgetItem *item = ui->table_patient->item(row_iterator, column_iterator);
                if (!item) {
                    stop_writing = true;
                    break;
                }
                if (item->text() != "")
                    st << ui->table_patient->item(row_iterator, column_iterator)->text() << ';';
            }
            if (stop_writing) break;
            st << '\n';
        }
        save_table.close();
    }
    else qDebug() << "Ошибка открытия файла для записи.";
    delete ui;
}

void PatientFileWindow::clear() {
    ui->name->clear();
    ui->surname->clear();
    ui->patronymic->clear();
}

QTableWidgetItem *PatientFileWindow::createItem(QString text) {
    QTableWidgetItem *itm =  new QTableWidgetItem(text);
    Qt::ItemFlags flags = itm->flags();
    flags.setFlag(Qt::ItemIsEditable, false);
    itm->setFlags(flags);
    return itm;
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
    if (name.contains(";") || surname.contains(";") || patronymic.contains(";")) {
        is_semicolon = true;
        QMessageBox msg;
        msg.setWindowTitle("Недопустимый символ");
        msg.setText("Внимание! "
                    "\nВ одном из полей ввода присутсвует недопустимый символ \";\""
                    "\nДобавление или изменение поля невозможно.");
        msg.exec();
    }
    return is_semicolon;
}


void PatientFileWindow::on_add_note_clicked() {
    if (!isSemicolon()) {
        if (ui->name->text() != "" && ui->surname->text() != "" && ui->patronymic->text() != "") {
            QString str;
            int last_row = ui->table_patient->rowCount();
            ui->table_patient->setRowCount(last_row + 1);
            for (int column_iterator = 0; column_iterator < ui->table_patient->columnCount(); column_iterator++) {
                if (column_iterator == 0)
                    str = ui->surname->text();
                else if (column_iterator == 1)
                    str = ui->name->text();
                else
                    str = ui->patronymic->text();
                itm =  createItem(str);
                ui->table_patient->setItem(last_row, column_iterator, itm);
            }
            clear();
        }
        else
            getNullMessage();
    }
}


void PatientFileWindow::on_change_note_clicked() {
    if (!isSemicolon()) {
        if (ui->name->text() != "" && ui->surname->text() != "" && ui->patronymic->text() != "") {
            int row = ui->table_patient->currentRow();
            QString str;
            for (int column_iterator = 0; column_iterator < ui->table_patient->columnCount(); column_iterator++) {
                if (column_iterator == 0)
                    str = ui->surname->text();
                else if (column_iterator == 1)
                    str = ui->name->text();
                else
                    str = ui->patronymic->text();
                itm =  createItem(str);
                ui->table_patient->setItem(row, column_iterator, itm);
            }
            clear();
        }
        else
            getNullMessage();
    }
}


void PatientFileWindow::on_table_patient_cellClicked(int row) {
    QTableWidgetItem *item = ui->table_patient->item(row, 0);
    if (!item) {
        clear();
        return;
    }
    if (item->text() != "") {
        ui->surname->setText(ui->table_patient->item(row, 0)->text());
        ui->name->setText(ui->table_patient->item(row, 1)->text());
        ui->patronymic->setText(ui->table_patient->item(row, 2)->text());
    }
}


void PatientFileWindow::on_delete_note_clicked() {
    int row = ui->table_patient->currentRow();
    if (ui->name->text() != "" && ui->surname->text() != "" && ui->patronymic->text() != "") {
        dia.setModal(true);
        dia.exec();
        if (dia.result() == Dialog::Accepted) {
            QString str = "";
            for (int column_iterator = 0; column_iterator < ui->table_patient->columnCount(); column_iterator++) {
                itm =  createItem(str);
                ui->table_patient->setItem(row, column_iterator, itm);
            }
            clear();
            for (int row_iterator = row; row_iterator < ui->table_patient->rowCount() - 1; ++row_iterator) {
                for (int column_iterator = 0; column_iterator < ui->table_patient->columnCount(); column_iterator++) {
                    str = ui->table_patient->item(row_iterator + 1, column_iterator)->text();
                    itm =  createItem(str);
                    ui->table_patient->setItem(row_iterator, column_iterator, itm);

                }
            }
            //очищаем все ячейки в строке
            int lastRow = ui->table_patient->rowCount() - 1;
            for (int column_iterator = 0; column_iterator < ui->table_patient->columnCount(); column_iterator++) {
                ui->table_patient->setItem(lastRow, column_iterator, 0);
            }
            ui->table_patient->setRowCount(lastRow);

        }
    }
}



