#include "mainwindow.h"
#include <QApplication>

int songNameType;
int songQuality;
QString mp3FileName;
QString mp3Dir;
QString vkey;
QString guid;
QString vkey2;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
