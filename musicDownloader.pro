#-------------------------------------------------
#
# Project created by QtCreator 2018-05-17T15:22:49
#
#-------------------------------------------------

QT       += core gui network

RC_ICONS=1.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = musicDownloader
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
        mainwindow.cpp \
    downloader.cpp \
    daye.cpp \
    qqmusicsong.cpp \
    id3tag.cpp \
    qqmusicplaylist.cpp \
    qqmusicalbum.cpp \
    qqmusic.cpp \
    qqmusicsearch.cpp \
    neteasesong.cpp \
    neteaseplaylist.cpp \
    thanks.cpp \
    help.cpp \
    proxyform.cpp

HEADERS += \
        mainwindow.h \
    downloader.h \
    config.h \
    daye.h \
    qqmusicsong.h \
    id3tag.h \
    qqmusicplaylist.h \
    qqmusicalbum.h \
    qqmusic.h \
    qqmusicsearch.h \
    neteasesong.h \
    neteaseplaylist.h \
    thanks.h \
    help.h \
    proxyform.h

FORMS += \
        mainwindow.ui \
    help.ui \
    thanks.ui \
    proxyform.ui

VERSION = 1.4.3

QMAKE_TARGET_PRODUCT = QQMusic&NeteaseMusicDownloader
QMAKE_TARGET_COMPANY = z52c
QMAKE_TARGET_DESCRIPTION = QQMusic&NeteaseMusicDownloader
QMAKE_TARGET_COPYRIGHT = z52c

RESOURCES += \
    pic.qrc


LIBS += -L"C:/OpenSSL-Win32/lib" -llibssl
LIBS += -L"C:/OpenSSL-Win32/lib" -llibcrypto

INCLUDEPATH += C:/OpenSSL-Win32/include
