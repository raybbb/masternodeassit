#include "changedialog.h"
#include "clocalsetting.h"
#include "database.h"
#include "cmasternode.h"

MnChangePage::MnChangePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("修改Masternode节点信息");

    AliasLabel  = new QLabel("节点别名:");
    HostLabel = new QLabel("服务器地址:");
    UserLabel = new QLabel("登录用户:");
    PwdLabel = new QLabel("登录密码:");
    HashLabel = new QLabel("交易哈希:");
    GenKeyLabel  = new QLabel("节点秘钥:");
    AliasLineEdit = new QLineEdit;
    HostLineEdit = new QLineEdit;
    UserLineEdit = new QLineEdit;
    PwdLineEdit = new QLineEdit;
    HashLineEdit = new QLineEdit;
    GenKeyLineEdit = new QLineEdit;

    PwdLineEdit->setEchoMode(QLineEdit::Password);
    PwdLineEdit->setMaxLength(32);

    QRegExp iprx("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\."\
               "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."\
               "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."\
               "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");
    HostLineEdit->setValidator(new QRegExpValidator(iprx,this));

    registerField("c_mn_name*", AliasLineEdit);
    registerField("c_mn_host*", HostLineEdit);
    registerField("c_mn_user*", UserLineEdit);
    registerField("c_mn_pwd*", PwdLineEdit);
    registerField("c_mn_hash*", HashLineEdit);
    registerField("c_mn_genkey*", GenKeyLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(AliasLabel, 0, 0);
    layout->addWidget(AliasLineEdit, 0, 1);
    layout->addWidget(HostLabel, 1, 0);
    layout->addWidget(HostLineEdit, 1, 1);
    layout->addWidget(UserLabel, 2, 0);
    layout->addWidget(UserLineEdit, 2, 1);
    layout->addWidget(PwdLabel, 3, 0);
    layout->addWidget(PwdLineEdit, 3, 1);
    layout->addWidget(HashLabel, 4, 0);
    layout->addWidget(HashLineEdit, 4, 1);
    layout->addWidget(GenKeyLabel, 5, 0);
    layout->addWidget(GenKeyLineEdit, 5, 1);
    setLayout(layout);
}

void MnChangePage::initializePage()
{
    Database mydb;
    CMasternode cmn = CMasternode::DeSerializable(mydb.queryData(g_current_ip));

    AliasLineEdit->setText(cmn.m_alias);
    HostLineEdit->setText(cmn.m_ip);
    UserLineEdit->setText(cmn.m_user);
    PwdLineEdit->setText(cmn.m_pwd);
    HashLineEdit->setText(cmn.m_clltrl_hash);
    GenKeyLineEdit->setText(cmn.m_mn_key);

}

ChangeDialog::ChangeDialog(QWidget *parent)
    :QWizard(parent)
{
    MnChangePage* page =  new MnChangePage;
    addPage(page);

    this->setWindowFlags(Qt::WindowSystemMenuHint
                         | Qt::WindowMinMaxButtonsHint
                         | Qt::WindowCloseButtonHint);
    this->setWindowIcon(QIcon(":/images/safe_logo.png"));
    this->setOption(QWizard::NoBackButtonOnStartPage);
    this->setWindowTitle("修改");
    this->setButtonText(WizardButton::FinishButton,"保 存");
    this->setButtonText(WizardButton::CancelButton,"取 消");
}



void ChangeDialog::accept()
{
     QByteArray rpcuser = field("rpcuser").toByteArray();
     QByteArray rpcpwd = field("rpcpwd").toByteArray();

     QDialog::accept();
}

void ChangeDialog::reject()
{
    QDialog::reject();
}
