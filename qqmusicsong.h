#ifndef QQMUSICSONG_H
#define QQMUSICSONG_H

#include <QObject>
#include "id3tag.h"
#include "downloader.h"
#include "daye.h"
#include "config.h"

extern int songNameType;
extern int songQuality;
extern QString mp3FileName;
extern QString mp3Dir;
extern QString vkey;
extern QString guid;


class qqMusicSong : public QObject
{
    Q_OBJECT
public:
    explicit qqMusicSong(QObject *parent = nullptr);
    void doJob(QString inMid);
signals:
    void progress(qint64, qint64);
    void finished(int);//成功0，html失败-1，音乐文件下载失败-2
    void status(QString);
public slots:
    void htmlGot();
    void htmlGotFailed(QString errorString);

    void musicDownloaded();
    void musicDownloadFailed(QString errorString);

    void picDownloaded();
    void picDownloadFailed(QString errorString);

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
    downloader *d;//下载html文件
    downloader *m;//下载音乐文件
    downloader *p;//下载图片文件

    ID3tag *tagtmp;
    QString tmpHtmlFileName;

    void downloadSong();
    void getSongLink();
};

#endif // QQMUSICSONG_H
