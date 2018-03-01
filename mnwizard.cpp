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
#include "cregistry.h"

#define _Debug

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

    this->setWindowIcon(QIcon(":/images/safe_logo.png"));
    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(tr("Masternode 配置向导"));

    this->setButtonText(WizardButton::BackButton, "上一步");
    this->setButtonText(WizardButton::NextButton, "下一步");
    this->setButtonText(WizardButton::FinishButton,"完 成");
    this->setButtonText(WizardButton::CancelButton,"取 消");
//! [2]
}
//! [1] //! [2]

//! [3]
void MnWizard::accept()
//! [3] //! [4]
{
    CMasternode cmn;

    QByteArray blocksafeconf;
    QByteArray blockmnconf;

    QByteArray mnalias = field("mnalias").toByteArray();

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

    cmn.m_alias = QString(mnalias);
    cmn.m_ip = QString(host);
    cmn.m_port = port.toShort();
    cmn.m_user = QString(user);
    cmn.m_pwd = QString(pwd);
    cmn.m_remote_rpc_user = QString(rpcuser);
    cmn.m_remote_rpc_pwd = QString(rpcpwd);
    cmn.m_remote_rpc_ip = cmn.m_ip;

    cmn.m_mn_key = QString(mnkey);
    cmn.m_clltrl_hash = QString(mnclltrlh);
    cmn.m_index = mnclltrlindex.toShort();
    cmn.m_safe_conf_path = QString(safeconf);
    cmn.m_mn_conf_path = QString(mnconf);

    emit sigMasternodeAdd(cmn);

    blocksafeconf += "rpcuser=" + rpcuser + "\n";
    blocksafeconf += "rpcpassword=" + rpcpwd + "\n";
    blocksafeconf += "rpcallowip=0.0.0.0/0\n";
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

    if (!headerFile.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,tr("提示"),
                     tr("保存文件时出现错误，请查看safeconf目录否被占用"));
        return;
    }

    headerFile.write(blocksafeconf);
    blocksafeconf.clear();
    headerFile.close();

    QString qMnconf = "";
    if(CRegistry::readDataDir(qMnconf))
    {
         QDir qDitMn;
         if (qDitMn.exists(qMnconf+"\\masternode.conf"))
         {
             QFile mnFile(qMnconf+"\\masternode.conf");
             if (!mnFile.open(QFile::Text | QFile::ReadWrite))
             {
                 QMessageBox::warning(this,tr("提示"),
                              tr("请查看masternode.conf是否存在，且未被其他程序占用。"));
                 return;
             }

             QByteArray qbData = mnFile.readAll();

             // 如果配置文件没有回车或者换行作为结尾，你们给他加一个。
             if (qbData.size() > 0)
             {
                 if (qbData.at(qbData.size()-1) != '\n'
                         && qbData.at(qbData.size()-1) != '\r' )
                 {
                     blockmnconf += "\n";
                 }
             }

             blockmnconf += QString(mnalias) + QString(" ") + QString(host)
                     + QString(":") + QString(port) + QString(" ");
             blockmnconf += QString(mnkey) + QString(" ") + QString(mnclltrlh)
                     + QString(" ") + QString(mnclltrlindex) + QString("\n");

             mnFile.seek(mnFile.size());
             mnFile.write(blockmnconf);
             mnFile.close();

             local_setting.mn_alias<<mnalias;  // 将别名放入别名列表，防止连续添加出现相同的别名
         }
         else
         {
             //qDebug()<<"can not find masternode.conf...";
             QMessageBox::warning(this,tr("提示"),
                          tr("请查看masternode.conf是否存在,如果不存在请重启钱包，或者手动添加。"));
         }
    }
    else
    {
        qDebug()<<"Can not find masternode.conf...";
        QMessageBox::warning(this,tr("提示"),
                     tr("未能从注册表中获取masternode.conf文件的位置。"\
                        "检查是否正确安装SAFE钱包。"));
    }

//! [5]
    QDialog::accept();
//! [5] //! [6]
}
//! [6]

//! [7]
IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("介绍"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/safe_banner.jpg"));

    label = new QLabel(tr("这个向导帮助用户配置Masternode的配置文件，会生成safe.conf和"
                          "masternode.conf。并帮助用户将文件放到合适的位置。免去繁琐的Linux"
                          "操作和配置文件编辑，用户只需要打开桌面钱包，输入正确的VPS服务器地址和"
                          "用户信息，把1000个safe打到指定地址。就可以完成Masternode的搭建。"));
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
    setTitle(tr("生成抵押SAFE的地址"));
    setSubTitle(tr("从钱包中生成地址,确保你的钱包处于运行状态。"
                   "将1000 SAFE 发送到这个地址."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/safe_logo.png"));

    GenAddressLabel = new QLabel(tr("设置masternote的别名(例如: mn1)"));
    AlliasLabel = new QLabel(tr("Masternode 别名:"));
    GenAddressLineEdit = new QLineEdit();
    AlliasLabel->setBuddy(GenAddressLineEdit);
    GencopypushButton = new QPushButton("生成地址");
    connect(GencopypushButton, &QPushButton::clicked,
            this, &AddressPage::generateAddr);

    //@todo 已经打过1000个safe,中断操作后，怎么继续？
    AddressLabel = new QLabel(tr("SAFE接收地址，先将1000个SAFE打到这个地址"));
    QLabel *aLabel  = new QLabel(tr("（如有可用的抵押交易，可跳过汇款）"));
    AddressLineEdit = new QLineEdit();
    AddressLineEdit->setEnabled(false);
    AddressLabel->setBuddy(AddressLineEdit);
    copypushButton = new QPushButton("拷贝");
    connect(copypushButton, &QPushButton::clicked, this, &AddressPage::copy);

    registerField("mnalias*", GenAddressLineEdit);
    registerField("address", AddressLineEdit);

    QGridLayout *qh1 = new QGridLayout;
    qh1->addWidget(GenAddressLabel,0,0);
    qh1->addWidget(GenAddressLineEdit,1,0,1,4);
    qh1->addWidget(GencopypushButton,1,4);
    qh1->addWidget(AddressLabel,2,0);
    qh1->addWidget(aLabel,3,0);
    qh1->addWidget(AddressLineEdit,4,0,1,5);
    qh1->addWidget(copypushButton,5,4);
    setLayout(qh1);
}

void AddressPage::generateAddr()
{
    // 先检查别名是否已经在Masternode列表里面
    QString mn_alias = GenAddressLineEdit->text();
    if (mn_alias == "")
    {
        QMessageBox::information(this,tr("提示"),tr("别名不能为空。"));
        return;
    }

    if (local_setting.mn_alias.contains(mn_alias))
    {
        // @todo show message to user
        qDebug("name is already in mansternodelist ");
        QMessageBox::information(this,tr("提示"),
                      tr("设置的别名已经在主节点列表里存在，请修改别名避免冲突"));
        GenAddressLineEdit->setFocus();
        return;
    }

    WalletRPC wallet("127.0.0.1",
                     local_setting.local_rpc_user,
                     local_setting.local_rpc_pwd);
    QString qsAdress = wallet.getaccountaddress(GenAddressLineEdit->text());
    if(qsAdress != "")
    {
        AddressLineEdit->setText(qsAdress);
    }
    else
    {
        QMessageBox::warning(this,tr("提示"),
                     tr("调用RPC获取地址失败，请检查SAFE钱包程序是否已经打开。"));
    }
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
    //AddressLineEdit->setText("XsxnV2NtwwAfiZLRcY74iccec57YJ6nAqE");
}

//! [8] //! [9]
ServerInfoPage::ServerInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
//! [8]
    setTitle(tr("Masternodes 服务端配置"));
    setSubTitle(tr("填写基本的Masternode信息，并确保VPS是开启状态，"
                   "填写的用户能够正常登陆。和有相应的权限，程序会帮助你生成配置文件。"));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/safe_logo.png"));

//! [10]
//!
    QLabel *MasternodeInfoLable = new QLabel(tr("填写远程服务器的基本信息"));
    HostLabel = new QLabel(tr(" &主机 IP:"));
    HostLineEdit = new QLineEdit;
    QRegExp iprx("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\."\
               "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."\
               "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."\
               "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");

    HostLineEdit->setValidator(new QRegExpValidator(iprx,this));
    HostLabel->setBuddy(HostLineEdit);

    PortLabel = new QLabel(tr("主机端口:"));
    PortLineEdit = new QLineEdit;
    PortLineEdit->setValidator(new QIntValidator(0,65536,PortLineEdit));
    PortLabel->setBuddy(PortLineEdit);

    UserLabel = new QLabel(tr(" &登陆用户名:"));
    UserLineEdit = new QLineEdit;
    UserLabel->setBuddy(UserLineEdit);

    PwdLabel = new QLabel(tr("登陆用户的密码:"));
    PwdLineEdit = new QLineEdit;
    PwdLineEdit->setEchoMode(QLineEdit::Password);
    PwdLineEdit->setMaxLength(32);
    PwdLabel->setBuddy(PwdLineEdit);

    RpcUserLabel = new QLabel(tr("远程RPC用户名:"));
    RpcUserLineEdit = new QLineEdit;
    RpcUserLabel->setBuddy(RpcUserLineEdit);

    RpcPwdPwdLabel = new QLabel(tr("远程RPC密码:"));
    RpcPwdLineEdit = new QLineEdit;
    RpcPwdLineEdit->setEchoMode(QLineEdit::Password);
    RpcPwdLineEdit->setMaxLength(32);
    RpcPwdPwdLabel->setBuddy(RpcPwdLineEdit);

    RpcIpLabel = new QLabel(tr("远程RPCIP:"));
    RpcIpLineEdit = new QLineEdit;
    //RpcIpLineEdit->setValidator(new QRegExpValidator(iprx,this));
    RpcIpLabel->setBuddy(RpcIpLineEdit);

    RpcLable = new QLabel(tr("远程RPC配置"));

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
    PortLineEdit->setEnabled(false);
    RpcUserLineEdit->setText(local_setting.local_rpc_user);
    RpcPwdLineEdit->setText(local_setting.local_rpc_pwd);
    RpcIpLineEdit->setText("0.0.0.0/0");
}

MasternodeInfoPage::MasternodeInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("MasterNode 属性"));
    setSubTitle(tr("配置关于Masternode的一些关键参数，这些由程序自动执行，"
                   "但必须确保桌面钱包处于运行状态。"));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/safe_logo.png"));

    protectLable = new QLabel(tr("生成Masternode 秘钥（没有花费能力，"\
                                 "不会影响1000个SAFE的使用）."));
    MasternodeKeyLabel = new QLabel(tr("&Masternode秘钥:"));
    MasternodeKeyLineEdit = new QLineEdit;
    MasternodeKeyLabel->setBuddy(MasternodeKeyLineEdit);

    includeBaseLable = new QLabel(tr("获取1000个SAFE抵押的交易哈希"));
    CollateralHashLabel = new QLabel(tr("抵押交易哈希:"));
    CollateralHashComboBox = new QComboBox;
    CollateralHashLineEdit = new QLineEdit;
    CollateralHashLabel->setBuddy(CollateralHashLineEdit);
    IndexLable= new QLabel(tr("抵押交易哈希的索引:"));
    IndexLineEdit = new QLineEdit;
    IndexLineEdit->setMaximumWidth(120);
    IndexLineEdit->setValidator(new QIntValidator(0,10000,IndexLineEdit));
    IndexLable->setBuddy(IndexLineEdit);

    connect(CollateralHashComboBox,
            &QComboBox::currentTextChanged,CollateralHashLineEdit,
            [=](){
                    CollateralHashLineEdit->setText(CollateralHashComboBox->currentText());
                    IndexLineEdit->setText(
                                local_setting.mn_new_info[CollateralHashComboBox->currentText()]);
                    qDebug()<<"hash:"<<CollateralHashComboBox->currentText();
            });

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

#ifdef _Debug
    registerField("masternodekey", MasternodeKeyLineEdit);
    registerField("collateralhash", CollateralHashLineEdit);
    registerField("collateralindex", IndexLineEdit);
#else
    registerField("masternodekey*", MasternodeKeyLineEdit);
    registerField("collateralhash*", CollateralHashLineEdit);
    registerField("collateralindex*", IndexLineEdit);
#endif

//! [15]
    setLayout(layout);
}
//! [15]

//! [16]
void MasternodeInfoPage::initializePage()
{
    WalletRPC walletRpc("127.0.0.1",
                        local_setting.local_rpc_user,
                        local_setting.local_rpc_pwd);

    QString qsGenKey = walletRpc.masternodeGenkey();
    if (qsGenKey != "")
    {
        MasternodeKeyLineEdit->setText(qsGenKey);
    }
    else
    {
        // @todo something;
        QMessageBox::warning(this,tr("提示"),
                     tr("请检查SAFE钱包程序是否已经打开。"));
    }

    QJsonObject qjsonOutput = walletRpc.masternodeOutputs();

    for (QJsonObject::Iterator it = qjsonOutput.begin();
         it!=qjsonOutput.end();it++)
    {
        qDebug()<<"key:"<<it.key();
        qDebug()<<"value:"<<it.value().toString();

        if (local_setting.mn_old_info.find(it.key())
                != local_setting.mn_old_info.end())
        {
            qDebug()<<"old key:"<<it.key();
        }
        else
        {
            qDebug()<<"new key:"<<it.key();
            CollateralHashComboBox->addItem(it.key());
        }
        local_setting.mn_new_info[it.key()] = it.value().toString();
    }

#ifdef _Debug
    CollateralHashComboBox->addItem("Testhas 1");
    local_setting.mn_new_info["Testhas 1"] ="250";
    CollateralHashComboBox->addItem("Testhas 2");
    local_setting.mn_new_info["Testhas 2"] = "251";
    CollateralHashComboBox->addItem("Testhas 3");
    local_setting.mn_new_info["Testhas 3"] = "252";
#endif

    CollateralHashLineEdit->setText(CollateralHashComboBox->currentText());
    IndexLineEdit->setText(local_setting.mn_new_info[
                           CollateralHashComboBox->currentText()
            ]);
}
//! [16]

OutputFilesPage::OutputFilesPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("配置文件的输出路径"));
    setSubTitle(tr("这是配置文件将要输出的路径，如果手动修改，后续的工作将无法完成."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/safe_logo.png"));

    outputDirLabel = new QLabel(tr("safe.conf的输出路径:"));
    SafeConfLineEdit = new QLineEdit;
    outputDirLabel->setBuddy(SafeConfLineEdit);

    headerLabel = new QLabel(tr("masternode.conf的输出路径:"));
    MasternodeConfLineEdit = new QLineEdit;
    headerLabel->setBuddy(MasternodeConfLineEdit);

    registerField("safeconf*", SafeConfLineEdit);
    registerField("masternodeconf*", MasternodeConfLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(outputDirLabel, 0, 0);
    layout->addWidget(SafeConfLineEdit, 1, 0,1,3);
    layout->addWidget(headerLabel, 2, 0);
    layout->addWidget(MasternodeConfLineEdit, 3, 0,1,3);

    setLayout(layout);
}

//! [17]
void OutputFilesPage::initializePage()
{
    QString userName = field("user").toString();
    SafeConfLineEdit->setEnabled(false);
    if(userName.compare("root",Qt::CaseInsensitive))
    {
        SafeConfLineEdit->setText(QString("/home/")+userName+QString("/safe.conf"));
        local_setting.remote_script_path = QString("/home/")+userName+QString("/");
    }
    else
    {
        SafeConfLineEdit->setText(QString("/")+userName+QString("/safe.conf"));
        local_setting.remote_script_path = QString("/") + userName+QString("/");
    }

    QString qMnconf = "";
    MasternodeConfLineEdit->setEnabled(false);
    if(CRegistry::readDataDir(qMnconf))
    {
        qMnconf += "\\masternode.conf";
        MasternodeConfLineEdit->setText(qMnconf);
    }
    else
    {
        //@TODO show file dialog user chonse
        QMessageBox::warning(this,tr("提示"),
                     tr("请查看masternode.conf是否存在,如果不存在请重启钱包，或者手动添加。"));
    }
}
//! [17]

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("结 束"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/safe_banner.jpg"));

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
    label->setText(tr("点击 “%1” 生成配置文件.")
                   .arg(finishText));
}
