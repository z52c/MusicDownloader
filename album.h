#ifndef ALBUM_H
#define ALBUM_H

#include <QObject>
#include <QList>
#include <QFile>
#include "song.h"
#include "downloader.h"

class album : public QObject
{
    Q_OBJECT
public:
    album();
    void init(QString mid);
    void getSongList();

signals:
    void beginToDownload();
    void progress(qint64, qint64);
    void finished();
    void nownum(qint32,qint32);
public slots:
    void songDownloaded();
    void songListGot();
private:
    song *tmpSong;  //通过song->init(mid) 开启下载
    downloader *d;  //下载专辑json数据，只用一次
    QList<QString> songMidList;
    QString albumMid;
    QString albumDownloadLink;
    QString nowSongMid;
    qint32 total;
};

#endif // ALBUM_H
