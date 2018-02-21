#ifndef ID3TAG_H
#define ID3TAG_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDataStream>

typedef struct ID3HEADER{
    char identifier[3];
    char version[2];
    char flags;
    char size[4];
}id3Header;

typedef struct FRAMEHEADER{
    char frameID[4];
    char size[4];
    char flags[2];
}frameHeader;

class ID3tag : public QObject
{
    Q_OBJECT
private:
    id3Header tagHeaderOld;
    QString mp3FileName;
    QString mp3FileNameTmp;

    id3Header tagHeader;
    frameHeader titleHeader;
    frameHeader artistHeader;
    frameHeader albumHeader;
    frameHeader picHeader;

    int titleSize;
    int artistSize;
    int albumSize;
    int picSize;

    QString title;
    QString artist;
    QString album;
    QByteArray image;
    QByteArray mp3File;
    char mimeInfo[14];

    QFile *fileN;  //新的的文件
    QFile *fileS;  //原来的文件
    QFile *fileImage;
    QDataStream *dStreamS;
    QDataStream *dStream;
//    QTextStream *tStream;

public:
    explicit ID3tag(QObject *parent = nullptr);
    ID3tag(QString fileName);
    ~ID3tag();
    void setTitle(QString t);
    void setArtist(QString a);
    void setAlbum(QString a);
    void setPic(QString imageFileName);
    void doJob();

signals:
    processState(QString );
    finished();
public slots:
};

#endif // ID3TAG_H
