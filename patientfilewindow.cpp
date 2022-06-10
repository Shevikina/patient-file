#include "patientfilewindow.h"
#include "ui_patientfilewindow.h"


PatientFileWindow::PatientFileWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PatientFileWindow) {
    ui->setupUi(this);
    patient_bd = QSqlDatabase::addDatabase("QSQLITE");
    patient_bd.setDatabaseName("patient_file");
    if (!patient_bd.open()) {
        qDebug() << "Ошибка открытия базы данных " << patient_bd.lastError().text();
    }
    else {
        QSqlQuery create_patient("CREATE TABLE patient("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "surname TEXT,"
                                 "name TEXT,"
                                 "patronymic TEXT)");
        QSqlQuery query("PRAGMA case_sensitive_like=OFF");

        model = new QSqlTableModel(this, patient_bd);
        model->setTable("patient");
        model->select();
        model->setEditStrategy(
            QSqlTableModel::OnManualSubmit);//сохранение происходит при выполнении submitAll()
        model->setHeaderData(1, Qt::Horizontal, tr("Фамилия"));
        model->setHeaderData(2, Qt::Horizontal, tr("Имя"));
        model->setHeaderData(3, Qt::Horizontal, tr("Отчество"));
        ui->table_patient_db->setModel(model);
        ui->horizontalLayout->setStretchFactor(ui->verticalLayout_5, 2);
        ui->horizontalLayout->setStretchFactor(ui->verticalLayout_3, 1);
        ui->table_patient_db->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->table_patient_db->setEditTriggers(QAbstractItemView::NoEditTriggers);

        ui->add_note->setEnabled(false);
        ui->change_note->setEnabled(false);
        ui->delete_note->setEnabled(false);

        QObject::connect(ui->name, &QLineEdit::textChanged, this, &PatientFileWindow::blockedButton_textChanget);
        QObject::connect(ui->surname, &QLineEdit::textChanged, this, &PatientFileWindow::blockedButton_textChanget);
        QObject::connect(ui->patronymic, &QLineEdit::textChanged, this, &PatientFileWindow::blockedButton_textChanget);
        QObject::connect(ui->name_for_filter, &QLineEdit::textChanged, this, &PatientFileWindow::filter_notes_textChanget);
        QObject::connect(ui->surname_for_filter, &QLineEdit::textChanged, this, &PatientFileWindow::filter_notes_textChanget);
        QObject::connect(ui->patronymic_for_filter, &QLineEdit::textChanged, this,
                         &PatientFileWindow::filter_notes_textChanget);

        ui->reset_button->setIcon(QIcon(":/cancel_icon.png"));
        QObject::connect(ui->reset_button, &QPushButton::clicked, this, &PatientFileWindow::reset_filter_notes_triggered);

        ui->name_for_filter->setClearButtonEnabled(true);
        ui->surname_for_filter->setClearButtonEnabled(true);
        ui->patronymic_for_filter->setClearButtonEnabled(true);

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


bool PatientFileWindow::isSemicolon() {
    bool is_semicolon = false;

    QString name = ui->name->text();
    QString surname = ui->surname->text();
    QString patronymic = ui->patronymic->text();

    QString name_filter = ui->name_for_filter->text();
    QString surname_filter = ui->surname_for_filter->text();
    QString patronymic_filter = ui->patronymic_for_filter->text();

    QMessageBox msg;
    msg.setWindowTitle("Недопустимый символ");
    msg.setText("Внимание! "
                "\nВ одном из полей ввода присутсвует недопустимый символ \";\""
                "\nДобавление, изменение или сохраннение поля невозможно.");

    if (name != "" && surname != "" && patronymic != "") {
        if (name.contains(";") || surname.contains(";") || patronymic.contains(";")) {
            is_semicolon = true;
            msg.exec();
        }
    }
    else {
        if (name_filter != "" || surname_filter != "" || patronymic_filter != "")
            if (name_filter.contains(";") || surname_filter.contains(";") || patronymic_filter.contains(";")) {
                is_semicolon = true;
                msg.exec();
            }
    }
    return is_semicolon;
}


void PatientFileWindow::on_add_note_clicked() {
    if (!isSemicolon()) {
        int last_row_db = model->rowCount();
        model->insertRow(last_row_db);
        QString str = "";
        for (int column_db_iterator = 1; column_db_iterator < model->columnCount(); column_db_iterator++) {
            if (column_db_iterator == 1)
                str = ui->surname->text();
            else if (column_db_iterator == 2)
                str = ui->name->text();
            else if (column_db_iterator == 3)
                str = ui->patronymic->text();
            model->setData(model->index(last_row_db, column_db_iterator), str);
        }
        if (!model->submitAll())
            qDebug() << "Ошибка ввода записей";
        clearInputFields();
    }
}


void PatientFileWindow::on_change_note_clicked() {
    if (!isSemicolon()) {
        int row_db = ui->table_patient_db->currentIndex().row();
        qDebug() << row_db;
        QString str = "";
        for (int column_db_iterator = 1; column_db_iterator < model->columnCount(); column_db_iterator++) {
            if (column_db_iterator == 1)
                str = ui->surname->text();
            else if (column_db_iterator == 2)
                str = ui->name->text();
            else if (column_db_iterator == 3)
                str = ui->patronymic->text();
            model->setData(model->index(row_db, column_db_iterator), str);
        }
        if (!model->submitAll())
            qDebug() << "Ошибка ввода записей";
        clearInputFields();
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

void PatientFileWindow::blockedButton_textChanget() {
    QString name = ui->name->text();
    QString surname = ui->surname->text();
    QString patronymic = ui->patronymic->text();
    if (model->data(model->index(0, 0)).isNull()) {
        if (name == "" || surname == "" || patronymic == "")
            ui->add_note->setEnabled(false);
        else
            ui->add_note->setEnabled(true);
    }
    else {
        if (name == "" || surname == "" || patronymic == "") {
            ui->add_note->setEnabled(false);
            ui->change_note->setEnabled(false);
            ui->delete_note->setEnabled(false);
        }
        else {
            ui->add_note->setEnabled(true);
            ui->change_note->setEnabled(true);
            ui->delete_note->setEnabled(true);
        }
    }
}


void PatientFileWindow::on_table_patient_db_clicked(const QModelIndex &index) {
    QSqlRecord row = model->record(index.row());
    QVariant value;
    for (int column_iterator = 1; column_iterator < model->columnCount(); column_iterator++) {
        value = row.value(column_iterator);
        if (column_iterator == 1)
            ui->surname->setText(value.toString());
        else if (column_iterator == 2)
            ui->name->setText(value.toString());
        else if (column_iterator == 3)
            ui->patronymic->setText(value.toString());
    }
}


void PatientFileWindow::on_export_cvs_triggered() {
    if (!isSemicolon()) { //добавлена проверка на запрещенные символы на случай, если кнопка фильтр не была нажатаю, а сохранение таблицы произошло
        QFileDialog export_table_dialog;
        QString url_export_table = export_table_dialog.getSaveFileName(0, "", "", "*.csv");
        qDebug() << url_export_table;
        if (url_export_table != "") {
            QFile save_table(url_export_table);
            if (save_table.open(QIODevice::WriteOnly)) {
                QTextStream st(&save_table);
                bool stop_writing = false;
                for (int row_iterator = 0; row_iterator < model->rowCount(); row_iterator++) {
                    for (int column_iterator = 0; column_iterator < model->columnCount(); column_iterator++) {
                        QVariant item = model->data(model->index(row_iterator, column_iterator));
                        if (item.isNull()) {
                            stop_writing = true;
                            break;
                        }
                        if (item.toString() != "")
                            st << item.toString() << ';';
                    }
                    if (stop_writing) break;
                    st << '\n';
                }
                save_table.close();
                qDebug() << "Запись таблицы прошла успешно";
            }
            else qDebug() << "Ошибка открытия файла для записи.";
        }
        else
            qDebug() << "Была нажата копка отмены. Таблица не экспортирована.";
    }
}

bool PatientFileWindow::isOverlapTable(QString line) {
    int row = 0;
    int overlap_count = 0;
    bool overlap = false;
    for (int column_iterator = 0; column_iterator < model->columnCount();) {
        for (QString item : line.split(";")) {
            if (column_iterator == 0) {
                column_iterator++;
                continue;
            }
            overlap = false;
            for (int row_iterator = row; row_iterator < model->rowCount(); row_iterator++) {
                QVariant cell_table = model->data(model->index(row_iterator, column_iterator));
                if (item == cell_table.toString()) {
                    overlap = true;
                    overlap_count++;
                    row = row_iterator;
                    column_iterator++;
                    break;
                }
            }
        }
        if (!overlap || overlap_count == model->columnCount())
            break;
    }
    if (overlap_count == model->columnCount()) {
        qDebug() << "Совпадение найдено, дальше...";
        return false;
    }
    else {
        qDebug() << "Совпадение не найдены. Добавляем!";
        return true;
    }
}

void PatientFileWindow::on_import_cvs_triggered() {
    QSettings setting("SHEVIKINA", "PatientFile");
    int msg_exec;
    if (setting.value("message_exec").toBool())
        msg_exec = setting.value("/setting/merger").toInt();
    else {
        QMessageBox msg;
        msg.setText("Сливать совпадающие данные?");
        msg.setWindowTitle("Настройка импорта");
        msg.addButton("Соединять", QMessageBox::YesRole);
        msg.addButton("Дублировать", QMessageBox::NoRole);
        QCheckBox msg_save_setting("Больше не спрашивать.");
        msg.setCheckBox(&msg_save_setting);
        msg_exec = msg.exec();
        if (msg_save_setting.isChecked()) {
            setting.setValue("message_exec", true);
            if (msg_exec == 0)
                setting.setValue("/setting/merger", 0);
            else
                setting.setValue("/setting/merger", 1);
        }
        qDebug() << "Больше не показывать";
    }
    QFileDialog import_table_dialog;
    QString url_import_file = import_table_dialog.getOpenFileName(0, "", "", "*.csv");
    if (url_import_file != "") {
        qDebug() << url_import_file;
        QFile import_table(url_import_file);
        if (import_table.open(QIODevice::ReadOnly)) {
            QTextStream imt(&import_table);
            while (!imt.atEnd())  {
                QString line = imt.readLine();
                int last_row = model->rowCount();
                if (msg_exec == 0) {
                    if (isOverlapTable(line)) {
                        model->insertRow(last_row);
                        for (int column_iterator = 0; column_iterator < model->columnCount();) {
                            for (QString item : line.split(";")) {
                                if (column_iterator != 0)
                                    model->setData(model->index(last_row, column_iterator), item);
                                column_iterator++;
                            }
                        }
                    }
                }
                else {
                    model->insertRow(last_row);
                    for (int column_iterator = 0; column_iterator < model->columnCount();) {
                        for (QString item : line.split(";")) {
                            if (column_iterator != 0)
                                model->setData(model->index(last_row, column_iterator), item);
                            column_iterator++;
                        }
                    }
                }
                import_table.close();
            }
            if (!model->submitAll())
                qDebug() << "Ошибка ввода записей";
        }
        else {
            qDebug() << "Ошибка открытия файла";
        }
    }
}

void PatientFileWindow::filter_notes_textChanget() {
    if (!isSemicolon()) {
        QString name = ui->name_for_filter->text();
        QString surname = ui->surname_for_filter->text();
        QString patronymic = ui->patronymic_for_filter->text();
        if (name != "" || surname != "" || patronymic != "") {
            if (name != "" && surname != "" && patronymic != "")
                model->setFilter(QString("name LIKE '%1%' AND surname LIKE '%2%' AND patronymic LIKE '%3%'").arg(name).arg(surname).arg(
                                     patronymic));
            else if (name == "" && surname != "" && patronymic != "")
                model->setFilter(QString("patronymic LIKE '%1%' AND surname LIKE '%2%'").arg(patronymic).arg(surname));
            else if (name != "" && surname == "" && patronymic != "")
                model->setFilter(QString("name LIKE '%1%' AND patronymic LIKE '%2%'").arg(name).arg(patronymic));
            else if (name != "" && surname != "" && patronymic == "")
                model->setFilter(QString("name LIKE '%1%' AND surname LIKE '%2%'").arg(name).arg(surname));
            else if (name != "" && surname == "" && patronymic == "")
                model->setFilter(QString("name LIKE '%1%'").arg(name));
            else if (name == "" && surname != "" && patronymic == "")
                model->setFilter(QString("surname LIKE '%1%'").arg(surname));
            else
                model->setFilter(QString("patronymic LIKE '%1%'").arg(patronymic));
        }
        else {
            model->setFilter(QString());
        }
    }
}

void PatientFileWindow::reset_filter_notes_triggered() {
    model->setFilter(QString());
    ui->name_for_filter->clear();
    ui->surname_for_filter->clear();
    ui->patronymic_for_filter->clear();
}

