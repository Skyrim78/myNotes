#ifndef TREB_DETAIL_H
#define TREB_DETAIL_H

#include "ui_treb_detail.h"
#include <QtSql/QtSql>

class trebDetail:public QDialog{
    Q_OBJECT
public:
    trebDetail(int _obj, int _pos, QList<int> _list, QWidget *parent = 0);
    Ui::treb_detail ui;

    int obj;
    int pos;
    QList<int> list;

public slots:

    void makeStatus(QString txt);


    void loadDetail();
    void loadPeople();
    void searchMaterial(const QString text);
    void selectMaterial();

    void saveDetail();
    void deleteDetail();
    void newDetail();

    void toFirst();
    void toPrev();
    void toNext();
    void toLast();
};

#endif // TREB_DETAIL_H
