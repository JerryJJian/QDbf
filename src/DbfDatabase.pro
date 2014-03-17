#-------------------------------------------------
#
# Project created by QtCreator 2012-09-17T20:57:18
#
#-------------------------------------------------

QT       -= gui

TARGET = DbfDatabase
TEMPLATE = lib

DEFINES += DBFDATABASE_LIBRARY

SOURCES += \
    dbfdatabaseheader.cpp \
    dbfrecord.cpp \
    dbffield.cpp \
    dbfdatabase.cpp \
    dataconvert.cpp

HEADERS +=\
        DbfDatabase_global.h \
    dbfdatabaseheader.h \
    dbfrecord.h \
    dbffield.h \
    dbfdatabase.h \
    dataconvert.h

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../../Debug/

    OBJECTS_DIR = $$PWD/../../output/debug/DbfDatabase/obj
    MOC_DIR = $$PWD/../../output/debug/DbfDatabase/moc
    UI_DIR = $$PWD/../../output/debug/DbfDatabase/ui
    RCC_DIR = $$PWD/../../output/debug/DbfDatabase/rcc
} else {
    DESTDIR = $$PWD/../../Release/

    OBJECTS_DIR = $$PWD/../../output/release/DbfDatabase/obj
    MOC_DIR = $$PWD/../../output/release/DbfDatabase/moc
    UI_DIR = $$PWD/../../output/release/DbfDatabase/ui
    RCC_DIR = $$PWD/../../output/release/DbfDatabase/rcc
}


