#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    h=new help();
    t=new thanks();
    p=new proxyForm();
    songNameType=0;
    songQuality=0;
    mp3Dir="";
    q=new qqmusic();
    connect(q,SIGNAL(finished()),this,SLOT(qqmusicFinished()));
    connect(q,SIGNAL(status(QString)),this,SLOT(status(QString)));
    connect(q,SIGNAL(progress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));

    n=new neteasePlaylist();
    connect(n,SIGNAL(status(QString)),this,SLOT(status(QString)));
    connect(n,SIGNAL(finished(int,QStringList)),q,SLOT(beginToDownload(int,QStringList)));

    vkeyinit=new vkeyInit();
    connect(vkeyinit,SIGNAL(status(QString)),this,SLOT(status(QString)));
    connect(vkeyinit,SIGNAL(finished()),this,SLOT(initFinished()));

    _vkeyinit2=new vkeyinit2();
    connect(_vkeyinit2,SIGNAL(status(QString)),this,SLOT(status(QString)));
    connect(_vkeyinit2,SIGNAL(finished()),this,SLOT(init2Finished()));

    //ui->radioButtonQualityape->setEnabled(false);
    //ui->radioButtonQualityflac->setEnabled(false);

    ui->pushButtonDownload->setEnabled(false);
    //vkeyinit->init();
    _vkeyinit2->init();



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initFinished()
{
    _vkeyinit2->init();
    ui->pushButtonDownload->setEnabled(true);
}

void MainWindow::init2Finished()
{
    //ui->radioButtonQualityape->setEnabled(true);
    //ui->radioButtonQualityflac->setEnabled(true);
    ui->pushButtonDownload->setEnabled(true);
}

void MainWindow::status(QString inString)
{
    ui->textEdit->append(inString);
}

void MainWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    ui->progressBar->setMaximum(bytesTotal);
    ui->progressBar->setValue(bytesReceived);
}

void MainWindow::qqmusicFinished()
{
    ui->pushButtonDownload->setEnabled(true);
    ui->pushButtonGray->setEnabled(true);
}

void MainWindow::on_pushButtonDownload_clicked()
{
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    mp3Dir=ui->lineEditMp3Path->text();
    ui->textEdit->clear();
    if(!songNameType || !songQuality || ui->lineEditMp3Path->text().isEmpty() || ui->lineEditLink->text().isEmpty())
    {
        ui->textEdit->append("请输入合法的设置");
    }
    else{
        if(ui->lineEditLink->text().contains("y.qq.com"))
        {
            ui->pushButtonDownload->setEnabled(false);
            ui->pushButtonGray->setEnabled(false);
            q->doJob(ui->lineEditLink->text());
        }
        else{
            if(ui->lineEditLink->text().contains("music.163.com"))
            {
                if(ui->lineEditLink->text().contains("playlist"))
                {
                    ui->textEdit->append("下载列表所有歌曲信息，此处需要耐心等待片刻,曲目数越多，等待时间越长");
                    ui->pushButtonDownload->setEnabled(false);
                    ui->pushButtonGray->setEnabled(false);
                    n->doJob(0,ui->lineEditLink->text());
                }
                else{
                    ui->textEdit->append("网易云音乐仅支持歌单下载，单曲以及专辑下载请使用qq音乐链接");
                }
            }
        }

    }
}

void MainWindow::on_radioButtonSongNameType1_clicked()
{
    songNameType=1;
}

void MainWindow::on_radioButtonSongNameType2_clicked()
{
    songNameType=2;
}

void MainWindow::on_radioButtonQuality128_clicked()
{
    songQuality=1;
}

void MainWindow::on_radioButtonQuality320_clicked()
{
    songQuality=2;
}

void MainWindow::on_radioButtonQualityflac_clicked()
{
    songQuality=3;
}

void MainWindow::on_radioButtonQualityape_clicked()
{
    songQuality=4;
}

void MainWindow::on_pushButtonChosePath_clicked()
{
    mp3Dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEditMp3Path->setText(mp3Dir);
}

void MainWindow::on_pushButtonGray_clicked()
{
    mp3Dir=ui->lineEditMp3Path->text();
    ui->textEdit->clear();
    if(ui->lineEditLink->text().contains("music.163.com"))
    {
        ui->textEdit->append("下载列表所有歌曲信息，此处需要耐心等待片刻,曲目数越多，等待时间越长");
        ui->pushButtonDownload->setEnabled(false);
        ui->pushButtonGray->setEnabled(false);
        n->doJob(1,ui->lineEditLink->text());
    }
}

void MainWindow::on_action_triggered()
{
    h->show();
}

void MainWindow::on_action_2_triggered()
{
    t->show();
}

void MainWindow::on_action_3_triggered()
{
    p->show();
}

void MainWindow::on_pushButton_clicked()
{
    qDebug()<<mp3Dir;
    QDesktopServices::openUrl(QUrl::fromLocalFile(mp3Dir));
}
