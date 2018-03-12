#ifndef NETEASEPLAYLIST_H
#define NETEASEPLAYLIST_H

#include <QObject>
#include "neteasesong.h"
#include "downloader.h"
#include "config.h"

class neteasePlaylist : public QObject
{
    Q_OBJECT
public:
    neteasePlaylist();
    void init(QString mid);

signals:
    finished();
    void progress(qint64, qint64);
    void beginToDownload();
    void nownum(qint32,qint32);
public slots:
    void htmlDownloaded();
    void songDownloaded();
private:
    neteaseSong *s;
    downloader *d;
    QList<QString> songMidList;
    QString nowSongMid;
    qint32 total;
    QString listTitle;


};

#endif // NETEASEPLAYLIST_H
