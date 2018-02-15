#include "playlist.h"

playlist::playlist()
{

}

void playlist::init(QString mid)
{
    playListMid=mid;
    refererString =QString("https://y.qq.com/n/yqq/playsquare/")+mid+QString(".html");
    d=NULL;
    tmpSong=NULL;
    connect(tmpSong,SIGNAL(finished()),this,SLOT(songDownloaded()));
    connect(tmpSong,SIGNAL(progress(qint64,qint64)),this,SIGNAL(progress(qint64,qint64)));
    connect(tmpSong,SIGNAL(beginToDownload()),this,SIGNAL(beginToDownload()));
    connect(d,SIGNAL(finished()),this,SLOT(songListGot()));
    qDebug()<<"playlist chushihua";
    getSongList();
}

void playlist::getSongList()
{
    playListDownloadLink=QString(PLAYLISTLINKHEAD)+playListMid+QString(PALYLISTLINKTAIL);
    d->init(playListDownloadLink,QString(PLAYLISTFILE));
    qDebug()<<refererString;
    d->setReferer(refererString);
    qDebug()<<"准备下载歌单列表";
    d->doDownload();
}

void playlist::songListGot()
{
    QFile file(PLAYLISTFILE);
    int index=0;
    QString songmid;
    file.open(QIODevice::ReadOnly);
    QString str=file.readAll();
    file.close();
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
void playlist::songDownloaded()
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
        if(d!=NULL)
            delete d;
        if(tmpSong!=NULL)
            delete tmpSong;
        emit finished();
    }
}
