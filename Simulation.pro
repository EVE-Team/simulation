#-------------------------------------------------
#
# Project created by QtCreator 2018-09-27T20:52:20
#
#-------------------------------------------------

QT       += core gui widgets charts

TARGET = Simulation
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    cell.cpp \
    world.cpp \
    resourcemanager.cpp \
    terraingenerator.cpp \
    rabbit.cpp \
    creature.cpp \
    hunter.cpp \
    wolf.cpp \
    populationgraph.cpp

HEADERS += \
        mainwindow.h \
    cell.h \
    world.h \
    resourcemanager.h \
    terraingenerator.h \
    terraingenerator.h \
    world.h \
    rabbit.h \
    creature.h \
    hunter.h \
    wolf.h \
    populationgraph.h

FORMS += \
        mainwindow.ui \
    populationgraph.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
