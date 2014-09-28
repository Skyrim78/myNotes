#ifndef DETAILNOTES_H
#define DETAILNOTES_H

#include "ui_detailNotes.h"
#include <QtSql/QtSql>

class detNotes:public QDialog{
    Q_OBJECT
public:
    detNotes(int _curr, QList<int> _list, QWidget *parent = 0);
    Ui::detailNotes ui;

    int ID;
    QList<int> LIST;

public slots:
    void loadData();

    void searchMaterial(const QString txt);
    void searchOpen();
    void searchClose();
    void searchSelect();

    void deleteNotes();
    void saveNotes();

    void toFirst();
    void toPrev();
    void toNext();
    void toLast();
};

#endif // DETAILNOTES_H
