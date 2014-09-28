#ifndef PEOPLE_H
#define PEOPLE_H

#include "ui_people.h"
#include <QtSql/QtSql>

class people:public QDialog{
    Q_OBJECT
public:
    people(QWidget *parent = 0);
    Ui::people ui;

    int id;

public slots:
    void makeStatus(const QString text);
    void makeFace();

    void loadPeople();
    void addPeople();
    void savePeople();
    void closeForm();
    void openPeople();
    void delPeople();
};

#endif // PEOPLE_H
