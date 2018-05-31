#include "qqmusicplaylist.h"

qqMusicPlaylist::qqMusicPlaylist(QObject *parent) : QObject(parent)
{
    d=new downloader();
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(playlistGotFailed(QString)));
    connect(d,SIGNAL(redirected(QString)),this,SLOT(playlistGotFailed(QString)));
    connect(d,SIGNAL(finished()),this,SLOT(playlistGot()));
}

void qqMusicPlaylist::doJob(QString inMid)
{
    playListMid=inMid;
    refererString =QString("https://y.qq.com/n/yqq/playsquare/")+inMid+QString(".html");
    playListDownloadLink=QString(PLAYLISTLINKHEAD)+playListMid+QString(PALYLISTLINKTAIL);
    d->init(playListDownloadLink,QString(PLAYLISTFILE));
    qDebug()<<refererString;
    d->setReferer(refererString);
    qDebug()<<"准备下载歌单列表";
    emit status("准备下载歌单json数据");
    d->doGet();
}

void qqMusicPlaylist::playlistGotFailed(QString errorString)
{
    QStringList a;
    emit status(QString("QQMusic playlist json data download failed:")+errorString);
    finished(-1,a);
}

void qqMusicPlaylist::playlistGot()
{
    songMidList.clear();
    QFile file(PLAYLISTFILE);
    int index=0;
    QString songmid;
    QString playListName;
    file.open(QIODevice::ReadOnly);
    QString str=file.readAll();
    file.close();

    //将歌单名字作为目录名
    if(-1!=(index=str.indexOf("dissname",index)))
    {
        index+=11;
        playListName="";
        while(str.at(index)!='\"')
        {
            playListName+=str.at(index);
            index++;
        }
        playListName.remove(QChar('|'));
        playListName.remove(QChar('>'));
        playListName.remove(QChar('<'));
        playListName.remove(QChar('*'));
        playListName.remove(QChar('?'));
        playListName.remove(QChar('/'));
        playListName.remove(QChar('\\'));
        qDebug()<<playListName;
    }
    else{
        emit status("json 文件下载内容错误");
        QStringList a;
        finished(-1,a);
    }

    if(!playListName.isEmpty())
    {
       mp3Dir=mp3Dir+QString("/")+playListName;
       QDir tmp(mp3Dir);
       if(!tmp.exists())
       {
           tmp.mkdir(mp3Dir);
       }
    }
    else{
        emit status("json 文件下载内容错误");
        QStringList a;
        finished(-1,a);
    }
    while(-1!=(index=str.indexOf("songmid",index)))
    {
        index+=10;
        songmid="";
        while(str.at(index)!='\"')
        {
            songmid+=str.at(index);
            index++;
        }
        songMidList.append(songmid);
    }
    qDebug()<<songMidList;
    finished(0,songMidList);
}
