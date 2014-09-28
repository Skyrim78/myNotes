#ifndef MAKENOTES_H
#define MAKENOTES_H

#include "ui_makeNotes.h"
#include <QtSql/QtSql>

#include <QPrintDialog>
#include <QPrinterInfo>
#include <QCloseEvent>

class makeNotes:public QDialog{
    Q_OBJECT
public:
    makeNotes(QWidget *parent=0);
    Ui::make_notes ui;

    QPrinter *printer;
    QPrintDialog *printDialog;


protected:
     virtual void closeEvent(QCloseEvent *event);

public slots:
    void loadData();
    void toDetail();

    void addNotes();
    void clearNotes();
    void deleteNotes();
    void prepareForPrint();


    void printNotes();
};

#endif // MAKENOTES_H
