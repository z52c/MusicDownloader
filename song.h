#ifndef SONG_H
#define SONG_H

#include <QFile>
#include <QObject>
#include "downloader.h"
#include "daye.h"
#include "config.h"

extern int songNameType;
extern int songQuality;
extern QString mp3FileName;
extern QString mp3Dir;



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

signals:
    void progress(qint64, qint64);
    void beginToDownload();
    void finished();

public slots:
    void shtmlLinkDownloaded();
   // void songFinished();
private:
    QString songMid;
    QString htmlLink;
    QString songName;
    QString singerName;
    QString vkey;
    QString guid;
    int sizeape;
    QString mediaMid;
    QString mp3Link;
    downloader *d;
    downloader *m;
    QString tmpHtmlFileName;
};

#endif // SONG_H
