#include "cstartmasternode.h"
#include "mnlibssh2.h"
#include "clocalsetting.h"
#include <QDebug>

CStartMasternode::CStartMasternode( QThread *parent)
    : QThread(parent)
{

}

void CStartMasternode::set(const QString& strIp, const QString& strUser,
                           const QString& strPwd)
{
    m_strIp = strIp;
    m_strUser = strUser;
    m_strPwd = strPwd;
}

void CStartMasternode::run()
{
    qDebug()<< "Start Script...";

    mn_libssh2 ssh2;
    ssh2.mn_init();

    int nRc = ssh2.mn_login(m_strIp.toStdString().c_str(),
                  m_strUser.toStdString().c_str(),
                  m_strPwd.toStdString().c_str());
    if (!nRc)
    {
        ssh2.mn_exec(QString("python" + QString(" ")
                     + local_setting.remote_script_path
                     + local_setting.install_script).toStdString().c_str());
        emit sigMasternodeFinishStart();
    }
}
