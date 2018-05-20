#ifndef QQMUSICALBUM_H
#define QQMUSICALBUM_H

#include <QObject>
#include "daye.h"
#include "config.h"
#include "downloader.h"

extern int songNameType;
extern int songQuality;
extern QString mp3FileName;
extern QString mp3Dir;
extern QString vkey;
extern QString guid;

class qqMusicAlbum : public QObject
{
    Q_OBJECT
public:
    explicit qqMusicAlbum(QObject *parent = nullptr);
    void doJob(QString inMid);
signals:
    void finished(int,QStringList);
    void status(QString);
public slots:
    void albumListGot();
    void albumListGotFailed(QString errorString);
private:
    downloader *d;  //下载专辑json数据，只用一次
    QStringList songMidList;
    QString albumMid;
    QString albumDownloadLink;
};

#endif // QQMUSICALBUM_H
