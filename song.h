#ifndef SONG_H
#define SONG_H

#include <QFile>
#include <QObject>
#include "downloader.h"
#include "daye.h"
#include "config.h"
#include "id3tag.h"

extern int songNameType;
extern int songQuality;
extern QString mp3FileName;
extern QString mp3Dir;
extern QString vkey;
extern QString guid;


class song : public QObject
{
    Q_OBJECT
public:
   // explicit song(QObject *parent = nullptr);
    song();
    song(QString mid);
    void init(QString mid);
    void getSongLink();
    void downloadSong();
    ~song();
signals:
    void progress(qint64, qint64);
    void beginToDownload();
    void finished();
    void qqMusicStat(QString);

public slots:
    void shtmlLinkDownloaded();
    void picDownloaded();
    void musicDownloaded();
    void tagDone();
private:
    QString songMid;
    QString htmlLink;
    QString songName;
    QString singerName;

    QString picUrl;
    QString albumName;
    int sizeape;
    QString mediaMid;
    QString mp3Link;
    downloader *d;
    downloader *m;
    downloader *p;
    ID3tag *tagtmp;
    QString tmpHtmlFileName;
};

#endif // SONG_H
