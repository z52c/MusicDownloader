#include "qqmusic.h"

qqmusic::qqmusic(QObject *parent) : QObject(parent)
{
    album=new qqMusicAlbum();
    connect(album,SIGNAL(status(QString)),this,SIGNAL(status(QString)));
    connect(album,SIGNAL(finished(int,QStringList)),this,SLOT(beginToDownload(int,QStringList)));
    playlist=new qqMusicPlaylist();
    connect(playlist,SIGNAL(status(QString)),this,SIGNAL(status(QString)));
    connect(playlist,SIGNAL(finished(int,QStringList)),this,SLOT(beginToDownload(int,QStringList)));
    song=new qqMusicSong();
    connect(song,SIGNAL(status(QString)),this,SIGNAL(status(QString)));
    connect(song,SIGNAL(progress(qint64,qint64)),this,SIGNAL(progress(qint64,qint64)));
    connect(song,SIGNAL(finished(int)),this,SLOT(songDownloadFinished(int)));
}

void qqmusic::doJob(QString inlink)
{
    songMidList.clear();
    index=0;
    total=0;
    char tmpMid[20];
    if(inlink.contains("y.qq.com/n/yqq/song"))
    {
        getStringBetweenAandB(inlink.toStdString().c_str(),"y.qq.com/n/yqq/song/",".html",tmpMid);
        songMidList.append(QString(tmpMid));
        downloadSong();
        return;
    }

    if(inlink.contains("y.qq.com/n/yqq/album"))
    {
        getStringBetweenAandB(inlink.toStdString().c_str(),"y.qq.com/n/yqq/album/",".html",tmpMid);
        album->doJob(QString(tmpMid));
        return;
    }

    if(inlink.contains("y.qq.com/n/yqq/playsquare"))
    {
        getStringBetweenAandB(inlink.toStdString().c_str(),"y.qq.com/n/yqq/playsquare/",".html",tmpMid);
        playlist->doJob(QString(tmpMid));
        return;
    }
    if(inlink.contains("y.qq.com/n/yqq/playlist"))
    {
        getStringBetweenAandB(inlink.toStdString().c_str(),"y.qq.com/n/yqq/playlist/",".html",tmpMid);
        playlist->doJob(QString(tmpMid));
        return;
    }
}


void qqmusic::beginToDownload(int inFlag, QStringList inSongMidList)
{
    if(inFlag)
    {
        emit finished();
        return;
    }
    else{
        songMidList=inSongMidList;
        downloadSong();
    }
}

void qqmusic::downloadSong()
{
    total=songMidList.count();
    emit status(QString("下载第1/")+QString::number(total)+QString("首..."));
    song->doJob(songMidList.at(0));
}

void qqmusic::songDownloadFinished(int inFlag)
{
    songMidList.removeAt(0);
    if(songMidList.count()>0)
    {
        emit status(QString("下载第")+QString::number(total-songMidList.count()+1)+QString("/")+QString::number(total)+QString("首..."));
        song->doJob(songMidList.at(0));
    }
    else{
        emit finished();
    }

}
