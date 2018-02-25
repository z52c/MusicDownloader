#include "album.h"



album::album()
{
    d=new downloader();
    tmpSong=new song();
    connect(tmpSong,SIGNAL(finished()),this,SLOT(songDownloaded()));
    connect(tmpSong,SIGNAL(progress(qint64,qint64)),this,SIGNAL(progress(qint64,qint64)));
    connect(tmpSong,SIGNAL(beginToDownload()),this,SIGNAL(beginToDownload()));
    connect(d,SIGNAL(finished()),this,SLOT(songListGot()));
}

void album::init(QString mid)
{
    albumMid=mid;
    qDebug()<<"album init";
    getSongList();
}

void album::getSongList()
{
    albumDownloadLink=QString(ALBUMLINKHEAD)+albumMid+QString(ALBUMLINKTAIL);
    d->init(albumDownloadLink,QString(ALBUMLISTFILE));
    qDebug()<<"准备下载专辑列表";
    d->doDownload();
}


//获取到专辑歌曲列表json后，找到歌曲mid保存到list中
void album::songListGot()
{
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
        qDebug()<<albumDirName;
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
    total=songMidList.length();
    songDownloaded();
}

//一首歌曲下载之后查询list中有没有内容，有就调用tmpSong->init(songMid)执行下一个
//如果没有内容，则发送 finished信号
void album::songDownloaded()
{
    if(!songMidList.isEmpty())
    {
        emit nownum(total-songMidList.length()+1,total);
        nowSongMid=songMidList.first();
        songMidList.removeAt(0);
        qDebug()<<"准备下载"<<nowSongMid;
        tmpSong->init(nowSongMid); 
        qDebug()<<"2";
    }
    else
    {
        emit finished();
    }
}
