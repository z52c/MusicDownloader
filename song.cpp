#include "song.h"

song::song()
{

}

song::song(QString mid)
{
    songMid=mid;
    if(d!=NULL)
        delete d;
    d=new downloader();
    htmlLink=QString(GETVKEYLINKHEAD)+songMid+QString(GETVKEYLINKTAIL);
    connect(d,SIGNAL(finished()),this,SLOT(shtmlLinkDownloaded()));
    d->init(htmlLink,QString(SONGHTMLFILE));
    d->setUserAgent(QString(USERAGENT));
    d->doDownload();
}

void song::init(QString mid)
{
    songMid=mid;
    htmlLink=QString(GETVKEYLINKHEAD)+songMid+QString(GETVKEYLINKTAIL);
    qDebug()<<"song init1 -htmllink:"<<htmlLink;
    if(d!=NULL)
    {
        qDebug()<<"song init delete d:";
      //  delete d;
    }
    qDebug()<<"song init new d";
    d=new downloader();
    connect(d,SIGNAL(finished()),this,SLOT(shtmlLinkDownloaded()));
    qDebug()<<"song init2 -htmllink:"<<htmlLink;
    d->init(htmlLink,QString(SONGHTMLFILE));
    d->setUserAgent(QString(USERAGENT));
    d->doDownload();
}

void song::shtmlLinkDownloaded()
{
    QString line;
    QFile file(SONGHTMLFILE);
    if(file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
        {
            line=file.readLine();
            if(line.contains(QString("window.songlist")))
            {
                char tmpGuid[200];
                char tmpVkey[200];
                char tmpSongName[100];
                char tmpSingerName[100];
                char tmpsizeape[20];
                char tmpMediaMid[20];

                getStringBetweenAandB(line.toStdString().c_str(),"songname\":\"","\"",tmpSongName);
                getStringBetweenAandB(line.toStdString().c_str(),"sizeape\":",",",tmpsizeape);
                getStringBetweenAandB(line.toStdString().c_str(),"singername\":\"","\"",tmpSingerName);
                getStringBetweenAandB(line.toStdString().c_str(),"strMediaMid\":\"","\"",tmpMediaMid);


                line=file.readLine();
                getStringBetweenAandB(line.toStdString().c_str(),"guid\":",",",tmpGuid);
                getStringBetweenAandB(line.toStdString().c_str(),"vkey\":\"","\"",tmpVkey);

                mediaMid=QString(tmpMediaMid);
                sizeape=QString(tmpsizeape).toInt();
                singerName=QString(tmpSingerName);
                singerName.remove("\\");
                singerName.remove("/");
                songName=QString(tmpSongName);
                songName.remove("\\");
                songName.remove("/");
                vkey=QString(tmpVkey);
                guid=QString(tmpGuid);
                qDebug()<<"sizeape:"<<sizeape;
                downloadSong();
                break;
            }
        }
        file.close();
    }
}

void song::getSongLink()
{

    if(sizeape==0 || songQuality == 1)
    {
        mp3Link=QString(DOWNLOADLINKHEAD)+QString("M500")+mediaMid+QString(".mp3")+QString(DOWNLOADLINKMID)+vkey+QString(DOWNLOADLINKGUID)+guid+QString(DOWNLOADLINKTAIL);
        return ;
    }


    switch (songQuality) {
    case 2:
        mp3Link=QString(DOWNLOADLINKHEAD)+QString("M800")+mediaMid+QString(".mp3")+QString(DOWNLOADLINKMID)+vkey+QString(DOWNLOADLINKGUID)+guid+QString(DOWNLOADLINKTAIL);
        break;
    case 3:
        mp3Link=QString(DOWNLOADLINKHEAD)+QString("F000")+mediaMid+QString(".flac")+QString(DOWNLOADLINKMID)+vkey+QString(DOWNLOADLINKGUID)+guid+QString(DOWNLOADLINKTAIL);
        break;
    case 4:
        mp3Link=QString(DOWNLOADLINKHEAD)+QString("A000")+mediaMid+QString(".ape")+QString(DOWNLOADLINKMID)+vkey+QString(DOWNLOADLINKGUID)+guid+QString(DOWNLOADLINKTAIL);
        break;
    default:
        mp3Link=QString(DOWNLOADLINKHEAD)+QString("M500")+mediaMid+QString(".mp3")+QString(DOWNLOADLINKMID)+vkey+QString(DOWNLOADLINKGUID)+guid+QString(DOWNLOADLINKTAIL);
        break;
    }
}


void song::downloadSong()
{
    getSongLink();
    if(songNameType==2)
        mp3FileName=mp3Dir+QString("/")+singerName+QString("-")+songName;
    else
        mp3FileName=mp3Dir+QString("/")+songName+QString("-")+singerName;
    switch (songQuality) {
    case 1:
        mp3FileName=mp3FileName+QString("-128")+".mp3";
        break;
    case 2:
        mp3FileName=mp3FileName+".mp3";
        break;
    case 3:
        mp3FileName=mp3FileName+".flac";
        break;
    case 4:
        mp3FileName=mp3FileName+".ape";
        break;
    }
    qDebug()<<"mp3link:"<<mp3Link;
    qDebug()<<"mp3filename:"<<mp3FileName;
    if(m!=NULL)
    {
        qDebug()<<"downloadsong : delete m";
        //delete m;
    }
    m=new downloader(mp3Link,mp3FileName);
    emit beginToDownload();
    connect(m,SIGNAL(progress(qint64,qint64)),this,SIGNAL(progress(qint64,qint64)));
    connect(m,SIGNAL(finished()),this,SIGNAL(finished()));
    m->doDownload();
}

