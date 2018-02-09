#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QList>
#include <QFile>
#include "song.h"
#include "downloader.h"

class playlist : public QObject
{
    Q_OBJECT
public:
   // explicit playlist(QObject *parent = nullptr);
    playlist();
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
    downloader *d;  //下载歌单json数据，只用一次
    QList<QString> songMidList;
    QString playListMid;
    QString playListDownloadLink;
    QString nowSongMid;
    QString refererString;
    qint32 total;
};

#endif // PLAYLIST_H
