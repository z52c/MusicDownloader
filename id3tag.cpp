#include "id3tag.h"
#include <QDebug>

ID3tag::ID3tag(QObject *parent) : QObject(parent)
{

}

ID3tag::~ID3tag()
{
    delete fileN;
    delete fileS;
    delete fileImage;
    delete dStreamS;
    delete dStream;
//    delete tStream;
}

ID3tag::ID3tag(QString fileName)
{
    mp3FileName=fileName;
    mp3FileNameTmp=fileName+QString(".tmp");
    fileS=new QFile(mp3FileName);
    fileS->open(QIODevice::ReadOnly);
    fileN=new QFile(mp3FileNameTmp);
    fileN->open(QIODevice::WriteOnly);
    dStream=new QDataStream(fileN);
    memset(&tagHeader,0,sizeof(tagHeader));
    memset(&titleHeader,0,sizeof(titleHeader));
    memset(&artistHeader,0,sizeof(artistHeader));
    memset(&albumHeader,0,sizeof(albumHeader));
    memset(&picHeader,0,sizeof(picHeader));
    tagHeader.identifier[0]='I';
    tagHeader.identifier[1]='D';
    tagHeader.identifier[2]='3';
    tagHeader.version[0]=0x03;
    titleHeader.frameID[0]='T';titleHeader.frameID[1]='I';titleHeader.frameID[2]='T';titleHeader.frameID[3]='2';
    artistHeader.frameID[0]='T';artistHeader.frameID[1]='P';artistHeader.frameID[2]='E';artistHeader.frameID[3]='1';
    albumHeader.frameID[0]='T';albumHeader.frameID[1]='A';albumHeader.frameID[2]='L';albumHeader.frameID[3]='B';
    picHeader.frameID[0]='A';picHeader.frameID[1]='P';picHeader.frameID[2]='I';picHeader.frameID[3]='C';

    //获取mp3音频内容
    dStreamS =new QDataStream(fileS);
    dStreamS->readRawData((char*)&tagHeaderOld,10);
    int oldHeaderSize=128*128*128*tagHeaderOld.size[0]+128*128*tagHeaderOld.size[1]+128*tagHeaderOld.size[2]+tagHeaderOld.size[3];
    fileS->seek(10+oldHeaderSize);
    mp3File=fileS->readAll();
}

void ID3tag::setTitle(QString t)
{
    title=t;
    titleSize=2*title.size()+3;
    titleHeader.size[3]=titleSize;
}

void ID3tag::setArtist(QString a)
{
    artist=a;
    artistSize=2*a.size()+3;
    artistHeader.size[3]=artistSize;
}

void ID3tag::setAlbum(QString a)
{
    album=a;
    albumSize=2*album.size()+3;
    albumHeader.size[3]=albumSize;
}

void ID3tag::setPic(QString imageFileName)
{
    fileImage=new QFile(imageFileName);
    fileImage->open(QIODevice::ReadOnly);
    image=fileImage->readAll();
    memset(mimeInfo,0,14);
    char tmp[14]={0x00,0x69,0x6D,0x61,0x67,0x65,0x2F,0x6A,0x70,0x65,0x67,0x00,0x03,0x00};
    memcpy(mimeInfo,tmp,14);
    picSize=image.size()+14;
    picHeader.size[0]=picSize/(256*256*256);
    picHeader.size[1]=picSize%(256*256*256)/(256*256);
    picHeader.size[2]=picSize%(256*256*256)%(256*256)/256;
    picHeader.size[3]=picSize%(256*256*256)%(256*256)%256;
}

void ID3tag::doJob()
{
    qDebug()<<"title QString:"<<title;
    qDebug()<<title.size();
    qDebug()<<sizeof(title);
    for(int i=0;i<2*title.size();i++)
    {
        qDebug()<<title.data()[i];
    }
    qDebug()<<"artist QString:"<<artist;
    qDebug()<<artist.size();
    qDebug()<<sizeof(artist);
    for(int i=0;i<2*artist.size();i++)
    {
        qDebug()<<artist.data()[i];
    }
    uchar unicodeBom[3]={0x01,0xFF,0xFE};
    int size=40+titleSize+artistSize+albumSize+picSize;
    tagHeader.size[0]=size/(128*128*128);
    tagHeader.size[1]=size%(128*128*128)/(128*128);
    tagHeader.size[2]=size%(128*128*128)%(128*128)/128;
    tagHeader.size[3]=size%(128*128*128)%(128*128)%128;
    dStream->writeRawData((char*)&tagHeader,10);
    dStream->writeRawData((char*)&titleHeader,10);
    dStream->writeRawData((char*)unicodeBom,3);
    dStream->writeRawData((char*)title.data(),title.size()*2);
    dStream->writeRawData((char*)&artistHeader,10);
    dStream->writeRawData((char*)unicodeBom,3);
    dStream->writeRawData((char*)artist.data(),artist.size()*2);
    dStream->writeRawData((char*)&albumHeader,10);
    dStream->writeRawData((char*)unicodeBom,3);
    dStream->writeRawData((char*)album.data(),album.size()*2);
    dStream->writeRawData((char*)&picHeader,10);
    dStream->writeRawData(mimeInfo,14);
    dStream->writeRawData((char*)image.data(),image.size());
    dStream->writeRawData((char*)mp3File.data(),mp3File.size());
    fileImage->close();
    fileN->close();
    fileS->close();
    fileS->remove();
    fileN->rename(mp3FileName);
    emit finished();
}
