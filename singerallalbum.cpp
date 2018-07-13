#include "singerallalbum.h"
#include "ui_singerallalbum.h"

singerAllAlbum::singerAllAlbum(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::singerAllAlbum)
{
    songNameType=0;
    songQuality=0;
    ui->setupUi(this);
    d=new downloader();
    connect(d,SIGNAL(finished()),this,SLOT(downloadFinished()));
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(downloadFailed(QString)));
    connect(d,SIGNAL(redirected(QString)),this,SLOT(downloadFailed(QString)));
}

singerAllAlbum::~singerAllAlbum()
{
    delete ui;
}

void singerAllAlbum::on_pushButtonChosePath_clicked()
{
    mp3Dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEditMp3Path->setText(mp3Dir);
}

void singerAllAlbum::on_radioButtonSongNameType1_clicked()
{
    songNameType=1;
}

void singerAllAlbum::on_radioButtonSongNameType2_clicked()
{
    songNameType=2;
}

void singerAllAlbum::on_radioButtonQuality128_clicked()
{
    songQuality=1;
}

void singerAllAlbum::on_radioButtonQuality320_clicked()
{
    songQuality=2;
}

void singerAllAlbum::on_radioButtonQualityflac_clicked()
{
    songQuality=3;
}

void singerAllAlbum::on_radioButtonQualityape_clicked()
{
    songQuality=4;
}

void singerAllAlbum::on_pushButton_clicked()
{
    ui->textEdit->clear();
    mp3Dir=ui->lineEditMp3Path->text();
    if(!songNameType || !songQuality || ui->lineEditMp3Path->text().isEmpty() || ui->lineEditSingerUrl->text().isEmpty())
    {
        ui->textEdit->append("请输入合法的设置");
    }
    else{
        if(ui->lineEditSingerUrl->text().contains("y.qq.com/n/yqq/singer"))
        {
            if(ui->lineEditSingerUrl->text().contains("num"))
            {
                ui->textEdit->append("请选择不含num的链接");
                return;
            }
            ui->pushButton->setEnabled(false);
            doJob();
        }
        else{
            ui->textEdit->append("请填入如https://y.qq.com/n/yqq/singer/002J4UUk29y8BY.html的歌手链接");
        }
    }
}

void singerAllAlbum::setDownloadQueue(downloadQueue *inDQ)
{
    dq=inDQ;
}

void singerAllAlbum::doJob()
{
    singerName="";
    albumList.clear();
    albumNum=0;
    nowNum=30;
    char tmp[30];
    getStringBetweenAandB(ui->lineEditSingerUrl->text().toStdString().c_str(),"yqq/singer/",".html",tmp);
    singerMid=tmp;

    QString link=QString(SINGERALBUMHEAD)+singerMid+QString(SINGERALBUMMID1)+QString("0")+QString(SINGERALBUMMID2)+QString("30")+QString(SINGERALBUMTAIL);
    d->init(link,"singerAllAlbum.tmp");
    d->setUserAgent(UA);
    d->setReferer("https://y.qq.com");
    d->doGet();
}

void singerAllAlbum::downloadFailed(QString inStr)
{
    ui->textEdit->append("获取专辑信息失败，请联系作者");
    ui->textEdit->append(inStr);
}

void singerAllAlbum::downloadFinished()
{
    char tmp[200];
    QFile file("singerAllAlbum.tmp");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray b=file.readAll();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(b,&jsonError);
        if(jsonError.error!=QJsonParseError::NoError)
        {
            qDebug() << "read singerAllAlbum.tmp file failed";
            qDebug()<<jsonError.error;
            qDebug()<<jsonError.errorString();
            ui->textEdit->append("read singerAllAlbum.tmp file failed");
            file.close();
            ui->pushButton->setEnabled(true);
            return;
        }
        QJsonObject obj=doucment.object();
        obj=obj["data"].toObject();
        if(nowNum==30)
        {
            singerName=obj["singer_name"].toString();
            albumNum=obj["total"].toInt();
        }
        QJsonArray tmpArray=obj["list"].toArray();

        while(tmpArray.count()>0)
        {
            obj=tmpArray.first().toObject();
            albumList.append(obj["albumMID"].toString());
            qDebug()<<albumList.last();
            tmpArray.removeFirst();
        }
        file.close();
    }
    if(nowNum<albumNum)
    {
        QString link=QString(SINGERALBUMHEAD)+singerMid+QString(SINGERALBUMMID1)+QString::number(nowNum)+QString(SINGERALBUMMID2)+QString::number(nowNum+30)+QString(SINGERALBUMTAIL);
        nowNum+=30;
        d->init(link,"singerAllAlbum.tmp");
        d->doGet();
    }
    else
    {
        while(albumList.count()>0)
        {
            dq->insertQueue(0,songQuality,songNameType,mp3Dir+QString("/%1").arg(singerName),QString("https://y.qq.com/n/yqq/album/%1.html").arg(albumList.first()));
            albumList.removeFirst();
        }
        ui->pushButton->setEnabled(true);
        ui->textEdit->append(QString("%1张专辑已经添加到下载队列，请回到主页面查看进度，当前界面可以关闭").arg(albumNum));
    }
}
