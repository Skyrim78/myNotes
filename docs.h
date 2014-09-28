#ifndef DOCS_H
#define DOCS_H

#include "ui_docs.h"
#include <QtSql/QtSql>


class docs:public QDialog{
    Q_OBJECT
public:
    docs(int _vid, QWidget *parent = 0);
    Ui::docs ui;

    int vid;

public slots:
    void loadDocs();
    void addDocs();
};

#endif // DOCS_H
