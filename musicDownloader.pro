#-------------------------------------------------
#
# Project created by QtCreator 2018-07-12T15:31:35
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = musicDownloader
TEMPLATE = app

RC_ICONS=1.ico

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
        mainwindow.cpp \
    daye.cpp \
    downloader.cpp \
    help.cpp \
    id3tag.cpp \
    proxyform.cpp \
    thanks.cpp \
    db4musicdownloader.cpp \
    downloadqueue.cpp \
    neteaseplaylist.cpp \
    neteasesong.cpp \
    qqmusic.cpp \
    qqmusicalbum.cpp \
    qqmusicplaylist.cpp \
    qqmusicsearch.cpp \
    qqmusicsong.cpp \
    vkeyinit2.cpp \
    singerallalbum.cpp \
    axeldownload.cpp

HEADERS += \
        mainwindow.h \
    config.h \
    daye.h \
    downloader.h \
    help.h \
    id3tag.h \
    proxyform.h \
    thanks.h \
    db4musicdownloader.h \
    downloadqueue.h \
    neteaseplaylist.h \
    neteasesong.h \
    qqmusic.h \
    qqmusicalbum.h \
    qqmusicplaylist.h \
    qqmusicsearch.h \
    qqmusicsong.h \
    vkeyinit2.h \
    singerallalbum.h \
    axeldownload.h

FORMS += \
        mainwindow.ui \
    help.ui \
    proxyform.ui \
    thanks.ui \
    singerallalbum.ui

RESOURCES += \
    pic.qrc

DISTFILES += \
    1.jpg \
    1.ico

VERSION = 1.8.4
