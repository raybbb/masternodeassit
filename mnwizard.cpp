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
#include "mnwizard.h"

//! [0] //! [1]
MnWizard::MnWizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new IntroPage);
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
    QByteArray className = field("className").toByteArray();
    QByteArray baseClass = field("baseClass").toByteArray();
    QByteArray macroName = field("macroName").toByteArray();
    QByteArray baseInclude = field("baseInclude").toByteArray();

    QString outputDir = field("outputDir").toString();
    QString header = field("header").toString();
    QString implementation = field("implementation").toString();
//! [4]

    QByteArray block;

    if (field("comment").toBool()) {
        block += "/*\n";
        block += "    " + header.toLatin1() + '\n';
        block += "*/\n";
        block += '\n';
    }
    if (field("protect").toBool()) {
        block += "#ifndef " + macroName + '\n';
        block += "#define " + macroName + '\n';
        block += '\n';
    }
    if (field("includeBase").toBool()) {
        block += "#include " + baseInclude + '\n';
        block += '\n';
    }

    block += "class " + className;
    if (!baseClass.isEmpty())
        block += " : public " + baseClass;
    block += '\n';
    block += "{\n";

    /* qmake ignore Q_OBJECT */

    if (field("qobjectMacro").toBool()) {
        block += "    Q_OBJECT\n";
        block += '\n';
    }
    block += "public:\n";

    if (field("qobjectCtor").toBool()) {
        block += "    " + className + "(QObject *parent = 0);\n";
    } else if (field("qwidgetCtor").toBool()) {
        block += "    " + className + "(QWidget *parent = 0);\n";
    } else if (field("defaultCtor").toBool()) {
        block += "    " + className + "();\n";
        if (field("copyCtor").toBool()) {
            block += "    " + className + "(const " + className + " &other);\n";
            block += '\n';
            block += "    " + className + " &operator=" + "(const " + className
                     + " &other);\n";
        }
    }
    block += "};\n";

    if (field("protect").toBool()) {
        block += '\n';
        block += "#endif\n";
    }

    QFile headerFile(outputDir + '/' + header);
    if (!headerFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Simple Wizard"),
                             QObject::tr("Cannot write file %1:\n%2")
                             .arg(headerFile.fileName())
                             .arg(headerFile.errorString()));
        return;
    }
    headerFile.write(block);

    block.clear();

    if (field("comment").toBool()) {
        block += "/*\n";
        block += "    " + implementation.toLatin1() + '\n';
        block += "*/\n";
        block += '\n';
    }
    block += "#include \"" + header.toLatin1() + "\"\n";
    block += '\n';

    if (field("qobjectCtor").toBool()) {
        block += className + "::" + className + "(QObject *parent)\n";
        block += "    : " + baseClass + "(parent)\n";
        block += "{\n";
        block += "}\n";
    } else if (field("qwidgetCtor").toBool()) {
        block += className + "::" + className + "(QWidget *parent)\n";
        block += "    : " + baseClass + "(parent)\n";
        block += "{\n";
        block += "}\n";
    } else if (field("defaultCtor").toBool()) {
        block += className + "::" + className + "()\n";
        block += "{\n";
        block += "    // missing code\n";
        block += "}\n";

        if (field("copyCtor").toBool()) {
            block += "\n";
            block += className + "::" + className + "(const " + className
                     + " &other)\n";
            block += "{\n";
            block += "    *this = other;\n";
            block += "}\n";
            block += '\n';
            block += className + " &" + className + "::operator=(const "
                     + className + " &other)\n";
            block += "{\n";
            if (!baseClass.isEmpty())
                block += "    " + baseClass + "::operator=(other);\n";
            block += "    // missing code\n";
            block += "    return *this;\n";
            block += "}\n";
        }
    }

    QFile implementationFile(outputDir + '/' + implementation);
    if (!implementationFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Simple Wizard"),
                             QObject::tr("Cannot write file %1:\n%2")
                             .arg(implementationFile.fileName())
                             .arg(implementationFile.errorString()));
        return;
    }
    implementationFile.write(block);

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

//! [8] //! [9]
ServerInfoPage::ServerInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
//! [8]
    setTitle(tr("Masternodes Information"));
    setSubTitle(tr("Specify basic information about the Masternode for which you "
                   "want to generate configuration files."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo1.png"));

//! [10]
//!
    QLabel *MasternodeInfoLable = new QLabel(tr("Fill the Masternode vps infomation."));
    HostLabel = new QLabel(tr(" &Host Name:"));
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
    RpcIpLineEdit->setValidator(new QRegExpValidator(iprx,this));
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
    qh1->addWidget(RpcIpLabel,2,0);
    qh1->addWidget(RpcIpLineEdit,2,1);
    QVBoxLayout *groupBoxLayout = new QVBoxLayout;

    groupBoxLayout->addLayout(qh1);

//! [11] //! [12]
    registerField("host", HostLineEdit);
    registerField("port", PortLineEdit);
    registerField("user", UserLineEdit);
    registerField("pwd", PwdLineEdit);
    registerField("rpcUser", RpcUserLineEdit);
    registerField("rpcPwd", RpcPwdLineEdit);
    registerField("rpcIp", RpcIpLineEdit);
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


MasternodeInfoPage::MasternodeInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("MasterNode Attribute"));
    setSubTitle(tr("Configure the most importance attribute to masternode. Make sure your desktop wallet is running..."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo2.png"));

    protectLable = new QLabel(tr("Get the Masternode key use cmd: \'masternode genkey\' under Debug console."));
    MasternodeKeyLabel = new QLabel(tr("&Masternode Key:"));
    MasternodeKeyLineEdit = new QLineEdit;
    MasternodeKeyLabel->setBuddy(MasternodeKeyLineEdit);

    includeBaseLable = new QLabel(tr("Get the Masternode key use cmd: \'masternode output\' under Debug console."));
    CollateralHashLabel = new QLabel(tr("Collateral Hash:"));
    CollateralHashLineEdit = new QLineEdit;
    CollateralHashLabel->setBuddy(CollateralHashLineEdit);
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
    layout->addWidget(CollateralHashLineEdit, 3, 2);
    layout->addWidget(IndexLable, 4, 1);
    layout->addWidget(IndexLineEdit, 4, 2);

    registerField("masternodekey", MasternodeKeyLineEdit);
    registerField("collateralhash", CollateralHashLineEdit);
    registerField("collateralindex", IndexLineEdit);

//! [15]
    setLayout(layout);
}
//! [15]

//! [16]
void MasternodeInfoPage::initializePage()
{
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
}
//! [16]

OutputFilesPage::OutputFilesPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Output Configure file"));
    setSubTitle(tr("Specify where you want the wizard to put the generated "
                   "files."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo3.png"));

    outputDirLabel = new QLabel(tr("&safe.conf output directory:"));
    SafeConfLineEdit = new QLineEdit;
    outputDirLabel->setBuddy(SafeConfLineEdit);

    headerLabel = new QLabel(tr("&masternode.conf output directory:"));
    MasternodeConfLineEdit = new QLineEdit;
    headerLabel->setBuddy(MasternodeConfLineEdit);

    registerField("safeconf", SafeConfLineEdit);
    registerField("masternodeconf", MasternodeConfLineEdit);

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
    SafeConfLineEdit->setText(QString("/home/")+userName+QString("/.safe/safe.conf"));
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
