#ifndef CMASTERNODE_H
#define CMASTERNODE_H
#include <QString>
#include <QDataStream>

class CMasternode
{
public:
    //CMasternode& operator =(const CMasternode &other);
    friend QDataStream& operator >>(QDataStream& in, CMasternode& data);
    friend QDataStream& operator <<(QDataStream& out, const CMasternode& data);
    static CMasternode DeSerializable(const QByteArray &datagram);
    static QByteArray Serializable(const CMasternode &cmn);

public:
    int m_num;
    QString m_alias;
    QString m_ip;
    int m_port;
    QString m_status;
    QString m_user;
    QString m_pwd;
    QString m_remote_rpc_user;
    QString m_remote_rpc_pwd;
    QString m_remote_rpc_ip;
    QString m_local_rpc_user;
    QString m_local_rpc_pwd;
    QString m_local_rpc_ip;
    QString m_mn_key;
    QString m_clltrl_hash;
    int m_index;
    QString m_safe_conf_path;
    QString m_mn_conf_path;
    QString m_mn_step;

};

#endif // CMASTERNODE_H
