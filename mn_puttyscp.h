#ifndef MN_PUTTYSCP_H
#define MN_PUTTYSCP_H

#include <QObject>
#include <QLibrary>

#include "include/scp.h"

typedef PSCPDLL_API int (*pscp)(const char *szIp, const char *szUser, const char *szPwd,
                    const char *szSource, const char *szDes, callback_exec func);

class mn_puttyscp : public QObject
{
    Q_OBJECT
public:
    explicit mn_puttyscp(QObject *parent = nullptr);
    ~mn_puttyscp();

    int init(const QString &qsIp, const QString &qsUser, const QString &qsPwd);
    int scp(const QString &qsSource, const QString &qsszDes);
    pscp scp_func;

private:
     QLibrary m_mylib;

     QString m_qsIp;
     QString m_qsUser;
     QString m_qsPwd;
     QString m_qsSource;
     QString m_qsszDes;

};

#endif // MN_PUTTYSCP_H
