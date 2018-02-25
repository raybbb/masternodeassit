#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QList>
#include <QMap>

class Database
{
public:
    Database();
    QSqlDatabase sqldb;
    bool addMn(QString ip, const QByteArray &seqStr);
    QMap<QString, QByteArray> queryData();
    QByteArray queryData(const QString &qsIp);

    bool updateMn(const QString &qsIp, const QByteArray &seqData);
    bool delMn(QString ip);
    bool dropMnTable();
    bool createMnTable();
};

#endif // DATABASE_H
