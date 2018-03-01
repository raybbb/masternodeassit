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
    bool addMn(QString qsKey, const QByteArray &seqStr);
    QMap<QString, QByteArray> queryData();
    QByteArray queryData(const QString &qsKey);

    bool updateMn(const QString &qsKey, const QByteArray &seqData);
    bool delMn(QString qsKey);
    bool dropMnTable();
    bool createMnTable();
};

#endif // DATABASE_H
