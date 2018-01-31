#include "mn_puttyscp.h"

#ifdef WIN32
#include <winsock2.h>
#endif

mn_puttyscp::mn_puttyscp(QObject *parent) : QObject(parent),m_mylib("pscp.dll")
{
    if(m_mylib.load())
    {
        try
        {
            scp_func = (pscp)m_mylib.resolve("scp");
        }
        catch(...)
        {

        }
    }
    else
    {
    }
}

mn_puttyscp::~mn_puttyscp()
{
    m_mylib.unload();
}

int mn_puttyscp::init(const QString &qsIp, const QString &qsUser, const QString &qsPwd)
{
    m_qsIp = qsIp;
    m_qsUser= qsUser;
    m_qsPwd= qsPwd;

    WSADATA wsadata;
    int err = 0;
    err = WSAStartup(MAKEWORD(2,0), &wsadata);
    if (err != 0) {
        fprintf(stderr, "WSAStartup failed with error: %d\n", err);
        return 1;
    }

    qDebug("start to login!");

    int sock = 0;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
        qDebug("failed to connect!");
        return -1;
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = inet_addr(qsIp.toStdString().c_str());

    qDebug("loging ip:%s\nport:%d",qsIp.toStdString().c_str(),22);

    try
    {
        if (::connect(sock, (struct sockaddr*)(&sin),sizeof(struct sockaddr_in)) != 0)
        {
            qDebug("failed to connect! error:%d", GetLastError());
            err = -2;
        }
        qDebug("success to connect!");
    }
    catch (...)
    {
        err = -3;
        qDebug("failed to connect!");
    }

    // 为什么关不了
    //close(sock);
    return err;
}

int mn_puttyscp::scp(const QString &qsSource, const QString &qsDes)
{
    qDebug("scp host is:%s",m_qsIp.toStdString().c_str());
    qDebug("scp user is:%s",m_qsUser.toStdString().c_str());
    qDebug("scp pass is:%s",m_qsPwd.toStdString().c_str());
    qDebug("scp src is:%s",qsSource.toStdString().c_str());
    qDebug("scp des is:%s",qsDes.toStdString().c_str());

    return scp_func(m_qsIp.toStdString().c_str(), m_qsUser.toStdString().c_str(),
                    m_qsPwd.toStdString().c_str(),qsSource.toStdString().c_str(),
                    qsDes.toStdString().c_str(), NULL);
}
