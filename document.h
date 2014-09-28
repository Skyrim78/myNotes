#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "ui_document.h"
#include <QtSql/QtSql>

class document:public QDialog{
    Q_OBJECT
public:
    document(int _vid, int _pos, QList<int> _list, QWidget *parent = 0);
    Ui::document ui;

    int vid;
    int pos;
    QList<int> list;

public slots:
    void loadDoc();
    void loadDocDetail(int v);

    void toFirst();
    void toPrev();
    void toNext();
    void toLast();
};

#endif // DOCUMENT_H
