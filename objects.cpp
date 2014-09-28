#include "objects.h"


objects::objects(QWidget *parent):QDialog(parent){
    ui.setupUi(this);
    ui.groupBox_obj->hide();
    ID = 0;

    loadObjects();

    connect(ui.toolButton_add, SIGNAL(clicked()), this, SLOT(addObject()));
    connect(ui.pushButton_save, SIGNAL(clicked()), this, SLOT(saveObject()));
    connect(ui.pushButton_del, SIGNAL(clicked()), this, SLOT(deleteObject()));
    connect(ui.toolButton_close, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(closeObject()));

    ui.tableWidget_obj->setContextMenuPolicy(Qt::DefaultContextMenu);

}

void objects::loadObjects(){
    for (int r = ui.tableWidget_obj->rowCount(); r >= 0; r--){
        ui.tableWidget_obj->removeRow(r);
    }

    QSqlQuery query ("select objects.id, objects.name, (select Count(treb.id) from treb where treb.object = objects.id) "
                     "from objects "
                     "order by objects.name ");
    int row = 0;
    while (query.next()){
        ui.tableWidget_obj->insertRow(row);
        for (int col = 0; col < 3; col++){
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui.tableWidget_obj->setItem(row, col, item);
        }
        row++;
    }
    ui.tableWidget_obj->resizeColumnsToContents();
    ui.tableWidget_obj->horizontalHeader()->setStretchLastSection(true);
}

void objects::openObject(){
    if (ui.tableWidget_obj->rowCount() > 0){
        int curr = ui.tableWidget_obj->currentRow();
        ID = ui.tableWidget_obj->item(curr, 0)->text().toInt();
        QString name = ui.tableWidget_obj->item(curr, 1)->text();
        int coun = ui.tableWidget_obj->item(curr, 2)->text().toInt();

        ui.groupBox_obj->setVisible(true);
        ui.lineEdit_name->setText(name);

        if (coun == 0){
            ui.pushButton_del->setEnabled(true);
        } else if (coun > 0){
            ui.pushButton_del->setEnabled(false);
        }

        ui.lineEdit_name->setFocus();
    }
}


void objects::addObject(){
    ID = 0;
    ui.groupBox_obj->setVisible(true);
    ui.lineEdit_name->clear();
    ui.lineEdit_name->setFocus();
    ui.pushButton_del->setEnabled(false);
}

void objects::saveObject(){
    if (ID == 0){
        QSqlQuery query("insert into objects (name) values (?) ");
        query.bindValue(0, ui.lineEdit_name->text());
        query.exec();
    } else if (ID > 0){
        QSqlQuery query(QString("update objects set name = \'%1\' where objects.id = \'%2\' ")
                        .arg(ui.lineEdit_name->text())
                        .arg(ID));
        query.exec();
    }
    loadObjects();

    //установить текущую строку
    if (ID == 0){
        ui.tableWidget_obj->setCurrentCell(ui.tableWidget_obj->rowCount() - 1, 0);
    } else {
        for (int r = 0; r < ui.tableWidget_obj->rowCount(); r++){
            if (ui.tableWidget_obj->item(r, 0)->text().toInt() == ID){
                ui.tableWidget_obj->setCurrentCell(r, 0);
                break;
            }
        }
    }

}

void objects::deleteObject(){
    QSqlQuery query(QString("delete from objects where objects.id = \'%1\'").arg(ID));
    query.exec();
    loadObjects();
    closeObject();
}

void objects::closeObject(){
    ui.lineEdit_name->clear();
    ui.groupBox_obj->hide();
}

void objects::openTreb(){
    treb *tr = new treb(ID, this);
    tr->exec();
    loadObjects();
}

void objects::contextMenuEvent(QContextMenuEvent *event){
    ID = ui.tableWidget_obj->item(ui.tableWidget_obj->currentRow(), 0)->text().toInt();

    context = new QMenu(this);
    toTreb = new QAction("treb", this);
    context->addAction(toTreb);
    connect(toTreb, SIGNAL(triggered()), this, SLOT(openTreb()));


    toEditObject = new QAction("edit", this);
    context->addAction(toEditObject);
    connect(toEditObject, SIGNAL(triggered()), this, SLOT(openObject()));

    context->exec(event->globalPos());
}
