#ifndef OBJECTS_H
#define OBJECTS_H

#include "ui_objects.h"
#include <QtSql/QtSql>
#include <QMenu>
#include <QContextMenuEvent>
#include "treb.h"

class objects:public QDialog{
    Q_OBJECT
public:
    objects(QWidget *parent = 0);
    Ui::objects ui;


    QMenu *context;
    QAction *toTreb;
    QAction *toEditObject;

    int ID;

protected slots:
    virtual void contextMenuEvent(QContextMenuEvent *event);
public slots:
    void loadObjects();

    void openObject();
    void closeObject();

    void addObject();
    void saveObject();
    void deleteObject();

    void openTreb();

};

#endif // OBJECTS_H
