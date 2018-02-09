#ifndef CMASTERNODE_H
#define CMASTERNODE_H
#include <QString>

class CMasternode
{
public:
    int m_num;
    QString m_alias;
    QString m_ip;
    int m_port;
    QString m_status;
    QString m_user;
    QString m_pwd;
    QString m_rpc_user;
    QString m_rpc_pwd;
    QString m_rpc_ip;
    QString m_mn_key;
    QString m_clltrl_hash;
    int m_index;
    QString m_safe_conf_path;
    QString m_mn_conf_path;
    QString m_mn_step;
};

#endif // CMASTERNODE_H
