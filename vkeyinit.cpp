#include "vkeyinit.h"

vkeyInit::vkeyInit(QObject *parent) : QObject(parent)
{
    d=new downloader();
    connect(d,SIGNAL(finished()),this,SLOT(htmlGot()));
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(htmlFailed()));
    tmpSongMidList.append("000QCwge3B6Ad1");
    tmpSongMidList.append("000fcbn33tw0lQ");
    tmpSongMidList.append("000GxVns4CRPED");
    tmpSongMidList.append("002ZdBsi2QzFIX");
    tmpSongMidList.append("000Seu8o0atbuw");
    tmpSongMidList.append("001Y73IO3VgZPK");
    tmpSongMidList.append("001POh7r2KxOb6");
}

void vkeyInit::init()
{
    QString htmlLink=QString(GETVKEYLINKHEAD)+tmpSongMidList.first()+QString(GETVKEYLINKTAIL);
    d->init(htmlLink,QString(SONGHTMLFILE));
    d->setUserAgent(QString(USERAGENT));
    d->doGet();
}

void vkeyInit::htmlFailed()
{
    emit status("未联网，初始化失败，请联网后重新打开程序");
}

void vkeyInit::htmlGot()
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

                int tmpFlag=0;
                tmpFlag=getStringBetweenAandB(line.toStdString().c_str(),"guid=","&",tmpGuid);
                if(tmpFlag==0)
                {
                    guid=QString(tmpGuid);
                }

                tmpFlag=getStringBetweenAandB(line.toStdString().c_str(),"vkey=","&",tmpVkey);
                if(tmpFlag==0)
                {
                    vkey=QString(tmpVkey);
                }
                qDebug()<<tmpGuid;
                qDebug()<<tmpVkey;

                file.close();
                //开始下载音乐文件
                if(vkey!="" && guid!="")
                {
                    emit finished();
                    return;
                }else
                {
                    if(tmpSongMidList.count()==0)
                    {
                        emit status("凉了，联系作者");
                    }
                    tmpSongMidList.removeFirst();
                    init();
                }
            }
        }
        file.close();
    }
}
