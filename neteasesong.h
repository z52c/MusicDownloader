#ifndef NETEASESONG_H
#define NETEASESONG_H

#include <QObject>
#include "downloader.h"
#include <QFile>
#include "song.h"

class neteaseSong : public QObject
{
    Q_OBJECT
public:
    neteaseSong();
    void init(QString mid);
    void songInfoGot();
    ~neteaseSong();
signals:
    finished();
    void progress(qint64, qint64);
    void beginToDownload();
public slots:
    void htmlDownloaded();
    void searchDownloaded();

private:
    downloader *d;
    downloader *search;
    QString songName;
    QString singer;
    QString album;
    song *a;
};

#endif // NETEASESONG_H
