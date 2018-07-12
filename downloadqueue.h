#ifndef DOWNLOADQUEUE_H
#define DOWNLOADQUEUE_H

#include <QObject>
#include "qqmusic.h"
#include "neteaseplaylist.h"
#include "config.h"
#include "db4musicdownloader.h"

extern int songNameType;
extern int songQuality;
extern QString mp3FileName;
extern QString mp3Dir;
extern QString vkey;
extern QString guid;
extern QString vkey2;

class downloadQueue : public QObject
{
    Q_OBJECT
public:
    explicit downloadQueue(QObject *parent = nullptr);
    void insertQueue(int inFlag,int inQuality,int inNameType,QString inDir,QString inUrl);
    QList<struct downloadQueueInfo> getQueue();
signals:
    void downloadProgress(qint64, qint64);
    void status(QString);
    void finished(QString);
private slots:
    void oneTaskFinished();

private:
    QList<struct downloadQueueInfo> queueList;
    qqmusic *q;
    neteasePlaylist *np;
    db4MusicDownloader *db;

    bool downloading;

    void doJob();
};

#endif // DOWNLOADQUEUE_H
