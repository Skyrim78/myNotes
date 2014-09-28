#include "docs.h"

docs::docs(int _vid, QWidget *parent):QDialog(parent){
    ui.setupUi(this);
    vid = _vid;
    ui.tableWidget_docs->setColumnHidden(0, true);

    loadDocs();

}

void docs::loadDocs(){
    for (int r = ui.tableWidget_docs->rowCount(); r >= 0; r--){
        ui.tableWidget_docs->removeRow(r);
    }

    QSqlQuery query(QString("select docs.id, docs.dat, docs.num, docs.name, docs.note "
                            "from docs "
                            "where docs.vid = \'%1\' "
                            "order by docs.dat").arg(vid));
    int row = 0;
    while (query.next()){
        ui.tableWidget_docs->insertRow(row);
        for (int col = 0; col < 4; col++){
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui.tableWidget_docs->setItem(row, col, item);
        }
        row++;
    }
    ui.tableWidget_docs->resizeColumnsToContents();
    ui.tableWidget_docs->horizontalHeader()->setStretchLastSection(true);
}

void docs::addDocs(){

}
