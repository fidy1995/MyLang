#-------------------------------------------------
#
# Project created by QtCreator 2015-12-27T01:21:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = compiler
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    analysis.cpp \
    astconst.cpp \
    compiler.cpp \
    myparser.cpp \
    tokenconst.cpp \
    scanner.cpp \
    compilermainwindow.cpp

HEADERS  += mainwindow.h \
    analysis.h \
    astconst.h \
    compiler.h \
    myparser.h \
    regionconst.h \
    scanner.h \
    tokenconst.h \
    compilermainwindow.h
