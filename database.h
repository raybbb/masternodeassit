#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QList>

class QMnInfo
{
public:
    int num;
    QString alias;
    QString ip;
    int port;
    QString status;
    QString user;
    QString pwd;
    QString rpc_user;
    QString rpc_pwd;
    QString rpc_ip;
    QString mn_key;
    QString clltrl_hash;
    int index;
    QString safe_conf_path;
    QString mn_conf_path;
    QString mn_step;
};

class Database
{
public:
    Database();
    QSqlDatabase sqldb;
    void getMnList(QList<QMnInfo> &qmnList);
    bool addMn(int num, QString alias, QString ip, int port,
                         QString status, QString user, QString pwd,
                         QString rpc_user, QString rpc_pwd, QString rpc_ip,
                         QString mn_key, QString clltrl_hash, int index,
                         QString safe_conf_path, QString mn_conf_path, QString mn_step);

    bool delMn(QString ip);
    bool dropMnTable();
    bool createMnTable();

};

#endif // DATABASE_H
