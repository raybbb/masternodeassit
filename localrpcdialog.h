#ifndef LOCALRPCDIALOG_H
#define LOCALRPCDIALOG_H
#include <QtWidgets>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>


class LocalRpcPage : public QWizardPage
{
    Q_OBJECT
public:
    LocalRpcPage(QWidget *parent = 0);
protected:
    void initializePage() override;
private:
    QLabel *RpcUserLabel;
    QLabel *RpcPwdLabel;
    QLineEdit *RpcUserLineEdit;
    QLineEdit *RpcPwdLineEdit;
};

class LocalrpcDialog : public QWizard
{
    Q_OBJECT
public:
    LocalrpcDialog(QWidget *parent = 0);
    void accept() override;
    void reject() override;
private:
};

#endif // LOCALRPCDIALOG_H
