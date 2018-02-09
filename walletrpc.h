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

    void getinfo();
    void masternodeGenkey();
    void getaccountaddress(QString label);

public slots:
    void httpFinished();
    void httpReadyRead();

private:
    QUrl hostUrl;
    QByteArray Authorization;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    void userInfo2AuthInfo(QString userName, QString passWord);
    void execCmd(QByteArray array);
};

#endif // WALLETRPC_H
