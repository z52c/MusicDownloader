#include "downloadqueue.h"

downloadQueue::downloadQueue(QObject *parent) : QObject(parent)
{
    downloading=false;
    db=new db4MusicDownloader();
    queueList=db->getQueue();
    q=new qqmusic();
    connect(q,SIGNAL(finished()),this,SLOT(oneTaskFinished()));
    connect(q,SIGNAL(progress(qint64,qint64)),this,SIGNAL(downloadProgress(qint64,qint64)));
    connect(q,SIGNAL(status(QString)),this,SIGNAL(status(QString)));
    np=new neteasePlaylist();
    connect(np,SIGNAL(finished(int,QStringList)),q,SLOT(beginToDownload(int,QStringList)));
    connect(np,SIGNAL(status(QString)),this,SIGNAL(status(QString)));

    doJob();

}

void downloadQueue::doJob()
{
    if(!queueList.count())
        return;
    downloading=true;
    struct downloadQueueInfo tmp=queueList.at(0);

    switch(tmp.flag)
    {
    case 0: //QQ音乐下载
        songNameType=tmp.nameType;songQuality=tmp.quality;mp3Dir=tmp.dir;q->doJob(tmp.url);break;
    case 1://网易云歌单下载（全部）
        songNameType=tmp.nameType;songQuality=tmp.quality;mp3Dir=tmp.dir;np->doJob(false,tmp.url);break;
    case 2://网易云歌单下载（灰掉的）
        songNameType=tmp.nameType;songQuality=tmp.quality;mp3Dir=tmp.dir;np->doJob(true,tmp.url);
    }
}

void downloadQueue::oneTaskFinished()
{

    downloading=false;
    db->deleteFromDownloadQueue(queueList.at(0).url);
    queueList.removeAt(0);
    doJob();
    emit finished(mp3Dir);
}

void downloadQueue::insertQueue(int inFlag, int inQuality, int inNameType, QString inDir, QString inUrl)
{
    struct downloadQueueInfo tmp;
    tmp.flag=inFlag;
    tmp.quality=inQuality;
    tmp.nameType=inNameType;
    tmp.dir=inDir;
    tmp.url=inUrl;
    db->insertDownloadQueue(inFlag,inQuality,inNameType,inDir,inUrl);
    queueList.append(tmp);
    if(!downloading)
        doJob();
}

QList<struct downloadQueueInfo> downloadQueue::getQueue()
{
    return queueList;
}
