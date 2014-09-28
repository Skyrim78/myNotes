#include "mynotes.h"
#include "ui_mynotes.h"
#include "people.h"

myNotes::myNotes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::myNotes)
{
    ui->setupUi(this);
    connectDB();

    connect(ui->toolButton_file, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->toolButton_del, SIGNAL(clicked()), this, SLOT(deleteLine()));
    connect(ui->toolButton_make, SIGNAL(clicked()), this, SLOT(makeNote()));
    //connect(ui->toolButton_print, SIGNAL(clicked()), this, SLOT(printNote()));
    //--test
    connect(ui->toolButton_db, SIGNAL(clicked()), this, SLOT(addToBase()));
    connect(ui->toolButton_toNotes, SIGNAL(clicked()), this, SLOT(addToNotes()));
    connect(ui->actionObjects, SIGNAL(triggered()), this, SLOT(toObjects()));
    connect(ui->actionPeople, SIGNAL(triggered()), this, SLOT(toPeople()));

}

myNotes::~myNotes()
{
    if (db.isOpen()){
        db.close();
    }
    delete ui;
}

void myNotes::connectDB(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dss.db3");
    db.open();
}

void myNotes::openFile(){
    ui->lineEdit_file->setText(QFileDialog::getOpenFileName(this, "Select file...", "/home/ev/", "TXT (*.txt)"));
    readFile(ui->lineEdit_file->text());
}

void myNotes::readFile(QString name){
    for (int r = ui->tableWidget_data->rowCount(); r >=0; r--){
        ui->tableWidget_data->removeRow(r);
    }

    QFile file(name);
    file.open(QFile::ReadOnly);
    QTextStream stream(&file);
    QString line;
    int row = 0;
    while (!stream.atEnd()){        
        line = stream.readLine();
        if (row >= 11){
            ui->tableWidget_data->insertRow(row - 11);
            QStringList list = line.split(QRegExp("\t"));
            if (list.size() > 0){
                QTableWidgetItem *itemName = new QTableWidgetItem(expandName(0, list.at(0)));
                ui->tableWidget_data->setItem(row - 11, 0, itemName);

                QTableWidgetItem *itemBC = new QTableWidgetItem(expandName(1, list.at(0)));
                ui->tableWidget_data->setItem(row - 11, 1, itemBC);
            }
        }
        row++;
    }
    file.close();
    ui->tableWidget_data->resizeColumnsToContents();
    ui->tableWidget_data->horizontalHeader()->setStretchLastSection(true);
}

QString myNotes::expandName(int vid, QString line){
    int ejectCount = 0;
    for (int w = line.size()-1; w >= 0; w--){
        QString search = line.at(w);
        if (search == ","){
            ejectCount = w;
            break;
        }
    }
    if (vid == 0){
        return line.left(ejectCount);
    } else if (vid == 1){
        return line.right(line.size() - ejectCount - 1);
    }
}

void myNotes::deleteLine(){
    int row = ui->tableWidget_data->currentRow();
    ui->tableWidget_data->removeRow(row);
}

void myNotes::addToBase(){
    int _in = 0;
    int _up = 0;
    for (int row = 0; row < ui->tableWidget_data->rowCount(); row++){

        QSqlQuery query(QString("select material.id from material where material.articul = \'%1\' ")
                        .arg(ui->tableWidget_data->item(row, 1)->text()));
        query.next();
        if (!query.isValid()){
            QSqlQuery ins("insert into material (name, articul) values (?, ?)");
            ins.bindValue(0, ui->tableWidget_data->item(row, 0)->text());
            ins.bindValue(1, ui->tableWidget_data->item(row, 1)->text().toInt());
            ins.exec();
            _in++;
        } else if (query.value(0).toInt() > 0){
            QSqlQuery upd(QString("update material set name = \'%1\', articul = \'%2\' where material.id = \'%3\' ")
                          .arg(ui->tableWidget_data->item(row, 0)->text())
                          .arg(ui->tableWidget_data->item(row, 1)->text().toInt())
                          .arg(query.value(0).toInt()));
            upd.exec();
            _up++;
        }
    }
    qDebug() << (QString("Добавлено: %1; Обновлено: %2.").arg(_in).arg(_up));
}

void myNotes::addToNotes(){
    for (int row = 0; row < ui->tableWidget_data->rowCount(); row++){
        if (ui->tableWidget_data->item(row, 0)->isSelected()){
            QSqlQuery queryIdMat(QString("select material.id from material where material.articul = \'%1\'  ")
                                 .arg(ui->tableWidget_data->item(row, 1)->text()));
            queryIdMat.next();

            QSqlQuery query("insert into notes (material, status, face, print) values (?, ?, ?, ?)");
            query.bindValue(0, queryIdMat.value(0).toString());
            query.bindValue(1, 1);
            query.bindValue(2, 1);
            query.bindValue(3, 1);
            query.exec();
        }
    }
    makeNotes *mNotes = new makeNotes(this);
    mNotes->exec();
}



void myNotes::makeNote(){
   // ui->tabWidget->setCurrentIndex(1);
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
                "TD.art {font-size: 15pt; background: black;}"
                "TD.town {font-size: 8pt; } "
                "</style>"
                "<body>");
    for (int r = 0; r < ui->tableWidget_data->rowCount(); r++){
        line.append("<div class=\"main\">");
        line.append(QString("<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" height=\"62%\">"
                    "<tr>"
                    "<td class=\"name\" valign=\"top\" width=\"85%\">%1</td>"
                    "<td  width=\"15%\"></td>"
                    "</tr>"
                    "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" height=\"15%\">"
                    "<tr>"
                    "<td class=\"town\">Проверил: </td>"
                    "<td class=\"town\">Состояние: </td>"
                    "</tr>"
                    "</table>"
                    "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" height=\"25%\">"
                    "<tr>"
                    "<td class=\"art\" align=\"center\"  width=\"30%\"><font color=\"white\"><b>%2</b></font></td>"
                    "<td class=\"town\" align=\"right\"  width=\"70%\">Rovenky | <i>15.06.2014</i></td>"
                    "</tr>"
                    "</table>""</div>")
                    .arg(ui->tableWidget_data->item(r, 0)->text())
                    .arg(ui->tableWidget_data->item(r, 1)->text()));

    }
    line.append("</body></html>");

//ui->webView->setHtml(line);
}

void myNotes::printNote(){
    //printerName = QPrinterInfo::defaultPrinter().printerName();
    printer = new QPrinter(QPrinterInfo::defaultPrinter());
    printDialog = new QPrintDialog(printer, this);
    if (printDialog->exec() == QDialog::Accepted){
        //ui->webView->print(printer);
    }
}


void myNotes::toObjects(){
    objects *obj = new objects(this);
    obj->exec();
}

void myNotes::toPeople(){
    people *peo = new people(this);
    peo->exec();
}
