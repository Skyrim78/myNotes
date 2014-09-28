#include "treb.h"

treb::treb(int _obj, QWidget *parent):QDialog(parent){
    ui.setupUi(this);
    obj = _obj;
    ui.dateEdit_from->setDate(QDate::currentDate());
    ui.dateEdit_from->setEnabled(false);
    ui.dateEdit_to->setDate(QDate::currentDate());
    ui.dateEdit_to->setEnabled(false);

    loadTreb();

    connect(ui.checkBox_period, SIGNAL(clicked(bool)), ui.dateEdit_from, SLOT(setEnabled(bool)));
    connect(ui.checkBox_period, SIGNAL(clicked(bool)), ui.dateEdit_to, SLOT(setEnabled(bool)));
    connect(ui.dateEdit_from, SIGNAL(dateChanged(QDate)), this, SLOT(loadTrebDetail()));
    connect(ui.dateEdit_to, SIGNAL(dateChanged(QDate)), this, SLOT(loadTrebDetail()));

    connect(ui.toolButton_add, SIGNAL(clicked()), this, SLOT(addTreb()));

}

void treb::loadTreb(){
    QSqlQuery queryObj(QString("select objects.name from objects where objects.id =  \'%1\' ").arg(obj));
    queryObj.next();
    ui.lineEdit_object->setText(queryObj.value(0).toString());

    loadTrebDetail();

}

void treb::loadTrebDetail(){
    for (int r = ui.tableWidget_treb->rowCount(); r >= 0; r--){
        ui.tableWidget_treb->removeRow(r);
    }

    QString txt(QString("select treb.id, treb.tdate, treb.ttime, material.articul, material.name, units.name, treb.coun, face.face "
                            "from treb, material, units, face "
                            "where material.id = treb.material and units.id = material.unit and face.id = treb.face and treb.object = \'%1\' ")
                    .arg(obj));
    if (ui.checkBox_period->isChecked()){
        txt.append(QString("and (treb.tdate between \'%1\' and \'%2\') ")
                   .arg(ui.dateEdit_from->date().toString("dd.MM.yyyy"))
                   .arg(ui.dateEdit_to->date().toString("dd.MM.yyyy")));
    }
    txt.append("order by treb.tdate");
    QSqlQuery query(txt);
    int row = 0;
    while (query.next()){
        ui.tableWidget_treb->insertRow(row);
        for (int col = 0; col < 1; col++){
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui.tableWidget_treb->setItem(row, col, item);
        }
        QTableWidgetItem *item = new QTableWidgetItem(QString("%1 - %2 ")
                                                      .arg(query.value(3).toString())
                                                      .arg(query.value(4).toString()));
        ui.tableWidget_treb->setItem(row, 3, item);
        for (int col = 5; col < 8; col++){
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui.tableWidget_treb->setItem(row, col - 1, item);
        }
        row++;
    }
}

void treb::addTreb(){
    QList<int> listTreb;
    trebDetail *td = new trebDetail(obj, 0, listTreb, this);
    td->exec();
    loadTrebDetail();
}
