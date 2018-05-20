#ifndef QQMUSICPLAYLIST_H
#define QQMUSICPLAYLIST_H

#include <QObject>
#include "downloader.h"
#include "daye.h"
#include "config.h"

extern int songNameType;
extern int songQuality;
extern QString mp3FileName;
extern QString mp3Dir;
extern QString vkey;
extern QString guid;

class qqMusicPlaylist : public QObject
{
    Q_OBJECT
public:
    explicit qqMusicPlaylist(QObject *parent = nullptr);
    void doJob(QString inMid);
signals:
    void finished(int,QStringList);
    void status(QString);
public slots:
    void playlistGot();
    void playlistGotFailed(QString errorString);

private:
    downloader *d;  //下载歌单json数据，只用一次
    QStringList songMidList;
    QString playListMid;
    QString refererString;
    QString playListDownloadLink;
};

#endif // QQMUSICPLAYLIST_H
