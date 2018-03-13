#ifndef GETGRAY_H
#define GETGRAY_H

#include <QObject>
#include "neteasesong.h"
#include "downloader.h"
#include "config.h"

class getGray: public QObject
{
    Q_OBJECT
public:
    getGray();
    void init(QString mid);

signals:
    finished();
    void progress(qint64, qint64);
    void beginToDownload();
    void nownum(qint32,qint32);

public slots:
    void htmlDownloaded();
    void songHtmlDownloaded();
    void songDownloaded();

private:
    neteaseSong *s;
    downloader *d;
    downloader *dsh;
    QList<QString> songMidList;
    QList<QString> grayMidList;
    QString nowSongMid;
    qint32 total;

    void grayJob();

};

#endif // GETGRAY_H
