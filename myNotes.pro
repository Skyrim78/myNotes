#-------------------------------------------------
#
# Project created by QtCreator 2014-08-15T15:16:11
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets printsupport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myNotes
TEMPLATE = app


SOURCES += main.cpp\
        mynotes.cpp \
    makeNotes.cpp \
    detailNotes.cpp \
    docs.cpp \
    document.cpp \
    objects.cpp \
    treb.cpp \
    treb_detail.cpp \
    people.cpp

HEADERS  += mynotes.h \
    makeNotes.h \
    detailNotes.h \
    docs.h \
    document.h \
    objects.h \
    treb.h \
    treb_detail.h \
    people.h \
    materials.h

FORMS    += mynotes.ui \
    makeNotes.ui \
    detailNotes.ui \
    docs.ui \
    document.ui \
    objects.ui \
    treb.ui \
    treb_detail.ui \
    people.ui \
    materials.ui

RESOURCES +=
