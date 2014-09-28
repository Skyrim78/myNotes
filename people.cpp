#include "people.h"


people::people(QWidget *parent):QDialog(parent){
    ui.setupUi(this);

    id = 0;
    ui.groupBox_people->hide();
    loadPeople();

    connect(ui.toolButton_close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui.toolButton_add, SIGNAL(clicked()), this, SLOT(addPeople()));
    connect(ui.toolButton_del, SIGNAL(clicked()), this, SLOT(delPeople()));
    connect(ui.pushButton_save, SIGNAL(clicked()), this, SLOT(savePeople()));
    connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(closeForm()));
    connect(ui.tableWidget_people, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openPeople()));

    connect(ui.lineEdit_fam, SIGNAL(editingFinished()), this, SLOT(makeFace()));
    connect(ui.lineEdit_name, SIGNAL(editingFinished()), this, SLOT(makeFace()));
    connect(ui.lineEdit_otc, SIGNAL(editingFinished()), this, SLOT(makeFace()));


}

void people::makeStatus(const QString text){
    ui.lineEdit_status->setText(text);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), ui.lineEdit_status, SLOT(clear()));
    timer->start(15000);
}

void people::makeFace(){
    QString face;
    face.append(QString("%1 %2.%3.")
                .arg(ui.lineEdit_fam->text())
                .arg(ui.lineEdit_name->text().left(1))
                .arg(ui.lineEdit_otc->text().left(1)));
    ui.lineEdit_face->setText(face);
}

void people::loadPeople(){
    for (int r = ui.tableWidget_people->rowCount(); r >= 0; r--){
        ui.tableWidget_people->removeRow(r);
    }
    QSqlQuery query("select face.id, face.face from face order by face.fam ");
    int row = 0;
    while (query.next()){
        ui.tableWidget_people->insertRow(row);
        for (int col = 0; col < 2; col++){
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui.tableWidget_people->setItem(row, col, item);
        }
        row++;
    }
    ui.tableWidget_people->resizeColumnsToContents();
    ui.tableWidget_people->horizontalHeader()->setStretchLastSection(true);
}

void people::addPeople(){
    ui.groupBox_people->setVisible(true);
    ui.lineEdit_fam->clear();
    ui.lineEdit_name->clear();
    ui.lineEdit_otc->clear();
    ui.lineEdit_fam->setFocus();
    makeStatus("Новый...");
    id = 0;
}

void people::openPeople(){
    int _c = ui.tableWidget_people->currentRow();
    id = ui.tableWidget_people->item(_c, 0)->text().toInt();

    QSqlQuery query(QString("select face.fam, face.name, face.otc, face.face from face where face.id = \'%1\'").arg(id));
    query.next();

    ui.lineEdit_fam->setText(query.value(0).toString());
    ui.lineEdit_name->setText(query.value(1).toString());
    ui.lineEdit_otc->setText(query.value(2).toString());
    ui.lineEdit_face->setText(query.value(3).toString());

    ui.lineEdit_fam->setFocus();

    ui.groupBox_people->setVisible(true);

    QString status(QString("ID%1 is open...").arg(id));
    makeStatus(status);
}

void people::closeForm(){
    ui.lineEdit_fam->clear();
    ui.lineEdit_name->clear();
    ui.lineEdit_otc->clear();
    ui.lineEdit_face->clear();
    ui.groupBox_people->hide();
}

void people::savePeople(){
    if (!ui.lineEdit_fam->text().isEmpty()){
        QString error;
        if (id == 0){
            QSqlQuery query("insert into face (fam, name, otc, face) values (?, ?, ?, ?)");
            query.bindValue(0, ui.lineEdit_fam->text());
            query.bindValue(1, ui.lineEdit_name->text());
            query.bindValue(2, ui.lineEdit_otc->text());
            query.bindValue(3, ui.lineEdit_face->text());
            query.exec();
            error.append(query.lastError().text());
        } else if (id > 0){
            QSqlQuery query(QString("update face set fam = \'%1\', name = \'%2\', otc = \'%3\', face = \'%4\' where face.id = \'%5\' ")
                            .arg(ui.lineEdit_fam->text())
                            .arg(ui.lineEdit_name->text())
                            .arg(ui.lineEdit_otc->text())
                            .arg(ui.lineEdit_face->text())
                            .arg(id));
            query.exec();
            error.append(query.lastError().text());
        }
        if (error.size() > 1){
            makeStatus(error);
        } else {
            makeStatus("Saved successfull...");
        }
        if (id == 0){
            addPeople();
        }
        loadPeople();
    } else {
        makeStatus("Enter the \"Fam\" ");
    }
}

void people::delPeople(){
    if (id > 0){
        QString error;
        QSqlQuery query(QString("delete from face where face.id = \'%1\' ").arg(id));
        query.exec();
        error.append(query.lastError().text());
        closeForm();

        if (error.size() > 1){
            makeStatus(error);
        } else {
            makeStatus(QString("ID%1 was deleted").arg(id));
            id = 0;
        }
        loadPeople();
    }
}
