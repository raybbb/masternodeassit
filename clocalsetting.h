#ifndef CLOCALSETTING_H
#define CLOCALSETTING_H
#include <QString>
#include <map>
using namespace std;

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
    QString local_script_path;
    QString remote_script_path;
    QString install_script;
    QString start_script;
    QString restart_script;
    QString stop_script;

    map<QString, QString> mn_old_info;
    map<QString, QString> mn_new_info;
};


extern CLocalSetting local_setting;

#endif // CLOCALSETTING_H
