#ifndef CHANGEDIALOG_H
#define CHANGEDIALOG_H


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
    QLabel *AliasLabel;   // 别名
    QLabel *HostLabel;    // 主机IP
    QLabel *UserLabel;    // 用户
    QLabel *PwdLabel;     // 密码
    QLabel *HashLabel;    // 交易哈希
    QLabel *GenKeyLabel;    // 交易哈希
    QLineEdit *AliasLineEdit;
    QLineEdit *HostLineEdit;
    QLineEdit *UserLineEdit;
    QLineEdit *PwdLineEdit;
    QLineEdit *HashLineEdit;
    QLineEdit *GenKeyLineEdit;
};

class ChangeDialog : public QWizard
{
    Q_OBJECT
public:
    ChangeDialog(QWidget *parent = 0);
    void accept() override;
    void reject() override;
private:
};


#endif // CHANGEDIALOG_H
