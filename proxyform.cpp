#include "proxyform.h"
#include "ui_proxyform.h"
#include <QFileInfo>
#include "daye.h"
#include <QTextStream>
proxyForm::proxyForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::proxyForm)
{
    ui->setupUi(this);
    QFileInfo a("proxy.conf");
    if(a.isFile())
    {
        QFile fileProxy("proxy.conf");
        QString line;
        char tmp[5000];
        if(fileProxy.open(QIODevice::ReadOnly))
        {
             line=fileProxy.readLine();
             fileProxy.close();
        }
        if(!line.isEmpty())
        {
             getStringBetweenAandB(line.toStdString().c_str(),"socks5Addr:\"","\"",tmp);
             ui->lineEditAddr->setText(tmp);
             getStringBetweenAandB(line.toStdString().c_str(),"socks5Port:\"","\"",tmp);
             ui->lineEditPort->setText(tmp);
             getStringBetweenAandB(line.toStdString().c_str(),"socks5User:\"","\"",tmp);
             ui->lineEditUser->setText(tmp);
             getStringBetweenAandB(line.toStdString().c_str(),"socks5Pass:\"","\"",tmp);
             ui->lineEditPass->setText(tmp);
        }
        fileProxy.close();
    }
}

proxyForm::~proxyForm()
{
    delete ui;
}

void proxyForm::on_pushButton_2_clicked()
{
    this->close();
}

void proxyForm::on_pushButton_clicked()
{
    QFile fileProxy("proxy.conf");
    QString writeData;
    writeData=QString("\"socks5Addr:\"")+ui->lineEditAddr->text()+QString("\",")+QString("\"socks5Port:\"")+ui->lineEditPort->text()+QString("\",")+QString("\"socks5User:\"")+ui->lineEditUser->text()+QString("\",")+QString("\"socks5Pass:\"")+ui->lineEditPass->text()+QString("\"");
    if(fileProxy.open(QIODevice::WriteOnly))
    {
        QTextStream in(&fileProxy);
        in<<writeData;
        fileProxy.close();
    }
    this->close();
}
