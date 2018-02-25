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

/*
CREATE TABLE t_master_node(
    num INT,
    alias TEXT,
    ip TEXT  PRIMARY KEY,
    port INT,
    status TEXT,
    user TEXT,
    pwd TEXT,
    rpc_user TEXT,
     rpc_pwd TEXT,
    rpc_ip TEXT,
    mn_key TEXT,
    clltrl_hash TEXT,
    index INT,
    safe_conf_path TEXT,
    mn_conf_path TEXT,
    mn_step TEXT
);
*/


void Database::getMnList(QList<QMnInfo> &qmnList)
{
    QString create_sql = "select * from t_master_node";
    QSqlQuery sql_query(sqldb);
    if(!sql_query.exec(create_sql))
    {
        qDebug() << "Error: Fail to select table." << sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            QMnInfo qmninfo;
            qmninfo.num = sql_query.value(0).toInt();
            qmninfo.alias = sql_query.value(1).toString();
            qmninfo.ip = sql_query.value(2).toString();
            qmninfo.port = sql_query.value(3).toInt();
            qmninfo.status = sql_query.value(4).toString();
            qmninfo.user = sql_query.value(5).toString();
            qmninfo.pwd = sql_query.value(6).toString();
            qmninfo.rpc_user = sql_query.value(7).toString();
            qmninfo.rpc_pwd = sql_query.value(8).toString();
            qmninfo.rpc_ip = sql_query.value(9).toString();
            qmninfo.mn_key = sql_query.value(10).toString();
            qmninfo.clltrl_hash = sql_query.value(11).toString();
            qmninfo.index = sql_query.value(12).toInt();
            qmninfo.safe_conf_path = sql_query.value(13).toString();
            qmninfo.mn_conf_path = sql_query.value(14).toString();
            qmninfo.mn_step = sql_query.value(15).toString();
            qmnList.append(qmninfo);
            qDebug()<<QString("num:%1, alias:%2, ip:%3, port:%4, status:%5, user:%6, pwd:%7, rpc_user:%8, rpc_pwd:%9, rpc_ip:%10, mn_key:%11, clltrl_hash:%12, index:%13, safe_conf_path:%14, mn_conf_path:%15, mn_step:%16")
                      .arg(sql_query.value(0).toString()).arg(sql_query.value(1).toString()).arg(sql_query.value(2).toString()).arg(sql_query.value(3).toString())
                      .arg(sql_query.value(4).toString()).arg(sql_query.value(5).toString()).arg(sql_query.value(6).toString()).arg(sql_query.value(7).toString())
                      .arg(sql_query.value(8).toString()).arg(sql_query.value(9).toString()).arg(sql_query.value(10).toString()).arg(sql_query.value(11).toString())
                      .arg(sql_query.value(12).toString()).arg(sql_query.value(13).toString()).arg(sql_query.value(14).toString()).arg(sql_query.value(15).toString());
        }
    }
    return;
}

bool Database::addMn(int num, QString alias, QString ip, int port,
                     QString status, QString user, QString pwd,
                     QString rpc_user, QString rpc_pwd, QString rpc_ip,
                     QString mn_key, QString clltrl_hash, int index,
                     QString safe_conf_path, QString mn_conf_path, QString mn_step)
{

    QSqlQuery sql_query(sqldb);

    QString insert_sql = "insert into t_master_node values (" + QString::number(num, 10) + ",\"" +  alias + "\",\"" + ip + "\"," + QString::number(port, 10) + ",\"" + status + "\",\"" + user + "\",\"" + pwd + "\",\""
            + rpc_user + "\",\"" + rpc_pwd + "\",\"" + rpc_ip + "\",\"" + mn_key + "\",\"" + clltrl_hash + "\",\"" + QString::number(index, 10) + "\",\"" + safe_conf_path + "\",\"" + mn_conf_path + "\",\"" + mn_step + "\")";

    qDebug() << insert_sql;

    if(!sql_query.exec(insert_sql))
    {
        qDebug() << "Error: Fail to insert table." << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "Table insert succ!";
        //getMnList();
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

bool Database::createMnTable()
{
    QSqlQuery sql_query(sqldb);
    QString createTable = "CREATE TABLE t_master_node(num INT, alias TEXT, ip TEXT  PRIMARY KEY, port INT, status TEXT, user TEXT, pwd TEXT, rpc_user TEXT, rpc_pwd TEXT, rpc_ip TEXT, mn_key TEXT, clltrl_hash TEXT, hx_index INT, safe_conf_path TEXT, mn_conf_path TEXT, mn_step TEXT)";
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
