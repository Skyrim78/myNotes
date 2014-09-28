#include "document.h"


document::document(int _vid, int _pos, QList<int> _list, QWidget *parent):QDialog(parent){
    ui.setupUi(this);

    vid = _vid;
    pos = _pos;
    list = _list;

    loadDoc();

    connect(ui.pushButton_toFirst, SIGNAL(clicked()), this, SLOT(toFirst()));
    connect(ui.pushButton_toLaxt, SIGNAL(clicked()), this, SLOT(toLast()));
    connect(ui.pushButton_toNext, SIGNAL(clicked()), this, SLOT(toNext()));
    connect(ui.pushButton_toPrev, SIGNAL(clicked()), this, SLOT(toPrev()));
}

void document::loadDoc(){
    if (list.size() == 0){
        ui.lineEdit_num->clear();
        ui.dateEdit->setDate(QDate::currentDate());
        ui.lineEdit_name->clear();
        ui.comboBox_firm->setCurrentIndex(-1);
        ui.lineEdit_note->clear();
        for (int r = ui.tableWidget_material->rowCount(); r >= 0; r--){
            ui.tableWidget_material->removeRow(r);
        }
    } else if (list.size() > 0){
        QSqlQuery query(QString("select doc.num, doc.dat, doc.name, firm.name, doc.note "
                                "from doc, firm "
                                "where doc.firm = firm.id and doc.id = \'%1\' ").arg(list.at(pos)));
        query.next();
        ui.lineEdit_num->setText(query.value(0).toString());
        ui.dateEdit->setDate(QDate::fromString(query.value(1).toString(), "dd.MM.yyyy"));
        ui.lineEdit_name->setText(query.value(2).toString());
        ui.comboBox_firm->setCurrentIndex(ui.comboBox_firm->findText(query.value(3).toString()));
        ui.lineEdit_note->setText(query.value(4).toString());
    }

    if (list.size() == 0){
        ui.pushButton_toFirst->setEnabled(false);
        ui.pushButton_toPrev->setEnabled(false);
        ui.pushButton_toNext->setEnabled(false);
        ui.pushButton_toLaxt->setEnabled(false);
    } else {
        if (pos == 0){
            ui.pushButton_toFirst->setEnabled(false);
            ui.pushButton_toPrev->setEnabled(false);
        } else {
            ui.pushButton_toFirst->setEnabled(true);
            ui.pushButton_toPrev->setEnabled(true);
        }
        if (list.size() - 1 == 0){
            ui.pushButton_toNext->setEnabled(false);
            ui.pushButton_toLaxt->setEnabled(false);
        } else {
            ui.pushButton_toNext->setEnabled(true);
            ui.pushButton_toLaxt->setEnabled(true);
        }
    }
}

void document::loadDocDetail(int v){
    for (int r = ui.tableWidget_material->rowCount(); r >= 0; r--){
        ui.tableWidget_material->removeRow(r);
    }
    QString txt;
    if (v == 1){ //+
        txt.append(QString("select doc_in.id, material.name, material.coun, unit.name "
                           "from doc_in, doc, material, units "
                           "where doc_in.doc = doc.id and doc.id = \'%1\' and doc_in.material = material.id and material.unit = units.id ")
                   .arg(list.at(pos)));
    } else if (vid == 2){ // -
        txt.append(QString("select doc_out.is"));
    }
}

void document::toFirst(){
    pos = 0;
    loadDoc();
}

void document::toLast(){
    pos = list.size() - 1;
    loadDoc();
}

void document::toNext(){
    pos = pos + 1;
    loadDoc();
}

void document::toPrev(){
    pos = pos - 1;
    loadDoc();
}
