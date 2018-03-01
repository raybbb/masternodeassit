#include "changedialog.h"
#include "clocalsetting.h"
#include "database.h"
#include "cmasternode.h"
#include "cregistry.h"
#include <QRegExp>

MnChangePage::MnChangePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("修改Masternode节点信息");

    AliasLabel  = new QLabel("节点别名:");
    HostLabel = new QLabel("服务器地址:");
    UserLabel = new QLabel("登录用户:");
    PwdLabel = new QLabel("登录密码:");
    HashLabel = new QLabel("交易哈希:");
    HashIndexLabel = new QLabel("哈希索引:");
    GenKeyLabel  = new QLabel("节点秘钥:");
    AliasLineEdit = new QLineEdit;
    HostLineEdit = new QLineEdit;
    UserLineEdit = new QLineEdit;
    PwdLineEdit = new QLineEdit;
    HashLineEdit = new QLineEdit;
    HashIndexEdit = new QLineEdit;
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
    registerField("c_mn_hash_index*", HashIndexEdit);
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
    layout->addWidget(HashIndexLabel, 5, 0);
    layout->addWidget(HashIndexEdit, 5, 1);
    layout->addWidget(GenKeyLabel, 6, 0);
    layout->addWidget(GenKeyLineEdit, 6, 1);
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
    HashIndexEdit->setText(QString::number(cmn.m_index,10));

    connect(HashLineEdit,
            &QLineEdit::editingFinished,HashIndexEdit,
            [=](){
                   if (local_setting.mn_new_info[HashLineEdit->text()] != "")
                   {
                       HashIndexEdit->setText(
                                   local_setting.mn_new_info[HashLineEdit->text()]
                               );
                   }
                   else
                   {
                       HashIndexEdit->setText("");
                   }
            });
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
    QByteArray blocksafeconf;
    QByteArray mn_name = field("c_mn_name").toByteArray();
    QByteArray mn_host = field("c_mn_host").toByteArray();
    QByteArray mn_user = field("c_mn_user").toByteArray();
    QByteArray mn_pwd = field("c_mn_pwd").toByteArray();
    QByteArray mn_hash = field("c_mn_hash").toByteArray();
    QByteArray mn_genkey = field("c_mn_genkey").toByteArray();
    QByteArray mn_index = field("c_mn_hash_index").toByteArray();

    Database mydb;
    CMasternode cmn = CMasternode::DeSerializable(mydb.queryData(g_current_ip));

    if(QString(mn_user).compare("root",Qt::CaseInsensitive))
    {
        cmn.m_safe_conf_path =  QString("/home/")+QString(mn_user)+QString("/safe.conf");
        local_setting.remote_script_path = QString("/home/")+mn_user+QString("/");
    }
    else
    {
        cmn.m_safe_conf_path = QString("/")+QString(mn_user)+QString("/safe.conf");
        local_setting.remote_script_path = QString("/") + mn_user+QString("/");
    }

    // 修改safe.conf
    blocksafeconf += "rpcuser=" + cmn.m_remote_rpc_user + "\n";
    blocksafeconf += "rpcpassword=" + cmn.m_remote_rpc_pwd + "\n";
    blocksafeconf += "rpcallowip=0.0.0.0/0\n";
    blocksafeconf += "listen=1\n"\
                     "server=1\n"\
                     "daemon=1\n"\
                     "logtimestamps=1\n"\
                     "maxconnections=256\n"\
                     "masternode=1\n";
    blocksafeconf += "masternodeprivkey="+mn_genkey+"\n";
    blocksafeconf += "externalip="+mn_host+":"+QString::number(cmn.m_port,10)+"\n";

    QDir qDit;
    if (!qDit.exists("./safeconf"))
    {
        qDit.mkdir("./safeconf");
    }

    QFile safeConfigFile("./safeconf/" + QString(mn_host));

    if (!safeConfigFile.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(0, QObject::tr("Masternode Wizard"),
                             QObject::tr("Cannot write file %1:\n%2")
                             .arg(safeConfigFile.fileName())
                             .arg(safeConfigFile.errorString()));
        return;
    }

    safeConfigFile.write(blocksafeconf);
    blocksafeconf.clear();
    safeConfigFile.close();

    // 修改masternode.conf
    QString qMnconf = "";
    if(CRegistry::readDataDir(qMnconf))
    {
         QDir qDitMn;
         if (qDitMn.exists(qMnconf+"\\masternode.conf"))
         {
             qDebug()<<"find masternode.conf...";
             QFile mnFile(qMnconf+"\\masternode.conf");
             if (!mnFile.open(QFile::Text | QFile::ReadWrite))
             {
                 QMessageBox::warning(0, QObject::tr("Masternode Wizard"),
                                      QObject::tr("Cannot write file %1:\n%2")
                                      .arg(mnFile.fileName())
                                      .arg(mnFile.errorString()));
                 return;
             }

             QString qbData = mnFile.readAll();

             QString qsReplace = QString("\n") + QString(mn_name) + QString(" ")
                     + QString(mn_host) + QString(":") + QString("5555") + QString(" ");
             qsReplace += QString(mn_genkey) + QString(" ") + QString(mn_hash)
                     + QString(" ") + QString(mn_index) + QString("\n");

             qbData.replace(QRegExp("\\n\\s*" + cmn.m_alias + "\\s*"+ cmn.m_ip + ":5555" + ".+\\n"),
                            qsReplace);

             mnFile.write(qbData.toLocal8Bit());
             mnFile.close();
         }
         else
         {
             qDebug()<<"can not find masternode.conf...";
         }
    }
    else
    {
        qDebug()<<"Can not find masternode.conf...";
        // @TODO show file dialog...
    }

    cmn.m_ip = QString(mn_host);
    cmn.m_alias = QString(mn_name);
    cmn.m_user = QString(mn_user);
    cmn.m_pwd = QString(mn_pwd);
    cmn.m_mn_key = QString(mn_genkey);
    cmn.m_clltrl_hash = QString(mn_hash);
    cmn.m_index = mn_index.toInt();

    emit sigMasternodeChange(cmn);

    QDialog::accept();
}

void ChangeDialog::reject()
{
    QDialog::reject();
}
