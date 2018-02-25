#include "database.h"

Database::Database()
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        sqldb = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        sqldb = QSqlDatabase::addDatabase("QSQLITE");
        sqldb.setDatabaseName("./masternode.db");

        if (!sqldb.open())
        {
            qDebug() << "Error: Failed to connect database." << sqldb.lastError();
        }
        else
        {
            qDebug() << "Database Opened!";
        }
    }
}

bool Database::createMnTable()
{
    QSqlQuery sql_query(sqldb);
    QString createTable = "CREATE TABLE t_master_node" \
                          "(ip TEXT NOT NULL PRIMARY KEY, seqdata BLOB NOT NULL)";
    if(!sql_query.exec(createTable))
    {
        qDebug() << "Error: Fail to create table." << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "Table Create succ!";
    }
    return true;
}

bool Database::addMn(QString ip, const QByteArray &seqStr)
{
    QSqlQuery sql_query(sqldb);
    QString insert_sql = "insert into t_master_node (ip, seqdata) values (:ip,:seqdata)";
    sql_query.prepare(insert_sql);
    sql_query.bindValue(":ip",ip);
    sql_query.bindValue(":seqdata", seqStr, QSql::Binary);

    if(!sql_query.exec())
    {
        qDebug() << "Error: Fail to insert table." << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "Table insert succ!";
    }
    return true;
}

bool Database::delMn(QString ip)
{
    QSqlQuery sql_query(sqldb);
    QString deleteMn = "DELETE FROM t_master_node WHERE ip=\"" + ip + "\"";
    qDebug() << deleteMn;

    if(!sql_query.exec(deleteMn))
    {
        qDebug() << "Error: Fail to delete from table." << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "Table delete succ!";
    }
    return true;
}

QMap<QString, QByteArray> Database::queryData()
{
    QMap<QString, QByteArray> dbMap;
    QSqlQuery query(sqldb);
    if (query.exec("SELECT * from t_master_node"))
    {
        while (query.next())
        {
            QString strDt;
            QByteArray data;
            strDt = query.value(1).toString();
            data = query.value(2).toByteArray();
            dbMap[strDt] = data;
        }
    }
    else
    {
        // @todo log...
        qDebug() << "QueryData error!";
    }
    return dbMap;
}


QByteArray Database::queryData(const QString &qsIp)
{
    QSqlQuery query(sqldb);
    QString strDt;
    QByteArray data;

    if (query.exec("SELECT * from t_master_node where ip=\"" + qsIp + "\""))
    {
        while (query.next())
        {
            strDt = query.value(1).toString();
            data = query.value(2).toByteArray();
        }
    }
    return data;
}

bool Database::updateMn(const QString &qsIp, const QByteArray &seqData)
{

    QSqlQuery query(sqldb);
    QString strSql = "update t_master_node set seqdata = :value where ip = :ip;";
    query.prepare(strSql);

    query.bindValue(":value", seqData, QSql::Binary);
    query.bindValue(":ip", qsIp);

    if(!query.exec())
    {
        qDebug() << "Error: Fail to update colum." << query.lastError();
        return false;
    }
    else
    {
        qDebug() << "update colum success.";
    }

    return true;
}

bool Database::dropMnTable()
{
    QSqlQuery sql_query(sqldb);
    QString deleteTable = "drop table t_master_node";
    if(!sql_query.exec(deleteTable))
    {
        qDebug() << "Error: Fail to delete table." << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "Table drop succ!";
    }
    return true;
}






