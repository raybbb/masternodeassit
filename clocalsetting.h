#ifndef CLOCALSETTING_H
#define CLOCALSETTING_H
#include <QString>

class CLocalSetting
{
public:
    QString safe_conf_path;
    QString safe_conf_file;
    QString new_safe_conf_files_path;
    QString masternode_conf_path;
    QString masternode_conf_file;
    QString local_rpc_user;
    QString local_rpc_pwd;
    QString remote_rpc_user;
    QString remote_rpc_pwd;
    QString remote_rpc_ip;
    QString script_path;
};


extern CLocalSetting local_setting;

#endif // CLOCALSETTING_H
