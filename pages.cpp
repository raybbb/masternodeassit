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

#include "pages.h"

ConfigurationPage::ConfigurationPage(QWidget *parent)
    : QWidget(parent)
{
    QGroupBox *configGroup = new QGroupBox(tr("Server configuration"));

    QGridLayout *packagesLayout = new QGridLayout;
    QLabel *serverLabel = new QLabel(tr("Server:"));
    QLineEdit *serverEdit = new QLineEdit ;
    QLabel *portLabel = new QLabel(tr("port:"));
    QLineEdit *portEdit = new QLineEdit ;

    QLabel *userLabel = new QLabel(tr("User:"));
    QLineEdit *userEdit = new QLineEdit ;
    QLabel *pwdLabel = new QLabel(tr("Password:"));
    QLineEdit *pwdEdit = new QLineEdit ;
    packagesLayout->addWidget(serverLabel,0,0);
    packagesLayout->addWidget(serverEdit,0,1);
    packagesLayout->addWidget(portLabel,1,0);
    packagesLayout->addWidget(portEdit,1,1);
    packagesLayout->addWidget(userLabel,2,0);
    packagesLayout->addWidget(userEdit,2,1);
    packagesLayout->addWidget(pwdLabel,3,0);
    packagesLayout->addWidget(pwdEdit,3,1);

    QVBoxLayout *configLayout = new QVBoxLayout;
    configLayout->addLayout(packagesLayout);
    configGroup->setLayout(configLayout);

    QGroupBox *masternodeGroup = new QGroupBox(tr("Masternode Configure"));
    QVBoxLayout *mnLayout= new QVBoxLayout;


    QHBoxLayout *hvthash = new QHBoxLayout;
    QLabel *collaterLable = new QLabel("Collater Hash");
    hvthash->addWidget(collaterLable);
    hvthash->addStretch(1);

    QTextEdit *collateralEdit = new QTextEdit;
    collateralEdit->setMaximumHeight(50);

    QHBoxLayout *hvtaddr = new QHBoxLayout;
    QLabel *addrLable = new QLabel("Reward Address");
    hvtaddr->addWidget(addrLable);
    hvtaddr->addStretch(1);

    QTextEdit *addrEdit = new QTextEdit;
    addrEdit->setMaximumHeight(50);

    mnLayout->addLayout(hvthash);
    mnLayout->addWidget(collateralEdit);
    mnLayout->addLayout(hvtaddr);
    mnLayout->addWidget(addrEdit);

    masternodeGroup->setLayout(mnLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(configGroup);
    mainLayout->addWidget(masternodeGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}
