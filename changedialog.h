#ifndef CHANGEDIALOG_H
#define CHANGEDIALOG_H

#include "cmasternode.h"
#include "clocalsetting.h"

#include <QtWidgets>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

class MnChangePage : public QWizardPage
{
    Q_OBJECT
public:
    MnChangePage(QWidget *parent = 0);
protected:
    void initializePage() override;
private:
    QLabel *AliasLabel;    // 别名
    QLabel *HostLabel;     // 主机IP
    QLabel *UserLabel;     // 用户
    QLabel *PwdLabel;      // 密码
    QLabel *HashLabel;     // 交易哈希
    QLabel *HashIndexLabel;  // 交易索引哈希
    QLabel *GenKeyLabel;    // 节点秘钥
    QLineEdit *AliasLineEdit;
    QLineEdit *HostLineEdit;
    QLineEdit *UserLineEdit;
    QLineEdit *PwdLineEdit;
    QLineEdit *HashLineEdit;
    QLineEdit *HashIndexEdit;
    QLineEdit *GenKeyLineEdit;
};

class ChangeDialog : public QWizard
{
    Q_OBJECT
signals:
    void sigMasternodeChange(const CMasternode &cmn);
public:
    ChangeDialog(QWidget *parent = 0);
    void accept() override;
    void reject() override;
private:
};


#endif // CHANGEDIALOG_H
