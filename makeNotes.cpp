#include "makeNotes.h"
#include "detailNotes.h"

makeNotes::makeNotes(QWidget *parent):QDialog(parent){
    ui.setupUi(this);

    printer = new QPrinter(QPrinterInfo::defaultPrinter());
    printer->setPageMargins(10.00, 15.00, 10.00, 25.63, QPrinter::Millimeter);

    ui.tableWidget_data->setColumnHidden(0, true);
    loadData();

    connect(ui.tableWidget_data, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(toDetail()));
    connect(ui.toolButton_addNotes, SIGNAL(clicked()), this, SLOT(addNotes()));
    connect(ui.toolButton_clearAll, SIGNAL(clicked()), this, SLOT(clearNotes()));
    connect(ui.toolButton_delNotes, SIGNAL(clicked()), this, SLOT(deleteNotes()));
    connect(ui.toolButton_prepare, SIGNAL(clicked()), this, SLOT(prepareForPrint()));

    connect(ui.pushButton_print, SIGNAL(clicked()), this, SLOT(printNotes()));

}

void makeNotes::loadData(){
    for (int r = ui.tableWidget_data->rowCount(); r >= 0; r--){
        ui.tableWidget_data->removeRow(r);
    }

    QSqlQuery query("select notes.id, material.articul, material.name, face.name, status.name, notes.town, notes.dat, notes.print  "
                    "from notes, material, face, status "
                    "where notes.material = material.id  and notes.face = face.id and notes.status = status.id and notes.print >\'0\' ");
    int row = 0;
    while (query.next()){
        ui.tableWidget_data->insertRow(row);
        for (int col = 0; col < 8; col++){
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui.tableWidget_data->setItem(row, col, item);
        }
        row++;
    }
    ui.tableWidget_data->resizeColumnsToContents();
    ui.tableWidget_data->horizontalHeader()->setStretchLastSection(true);
}


void makeNotes::toDetail(){
    int id = ui.tableWidget_data->currentRow();
    QList<int> list;
    for (int r = 0; r < ui.tableWidget_data->rowCount(); r++){
        list << ui.tableWidget_data->item(r, 0)->text().toInt();
    }

    detNotes *dn = new detNotes(id, list, this);
    dn->exec();

    loadData();
}

void makeNotes::addNotes(){
    QList<int> list;
    detNotes *dn = new detNotes(0, list, this);
    dn->exec();

    loadData();
}

void makeNotes::deleteNotes(){
    if (ui.tableWidget_data->rowCount() > 0){
        for (int row = 0; row < ui.tableWidget_data->rowCount(); row++){
            if(ui.tableWidget_data->item(row, 0)->isSelected()){
                QSqlQuery queryDel(QString("delete from notes where notes.id = \'%1\' ")
                                   .arg(ui.tableWidget_data->item(row, 0)->text()));
                queryDel.exec();
            }
        }
    }
    loadData();
}

void makeNotes::clearNotes(){
    QSqlQuery query("delete from notes");
    query.exec();
    loadData();
}

void makeNotes::prepareForPrint(){
    ui.tabWidget->setCurrentIndex(1);
    QString line;
    line.append("<!DOCTYPE html> "
                "<html><head> <meta charset=\"utf-8\"></head>"
                "<style type=\"text/css\"> "
                ".main { "
                "width: 75mm; "
                "height: 45mm; "
                "border: solid 1px black; "
                "float: left; "
                "} "
                "TD {margin: 0; padding: 0;} "
                "TD.name {font-size: 13pt; } "
                "TD.art {border: solid 1px black; font-size: 17pt; }"//background: black;}"
                "TD.town {font-size: 8pt; } "
                "</style>"
                "<body>");
    for (int r = 0; r < ui.tableWidget_data->rowCount(); r++){
        for (int col = 1; col <= ui.tableWidget_data->item(r, 7)->text().toInt(); col++){
            line.append("<div class=\"main\">");
            line.append(QString("<table border=\"0\" cellspacing=\"2\" cellpadding=\"0\" width=\"100%\" height=\"62%\">"
                        "<tr>"
                        "<td class=\"name\" valign=\"top\" width=\"85%\"><font color=\"blue\">%1</font></td>"
                        "<td  width=\"15%\"></td>"
                        "</tr>"
                        "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" height=\"15%\">"
                        "<tr>")
                        .arg(ui.tableWidget_data->item(r, 2)->text()));
            if (ui.tableWidget_data->item(r, 3)->text().isEmpty()){
                line.append("<td class=\"town\"></td>");
            } else {
                line.append(QString("<td class=\"town\">Проверил: %1</td>").arg(ui.tableWidget_data->item(r, 3)->text()));
            }

            if (ui.tableWidget_data->item(r, 4)->text().isEmpty()){
                line.append("<td class=\"town\"></td>");
            } else {
                line.append(QString("<td class=\"town\">Состояние: %1</td>").arg(ui.tableWidget_data->item(r, 4)->text()));
            }

            line.append(QString("</tr>"
                        "</table>"
                        "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" height=\"25%\">"
                        "<tr>"
                        "<td class=\"art\" align=\"center\"  width=\"30%\"><font color=\"black\"><b>%1</b></font></td>"
                        "<td class=\"town\" align=\"right\"  width=\"70%\">%2 | <i>%3</i></td>"
                        "</tr>"
                        "</table>""</div>")
                        .arg(ui.tableWidget_data->item(r, 1)->text())
                        .arg(ui.tableWidget_data->item(r, 5)->text())
                        .arg(ui.tableWidget_data->item(r, 6)->text()));
        }
    }
    line.append("</body></html>");
    ui.webView->setHtml(line);
}

void makeNotes::printNotes(){
    printDialog = new QPrintDialog(printer, this);
    if (printDialog->exec() == QDialog::Accepted){
        ui.webView->print(printer);
    }
}

void makeNotes::closeEvent(QCloseEvent *event){
    QSqlQuery query("delete from notes");
    query.exec();
}
