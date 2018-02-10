#include "mnlibssh2.h"
#include<QLibrary>
#include<QDebug>
#include<QFile>

#ifdef WIN32
#include <winsock2.h>
#endif

mn_libssh2::mn_libssh2()
    :m_mylib(("libssh2.dll")), mp_session(NULL),isLoad(false)
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
            m_ssh2.ssh2_userauth = (plibssh2_userauth_password_ex)m_mylib.resolve("libssh2_userauth_password_ex");
            m_ssh2.ssh2_scp_recv2 = (plibssh2_scp_recv2)m_mylib.resolve("libssh2_scp_recv2");
            m_ssh2.ssh2_session_laster_error = (plibssh2_session_last_errno)m_mylib.resolve("libssh2_session_last_errno");
            m_ssh2.ssh2_channel_read = (plibssh2_channel_read_ex)m_mylib.resolve("libssh2_channel_read_ex");
            m_ssh2.ssh2_scp_send = (plibssh2_scp_send_ex)m_mylib.resolve("libssh2_scp_send_ex");
            m_ssh2.ssh2_channel_write = (plibssh2_channel_write_ex)m_mylib.resolve("libssh2_channel_write_ex");
            m_ssh2.ssh2_session_laster_error_msg = (plibssh2_session_last_error)m_mylib.resolve("libssh2_session_last_error");
            m_ssh2.ssh2_channel_send_eof =  (plibssh2_channel_send_eof)m_mylib.resolve("libssh2_channel_send_eof"); ;
            m_ssh2.ssh2_channel_eof = (plibssh2_channel_eof)m_mylib.resolve("libssh2_channel_eof"); ;
            m_ssh2.ssh2_channel_wait_eof =  (plibssh2_channel_wait_eof)m_mylib.resolve("libssh2_channel_wait_eof"); ;
            m_ssh2.ssh2_channel_close = (plibssh2_channel_close)m_mylib.resolve("libssh2_channel_close"); ;
            m_ssh2.ssh2_channel_wait_closed = (plibssh2_channel_wait_closed)m_mylib.resolve("libssh2_channel_wait_closed"); ;
            m_ssh2.ssh2_channel_free = (plibssh2_channel_free)m_mylib.resolve("libssh2_channel_free"); ;
            m_ssh2.ssh2_session_disconnect = (plibssh2_session_disconnect_ex)m_mylib.resolve("libssh2_session_disconnect_ex"); ;
            m_ssh2.ssh2_session_free = (plibssh2_session_free)m_mylib.resolve("libssh2_session_free");

            m_ssh2.ssh2_channel_open = (plibssh2_channel_open_ex)m_mylib.resolve("libssh2_channel_open_ex");
            m_ssh2.ssh2_channel_process_startup = (plibssh2_channel_process_startup)m_mylib.resolve("libssh2_channel_process_startup");
            m_ssh2.ssh2_channel_get_exit_status = (plibssh2_channel_get_exit_status)m_mylib.resolve("libssh2_channel_get_exit_status");
            m_ssh2.ssh2_channel_get_exit_signal = (plibssh2_channel_get_exit_signal)m_mylib.resolve("libssh2_channel_get_exit_signal");
            m_ssh2.ssh2_session_block_directions = (plibssh2_session_block_directions)m_mylib.resolve("libssh2_session_block_directions");
            isLoad = true;
        }
        catch(...)
        {
            isLoad = false;
        }
    }
    else
    {
        isLoad = false;
        qDebug("Load ssh2 Dll failed!");
    }
}

mn_libssh2::~mn_libssh2()
{
    if(m_mylib.isLoaded())
    {
        m_mylib.unload();
    }
    if(mp_session)
    {
        /*
         m_ssh2.ssh2_session_disconnect
                (mp_session,SSH_DISCONNECT_BY_APPLICATION,
                 "Normal Shutdown, Thank you for playing","");
        m_ssh2.ssh2_session_free(mp_session);
        */
    }

#ifdef WIN32
    closesocket(m_sock);
#else
    close(m_sock);
#endif
}

int mn_libssh2::mn_init()
{
    int rc = 0;
#ifdef WIN32
    WSADATA wsadata;
    int err;
    err = WSAStartup(MAKEWORD(2,0), &wsadata);
    if (err != 0) {
        qDebug("WSAStartup failed with error: %d\n", err);
        return 1;
    }
#endif

    rc = m_ssh2.ssh2_init(0);
    if (rc != 0)
    {
        qDebug("libssh2 initialization failed (%d)\n", rc);
        return 1;
    }
    return 0;

}

int mn_libssh2::mn_login(const QString &strIp,const QString &strUsr,
                         const QString &strPwd)
{
    int rc = 0;
    if (strIp!="" && strUsr!="" && strPwd!="")
    {
        m_username = strUsr;
        m_password = strPwd;
        qDebug("start to login!");
        m_sock = socket(AF_INET, SOCK_STREAM, 0);
        m_sin.sin_family = AF_INET;
        m_sin.sin_port = htons(22);
        m_sin.sin_addr.s_addr = inet_addr(strIp.toStdString().c_str());

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

            if (m_ssh2.ssh2_userauth(mp_session,
                                     m_username.toStdString().c_str(),
                                     m_username.length(),
                                     m_password.toStdString().c_str(),
                                     m_password.length(),NULL))
            {
                qDebug("user name:%s\npasswd:%s",
                       m_username.toStdString().c_str(),
                       m_password.toStdString().c_str());
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

int mn_libssh2::mn_scp_read(const QString &strDest, const QString &strSrc)
{
    libssh2_struct_stat_size got = 0;
    libssh2_struct_stat fileinfo;
    LIBSSH2_CHANNEL* channel = m_ssh2.ssh2_scp_recv2(
                mp_session, strSrc.toStdString().c_str(), &fileinfo);

    if (!channel)
    {
        int err = m_ssh2.ssh2_session_laster_error(mp_session);
        qDebug("Unable to open a session: %d\n", err);
        return err;
    }

    int rc = 0;
    QFile destFile(strDest);
    if (!destFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug("Unable to open a dest file.");
        return -1;
    }

    while(got < fileinfo.st_size)
    {
        char mem[1024]={0};
        int amount = sizeof(mem);

        if((fileinfo.st_size - got) < amount)
        {
            amount = (int)(fileinfo.st_size - got);
            qDebug("amount is %d", amount);
        }

        rc = m_ssh2.ssh2_channel_read(channel, 0, mem, amount);

        if(rc > 0)
        {
            destFile.write(mem);
        }
        else if(rc < 0)
        {
            qDebug("libssh2_channel_read() failed: %d\n", rc);
            break;
        }
        got += rc;
    }

    m_ssh2.ssh2_channel_free(channel);

    destFile.close();

    return 0;
}

int mn_libssh2::mn_scp_write(const QString &strDest, const QString &strSrc)
{
    FILE *local;
    struct stat fileinfo;
    local = fopen(strSrc.toStdString().c_str(), "rb");
    if (!local)
    {
        qDebug("Can't open local file\n");
        return -1;
    }
    stat(strSrc.toStdString().c_str(), &fileinfo);

    LIBSSH2_CHANNEL* channel = m_ssh2.ssh2_scp_send(mp_session, strDest.toStdString().c_str(),
                                                fileinfo.st_mode & 0777,
                                                (unsigned long)fileinfo.st_size,0,0);
    if (!channel)
    {
        char *errmsg;
        int errlen;
        int err = m_ssh2.ssh2_session_laster_error_msg(mp_session, &errmsg, &errlen,0);
        qDebug("Unable to open a session: (%d) %s\n", err, errmsg);
        return err;
    }

    int rc;
    char mem[1024];
    size_t nread;
    char *ptr;

    do {
        nread = fread(mem, 1, sizeof(mem), local);
        if (nread <= 0) {
            /* end of file */
            break;
        }
        ptr = mem;
        do {
            /* write the same data over and over, until error or completion */
            rc = m_ssh2.ssh2_channel_write(channel, 0, ptr, nread);
            if (rc < 0) {
                qDebug("ERROR %d\n", rc);
                break;
            }
            else
            {
                /* rc indicates how many bytes were written this time */
                ptr += rc;
                nread -= rc;
            }
        } while (nread);
    } while (1);

    qDebug("Sending EOF\n");
    m_ssh2.ssh2_channel_send_eof(channel);
    qDebug("Waiting for EOF\n");
    m_ssh2.ssh2_channel_wait_eof(channel);
    qDebug("Waiting for channel to close\n");
    m_ssh2.ssh2_channel_wait_closed(channel);
    m_ssh2.ssh2_channel_free(channel);
    channel = NULL;

    if (local)
    {
        fclose(local);
    }

    return 0;
}

int mn_libssh2::waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
    struct timeval timeout;
    int rc;
    fd_set fd;
    fd_set *writefd = NULL;
    fd_set *readfd = NULL;
    int dir;

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    FD_ZERO(&fd);

    FD_SET(socket_fd, &fd);

    /* now make sure we wait in the correct direction */
    dir = m_ssh2.ssh2_session_block_directions(session);

    if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
        readfd = &fd;

    if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
        writefd = &fd;

    rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

    return rc;
}

int mn_libssh2::mn_exec(const QString &strCmdline)
{
    int rc;
    int exitcode;
    char *exitsignal=(char *)"none";
    int bytecount = 0;
    /* Exec non-blocking on the remove host */
    LIBSSH2_CHANNEL* channel;

    while( (channel = m_ssh2.ssh2_channel_open(mp_session,"session",sizeof("session") - 1,
                                               LIBSSH2_CHANNEL_WINDOW_DEFAULT,
                                               LIBSSH2_CHANNEL_PACKET_DEFAULT, NULL, 0)) == NULL
           &&m_ssh2.ssh2_session_laster_error_msg(mp_session,NULL,NULL,0) ==LIBSSH2_ERROR_EAGAIN )
    {
        waitsocket(m_sock, mp_session);
    }
    if( channel == NULL )
    {
        qDebug("Error\n");
        return -1;
    }
    while( (rc = m_ssh2.ssh2_channel_process_startup
            (channel,  "exec", sizeof("exec") - 1,
             strCmdline.toStdString().c_str(),
             (unsigned int)strCmdline.length()))
           == LIBSSH2_ERROR_EAGAIN )
    {
        waitsocket(m_sock, mp_session);
    }

    if( rc != 0 )
    {
        qDebug("Error\n");
        goto shutdown;
    }

    for( ;; )
    {
        /* loop until we block */
        int rc;
        do
        {
            char buffer[0x4000];
            rc = m_ssh2.ssh2_channel_read( channel, 0, buffer, sizeof(buffer) );
            if( rc > 0 )
            {
                int i;
                bytecount += rc;
                qDebug("We read:\n");
                for( i=0; i < rc; ++i )
                {
                    fputc( buffer[i], stderr);
                }
                qDebug("\n");
            }
            else {
                if( rc != LIBSSH2_ERROR_EAGAIN )
                {
                    /* no need to output this for the EAGAIN case */
                    qDebug("libssh2_channel_read returned %d\n", rc);
                }
            }
        }
        while( rc > 0 );
        /* this is due to blocking that would occur otherwise so we loop on
           this condition */
        if( rc == LIBSSH2_ERROR_EAGAIN )
        {
            waitsocket(m_sock, mp_session);
        }
        else
        {
            break;
        }
    }

shutdown:
    exitcode = 127;
    while( (rc = m_ssh2.ssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN )
    {
        waitsocket(m_sock, mp_session);
    }

    if( rc == 0 )
    {
        exitcode = m_ssh2.ssh2_channel_get_exit_status(channel);
        m_ssh2.ssh2_channel_get_exit_signal(channel, &exitsignal,
                                        NULL, NULL, NULL, NULL, NULL);
    }

    if (exitsignal)
        qDebug("\nGot signal: %s\n", exitsignal);
    else
        qDebug("\nEXIT: %d bytecount: %d\n", exitcode, bytecount);

    m_ssh2.ssh2_channel_free(channel);
    channel = NULL;
    return 0;
}
