#include "neteaseplaylist.h"

neteasePlaylist::neteasePlaylist(QObject *parent) : QObject(parent)
{
    isGray=false;
    d=new downloader();
    d->setUserAgent(UA);
    connect(d,SIGNAL(finished()),this,SLOT(htmlGot()));
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(htmlGotFailed(QString)));
    connect(d,SIGNAL(redirected(QString)),this,SLOT(htmlGotRedirected(QString)));
    s=new neteaseSong();
    connect(s,SIGNAL(finished(int,bool,QString)),this,SLOT(neteaseSongFinished(int,bool,QString)));
}

void neteasePlaylist::doJob(bool inIsGray,QString inUrl)
{
    emit status("下载列表所有歌曲信息，此处需要耐心等待片刻,曲目数越多，等待时间越长");
    isGray=inIsGray;
    infoList.clear();
    grayList.clear();
    songMidList.clear();
    listTitle="";
    int pos;
    if(inUrl.contains("playlist?id=")&&inUrl.contains("music.163.com"))
    {
        if(inUrl.contains("&"))
        {
            char a[20];
            getStringBetweenAandB(inUrl.toStdString().c_str(),"=","&",a);
            mid=QString(a);
        }else{
            pos=inUrl.indexOf(QString("="));
            mid=inUrl.mid(pos+1);
        }
    }else if(inUrl.contains("playlist/"))
    {
        char a[20];
        getStringBetweenAandB(inUrl.toStdString().c_str(),"playlist/","/",a);
        mid=QString(a);
    }
    else
    {
        finished(-1,infoList);
    }

    QString listUrl=QString("http://music.163.com/playlist?id=")+mid;
    d->init(listUrl,SONGHTMLFILE);
    d->doGet();
}

void neteasePlaylist::htmlGotFailed(QString errorString)
{
    emit status(QString("neteasePlaylist html failed:")+errorString);
    finished(-1,infoList);
}

void neteasePlaylist::htmlGotRedirected(QString redirectString)
{
    d->init(redirectString,SONGHTMLFILE);
    d->doGet();
}

void neteasePlaylist::htmlGot()
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

    if(listTitle.isEmpty())
    {
        emit status("失败");
        finished(-2,infoList);
        return;
    }
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
    s->doJob(songMidList.at(0));
}


void neteasePlaylist::neteaseSongFinished(int inFlag,bool inIsGray,QString inString)
{
    if(inFlag)
    {
        emit status(songMidList.at(0)+QString("html下载失败:")+inString);
    }
    else{
        if(inIsGray)
            grayList.append(inString);
        infoList.append(inString);
        songMidList.removeAt(0);
        if(songMidList.count()>0)
        {
            s->doJob(songMidList.at(0));
        }
        else{
            if(isGray){
                emit finished(0,grayList);
            }
            else{
                finished(0,infoList);
            }
        }
    }
}
