#include "mainwindow.h"
#include <QApplication>
#include <QNetworkProxy>
#include <QFile>
#include <QFileInfo>

int songNameType;
int songQuality;
QString mp3FileName;
QString mp3Dir;
QString vkey;
QString guid;
QString vkey2;

int main(int argc, char *argv[])
{
    QString proxyAddr;
    QString proxyPort;
    QString proxyUser;
    QString proxyPass;
    QNetworkProxy proxy;
    QFileInfo b("proxy.conf");
    if(b.isFile())
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
             proxyAddr=QString(tmp);
             getStringBetweenAandB(line.toStdString().c_str(),"socks5Port:\"","\"",tmp);
             proxyPort=QString(tmp);
             getStringBetweenAandB(line.toStdString().c_str(),"socks5User:\"","\"",tmp);
             proxyUser=QString(tmp);
             getStringBetweenAandB(line.toStdString().c_str(),"socks5Pass:\"","\"",tmp);
             proxyPass=QString(tmp);
             if(proxyAddr!=QString(""))
             {
                 proxy.setType(QNetworkProxy::Socks5Proxy);
                 proxy.setHostName(proxyAddr);
                 proxy.setPort(proxyPort.toInt());
                 if(proxyUser!=QString(""))
                 {
                     proxy.setUser(proxyUser);
                     proxy.setPassword(proxyPass);
                 }
                 QNetworkProxy::setApplicationProxy(proxy);
             }
        }
        fileProxy.close();
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
