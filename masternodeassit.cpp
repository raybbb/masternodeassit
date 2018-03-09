#include "masternodeassit.h"
#include "ui_masternodeassit.h"
#include "mnlibssh2.h"
#include "configdialog.h"
#include "uimnmain.h"

masternodeassit::masternodeassit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::masternodeassit)
{
    ui->setupUi(this);
    initDatabase();

    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(0);

    ui->tableWidget->verticalHeader()->setVisible(false);   //隐藏表头
    ui->tableWidget->horizontalHeader()->setVisible(false); //隐藏表头

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("标签"));
    ui->tableWidget->setItem(0,1,new QTableWidgetItem("IP"));
    ui->tableWidget->setItem(0,2,new QTableWidgetItem("端口"));
    ui->tableWidget->setItem(0,3,new QTableWidgetItem("用户名"));
    ui->tableWidget->setItem(0,4,new QTableWidgetItem("密码"));
    ui->tableWidget->setItem(0,5,new QTableWidgetItem("交易信息"));
    ui->tableWidget->setItem(0,6,new QTableWidgetItem("状态"));
    getRemoteServers();
}

masternodeassit::~masternodeassit()
{
    delete ui;
}

void masternodeassit::initDatabase(){

    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("./masternode.db");

        if (!database.open())
        {
            qDebug() << "Error: Failed to connect database." << database.lastError();
        }
        else
        {
            qDebug() << "Database Opened!";
        }
    }
    return;
}

void masternodeassit::getRemoteServers()
{
    //清除已有数据
    int rows = ui->tableWidget->rowCount();
    for(int i=0; i<rows; i++)
    {
        qDebug() << QString("i=%1").arg(i);
        ui->tableWidget->removeRow(0);
    }

    //重新获取最新列表
    int cols = ui->tableWidget->columnCount();
    rows = 0;
    QString create_sql = "select * from RemoteServers";
    QSqlQuery sql_query(database);
    if(!sql_query.exec(create_sql))
    {
        qDebug() << "Error: Fail to select table." << sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            //CREATE TABLE RemoteServers(Lable text PRIMARY KEY, IP text, Port text, Username text, UserPasswd text, TransID text, Status text);
            //测试获取数据正确性
            QString Lable = sql_query.value(0).toString();
            QString IP = sql_query.value(1).toString();
            QString Port = sql_query.value(2).toString();
            QString Username = sql_query.value(3).toString();
            QString UserPasswd = sql_query.value(4).toString();
            QString TransID = sql_query.value(5).toString();
            QString Status = sql_query.value(6).toString();
            qDebug()<<QString("Lable:%1  IP:%2  Port:%3  Username:%4  UserPasswd:%5  TransID:%6  Status:%7")
                      .arg(Lable).arg(IP).arg(Port).arg(Username).arg(UserPasswd).arg(TransID).arg(Status);

            //逐行加入列表中显示
            rows = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rows);
            for(int i=0;i<cols;i++)
            {
                ui->tableWidget->setItem(rows,i,new QTableWidgetItem(sql_query.value(i).toString()));
            }
        }
    }
    return;
}

//void masternodeassit::addRemoteServers(QString Lable, QString IP, QString Port, QString Username, QString UserPasswd, QString TransID, QString Status){
void masternodeassit::addRemoteServers(){

    QString Lable, IP, Port, Username, UserPasswd, TransID, Status;

    int rows = ui->tableWidget->rowCount();
    Lable = "mn" + QString("%1").arg(rows+1);
    IP = "127.0.0.1";
    Port = "5555";
    Username = "root";
    UserPasswd = "passwd";
    TransID = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    Status = "Off";

    //拼装插入数据sql语句
    QString insert_sql = "insert into RemoteServers values (?, ?, ?, ?, ?, ?, ?)";
    QSqlQuery sql_query(database);
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(Lable);
    sql_query.addBindValue(IP);
    sql_query.addBindValue(Port);
    sql_query.addBindValue(Username);
    sql_query.addBindValue(UserPasswd);
    sql_query.addBindValue(TransID);
    sql_query.addBindValue(Status);

    //执行插入语句
    if(!sql_query.exec())
    {
        qDebug() << "Error: Fail to insert table." << sql_query.lastError();
    }
    else
    {
        qDebug() << "Table insert succ!";
        //刷新列表
        getRemoteServers();
    }
    return;
}


void masternodeassit::on_pb_add_clicked()
{
    ConfigDialog dialog;
    dialog.exec();
}

void masternodeassit::on_pb_remove_clicked()
{
    UIMnMain mn;
    mn.show();
}
