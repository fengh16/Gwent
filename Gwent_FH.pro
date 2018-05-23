#-------------------------------------------------
#
# Project created by QtCreator 2017-09-04T08:57:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gwent_FH
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    base/Cards.cpp \
    base/Functions.cpp \
    base/GameController.cpp \
    base/PlayerInfo.cpp \
    load.cpp \
    login.cpp \
    choosing.cpp \
    collection.cpp \
    showncard.cpp \
    friends.cpp \
    deckbuilder.cpp \
    playing.cpp \
    openkeg.cpp \
    buykeg.cpp \
    settings.cpp \
    reward.cpp \
    base/iobase.cpp \
    deckbuildingchoose.cpp \
    choosefromlist.cpp \
    cardshowclass.cpp \
    switchhandcard.cpp \
    ansshow.cpp \
    base/DataControl.cpp

HEADERS  += mainwindow.h \
    base/Cards.h \
    base/CONST.h \
    base/DataControl.h \
    base/Functions.h \
    base/GameController.h \
    load.h \
    login.h \
    choosing.h \
    collection.h \
    showncard.h \
    friends.h \
    deckbuilder.h \
    playing.h \
    openkeg.h \
    buykeg.h \
    settings.h \
    reward.h \
    base/iobase.h \
    deckbuildingchoose.h \
    choosefromlist.h \
    cardshowclass.h \
    switchhandcard.h \
    ansshow.h \
    base/PlayerInfo.h

FORMS    += mainwindow.ui \
    load.ui \
    login.ui \
    choosing.ui \
    collection.ui \
    friends.ui \
    deckbuilder.ui \
    playing.ui \
    openkeg.ui \
    buykeg.ui \
    settings.ui \
    reward.ui \
    deckbuildingchoose.ui \
    choosefromlist.ui \
    cardshowclass.ui \
    switchhandcard.ui \
    ansshow.ui

RESOURCES += \
    pics.qrc

DISTFILES += \
    base/psw.fhp

CONFIG += resources_big
