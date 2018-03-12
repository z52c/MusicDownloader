#include "neteaseplaylist.h"

neteasePlaylist::neteasePlaylist()
{
    d=new downloader();
    connect(d,SIGNAL(finished()),this,SLOT(htmlDownloaded()));

    s=new neteaseSong();
    connect(s,SIGNAL(finished()),this,SLOT(songDownloaded()));
    connect(s,SIGNAL(progress(qint64,qint64)),this,SIGNAL(progress(qint64,qint64)));
    connect(s,SIGNAL(beginToDownload()),this,SIGNAL(beginToDownload()));
}

void neteasePlaylist::init(QString mid)
{
    QString listUrl=QString("http://music.163.com/playlist?id=")+mid;
    d->init(listUrl,SONGHTMLFILE);
    d->doDownload();
}

void neteasePlaylist::htmlDownloaded()
{
    QFile file(SONGHTMLFILE);
    QString line;
    int pos;
    char tmpString[200];
    if(file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
        {
            line=file.readLine();
            if(line.contains("appid"))
            {
                line=file.readLine();
                getStringBetweenAandB(line.toStdString().c_str(),"\"title\": \"","\"",tmpString);
                listTitle=QString(tmpString);
                break;
            }
        }
        qDebug()<<listTitle;
        while(!file.atEnd())
        {
            line=file.readLine();
            if(line.contains("class=\"f-hide\""))
            {
                while((pos=line.indexOf(QString("song?id")))!=-1)
                {
                    getStringBetweenAandB(line.toStdString().c_str(),"song?id=","\"",tmpString);
                    songMidList.append(QString(tmpString));
                    line=line.mid(pos+10);
                    qDebug()<<QString(tmpString);
                }
                break;
            }
        }
    }
    file.close();

    listTitle.remove(QChar('|'));
    listTitle.remove(QChar('>'));
    listTitle.remove(QChar('<'));
    listTitle.remove(QChar('*'));
    listTitle.remove(QChar('?'));
    listTitle.remove(QChar('/'));
    listTitle.remove(QChar('\\'));

    mp3Dir=mp3Dir+QString("/")+listTitle;
    QDir tmp(mp3Dir);
    if(!tmp.exists())
    {
        tmp.mkdir(mp3Dir);
    }
    total=songMidList.length();
    songDownloaded();
}

void neteasePlaylist::songDownloaded()
{
    if(!songMidList.isEmpty())
    {
        emit nownum(total-songMidList.length()+1,total);
        nowSongMid=songMidList.first();
        songMidList.removeAt(0);
        s->init(nowSongMid);
    }else{
        emit finished();
    }
}
