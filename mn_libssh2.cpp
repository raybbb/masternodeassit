#include "mn_libssh2.h"
#include<QLibrary>
#include<QDebug>


#ifdef WIN32
#include <winsock2.h>
#endif

mn_libssh2::mn_libssh2()
    :m_mylib(("libssh2.dll")),mp_session(NULL),mp_channel(NULL)
{
    if(m_mylib.load())
    {
        try
        {
            m_ssh2.ssh2_init = (plibssh2_init)m_mylib.resolve("libssh2_init");
            m_ssh2.ssh2_exit = (plibssh2_exit)m_mylib.resolve("libssh2_exit");
            m_ssh2.ssh2_free = (plibssh2_free)m_mylib.resolve("libssh2_free");
            m_ssh2.ssh2_ssa = (plibssh2_session_supported_algs)m_mylib.resolve("libssh2_session_supported_algs");
            m_ssh2.ssh2_session_init = (plibssh2_session_init_ex)m_mylib.resolve("libssh2_session_init_ex");
            m_ssh2.ssh2_session_handshake = (plibssh2_session_handshake)m_mylib.resolve("libssh2_session_handshake");
            m_ssh2.ssh2_hostkey_hash = (plibssh2_hostkey_hash)m_mylib.resolve("libssh2_hostkey_hash");
            m_ssh2.ssh2_userauth =  (plibssh2_userauth_password_ex)m_mylib.resolve("libssh2_userauth_password_ex");

        }
        catch(...)
        {

        }
    }
    else
    {
       //log error
    }
}

mn_libssh2::~mn_libssh2()
{
    m_mylib.unload();
}

int mn_libssh2::mn_init()
{
    int rc = 0;
#ifdef WIN32
    WSADATA wsadata;
    int err;
    err = WSAStartup(MAKEWORD(2,0), &wsadata);
    if (err != 0) {
        fprintf(stderr, "WSAStartup failed with error: %d\n", err);
        return 1;
    }
#endif

    rc = m_ssh2.ssh2_init(0);
    if (rc != 0)
    {
        //fprintf (stderr, "libssh2 initialization failed (%d)\n", rc);
        return 1;
    }

    return 0;
}

int mn_libssh2::mn_login(const QString &strIp,const QString &strUsr,
                         const QString &strPwd)
{
    int rc = 0;
    if (strIp!=""&&strUsr!=""&&strPwd!="")
    {
        m_username = strUsr;
        m_password = strPwd;
        qDebug("start to login!");
        m_sock = socket(AF_INET, SOCK_STREAM, 0);
        m_sin.sin_family = AF_INET;
        m_sin.sin_port = htons(22);
        m_sin.sin_addr.s_addr = inet_addr(strIp.toStdString().c_str());

        qDebug("loging ip:%s\nport:%d",strIp.toStdString().c_str(),22);

        try
        {
            if (connect(m_sock, (struct sockaddr*)(&m_sin),sizeof(struct sockaddr_in)) != 0)
            {
                qDebug("failed to connect! error:%d",GetLastError());
                return -1;
            }
            qDebug("success to connect!");


            mp_session = m_ssh2.ssh2_session_init(NULL, NULL, NULL, NULL);

            if(!mp_session)
            {
                qDebug("ssh init failed!");
                return -1;
            }

            rc = m_ssh2.ssh2_session_handshake(mp_session, m_sock);

            if (rc != 0)
            {
                qDebug("failure establishing SSH session: %d\n", rc);
                return -1;
            }
            qDebug("establishing SSH session ok");

            qDebug("user name:%s\npasswd:%s", m_username.toStdString().c_str(), m_password.toStdString().c_str());
            if (m_ssh2.ssh2_userauth(mp_session,
                                     m_username.toStdString().c_str(),m_username.length(),
                                     m_password.toStdString().c_str(),m_password.length(),NULL))
            {
                qDebug("Authentication by password failed.");
                return -1;
            }
            else
            {
                qDebug("Authentication ok.");
            }


        }
        catch(...)
        {
            // log error
        }
    }
    else
    {
        rc = 1;
    }

    return rc;
}

int mn_libssh2::mn_scp(const QString &strDest, const QString &strSrc)
{
    return 0;
}
