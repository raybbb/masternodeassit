#ifndef CSTARTMASTERNODE_H
#define CSTARTMASTERNODE_H

#include <QThread>

class CStartMasternode :public QThread
{
    Q_OBJECT
public:
    explicit CStartMasternode(QThread *parent = nullptr);
   void run();
   void set(const QString& strIp, const QString& strUser,
            const QString& strPwd);
private:
    QString m_strIp;
    QString m_strUser;
    QString m_strPwd;

signals:
    void sigMasternodeFinishStart();
};

#endif // CSTARTMASTERNODE_H
