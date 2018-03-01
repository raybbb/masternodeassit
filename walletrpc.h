#ifndef WALLETRPC_H
#define WALLETRPC_H

#include <QObject>
#include <QtNetwork>
#include <QByteArray>
#include <QString>

class WalletRPC : public QObject
{
    Q_OBJECT
public:
    WalletRPC(QString hostName, QString rpcUser, QString rpcPassword, QObject *parent = 0);

    QJsonObject getinfo();
    QString masternodeGenkey();
    QString getblockcount();
    QString getaccountaddress(QString label);
    //void masternodeStop();
    QJsonObject masternodeStatus();
    QJsonObject masternodeOutputs();

public slots:
    QJsonValue parseResponse(QByteArray &responseData);

private:
    QUrl hostUrl;
    QByteArray Authorization;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    void userInfo2AuthInfo(QString userName, QString passWord);
    QJsonValue execCmd(QByteArray array);
};

#endif // WALLETRPC_H
