/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QVBoxLayout>
#include <QDir>
#include <QClipboard>
#include "mnwizard.h"
#include "walletrpc.h"

//! [0] //! [1]
MnWizard::MnWizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new AddressPage);
    addPage(new ServerInfoPage);
    addPage(new MasternodeInfoPage);
    addPage(new OutputFilesPage);
    addPage(new ConclusionPage);
//! [0]

    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(tr("Masternode Wizard"));
//! [2]
}
//! [1] //! [2]

//! [3]
void MnWizard::accept()
//! [3] //! [4]
{
    CMasternode cmn;

    QByteArray blocksafeconf;

    QByteArray host = field("host").toByteArray();
    QByteArray port = field("port").toByteArray();
    QByteArray user = field("user").toByteArray();
    QByteArray pwd = field("pwd").toByteArray();
    QByteArray rpcuser = field("rpcUser").toByteArray();
    QByteArray rpcpwd = field("rpcPwd").toByteArray();
    QByteArray rpcip=  field("rpcIp").toByteArray();

    QByteArray mnkey= field("masternodekey").toByteArray();
    QByteArray mnclltrlh = field("collateralhash").toByteArray();
    QByteArray mnclltrlindex = field("collateralindex").toByteArray();
    QByteArray safeconf = field("safeconf").toByteArray();
    QByteArray mnconf = field("masternodeconf").toByteArray();

    qDebug("hash:") ;
    qDebug(QString(mnclltrlh).toStdString().c_str());

    qDebug(CollateralHashComboBox);


    cmn.m_ip = QString(host);
    cmn.m_port = port.toShort();
    cmn.m_user = QString(user);
    cmn.m_pwd = QString(pwd);
    cmn.m_rpc_user = QString(rpcuser);
    cmn.m_rpc_pwd = QString(rpcpwd);
    cmn.m_rpc_ip = QString(rpcip);

    cmn.m_mn_key = QString(mnkey);
    cmn.m_clltrl_hash = QString(mnclltrlh);
    cmn.m_index = mnclltrlindex.toShort();
    cmn.m_safe_conf_path = QString(safeconf);
    cmn.m_mn_conf_path = QString(mnconf);

    emit sigMasternodeAdd(cmn);

    blocksafeconf += "rpcuser=" + rpcuser + "\n";
    blocksafeconf += "rpcpassword=" + rpcpwd + "\n";
    blocksafeconf += "rpcallowip=" + rpcip + "\n";
    blocksafeconf += "listen=1\n"\
                       "server=1\n"\
                       "daemon=1\n"\
                       "logtimestamps=1\n"\
                       "maxconnections=256\n"\
                       "masternode=1\n";

    blocksafeconf += "masternodeprivkey="+mnkey+"\n";
    blocksafeconf += "externalip="+host+":"+QString(port)+"\n";

    QDir qDit;
    if (!qDit.exists("./safeconf"))
    {
        qDit.mkdir("./safeconf");
    }

    QFile headerFile("./safeconf/" + QString(host));

    if (!headerFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Masternode Wizard"),
                             QObject::tr("Cannot write file %1:\n%2")
                             .arg(headerFile.fileName())
                             .arg(headerFile.errorString()));
        return;
    }

    headerFile.write(blocksafeconf);
    blocksafeconf.clear();
    headerFile.close();


//! [5]
    QDialog::accept();
//! [5] //! [6]
}
//! [6]

//! [7]
IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

    label = new QLabel(tr("This wizard will generate a Masternode configuration "
                          "file, including safe.conf and masternode.conf. "
                          "You simply need to specify  set a few options "
                          "to produce implementation file for your new masternode."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}
//! [7]
//!
AddressPage::AddressPage(QWidget *parent)
    :QWizardPage(parent)
{
    setTitle(tr("Generate Address"));
    setSubTitle(tr("Gendrate the address from your Desktop wallet, make sure your wallet is opening. "
                   "Pay the 1000 SAFE to this address."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/safe_logo.png"));

    AddressLabel = new QLabel(tr("collateral transation recive address"));
    AddressLineEdit = new QLineEdit();
    AddressLabel->setBuddy(AddressLineEdit);
    copypushButton = new QPushButton("copy");
    connect(copypushButton, &QPushButton::clicked, this, &AddressPage::copy);

    QGridLayout *qh1 = new QGridLayout;

    qh1->addWidget(AddressLabel,0,0);
    qh1->addWidget(AddressLineEdit,1,0,1,4);
    qh1->addWidget(copypushButton,2,3);
    setLayout(qh1);
}

void AddressPage::copy()
{
#ifdef _WIN32
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(AddressLineEdit->text());
#elif
    //QClipboard *clipboard = QApplication::clipboard();
    //clipboard->setText();
#endif
}

void AddressPage::initializePage()
{
    AddressLineEdit->setText("XsxnV2NtwwAfiZLRcY74iccec57YJ6nAqE");
}

//! [8] //! [9]
ServerInfoPage::ServerInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
//! [8]
    setTitle(tr("Masternodes Information"));
    setSubTitle(tr("Specify basic information about the Masternode for which you "
                   "want to generate configuration files."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/safe_logo.png"));

//! [10]
//!
    QLabel *MasternodeInfoLable = new QLabel(tr("Fill the Masternode vps infomation."));
    HostLabel = new QLabel(tr(" &Host Ip:"));
    HostLineEdit = new QLineEdit;
    QRegExp iprx("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\."\
               "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."\
               "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."\
               "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");

    HostLineEdit->setValidator(new QRegExpValidator(iprx,this));
    HostLabel->setBuddy(HostLineEdit);

    PortLabel = new QLabel(tr("&Port:"));
    PortLineEdit = new QLineEdit;
    PortLineEdit->setValidator(new QIntValidator(0,65536,PortLineEdit));
    PortLabel->setBuddy(PortLineEdit);

    UserLabel = new QLabel(tr(" &User Name:"));
    UserLineEdit = new QLineEdit;
    UserLabel->setBuddy(UserLineEdit);

    PwdLabel = new QLabel(tr("Passwd:"));
    PwdLineEdit = new QLineEdit;
    PwdLineEdit->setEchoMode(QLineEdit::Password);
    PwdLineEdit->setMaxLength(32);
    PwdLabel->setBuddy(PwdLineEdit);

    RpcUserLabel = new QLabel(tr("Rpc User:"));
    RpcUserLineEdit = new QLineEdit;
    RpcUserLabel->setBuddy(RpcUserLineEdit);

    RpcPwdPwdLabel = new QLabel(tr("Rpc Pwd:"));
    RpcPwdLineEdit = new QLineEdit;
    RpcPwdLineEdit->setEchoMode(QLineEdit::Password);
    RpcPwdLineEdit->setMaxLength(32);
    RpcPwdPwdLabel->setBuddy(RpcPwdLineEdit);

    RpcIpLabel = new QLabel(tr("Rpc Ip:"));
    RpcIpLineEdit = new QLineEdit;
    //RpcIpLineEdit->setValidator(new QRegExpValidator(iprx,this));
    RpcIpLabel->setBuddy(RpcIpLineEdit);

    RpcLable = new QLabel(tr("Rpc Configuration"));

//! [10]
    groupBox = new QGroupBox(tr("Constructor"));
//! [9]
    QGridLayout *qh1 = new QGridLayout;
    qh1->addWidget(RpcUserLabel, 0,0);
    qh1->addWidget(RpcUserLineEdit,0,1);
    qh1->addWidget(RpcPwdPwdLabel,1,0);
    qh1->addWidget(RpcPwdLineEdit,1,1);
    //qh1->addWidget(RpcIpLabel,2,0);
    //qh1->addWidget(RpcIpLineEdit,2,1);
    QVBoxLayout *groupBoxLayout = new QVBoxLayout;

    groupBoxLayout->addLayout(qh1);

//! [11] //! [12]
    registerField("host*", HostLineEdit);
    registerField("port*", PortLineEdit);
    registerField("user*", UserLineEdit);
    registerField("pwd*", PwdLineEdit);
    registerField("rpcUser*", RpcUserLineEdit);
    registerField("rpcPwd*", RpcPwdLineEdit);
    registerField("rpcIp*", RpcIpLineEdit);
//! [11]

//! [12]
//!
    groupBox->setLayout(groupBoxLayout);
    QVBoxLayout *qv = new QVBoxLayout;

    qv->addWidget(MasternodeInfoLable);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(HostLabel, 0, 0);
    layout->addWidget(HostLineEdit, 0, 1);
    layout->addWidget(PortLabel, 0, 2);
    layout->addWidget(PortLineEdit, 0, 3);
    layout->addWidget(UserLabel, 1, 0);
    layout->addWidget(UserLineEdit, 1, 1);
    layout->addWidget(PwdLabel, 1, 2);
    layout->addWidget(PwdLineEdit, 1, 3);
    QGridLayout *layoutRpc = new QGridLayout;
    layoutRpc->addWidget(RpcLable, 0, 0, 1, 2);
    layoutRpc->addWidget(groupBox, 1, 0, 1, 2);

    qv->addLayout(layout);
    qv->addLayout(layoutRpc);
    setLayout(qv);
//! [13]
}
//! [13]



void ServerInfoPage::initializePage()
{
    PortLineEdit->setText("5555");
    RpcUserLineEdit->setText("rpcuser");
    RpcPwdLineEdit->setText("123456");
    RpcIpLineEdit->setText("0.0.0.0");

}

MasternodeInfoPage::MasternodeInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("MasterNode Attribute"));
    setSubTitle(tr("Configure the most importance attribute to masternode. Make sure your desktop wallet is running..."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/safe_logo.png"));

    protectLable = new QLabel(tr("Get the Masternode key use cmd: \'masternode genkey\' under Debug console."));
    MasternodeKeyLabel = new QLabel(tr("&Masternode Key:"));
    MasternodeKeyLineEdit = new QLineEdit;
    MasternodeKeyLabel->setBuddy(MasternodeKeyLineEdit);

    includeBaseLable = new QLabel(tr("Get the Collateral Hash use cmd: \'masternode output\' under Debug console."));
    CollateralHashLabel = new QLabel(tr("Collateral Hash:"));
    CollateralHashComboBox = new QComboBox;
    CollateralHashLabel->setBuddy(CollateralHashComboBox);
    IndexLable= new QLabel(tr("Collateral Index:"));
    IndexLineEdit = new QLineEdit;
    IndexLineEdit->setMaximumWidth(120);
    IndexLineEdit->setValidator(new QIntValidator(0,10000,IndexLineEdit));
    IndexLable->setBuddy(IndexLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->setColumnMinimumWidth(0, 20);
    layout->addWidget(protectLable, 0, 0, 1, 3);
    layout->addWidget(MasternodeKeyLabel, 1, 1);
    layout->addWidget(MasternodeKeyLineEdit,1, 2);
    layout->addWidget(includeBaseLable, 2, 0, 1, 3);
    layout->addWidget(CollateralHashLabel, 3, 1);
    layout->addWidget(CollateralHashComboBox, 3, 2);
    layout->addWidget(IndexLable, 4, 1);
    layout->addWidget(IndexLineEdit, 4, 2);

    registerField("masternodekey*", MasternodeKeyLineEdit);
    registerField("collateralhash*", CollateralHashComboBox->lineEdit());
    registerField("collateralindex*", IndexLineEdit);

//! [15]
    setLayout(layout);
}
//! [15]

//! [16]
void MasternodeInfoPage::initializePage()
{
    WalletRPC walletRpc("127.0.0.1","s","123456");
    walletRpc.masternodeGenkey();

    CollateralHashComboBox->addItem(tr("hash 1"));
    CollateralHashComboBox->addItem(tr("hash 2"));
    CollateralHashComboBox->addItem(tr("hash 3"));
    CollateralHashComboBox->addItem(tr("hash 4"));

    /*
    // use rpc to get info ...
    QString baseClass = field("baseClass").toString();
    QRegularExpression rx("Q[A-Z].*");
    if (baseClass.isEmpty()) {
        //baseIncludeLineEdit->clear();
    } else if (rx.match(baseClass).hasMatch()) {
        //baseIncludeLineEdit->setText('<' + baseClass + '>');
    } else {
        //baseIncludeLineEdit->setText('"' + baseClass.toLower() + ".h\"");
    }
    */
}
//! [16]

OutputFilesPage::OutputFilesPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Output Configure file"));
    setSubTitle(tr("Specify where you want the wizard to put the generated "
                   "files."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/safe_logo.png"));

    outputDirLabel = new QLabel(tr("&safe.conf output directory:"));
    SafeConfLineEdit = new QLineEdit;
    outputDirLabel->setBuddy(SafeConfLineEdit);

    headerLabel = new QLabel(tr("&masternode.conf output directory:"));
    MasternodeConfLineEdit = new QLineEdit;
    headerLabel->setBuddy(MasternodeConfLineEdit);

    registerField("safeconf*", SafeConfLineEdit);
    registerField("masternodeconf*", MasternodeConfLineEdit);

    /*
    uploadpushButton = new QPushButton;
    uploadpushButton->setText("Upload");
    */

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(outputDirLabel, 0, 0);
    layout->addWidget(SafeConfLineEdit, 1, 0,1,3);
    layout->addWidget(headerLabel, 2, 0);
    layout->addWidget(MasternodeConfLineEdit, 3, 0,1,3);

    //layout->addWidget(uploadpushButton, 4, 2);
    setLayout(layout);
}

//! [17]
void OutputFilesPage::initializePage()
{
    QString userName = field("user").toString();
    if(userName.compare("root",Qt::CaseInsensitive))
    {
        SafeConfLineEdit->setText(QString("/home/")+userName+QString("/.safe/safe.conf"));
    }
    else
    {
        SafeConfLineEdit->setText(QString("/")+userName+QString("/.safe/safe.conf"));
    }

    MasternodeConfLineEdit->setText("on windows, read from registry.");
}
//! [17]

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Conclusion"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

    label = new QLabel;
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

void ConclusionPage::initializePage()
{
    QString finishText = wizard()->buttonText(QWizard::FinishButton);
    finishText.remove('&');
    label->setText(tr("Click %1 to generate the configuratio files.")
                   .arg(finishText));
}
