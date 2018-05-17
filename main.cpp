#include "qqmusic.h"
#include <QApplication>

int songNameType;
int songQuality;
QString mp3FileName;
QString mp3Dir;
QString vkey;
QString guid;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qqmusic w;
    w.show();

    return a.exec();
}
