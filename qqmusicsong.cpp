#include "qqmusicsong.h"

qqMusicSong::qqMusicSong(QObject *parent) : QObject(parent)
{
    d=new downloader();
    d->setUserAgent(USERAGENT);
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(htmlGotFailed(QString)));
    connect(d,SIGNAL(redirected(QString)),this,SLOT(htmlGotFailed(QString)));
    connect(d,SIGNAL(finished()),this,SLOT(htmlGot()));

    m=new downloader();
    connect(m,SIGNAL(downloadError(QString)),this,SLOT(musicDownloadFailed(QString)));
    connect(m,SIGNAL(redirected(QString)),this,SLOT(musicDownloadFailed(QString)));
    connect(m,SIGNAL(finished()),this,SLOT(musicDownloaded()));
    connect(m,SIGNAL(progress(qint64,qint64)),this,SIGNAL(progress(qint64,qint64)));

    p=new downloader();
    connect(p,SIGNAL(downloadError(QString)),this,SLOT(picDownloadFailed(QString)));
    connect(p,SIGNAL(redirected(QString)),this,SLOT(picDownloadFailed(QString)));
    connect(p,SIGNAL(finished()),this,SLOT(picDownloaded()));
}

void qqMusicSong::doJob(QString inMid)
{
    songMid=inMid;
    htmlLink=QString(GETVKEYLINKHEAD)+songMid+QString(GETVKEYLINKTAIL);

    d->init(htmlLink,QString(SONGHTMLFILE));
    d->setUserAgent(QString(USERAGENT));
    d->doGet();
}

void qqMusicSong::htmlGotFailed(QString errorString)
{
    emit status(songMid+QString("-html下载失败，失败原因：")+errorString);
    finished(-1);
}


/****************************
 * html文件已经下载
 * 找出mediaMid,vkey,guid,songName,albumName,picUrl,singerName
 * **************************/

void qqMusicSong::htmlGot()
{
    int flag=0;
    QString line;
    QFile file(SONGHTMLFILE);
    if(file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
        {
            line=file.readLine();
            if(line.contains(QString("window.songlist")))
            {
                flag=1;
                char tmpGuid[200];
                char tmpVkey[200];
                char tmpPicUrl[200];
                char tmpSongName[100];
                char tmpSingerName[100];
                char tmpsizeape[20];
                char tmpMediaMid[20];
                char tmpAlbumName[100];
                char tmpM4aUrl[200];

                int tmpFlag=0;
                getStringBetweenAandB(line.toStdString().c_str(),"songname\":\"","\"",tmpSongName);
                getStringBetweenAandB(line.toStdString().c_str(),"sizeape\":",",",tmpsizeape);
                getStringBetweenAandB(line.toStdString().c_str(),"singername\":\"","\"",tmpSingerName);
                getStringBetweenAandB(line.toStdString().c_str(),"strMediaMid\":\"","\"",tmpMediaMid);
                getStringBetweenAandB(line.toStdString().c_str(),"\"pic\":\"//","\"",tmpPicUrl);
                getStringBetweenAandB(line.toStdString().c_str(),"\"albumname\":\"","\",",tmpAlbumName);
                getStringBetweenAandB(line.toStdString().c_str(),"m4aUrl\":\"","\"",tmpM4aUrl);
                //line=file.readLine();
               // getStringBetweenAandB(line.toStdString().c_str(),"guid\":",",",tmpGuid);
               // getStringBetweenAandB(line.toStdString().c_str(),"vkey\":\"","\"",tmpVkey);


                tmpFlag=getStringBetweenAandB(tmpM4aUrl,"guid=","&",tmpGuid);
                if(tmpFlag==0)
                {
                    guid=QString(tmpGuid);
                }

                tmpFlag=getStringBetweenAandB(tmpM4aUrl,"vkey=","&",tmpVkey);
                if(tmpFlag==0)
                {
                    vkey=QString(tmpVkey);
                }
                qDebug()<<tmpGuid;
                qDebug()<<tmpVkey;

                mediaMid=QString(tmpMediaMid);
                sizeape=QString(tmpsizeape).toInt();
                singerName=QString(tmpSingerName);
                singerName.remove("\\");
                singerName.remove("/");
                songName=QString(tmpSongName);
                songName.remove("\\");
                songName.remove("/");


                picUrl=QString("https://")+QString(tmpPicUrl);
                picUrl.replace("300x300","800x800");
                albumName=QString(tmpAlbumName);
                qDebug()<<"sizeape:"<<sizeape;
                file.close();
                //开始下载音乐文件
                downloadSong();
                break;
            }
        }
        file.close();
        if(flag==0)
        {
            emit status(songMid+QString("错误"));
            emit finished(-2);
        }

    }
}

void qqMusicSong::musicDownloadFailed(QString errorString)
{
    emit status(QString("下载失败：")+errorString);
    finished(-2);
}

void qqMusicSong::musicDownloaded()
{
    if(songQuality==1 || songQuality==2)
    {
        qDebug()<<"准备下载图片";
        qDebug()<<picUrl;
        emit status("准备下载封面图片");
        p->init(picUrl,"tmp.jpg");
        p->doGet();
    }
    else
    {
        emit finished(0);
    }
}

void qqMusicSong::picDownloadFailed(QString errorString)
{
    emit status(QString("封面图片下载失败:")+errorString+QString(",不设置封面图"));
    finished(0);
}

void qqMusicSong::picDownloaded()
{
    tagtmp=new ID3tag(mp3FileName);
    connect(tagtmp,SIGNAL(finished()),this,SLOT(tagDone()));
    tagtmp->setTitle(songName);
    tagtmp->setArtist(singerName);
    tagtmp->setAlbum(albumName);
    tagtmp->setPic(QString("tmp.jpg"));
    tagtmp->doJob();
}

void qqMusicSong::tagDone()
{
    delete tagtmp;
    emit status("封面图设置结束");
    emit finished(0);
}

void qqMusicSong::getSongLink()
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

void qqMusicSong::downloadSong()
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
    QFile a(mp3FileName);
    if(a.exists())
    {
        qDebug()<<mp3FileName<<"已存在";
        emit status(mp3FileName+QString("已经存在"));
        emit finished(0);
    }
    else
    {
        m->init(mp3Link,mp3FileName);
        emit status(QString("准备下载")+mp3FileName);
        m->doGet();
    }
}
