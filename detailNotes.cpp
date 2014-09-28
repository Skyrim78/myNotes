#include "detailNotes.h"

detNotes::detNotes(int _curr, QList<int> _list, QWidget *parent):QDialog(parent){
    ui.setupUi(this);
    ID = _curr;
    LIST = _list;
    ui.groupBox_search->hide();
    ui.tableWidget_search->setColumnHidden(0, true);

    QSqlQuery queryFace("select face.name from face order by face.name");
    while (queryFace.next()){
        ui.comboBox_face->addItem(queryFace.value(0).toString());
    }

    QSqlQuery queryStatus("select status.name from status order by status.name");
    while (queryStatus.next()){
        ui.comboBox_status->addItem(queryStatus.value(0).toString());
    }

    loadData();

    connect(ui.lineEdit_search, SIGNAL(textChanged(QString)), this, SLOT(searchMaterial(QString)));
    connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(searchOpen()));
    connect(ui.toolButton_closeSearch, SIGNAL(clicked()), this, SLOT(searchClose()));
    connect(ui.tableWidget_search, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(searchSelect()));

    connect(ui.checkBox_date, SIGNAL(clicked(bool)), ui.dateEdit, SLOT(setEnabled(bool)));
    connect(ui.pushButton_toFirst, SIGNAL(clicked()), this, SLOT(toFirst()));
    connect(ui.pushButton_toLast, SIGNAL(clicked()), this, SLOT(toLast()));
    connect(ui.pushButton_toPrev, SIGNAL(clicked()), this, SLOT(toPrev()));
    connect(ui.pushButton_toNext, SIGNAL(clicked()), this, SLOT(toNext()));

    connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui.pushButton_save, SIGNAL(clicked()), this, SLOT(saveNotes()));
    connect(ui.pushButton_delete, SIGNAL(clicked()), this, SLOT(deleteNotes()));
}

void detNotes::loadData(){
    if (LIST.size() == 0){
        ui.groupBox_search->setVisible(true);
        ui.lineEdit_search->setFocus();

        ui.lineEdit_articul->clear();
        ui.lineEdit_name->clear();
        ui.comboBox_face->setCurrentIndex(1);
        ui.comboBox_status->setCurrentIndex(1);
        ui.lineEdit_town->clear();
        ui.checkBox_date->setChecked(false);
        ui.dateEdit->setDate(QDate::currentDate());
        ui.dateEdit->setEnabled(false);
        ui.spinBox_print->setValue(1);



    } else if (LIST.size() > 0){
        QSqlQuery query(QString("select material.articul, material.name, face.name, status.name, "
                                "notes.town, notes.dat, notes.print "
                                "from notes, material, face, status "
                                "where notes.material = material.id and notes.status = status.id and notes.face = face.id and notes.id = \'%1\' ")
                        .arg(LIST.at(ID)));
        query.next();
        ui.lineEdit_articul->setText(query.value(0).toString());
        ui.lineEdit_name->setText(query.value(1).toString());
        ui.comboBox_face->setCurrentIndex(ui.comboBox_face->findText(query.value(2).toString()));
        ui.comboBox_status->setCurrentIndex(ui.comboBox_status->findText(query.value(3).toString()));
        ui.lineEdit_town->setText(query.value(4).toString());
        if (query.value(5).toString() == ""){
            ui.checkBox_date->setChecked(false);
            ui.dateEdit->setEnabled(false);
            ui.dateEdit->setDate(QDate::currentDate());
        } else {
            ui.checkBox_date->setChecked(true);
            ui.dateEdit->setEnabled(true);
            ui.dateEdit->setDate(QDate::fromString(query.value(5).toString(), "dd.MM.yyyy"));
        }

        ui.spinBox_print->setValue(query.value(6).toInt());
    }

    if (ID == 0){
        ui.pushButton_toFirst->setEnabled(false);
        ui.pushButton_toPrev->setEnabled(false);
    } else {
        ui.pushButton_toFirst->setEnabled(true);
        ui.pushButton_toPrev->setEnabled(true);
    }
    if (ID == LIST.size() - 1){
        ui.pushButton_toNext->setEnabled(false);
        ui.pushButton_toLast->setEnabled(false);
    } else {
        ui.pushButton_toNext->setEnabled(true);
        ui.pushButton_toLast->setEnabled(true);
    }
    if (LIST.size() == 0){
        ui.pushButton_toFirst->setEnabled(false);
        ui.pushButton_toPrev->setEnabled(false);
        ui.pushButton_toNext->setEnabled(false);
        ui.pushButton_toLast->setEnabled(false);
    }

}

void detNotes::toFirst(){
    ID = 0;
    loadData();
}

void detNotes::toPrev(){
    ID = ID - 1;
    loadData();
}

void detNotes::toNext(){
    ID = ID + 1;
    loadData();
}

void detNotes::toLast(){
    ID = LIST.size() - 1;
    loadData();
}

void detNotes::searchOpen(){
    ui.groupBox_search->setVisible(true);
}

void detNotes::searchClose(){
    ui.groupBox_search->hide();
    for (int r = ui.tableWidget_search->rowCount(); r >= 0; r--){
        ui.tableWidget_search->removeRow(r);
    }
    ui.lineEdit_search->clear();
}

void detNotes::searchMaterial(const QString txt){
    for (int r = ui.tableWidget_search->rowCount(); r >= 0; r--){
        ui.tableWidget_search->removeRow(r);
    }
    QSqlQuery query(QString("select material.id, material.articul, material.name "
                            "from material "
                            "where material.articul like \'%%1%\' or material.name like \'%%1%\' ")
                    .arg(txt));
    int row = 0;
    while (query.next()){
        ui.tableWidget_search->insertRow(row);
        for (int col = 0; col < 3; col++){
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui.tableWidget_search->setItem(row, col, item);
        }
        row++;
    }
}

void detNotes::searchSelect(){
    int crow = ui.tableWidget_search->currentRow();
    int id = ui.tableWidget_search->item(crow, 0)->text().toInt();
    QSqlQuery query("insert into notes (material, print, status, face) values (?, ?, ?, ?)");
    query.bindValue(0, id);
    query.bindValue(1, 1);
    query.bindValue(2, 1);
    query.bindValue(3, 1);
    query.exec();

    QSqlQuery queryLastId("select Max(notes.id) from notes");
    queryLastId.next();

    LIST << queryLastId.value(0).toInt();
    toLast();
}

void detNotes::saveNotes(){
    QSqlQuery queryFace(QString("select face.id from face where face.name = \'%1\' ")
                        .arg(ui.comboBox_face->currentText()));
    queryFace.next();
    QSqlQuery queryStatus(QString("select status.id from status where status.name = \'%1\' ")
                          .arg(ui.comboBox_status->currentText()));
    queryStatus.next();

    QString str(QString("update notes set face = \'%1\', status = \'%2\', town = \'%3\', ")
                .arg(queryFace.value(0).toString())
                .arg(queryStatus.value(0).toString())
                .arg(ui.lineEdit_town->text()));
    if (ui.checkBox_date->isChecked()){
        str.append(QString("dat = \'%1\', ").arg(ui.dateEdit->date().toString("dd.MM.yyyy")));
    }
    str.append(QString("print = \'%1\' where notes.id = \'%2\' ")
               .arg(ui.spinBox_print->value())
               .arg(LIST.at(ID)));
    QSqlQuery querySave(str);
    querySave.exec();
}

void detNotes::deleteNotes(){
    QSqlQuery query(QString("delete from notes where notes.id = \'%1\' ").arg(LIST.at(ID)));
    query.exec();

    LIST.removeAt(ID);
    toFirst();
}
