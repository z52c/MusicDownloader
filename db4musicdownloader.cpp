#include "db4musicdownloader.h"

db4MusicDownloader::db4MusicDownloader(QObject *parent) : QObject(parent)
{
    db=new QSqlDatabase();
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        *db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        *db = QSqlDatabase::addDatabase("QSQLITE");
        db->setDatabaseName("daye.db");
    }
    if (!db->open())
    {
        qDebug() << "Error: Failed to connect database." << db->lastError();
    }
    else
    {
        _createTable();
    }
}

bool db4MusicDownloader::_createTable()
{
    QSqlQuery sql_query;
    QString create_sql = "create table downloadQueue (id INTEGER PRIMARY KEY AUTOINCREMENT, flag INTEGER, quality INTEGER,nameType INTEGER,dir varchar(255),url varchar(255))";
    sql_query.prepare(create_sql);
    if(!sql_query.exec())
    {
        qDebug() << "Error: Fail to create table." << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "Table downloadQueue created!";
        return true;
    }
}

bool db4MusicDownloader::deleteFromDownloadQueue(QString inUrl)
{
    QSqlQuery sql_query;
    QString create_sql =QString( "DELETE FROM downloadQueue where url=\'%1\'").arg(inUrl);
    sql_query.prepare(create_sql);
    if(!sql_query.exec())
    {
        qDebug() << "Error while delete:" << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "delete sucess!";
        return true;
    }
}

bool db4MusicDownloader::insertDownloadQueue(int inFlag, int inQuality, int inNameType,QString inDir, QString inUrl)
{
    QSqlQuery sql_query;
    QString insert_sql=QString("INSERT INTO downloadQueue VALUES(NULL,%1,%2,%3,\'%4\',\'%5\')").arg(inFlag).arg(inQuality).arg(inNameType).arg(inDir).arg(inUrl);
    if(!sql_query.exec(insert_sql))
    {
        qDebug()<<insert_sql;
        qDebug() << sql_query.lastError();
        return false;
    }
    else{
        return true;
    }
}

QList<struct downloadQueueInfo> db4MusicDownloader::getQueue()
{
    struct downloadQueueInfo tmp;
    QList<struct downloadQueueInfo> rtn;
    QSqlQuery sql_query;
    QString select_sql ="select * from downloadQueue";
    if(!sql_query.exec(select_sql))
    {
        qDebug()<<sql_query.lastError();
        return rtn;
    }
    else
    {
        while(sql_query.next())
        {
            tmp.flag=sql_query.value(1).toInt();
            tmp.quality=sql_query.value(2).toInt();
            tmp.nameType=sql_query.value(3).toInt();
            tmp.dir=sql_query.value(4).toString();
            tmp.url=sql_query.value(5).toString();
            rtn.append(tmp);
        }
    }
        return rtn;
}
