#include "getgray.h"
#include <QDebug>

getGray::getGray()
{
    d=new downloader();
    connect(d,SIGNAL(finished()),this,SLOT(htmlDownloaded()));

    s=new neteaseSong();
    connect(s,SIGNAL(finished()),this,SLOT(songDownloaded()));
    connect(s,SIGNAL(progress(qint64,qint64)),this,SIGNAL(progress(qint64,qint64)));
    connect(s,SIGNAL(beginToDownload()),this,SIGNAL(beginToDownload()));

    dsh=new downloader();
    connect(dsh,SIGNAL(finished()),this,SLOT(songHtmlDownloaded()));
}

void getGray::init(QString mid)
{
    QString listUrl=QString("http://music.163.com/playlist?id=")+mid;
    d->init(listUrl,SONGHTMLFILE);
    d->doDownload();
}


void getGray::htmlDownloaded()
{
    QFile file(SONGHTMLFILE);
    QString line;
    QString listTitle;
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
    QString url=QString("http://music.163.com/song?id=")+songMidList.at(0);
    dsh->init(url,SONGHTMLFILE);
    dsh->doDownload();
}


void getGray::songHtmlDownloaded()
{
    grayJob();
    if(!songMidList.isEmpty())
    {
        QString url=QString("http://music.163.com/song?id=")+songMidList.at(0);
        dsh->init(url,SONGHTMLFILE);
        dsh->doDownload();
    }else{
        total=grayMidList.length();
        songDownloaded();
    }
}

void getGray::grayJob()
{
    QFile file(SONGHTMLFILE);
    QString line;
    if(file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
        {
            line=file.readLine();
            if(line.contains("u-btni-play"))
            {
                grayMidList.append(songMidList.at(0));
                break;
            }
        }
    }
    file.close();
    qDebug()<<"im here";
    songMidList.removeAt(0);
}


void getGray::songDownloaded()
{
    if(!grayMidList.isEmpty())
    {
        emit nownum(total-grayMidList.length()+1,total);
        nowSongMid=grayMidList.at(0);
        grayMidList.removeAt(0);
        s->init(nowSongMid);
    }else{
        emit finished();
    }
}
