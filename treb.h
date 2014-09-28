#ifndef TREB_H
#define TREB_H

#include "ui_treb.h"
#include "treb_detail.h"
#include <QtSql/QtSql>

class treb:public QDialog{
    Q_OBJECT
public:
    treb(int _obj, QWidget *parent = 0);
    Ui::treb ui;

    int obj;

public slots:
    void loadTreb();
    void loadTrebDetail();

    void addTreb();
};

#endif // TREB_H
