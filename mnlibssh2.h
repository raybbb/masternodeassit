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
typedef int (*plibssh2_session_supported_algs)
                        (LIBSSH2_SESSION* session,
                         int method_type,
                         const char*** algs);
typedef LIBSSH2_API LIBSSH2_SESSION
                        *(*plibssh2_session_init_ex)(LIBSSH2_ALLOC_FUNC((*my_alloc)),
                        LIBSSH2_FREE_FUNC((*my_free)),
                        LIBSSH2_REALLOC_FUNC((*my_realloc)), void *abstract);
typedef LIBSSH2_API int (*plibssh2_session_handshake)
                        (LIBSSH2_SESSION *session,
                        libssh2_socket_t sock);
typedef LIBSSH2_API const char* (*plibssh2_hostkey_hash)
                        (LIBSSH2_SESSION *session,int hash_type);
typedef LIBSSH2_API int (*plibssh2_userauth_password_ex)
                        (LIBSSH2_SESSION *session,const char *username,
                         unsigned int username_len,
                         const char *password,
                         unsigned int password_len,
                         LIBSSH2_PASSWD_CHANGEREQ_FUNC((*passwd_change_cb)));
typedef LIBSSH2_API LIBSSH2_CHANNEL* (*plibssh2_scp_recv2)
                        (LIBSSH2_SESSION *session,
                         const char *path,
                         libssh2_struct_stat *sb);
typedef LIBSSH2_API int (*plibssh2_session_last_errno)(LIBSSH2_SESSION *session);
typedef LIBSSH2_API int (*plibssh2_session_last_error)
                        (LIBSSH2_SESSION * session,
                         char **errmsg,
                         int *errmsg_len, int want_buf);
typedef LIBSSH2_API ssize_t (*plibssh2_channel_read_ex)
                        (LIBSSH2_CHANNEL *channel,
                         int stream_id, char *buf,size_t buflen);
typedef LIBSSH2_API LIBSSH2_CHANNEL* (*plibssh2_scp_send_ex)
                        (LIBSSH2_SESSION *session,
                         const char *path, int mode,
                         size_t size, long mtime,
                         long atime);
typedef LIBSSH2_API ssize_t (*plibssh2_channel_write_ex)
                        (LIBSSH2_CHANNEL *channel,
                         int stream_id, const char *buf,
                         size_t buflen);
typedef LIBSSH2_API int (*plibssh2_channel_send_eof)(LIBSSH2_CHANNEL *channel);
typedef LIBSSH2_API int (*plibssh2_channel_eof)(LIBSSH2_CHANNEL *channel);
typedef LIBSSH2_API int (*plibssh2_channel_wait_eof)(LIBSSH2_CHANNEL *channel);
typedef LIBSSH2_API int (*plibssh2_channel_close)(LIBSSH2_CHANNEL *channel);
typedef LIBSSH2_API int (*plibssh2_channel_wait_closed)(LIBSSH2_CHANNEL *channel);
typedef LIBSSH2_API int (*plibssh2_channel_free)(LIBSSH2_CHANNEL *channel);
typedef LIBSSH2_API int (*plibssh2_channel_get_exit_status)(LIBSSH2_CHANNEL* channel);
typedef LIBSSH2_API int (*plibssh2_channel_get_exit_signal)(LIBSSH2_CHANNEL* channel,
                         char **exitsignal,size_t *exitsignal_len,
                         char **errmsg,size_t *errmsg_len,
                         char **langtag,size_t *langtag_len);
typedef LIBSSH2_API int (*plibssh2_session_disconnect_ex)
                        (LIBSSH2_SESSION *session,
                         int reason,
                         const char *description,
                         const char *lang);
typedef LIBSSH2_API int (*plibssh2_session_free)(LIBSSH2_SESSION *session);
typedef LIBSSH2_API LIBSSH2_CHANNEL *
(*plibssh2_channel_open_ex)(LIBSSH2_SESSION *session, const char *channel_type,
                            unsigned int channel_type_len,
                            unsigned int window_size, unsigned int packet_size,
                            const char *message, unsigned int message_len);
typedef LIBSSH2_API int
(*plibssh2_channel_process_startup)(LIBSSH2_CHANNEL *channel,
                                   const char *request,
                                   unsigned int request_len,
                                   const char *message,
                                   unsigned int message_len);
typedef LIBSSH2_API int (*plibssh2_session_block_directions)
                        (LIBSSH2_SESSION *session);

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
    plibssh2_scp_recv2 ssh2_scp_recv2;
    plibssh2_session_last_errno ssh2_session_laster_error;
    plibssh2_channel_read_ex ssh2_channel_read;
    plibssh2_scp_send_ex ssh2_scp_send;
    plibssh2_channel_write_ex ssh2_channel_write;
    plibssh2_session_last_error ssh2_session_laster_error_msg;
    plibssh2_channel_send_eof ssh2_channel_send_eof;
    plibssh2_channel_eof ssh2_channel_eof;
    plibssh2_channel_wait_eof ssh2_channel_wait_eof;
    plibssh2_channel_close ssh2_channel_close;
    plibssh2_channel_wait_closed ssh2_channel_wait_closed;
    plibssh2_channel_free ssh2_channel_free;
    plibssh2_session_disconnect_ex ssh2_session_disconnect;
    plibssh2_session_free ssh2_session_free;
    plibssh2_channel_open_ex ssh2_channel_open;
    plibssh2_channel_process_startup ssh2_channel_process_startup;
    plibssh2_channel_get_exit_status ssh2_channel_get_exit_status;
    plibssh2_channel_get_exit_signal ssh2_channel_get_exit_signal;
    plibssh2_session_block_directions ssh2_session_block_directions;
}SSH_FUNC;

class mn_libssh2
{
public:
    mn_libssh2();
    ~mn_libssh2();
    int mn_init();
    int mn_login(const QString &strIp, const QString &strUsr,
                 const QString &strPwd);
    //int mn_host_check();
    int mn_scp_read(const QString &strDest, const QString &strSrc);
    int mn_scp_write(const QString &strDest, const QString &strSrc);
    int mn_exec(const QString &strCmdline);
private:
    int waitsocket(int socket_fd, LIBSSH2_SESSION *session);
private:
    int m_sock;
    int m_auth_pw;
    bool isLoad;
    struct sockaddr_in m_sin;
    QLibrary m_mylib;
    SSH_FUNC m_ssh2;
    QString m_username;
    QString m_password;
    LIBSSH2_SESSION* mp_session;

};

#endif // MN_LIBSSH2_H
