#ifndef MN_LIBSSH2_H
#define MN_LIBSSH2_H

#include <QObject>
#include <QLibrary>
#include <QSharedPointer>

#include "include/libssh2.h"
#include "include/scp.h"

typedef int (*plibssh2_init)(int flags);
typedef void (*plibssh2_exit)();
typedef void (*plibssh2_free)(LIBSSH2_SESSION *session, void *ptr);
typedef int (*plibssh2_session_supported_algs)(LIBSSH2_SESSION* session,
                                              int method_type,
                                              const char*** algs);
typedef LIBSSH2_API LIBSSH2_SESSION *
(*plibssh2_session_init_ex)(LIBSSH2_ALLOC_FUNC((*my_alloc)),
                            LIBSSH2_FREE_FUNC((*my_free)),
                            LIBSSH2_REALLOC_FUNC((*my_realloc)), void *abstract);
typedef LIBSSH2_API int (*plibssh2_session_handshake)(LIBSSH2_SESSION *session,
                                                      libssh2_socket_t sock);
typedef LIBSSH2_API const char* (*plibssh2_hostkey_hash)(LIBSSH2_SESSION *session,
                                             int hash_type);
typedef LIBSSH2_API int (*plibssh2_userauth_password_ex)(LIBSSH2_SESSION *session,
                                             const char *username,
                                             unsigned int username_len,
                                             const char *password,
                                             unsigned int password_len,
                                             LIBSSH2_PASSWD_CHANGEREQ_FUNC((*passwd_change_cb)));


typedef PSCPDLL_API int (*psun)(int num1, int num2);


typedef struct ssh_func
{
    plibssh2_init ssh2_init;
    plibssh2_exit ssh2_exit;
    plibssh2_free ssh2_free;
    plibssh2_session_supported_algs ssh2_ssa;
    plibssh2_session_init_ex ssh2_session_init;
    plibssh2_session_handshake ssh2_session_handshake;
    plibssh2_hostkey_hash ssh2_hostkey_hash;
    plibssh2_userauth_password_ex ssh2_userauth;
}SSH_FUNC;

class mn_libssh2
{
public:
    mn_libssh2();
    ~mn_libssh2();

    int mn_init();
    int mn_login(const QString &strIp, const QString &strUsr,
                 const QString &strPwd);
    int mn_scp(const QString &strDest, const QString &strSrc);
private:
    int m_sock;
    int m_auth_pw;
    struct sockaddr_in m_sin;
    libssh2_struct_stat m_fileinfo;

    QLibrary m_mylib;
    SSH_FUNC m_ssh2;
    QString m_username;
    QString m_password;
    LIBSSH2_SESSION* mp_session;
    LIBSSH2_CHANNEL* mp_channel;
};

#endif // MN_LIBSSH2_H
