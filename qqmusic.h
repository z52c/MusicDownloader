#ifndef QQMUSIC_H
#define QQMUSIC_H

#include <QObject>
#include "qqmusicalbum.h"
#include "qqmusicplaylist.h"
#include "qqmusicsong.h"

extern int songNameType;
extern int songQuality;
extern QString mp3FileName;
extern QString mp3Dir;
extern QString vkey;
extern QString guid;

class qqmusic : public QObject
{
    Q_OBJECT
public:
    explicit qqmusic(QObject *parent = nullptr);
    void doJob(QString inlink);
signals:
    void progress(qint64, qint64);
    void finished();
    void status(QString);
public slots:
    void beginToDownload(int inFlag,QStringList inSongMidList);
    void songDownloadFinished(int inFlag);
private:
    qqMusicAlbum *album;
    qqMusicPlaylist *playlist;
    qqMusicSong *song;

    QStringList songMidList;
    int index;
    int total;

    void downloadSong();
};

#endif // QQMUSIC_H
