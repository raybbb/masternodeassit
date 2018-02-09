#include "walletrpc.h"

WalletRPC::WalletRPC(QString hostName, QString rpcUser, QString rpcPassword, QObject *parent)
    : QObject(parent)
{
    hostUrl = "http://" + hostName + ":" + "5554";
    userInfo2AuthInfo(rpcUser ,rpcPassword);
}

void WalletRPC::httpFinished()
{
    return;
}

void WalletRPC::httpReadyRead()
{
    qDebug() << reply->readAll();
}

void WalletRPC::userInfo2AuthInfo(QString userName, QString passWord)
{
    //Authorization = "Basic YW5ndXM6YW5ndXM=";
    QString preAuth = "Basic ";
    //QString user_and_password = "user:password";
    QString user_and_password = userName + ":" + passWord;
    QByteArray  tmp(user_and_password.toUtf8());
    QString base64NameAndPassword(tmp.toBase64());
    Authorization = (preAuth + base64NameAndPassword).toUtf8();
    //qDebug()<< Authorization;
}

void WalletRPC::execCmd(QByteArray array)
{
    QNetworkRequest request(hostUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/plain"));
    request.setRawHeader("Authorization",Authorization);

    reply = qnam.post(request,array);

    connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
    return;
}

void WalletRPC::getinfo()
{
    qDebug() << "getinfo";
    QByteArray array("{\"jsonrpc\":\"1.0\", \"method\":\"getinfo\", \"params\": []}");
    execCmd(array);
}

void WalletRPC::masternodeGenkey()
{
    qDebug() << "masternodeGenkey";
    QByteArray array("{\"jsonrpc\":\"1.0\", \"method\":\"masternode\", \"params\": [\"genkey\"]}");
    execCmd(array);
}

void WalletRPC::getaccountaddress(QString label)
{
    QString cmdInfo = "{\"jsonrpc\":\"1.0\", \"method\":\"getaccountaddress\", \"params\": [\"" + label + "\"]}";
    QByteArray array(cmdInfo.toUtf8());
    execCmd(array);
}


