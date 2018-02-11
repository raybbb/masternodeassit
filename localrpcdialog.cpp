#include "localrpcdialog.h"
#include "clocalsetting.h"

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
    /*
    listen=1
    server=1
    daemon=1
    logtimestamps=1
    maxconnections=256
    rpcuser=rbai
    rpcpassword=092092092
    rpcallowip=0.0.0.0/0
    */

     QByteArray rpcuser = field("rpcuser").toByteArray();
     QByteArray rpcpwd = field("rpcpwd").toByteArray();

     QString localSafeConfFile = local_setting.safe_conf_path + "/safe.conf";
     qDebug()<< "*Local safe file: " << localSafeConfFile;
     QFile file(localSafeConfFile);
     if(file.exists())
     {
         if(file.open(QIODevice::ReadWrite))
         {
             QString conf = "listen=1\r\n"
                             "server=1\r\n"
                             "daemon=1\r\n"
                             "logtimestamps=1\r\n"
                             "maxconnections=256\r\n"
                             "rpcallowip=0.0.0.0/0\r\n";
             file.write(conf.toStdString().c_str());
             file.write("rpcuser="+rpcuser+"\r\n");
             file.write("rpcpassword="+rpcpwd+"\r\n");
             file.close();

             local_setting.local_rpc_user = rpcuser;
             local_setting.local_rpc_pwd = rpcpwd;
         }
         else
         {
             // @todo do some...
         }
     }

     QDialog::accept();
}

void LocalrpcDialog::reject()
{
    exit(0);
}
