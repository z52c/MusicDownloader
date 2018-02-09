#-------------------------------------------------
#
# Project created by QtCreator 2018-02-01T16:47:13
#
#-------------------------------------------------

QT       += core gui
QT +=network

RC_ICONS=1.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qqmusic
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


SOURCES += \
        main.cpp \
        qqmusic.cpp \
    downloader.cpp \
    daye.cpp \
    song.cpp \
    album.cpp \
    playlist.cpp

HEADERS += \
        qqmusic.h \
    downloader.h \
    config.h \
    daye.h \
    song.h \
    album.h \
    playlist.h

FORMS += \
        qqmusic.ui
