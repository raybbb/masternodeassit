#include "localrpcdialog.h"

LocalRpcPage::LocalRpcPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("设置本地RPC");
    setSubTitle("由于检测到没有设置本地的RPC，请填写下面内容，然后重启本地钱包。");
    RpcUserLabel = new QLabel("RPC用户名:");
    RpcUserLineEdit = new QLineEdit;

    RpcPwdLabel = new QLabel("RPC密码:");
    RpcPwdLineEdit = new QLineEdit;
    RpcPwdLineEdit->setEchoMode(QLineEdit::Password);
    RpcPwdLineEdit->setMaxLength(16);

    registerField("rpcuser*", RpcUserLineEdit);
    registerField("rpcpwd*", RpcPwdLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(RpcUserLabel, 0, 0);
    layout->addWidget(RpcUserLineEdit, 0, 1);
    layout->addWidget(RpcPwdLabel, 1, 0);
    layout->addWidget(RpcPwdLineEdit, 1, 1);
    setLayout(layout);
}

void LocalRpcPage::initializePage()
{

}

LocalrpcDialog::LocalrpcDialog(QWidget *parent):
    QWizard(parent)
{
   QWizardPage* page =  new LocalRpcPage;
   addPage(page);

   this->setWindowFlags(Qt::WindowSystemMenuHint
                        | Qt::WindowMinMaxButtonsHint
                        | Qt::WindowCloseButtonHint);
   this->setWindowIcon(QIcon(":/images/safe_logo.png"));
   this->setOption(QWizard::NoBackButtonOnStartPage);
   this->setWindowTitle("本地RPC输入框");
   this->setButtonText(WizardButton::FinishButton,"完 成");
   this->setButtonText(WizardButton::CancelButton,"取 消");
}

void LocalrpcDialog::accept()
{
     QDialog::accept();
}

void LocalrpcDialog::reject()
{
    exit(0);
}
