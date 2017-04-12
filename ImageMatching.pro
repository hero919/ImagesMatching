
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia
TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow/mainwindow.cpp \
    BasicModeWindow/basicmodewindow.cpp \
    RelaxedModeWindow/relaxedmodewindow.cpp \
    LevelModeWindow/levelmodewindow.cpp \
    Model/game.cpp \
    HelpDialog/helpdialog.cpp \
    Model/DrawLineLayer.cpp \
    ToolNotification/toolnotification.cpp \
    toolnotification.cpp \
    Mode/mode.cpp

HEADERS  += MainWindow/mainwindow.h \
    BasicModeWindow/basicmodewindow.h \
    RelaxedModeWindow/relaxedmodewindow.h \
    LevelModeWindow/levelmodewindow.h \
    Model/mapbutton.h \
    Model/game.h \
    Dao/scoredao.h \
    HelpDialog/helpdialog.h \
    Model/DrawLineLayer.h \
    ToolNotification/toolnotification.h \
    toolnotification.h \
    Mode/mode.h

FORMS    += mainwindow.ui \
    basicmodewindow.ui \
    relaxedmodewindow.ui \
    levelmodewindow.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    FindatoolForm.ui.qml \
    Findatool.qml
