#-------------------------------------------------
#
# Project created by QtCreator 2014-09-23T10:54:38
#
#-------------------------------------------------

QT       += core gui qml quick quickwidgets webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = widgetInsideQml
TEMPLATE = app


SOURCES += main.cpp \
    QmlWebViewWidget.cpp

HEADERS  += \
    QmlWebViewWidget.h

FORMS    += widget.ui

OTHER_FILES +=

RESOURCES += \
    qrc.qrc
CONFIG +=
CONFIG += c++11
