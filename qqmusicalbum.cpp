#include "qqmusicalbum.h"

qqMusicAlbum::qqMusicAlbum(QObject *parent) : QObject(parent)
{
    d=new downloader();
    connect(d,SIGNAL(finished()),this,SLOT(albumListGot()));
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(albumListGotFailed(QString)));
    connect(d,SIGNAL(redirected(QString)),this,SLOT(albumListGotFailed(QString)));
}

void qqMusicAlbum::doJob(QString inMid)
{
    albumMid=inMid;
    albumDownloadLink=QString(ALBUMLINKHEAD)+albumMid+QString(ALBUMLINKTAIL);
    d->init(albumDownloadLink,QString(ALBUMLISTFILE));
    qDebug()<<"准备下载专辑列表";
    d->doGet();
}

void qqMusicAlbum::albumListGotFailed(QString errorString)
{
    QStringList a;
    emit status(QString("QQMusic albumlist json data download failed:")+errorString);
    finished(-1,a);
}

void qqMusicAlbum::albumListGot()
{
    songMidList.clear();
    QFile file(ALBUMLISTFILE);
    int index=0;
    QString songmid;
    QString albumDirName;
    file.open(QIODevice::ReadOnly);
    QString str=file.readAll();
    file.close();

    //将专辑名字作为目录名
    if(-1!=(index=str.indexOf("albumname",index)))
    {
        index+=12;
        albumDirName="";
        while(str.at(index)!='\"')
        {
            albumDirName+=str.at(index);
            index++;
        }
        albumDirName.remove(QChar('|'));
        albumDirName.remove(QChar('>'));
        albumDirName.remove(QChar('<'));
        albumDirName.remove(QChar('*'));
        albumDirName.remove(QChar('?'));
        albumDirName.remove(QChar('/'));
        albumDirName.remove(QChar('\\'));
        albumDirName.remove(QChar(':'));
        qDebug()<<albumDirName;
    }else{
        emit status("json 文件下载内容错误");
        QStringList a;
        finished(-1,a);
    }

    if(!albumDirName.isEmpty())
    {
       mp3Dir=mp3Dir+QString("/")+albumDirName;
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
