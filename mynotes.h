#ifndef MYNOTES_H
#define MYNOTES_H

#include <QMainWindow>
//#include <QtWebKit/QtWebKit>
#include <QtWebKitWidgets/QtWebKitWidgets>
#include<QtWebKitWidgets/QWebView>
#include <QFileDialog>
#include <QTextStream>
#include <QPrintDialog>
#include <QPrinter>
#include <QtSql/QtSql>
#include "makeNotes.h"

//test
#include "objects.h"

namespace Ui {
class myNotes;
}

class myNotes : public QMainWindow
{
    Q_OBJECT

public:
    explicit myNotes(QWidget *parent = 0);
    ~myNotes();
    QPrinter *printer;
    QPrintDialog *printDialog;

    QSqlDatabase db;

public slots:
    void connectDB();
    void addToBase();
    void addToNotes();

    void openFile();
    void readFile(QString name);
    QString expandName(int vid, QString line);
    void deleteLine();

    void makeNote();//QString num, QString name);
    void printNote();


    void toObjects();
    void toPeople();

private:
    Ui::myNotes *ui;
};

#endif // MYNOTES_H
