#include "treb_detail.h"

trebDetail::trebDetail(int _obj, int _pos, QList<int> _list, QWidget *parent):QDialog(parent){
    ui.setupUi(this);
    obj = _obj;
    pos = _pos;
    list = _list;

    loadPeople();
    loadDetail();


    connect(ui.lineEdit_search, SIGNAL(textEdited(QString)), this, SLOT(searchMaterial(QString)));
    connect(ui.tableWidget_material, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectMaterial()));

    connect(ui.pushButton_save, SIGNAL(clicked()), this, SLOT(saveDetail()));
    connect(ui.toolButton_delete, SIGNAL(clicked()), this, SLOT(deleteDetail()));
    connect(ui.toolButton_close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui.toolButton_add, SIGNAL(clicked()), this, SLOT(newDetail()));

    connect(ui.pushButton_toFirst, SIGNAL(clicked()), this, SLOT(toFirst()));
    connect(ui.pushButton_toLast, SIGNAL(clicked()), this, SLOT(toLast()));
    connect(ui.pushButton_toNext, SIGNAL(clicked()), this, SLOT(toNext()));
    connect(ui.pushButton_toPrev, SIGNAL(clicked()), this, SLOT(toPrev()));

}
void trebDetail::loadPeople(){
    ui.comboBox_face->clear();
    QSqlQuery query("select face.face from face order by face.face");
    while (query.next()){
        ui.comboBox_face->addItem(query.value(0).toString());
    }
}

void trebDetail::loadDetail(){

    QString messa;

    QSqlQuery qObject(QString("select objects.name from objects where objects.id = \'%1\' ").arg(obj));
    qObject.next();

    messa.append(QString("Объект: %1 ").arg(qObject.value(0).toString()));

    if (list.size() == 0){
        ui.lineEdit_material->clear();
        ui.dateEdit->setDate(QDate::currentDate());
        ui.timeEdit->setTime(QTime::currentTime());
        ui.spinBox_articul->setValue(0);
        ui.doubleSpinBox_coun->setValue(0.00);
        ui.comboBox_face->setCurrentIndex(-1);
        ui.lineEdit_note->clear();
        ui.lineEdit_status->clear();

        ui.toolButton_delete->setEnabled(false);

        ui.lineEdit_search->clear();
        ui.lineEdit_search->setFocus();
        for (int r = ui.tableWidget_material->rowCount(); r >= 0; r--){
            ui.tableWidget_material->removeRow(r);
        }

        messa.append("Новое требование...");

    } else if (list.size() > 0){
        QSqlQuery query(QString("select treb.tdate, treb.ttime, material.articul, material.name, treb.coun, face.face, treb.note "
                                "from treb, material, face "
                                "where treb.material = material.id and face.id = treb.face and treb.id = \'%1\' ")
                        .arg(list.at(pos)));
        query.next();
        ui.dateEdit->setDate(query.value(0).toDate());
        ui.timeEdit->setTime(query.value(1).toTime());
        ui.spinBox_articul->setValue(query.value(2).toInt());
        ui.lineEdit_material->setText(query.value(3).toString());
        ui.doubleSpinBox_coun->setValue(query.value(4).toDouble());
        ui.comboBox_face->setCurrentIndex(ui.comboBox_face->findText(query.value(5).toString()));
        ui.lineEdit_note->setText(query.value(6).toString());

        messa.append(QString("Требование: ID%1").arg(list.at(pos)));
    }
    if (pos == 0){
        ui.pushButton_toFirst->setEnabled(false);
        ui.pushButton_toPrev->setEnabled(false);
    } else {
        ui.pushButton_toFirst->setEnabled(true);
        ui.pushButton_toPrev->setEnabled(true);
    }
    if (pos == list.size() - 1){
        ui.pushButton_toNext->setEnabled(false);
        ui.pushButton_toLast->setEnabled(false);
    } else {
        ui.pushButton_toNext->setEnabled(true);
        ui.pushButton_toLast->setEnabled(true);
    }
    if (list.size() == 0){
        ui.pushButton_toFirst->setEnabled(false);
        ui.pushButton_toPrev->setEnabled(false);
        ui.pushButton_toNext->setEnabled(false);
        ui.pushButton_toLast->setEnabled(false);
    }

    makeStatus(messa);
}

void trebDetail::makeStatus(QString txt){
    ui.lineEdit_status->setText(txt);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), ui.lineEdit_status, SLOT(clear()));
    timer->start(10000);
}


void trebDetail::searchMaterial(const QString text){
    for (int r = ui.tableWidget_material->rowCount(); r >= 0; r--){
        ui.tableWidget_material->removeRow(r);
    }
    QSqlQuery query(QString("select material.id, material.articul, material.name from material "
                            "where material.articul like \'%%1%\' or material.name like \'%%2%\' "
                            "order by material.name ")
                    .arg(text)
                    .arg(text));
    int row = 0;
    while (query.next()){
        ui.tableWidget_material->insertRow(row);
        for (int col = 0; col < 3; col++){
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui.tableWidget_material->setItem(row, col, item);
        }
        row++;
    }
    ui.tableWidget_material->resizeColumnsToContents();
    ui.tableWidget_material->horizontalHeader()->setStretchLastSection(true);
}

void trebDetail::selectMaterial(){
    int crow = ui.tableWidget_material->currentRow();
    ui.lineEdit_material->setText(ui.tableWidget_material->item(crow, 2)->text());
    ui.spinBox_articul->setValue(ui.tableWidget_material->item(crow, 1)->text().toInt());
}

void trebDetail::saveDetail(){
    QString error;
    if (!ui.lineEdit_material->text().isEmpty()){
        if (ui.doubleSpinBox_coun->value() > 0.000){
            if (ui.comboBox_face->currentIndex() >= 0){
                QSqlQuery qMaterial(QString("select material.id from material where material.articul = \'%1\' ")
                                   .arg(ui.spinBox_articul->value()));
                qMaterial.next();
                QSqlQuery qFace(QString("select face.id from face where face.face = \'%1\' ")
                                .arg(ui.comboBox_face->currentText()));
                qFace.next();
                if (list.size() == 0){
                    QSqlQuery query("insert into treb (object, tdate, ttime, material, coun, face, note) "
                                    "values (?, ?, ?, ?, ?, ?, ?) ");
                    query.bindValue(0, obj);
                    query.bindValue(1,  ui.dateEdit->date().toString("dd.MM.yyyy"));
                    query.bindValue(2, ui.timeEdit->time().toString("hh:mm"));
                    query.bindValue(3, qMaterial.value(0).toString());
                    query.bindValue(4, ui.doubleSpinBox_coun->value());
                    query.bindValue(5, qFace.value(0).toString());
                    query.bindValue(6, ui.lineEdit_note->text());
                    query.exec();
                    error.append(query.lastError().text());
                } else {
                    QSqlQuery query(QString("update treb set tdate = \'%1\', ttime = \'%2\', material = \'%3\', coun = \'%4\', face = \'%5\', "
                                            "note = \'%6\' where treb.id = \'%7\' ")
                                    .arg(ui.dateEdit->date().toString("dd.MM.yyyy"))
                                    .arg(ui.timeEdit->date().toString("hh:mm"))
                                    .arg(qMaterial.value(0).toInt())
                                    .arg(ui.doubleSpinBox_coun->value())
                                    .arg(qFace.value(0).toInt())
                                    .arg(ui.lineEdit_note->text())
                                    .arg(list.at(pos)));
                    query.exec();
                    error.append(query.lastError().text());
                }
            } else {
                error.append("Select face!");
            }
        } else {
            error.append("Coun must be more than 0.000");
        }
    } else {
        error.append("Select material!");
    }
    if (error.size() > 5){
        makeStatus(error);
    } else {
        makeStatus("Saved...");
    }
}

void trebDetail::toFirst(){
    pos = 0;
    loadDetail();
}

void trebDetail::toPrev(){
    pos = pos - 1;
    loadDetail();
}

void trebDetail::toNext(){
    pos = pos + 1;
    loadDetail();
}

void trebDetail::toLast(){
    pos = list.size() - 1;
    loadDetail();
}

void trebDetail::deleteDetail(){
    QString error;
    QSqlQuery qDel(QString("delete from treb where treb.id = \'%1\' ").arg(list.at(pos)));
    qDel.exec();
    error.append(qDel.lastError().text());

    if (error.size() > 5){
        makeStatus(error);
    } else {
        if (list.size() > 0){
            makeStatus(QString("ID %1 was deleted...").arg(list.at(pos)));
            list.removeAt(pos);
            toFirst();
        } else {
            close();
        }
    }
}

void trebDetail::newDetail(){
    list.clear();
    pos = 0;
    loadDetail();
}
