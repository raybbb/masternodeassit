#include "uimnmain.h"
#include "ui_uimnmain.h"
#include "mnlibssh2.h"
#include "clocalsetting.h"
#include "cregistry.h"
#include "localrpcdialog.h"
#include "changedialog.h"

#include <QDesktopWidget>
#include <QDateTime>
#include <QFile>
#include <QDebug>

#include <QEvent>
#include <QMouseEvent>
#include <QApplication>

#ifdef Q_OS_WIN
    #pragma comment(lib, "user32.lib")
    #include <qt_windows.h>
#endif

QString ARR_MN_FILDNAME[] = {"编号","别名","地址","端口","状态"};

UIMnMain::UIMnMain(QWidget *parent):
    QDialog(parent),
    ui(new Ui::UIMnMain)
{
    ui->setupUi(this);
    this->initForm();

    S_MNSTATUS<<QString("UNLOAD")<<QString("UPLOADED")<<QString("STARTED")<<QString("NEW_START_REQUIRED")
             <<QString("ENABLED")<<QString("EXPIRED")<<QString("MISSING");

    connect(&mnwizard, &MnWizard::sigMasternodeAdd, this, &UIMnMain::recvMnInfo);
    connect(&changedialog, &ChangeDialog::sigMasternodeChange, this, &UIMnMain::recvChangeMnInfo);
    connect(&mns, &CStartMasternode::sigMasternodeFinishStart, this, &UIMnMain::mnSetupComplete);

    qApp->setStyleSheet("QTableCornerButton::section{background-color:rgba(232,255,213,5);}");

    QFile file(":/images/s.qss");
    if (file.open(QFile::ReadOnly))
    {
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
        file.close();
    }
    else
    {
        qDebug("open file faild...");
    }

    showProcessMessage("启动 SAFE MASTERNODE 搭建工具", E_MESSAGE);

    initTableWidget();
    initSetting();
    initDatabase();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ShowInfoMessage()));
    connect(timer, SIGNAL(timeout()), this, SLOT(ShowMasternodeStatusMessage()));
}

void UIMnMain::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WIN
    if (ReleaseCapture())
    {
        QWidget *pWindow = this->window();
        if (pWindow->isTopLevel())
        {
           SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
    event->ignore();
    #else
    #endif
}

void UIMnMain::ShowMasternodeStatusMessage()
{
    /*
    WalletRPC wallet(local_setting.remote_rpc_ip,
                     local_setting.remote_rpc_user,
                     local_setting.remote_rpc_pwd);

    */

    WalletRPC wallet("127.0.0.1",
                     local_setting.local_rpc_user,
                     local_setting.local_rpc_pwd);

    ui->textEdit_log->append("======================================================================");
    ui->textEdit_log->append("Masternode节点信息：");

    QJsonObject qjObj = wallet.masternodeStatus();
    //QJsonObject qjObj = wallet.getinfo();

    if(qjObj.size() > 0)
    {
        for (QJsonObject::Iterator it = qjObj.begin();
             it!=qjObj.end();it++)
        {
            qDebug()<<"key:"<<it.key() << ": " <<it.value();

            if (it.value().isDouble())
            {
                ui->textEdit_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                         +"\t" + it.key()+" : "+ QString("%1").arg(it.value().toDouble()) );
            }
            else if (it.value().isString())
            {
                ui->textEdit_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                         +"\t" + it.key()+" : "+it.value().toString());
            }
        }
    }
    else
    {
        ui->textEdit_log->append("无~");
    }

}

void UIMnMain::ShowInfoMessage()
{
    WalletRPC wallet("127.0.0.1",
                     local_setting.local_rpc_user,
                     local_setting.local_rpc_pwd);

    QJsonObject qjObj = wallet.getinfo();

    ui->textEdit_log->append("======================================================================");
    ui->textEdit_log->append("区块信息：");
    if (qjObj.size()>0)
    {
        for (QJsonObject::Iterator it = qjObj.begin();
             it!=qjObj.end();it++)
        {
            qDebug()<<"key:"<<it.key() << ": " <<it.value();

            if (it.value().isDouble())
            {
                ui->textEdit_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                         +"\t" + it.key()+" : "+ QString("%1").arg(it.value().toDouble()) );
            }
            else if (it.value().isString())
            {
                ui->textEdit_log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                         +"\t" + it.key()+" : "+it.value().toString());
            }
        }
    }
    else
    {
         ui->textEdit_log->append("无~");
    }

    //ui->textEdit_log->append("======================================================================");
}


void UIMnMain::initDatabase()
{
    mydb.createMnTable();
}

void UIMnMain::initSetting()
{
    //@todo read rpc from /safe.conf
    local_setting.new_safe_conf_files_path = "./safeconf/";
    local_setting.local_script_path = "./script/";
    local_setting.install_script = "auto_install_safe.py";
    local_setting.start_script = "masternode_start.sh";
    local_setting.restart_script = "masternode_restart.sh";
    local_setting.stop_script = "masternode_stop.sh";

    if(!CRegistry::readDataDir(local_setting.safe_conf_path))
    {
        QMessageBox::warning(this,tr("提示"),
                     tr("未能从注册表中获取钱包路径，请查看是否正确安装钱包。"));
        exit(0);
    }

    showProcessMessage(QString("钱包路径是 %1")
                       .arg(local_setting.safe_conf_path), E_MESSAGE);
    QString localSafeConfFile = local_setting.safe_conf_path + "\\safe.conf";
    QFile file(localSafeConfFile);
    if(file.exists())
    {
        showProcessMessage(QString("配置文件 %1")
                           .arg(localSafeConfFile), E_MESSAGE);

        int hasConfigRpc = 0;
        if(file.open(QIODevice::ReadWrite))
        {
            QByteArray safe_conf_line =  file.readLine();
            while(safe_conf_line.size())
            {
                if (safe_conf_line.at(0) == '#')
                {
                    safe_conf_line =  file.readLine();
                    continue;
                }

                if (safe_conf_line.startsWith("rpcuser"))
                {
                    int begin = safe_conf_line.indexOf("=",0);
                    int end = safe_conf_line.indexOf("\r",0);
                    if (begin < --end)
                    {
                        local_setting.local_rpc_user = safe_conf_line.mid(++begin,--end);
                        local_setting.local_rpc_user.replace("\r\n", "\0");
                        hasConfigRpc ++;
                        showProcessMessage(QString("RPC用户名 %1")
                                           .arg(local_setting.local_rpc_user), E_MESSAGE);
                    }
                }
                if (safe_conf_line.startsWith("rpcpassword"))
                {
                    int begin = safe_conf_line.indexOf("=",0);
                    int end = safe_conf_line.indexOf("\r",0);
                    if (begin < --end)
                    {
                        local_setting.local_rpc_pwd = safe_conf_line.mid(++begin,--end);
                        local_setting.local_rpc_pwd.replace("\r\n", "\0");
                        hasConfigRpc ++;
                        showProcessMessage(QString("RPC密码 %1")
                                           .arg(local_setting.local_rpc_pwd), E_MESSAGE);
                    }
                }
                safe_conf_line =  file.readLine();
            }
            file.close();
        }

        if(hasConfigRpc != 2)
        {
            showProcessMessage("没有检查到RPC的相关配置", E_ERROR);
            LocalrpcDialog llrpc;
            llrpc.exec();
        }
    }
    else
    {
        QMessageBox::warning(this,tr("提示"),tr("safe.conf 丢失。"));
    }

    local_setting.masternode_conf_file
            = local_setting.safe_conf_path + "/masternode.conf";

    QFile mnfile(local_setting.masternode_conf_file);
    if(mnfile.exists())
    {
        if(mnfile.open(QIODevice::ReadWrite))
        {
            while(true)
            {
                QByteArray mn_conf_line =  mnfile.readLine();
                if(mn_conf_line.size()>0)
                {
                    QList<QByteArray> qList = mn_conf_line.split(' ');
                    if(qList.size() > 4)
                    {
                        local_setting.mn_old_info[qList[3]] = qList[4]; // 保存hash与索引
                        local_setting.mn_alias<<qList[0];  // 保存别名
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            QMessageBox::information(this,tr("提示"),
                         tr("打开masternode.conf配置文件失败，请检查改文件是否被占用。"));
        }
    }
    else
    {
        QMessageBox::information(this,tr("提示"),
                     tr("未找到masternode.conf文件。"));
    }

}

UIMnMain::~UIMnMain()
{
    if (mns.isRunning())
    {
        mns.terminate();
    }
    delete ui;
}

void UIMnMain::initTableWidget()
{
    /*
    //设置列数和列宽
    int width = qApp->desktop()->availableGeometry().width() - 120;
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setColumnWidth(0, width * 0.06);
    ui->tableWidget->setColumnWidth(1, width * 0.10);
    ui->tableWidget->setColumnWidth(2, width * 0.16);
    ui->tableWidget->setColumnWidth(3, width * 0.05);
    ui->tableWidget->setColumnWidth(4, width * 0.10);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(25);

    QStringList headText;
    headText << "编号" << "别名" << "地址" << "端口" << "状态";
    ui->tableWidget->setHorizontalHeaderLabels(headText);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
*/
    /*
    //设置行高
    ui->tableWidget->setRowCount(40);

    for (int i = 0; i < 40; i++) {
        ui->tableWidget->setRowHeight(i, 24);

        QTableWidgetItem *itemDeviceID = new QTableWidgetItem(QString::number(i + 1));
        QTableWidgetItem *itemDeviceName = new QTableWidgetItem(QString("MN%1").arg(i + 1));
        QTableWidgetItem *itemDeviceAddr = new QTableWidgetItem(QString("192.168.18.%1").arg(i + 1));
        QTableWidgetItem *itemContent = new QTableWidgetItem("5555");
        //QTableWidgetItem *itemTime = new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        QTableWidgetItem *itemTime = new QTableWidgetItem("ENABLE");

        itemDeviceID->setTextAlignment(Qt::AlignCenter);
        itemDeviceName->setTextAlignment(Qt::AlignCenter);
        itemDeviceAddr->setTextAlignment(Qt::AlignCenter);
        itemContent->setTextAlignment(Qt::AlignCenter);
        itemTime->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(i, 0, itemDeviceID);
        ui->tableWidget->setItem(i, 1, itemDeviceName);
        ui->tableWidget->setItem(i, 2, itemDeviceAddr);
        ui->tableWidget->setItem(i, 3, itemContent);
        ui->tableWidget->setItem(i, 4, itemTime);

    }
    */
}

void UIMnMain::initForm()
{
    this->max = false;
    this->location = this->geometry();
    this->setProperty("form", true);
    this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    //ui->widgetTitle->setVisible(false);

    ui->labTitle->setText("Masternode快速搭建工具");
    this->setWindowTitle("Masternode快速搭建工具");

    QList<QToolButton *> btns = ui->widgetLeft->findChildren<QToolButton *>();
    foreach (QToolButton *btn, btns) {
        btn->setMaximumHeight(80);
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(buttonClick()));
    }
}

void UIMnMain::insertTableWidgetItem(CMasternode cmn)
{
    /*
    int nRc = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(++nRc);
    ui->tableWidget->setRowHeight(nRc, 24);

    QTableWidgetItem *itemDeviceID = new QTableWidgetItem(QString::number(nRc));
    QTableWidgetItem *itemDeviceName = new QTableWidgetItem(cmn.m_alias);
    QTableWidgetItem *itemDeviceAddr = new QTableWidgetItem(cmn.m_ip);
    QTableWidgetItem *itemContent = new QTableWidgetItem(QString("%1").arg(cmn.m_port));
    //QTableWidgetItem *itemTime = new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    QTableWidgetItem *itemTime = new QTableWidgetItem(cmn.m_status);

    itemDeviceID->setTextAlignment(Qt::AlignCenter);
    itemDeviceName->setTextAlignment(Qt::AlignCenter);
    itemDeviceAddr->setTextAlignment(Qt::AlignCenter);
    itemContent->setTextAlignment(Qt::AlignCenter);
    itemTime->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(nRc-1, 0, itemDeviceID);
    ui->tableWidget->setItem(nRc-1, 1, itemDeviceName);
    ui->tableWidget->setItem(nRc-1, 2, itemDeviceAddr);
    ui->tableWidget->setItem(nRc-1, 3, itemContent);
    ui->tableWidget->setItem(nRc-1, 4, itemTime);
    ui->tableWidget->selectRow(nRc);
    */

}

void UIMnMain::removeTableWidgetItem(CMasternode cmn)
{
}

void UIMnMain::removeTableWidgetItem(int nIndex)
{
    //ui->tableWidget->removeRow(nIndex);
}

void UIMnMain::buttonClick()
{
    QToolButton *b = (QToolButton *)sender();
    QString text = b->text();    

    QList<QToolButton *> btns = ui->widgetLeft->findChildren<QToolButton *>();
    foreach (QToolButton *btn, btns) {
        QString icoName = btn->property("icoName").toString();
        if (btn != b) {
            btn->setChecked(false);
            btn->setIcon(QIcon(QString(":/image/%1.png").arg(icoName)));
        } else {
            btn->setChecked(true);
            btn->setIcon(QIcon(QString(":/image/%1_focus.png").arg(icoName)));
        }
    }
}

void UIMnMain::recvMnInfo(const CMasternode &cmn)
{
    mMasternodes.insert(pair<QString, CMasternode>(cmn.m_ip, cmn));
    mMasternodes[cmn.m_ip].m_status = S_MNSTATUS[UNLOAD];

    //insertTableWidgetItem(mMasternodes[cmn.m_ip]);
    show_masternode(mMasternodes[cmn.m_ip]);

    // 设置按钮可以上传，不可添加,可以修改
    ui->pb_upload->setEnabled(true);
    ui->pb_rechange->setEnabled(true);
    ui->pb_add->setEnabled(false);

    current_ip = cmn.m_ip;
    g_current_ip = cmn.m_ip;

    local_setting.remote_rpc_ip = cmn.m_remote_rpc_ip;
    local_setting.remote_rpc_user = cmn.m_remote_rpc_user;
    local_setting.remote_rpc_pwd = cmn.m_remote_rpc_pwd;
    local_setting.safe_conf_file = cmn.m_safe_conf_path;
    local_setting.masternode_conf_path = cmn.m_mn_conf_path;

    showLocalSetting();

    QByteArray tmp_array = CMasternode::Serializable(mMasternodes[cmn.m_ip]);
    mydb.addMn(cmn.m_ip, tmp_array);
}

void UIMnMain::recvChangeMnInfo(const CMasternode &cmn)
{
    if (g_current_ip != cmn.m_ip)
    {
        mydb.delMn(g_current_ip);
        g_current_ip = cmn.m_ip;
        current_ip = cmn.m_ip;
        mydb.addMn(g_current_ip, CMasternode::Serializable(cmn));
    }
    else
    {
        mydb.updateMn(g_current_ip, CMasternode::Serializable(cmn));
    }

    // 修改后，变成可上传
    ui->pb_upload->setEnabled(true);
    show_masternode(cmn);
}

// Masternode节点完成安装
void UIMnMain::mnSetupComplete()
{
    ui->pb_add->setEnabled(true);
}

void UIMnMain::showProcessMessage(const QString &msg, E_MESSAGE_LEVEL ml)
{
    QString qtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    switch(ml)
    {
    case E_MESSAGE:
        ui->textEdit_log->append("<div style='color:#008000'>"+ qtime + "  [MSG]"
                                 + "</div>" + "  "
                                 + "<h style='color:#3CB371'>" + msg + "</h>");
        break;
    case E_ERROR:
        ui->textEdit_log->append("<div style='color:#FF0000'>"+ qtime  + "  [ERR]"
                                 + "</div>" + "  "
                                 + "<h style='color:#FF0000'>" + msg + "</h>");
        break;
    case E_INFO:
        ui->textEdit_log->append("<div style='color:#008000'>"+ qtime + " [INFO]"
                                 + "</div>" + "  "
                                 + "<h style='color:#D3D3D3'>" + msg + "</h>");
    break;
    default:
        break;
    }

    QTextCursor cursor = ui->textEdit_log->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit_log->setTextCursor(cursor);

}

void UIMnMain::showLocalSetting()
{
    QString qsHtml = "本地配置信息";
    qsHtml.append("<hr>");
    qsHtml.append("<table width='80%' border='0.1' cellspacing='50%' cellpadding='100%'>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("钱包目录");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.safe_conf_path);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("SAFE配置文件");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.safe_conf_file);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("临时SAFE配置文件");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.new_safe_conf_files_path);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("Masternode配置文件");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.safe_conf_path);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("本地RPC用户名称");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.local_rpc_user);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("本地RPC用户密码");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.local_rpc_pwd);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("远程RPC用户名称");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.remote_rpc_user);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("远程RPC用户密码");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.remote_rpc_pwd);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("远程RPC IP");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.remote_rpc_ip);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("本地脚本目录");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.local_script_path);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("远程脚本目录");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.remote_script_path);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("安装脚本");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.install_script);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("启动脚本");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.start_script);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("重启脚本");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.restart_script);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("停止脚本");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(local_setting.stop_script);
    qsHtml.append("</td>");
    qsHtml.append("</tr>");

    qsHtml.append("</table>");
    showProcessMessage(qsHtml, E_MESSAGE);
}

void UIMnMain::ShowMnToMessageBox(const CMasternode &cmn)
{

}


void UIMnMain::show_masternode(const CMasternode &cmn)
{
    QString qsHtml = "主节点信息";
    qsHtml.append("<hr>");
    qsHtml.append("<table width='100%' border='0.1' cellspacing='50%' cellpadding='100%'>");
    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("别名");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(cmn.m_alias);
    qsHtml.append("</td>");qsHtml.append("</tr>");

    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("服务器");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(cmn.m_ip);
    qsHtml.append("</td>");qsHtml.append("</tr>");

    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("端口");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(QString::number(cmn.m_port));
    qsHtml.append("</td>");qsHtml.append("</tr>");

    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("交易哈希");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(cmn.m_clltrl_hash);
    qsHtml.append("</td>");qsHtml.append("</tr>");

    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("节点秘钥");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(cmn.m_mn_key);
    qsHtml.append("</td>");qsHtml.append("</tr>");

    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("状态");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(cmn.m_status);
    qsHtml.append("</td>");qsHtml.append("</tr>");

    qsHtml.append("</table>");
    ui->textEdit->setHtml(qsHtml);

    showProcessMessage(qsHtml, E_MESSAGE);
}

void UIMnMain::on_btnMenu_Min_clicked()
{
    showMinimized();
}

void UIMnMain::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(location);
        this->setProperty("canMove", true);
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        this->setProperty("canMove", false);
    }
    max = !max;
}

void UIMnMain::on_btnMenu_Close_clicked()
{
    close();
}

void UIMnMain::on_pb_add_clicked()
{
    mnwizard.exec();
    mnwizard.restart();
}

void UIMnMain::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
}

void UIMnMain::on_tableWidget_clicked(const QModelIndex &index)
{
    /*
    int iRow = ui->tableWidget->currentRow();
    int iC = ui->tableWidget->columnCount();
    QString qsHtml = "<th>Masternoede Info</th>";
    qsHtml.append("<hr>");
    qsHtml.append("<table border='0.1' width='100%'>");
    for(int i=0; i<iC; i++)
    {
        QTableWidgetItem *qtwim = ui->tableWidget->item(iRow, i);
        qsHtml.append("<tr>");
        qsHtml.append("<td>");
        qsHtml.append(ARR_MN_FILDNAME[i]);
        qsHtml.append("</td>");
        qsHtml.append("<td>");
        qsHtml.append(qtwim->data(Qt::DisplayRole).toString());
        qsHtml.append("</td>");
        qsHtml.append("</tr>");
    }
    qsHtml.append("</table>");
    ui->textEdit->setHtml(qsHtml);
    current_ip = ui->tableWidget->item(iRow, 2)->data(Qt::DisplayRole).toString();

    //@todo read data from database or map
    CMasternode cmn = mMasternodes[current_ip];
    local_setting.remote_rpc_ip = cmn.m_remote_rpc_ip;
    local_setting.remote_rpc_user = cmn.m_remote_rpc_user;
    local_setting.remote_rpc_pwd = cmn.m_remote_rpc_pwd;
    local_setting.safe_conf_file = cmn.m_safe_conf_path;
    local_setting.masternode_conf_path = cmn.m_mn_conf_path;
    */
}

void UIMnMain::on_pb_remove_clicked()
{
    //removeTableWidgetItem(ui->tableWidget->currentRow());
}

void UIMnMain::on_pb_upload_clicked()
{
     if (current_ip != "")
     {
         ui->pb_upload->setEnabled(false);
         mn_libssh2 ssh2;
         ssh2.mn_init();

         //CMasternode cmd = mMasternodes[current_ip];
         CMasternode cmn = CMasternode::DeSerializable(mydb.queryData(current_ip));

         if(cmn.m_status == "")
         {
             // @TODO cannot find the data,重新操作
             QMessageBox::information(this,tr("提示"),
                          tr("请检查masternode.db是否存在。"));
             return;
         }

         int nRc = ssh2.mn_login(cmn.m_ip.toStdString().c_str(),
                       cmn.m_user.toStdString().c_str(),
                       cmn.m_pwd.toStdString().c_str());
         if (!nRc)
         {
             nRc = ssh2.mn_scp_write(cmn.m_safe_conf_path.toStdString().c_str(),
                               (local_setting.new_safe_conf_files_path+current_ip)
                                     .toStdString().c_str());
             if (!nRc)
             {
                 qDebug("upload the safe.conf file success.");
             }
             else
             {
                 QMessageBox::information(this,tr("提示"),
                              tr("上传配置文件到服务器失败，详情查看日志。"));
                 return;
             }

             // @todo upload those script...
             nRc = ssh2.mn_scp_write(
                         (local_setting.remote_script_path+local_setting.install_script)
                         .toStdString().c_str(),
                         (local_setting.local_script_path+local_setting.install_script)
                         .toStdString().c_str());
             if (!nRc)
             {
                 qDebug("upload the install file success.");
             }
             else
             {
                 QMessageBox::information(this,tr("提示"),
                              tr("上传配置文件到服务器失败，详情查看日志。"));
                 return;
             }

             nRc = ssh2.mn_scp_write(
                         (local_setting.remote_script_path+local_setting.start_script)
                         .toStdString().c_str(),
                         (local_setting.local_script_path+local_setting.start_script)
                         .toStdString().c_str());
             if (!nRc)
             {
                 qDebug("upload the start file success.");
             }
             else
             {
                 QMessageBox::information(this,tr("提示"),
                              tr("上传配置文件到服务器失败，详情查看日志。"));
                 return;
             }
             // @todo upload those script...
             nRc = ssh2.mn_scp_write(
                         (local_setting.remote_script_path+local_setting.restart_script)
                         .toStdString().c_str(),
                         (local_setting.local_script_path+local_setting.restart_script)
                         .toStdString().c_str());
             if (!nRc)
             {
                 qDebug("upload the restart file success.");
             }
             else
             {
                 QMessageBox::information(this,tr("提示"),
                              tr("上传配置文件到服务器失败，详情查看日志。"));
                 return;
             }
             // @todo upload those script...
             nRc = ssh2.mn_scp_write(
                         (local_setting.remote_script_path+local_setting.stop_script)
                         .toStdString().c_str(),
                         (local_setting.local_script_path+local_setting.stop_script)
                         .toStdString().c_str());
             if (!nRc)
             {
                 qDebug("upload the stop file success.");
             }
             else
             {
                 QMessageBox::information(this,tr("提示"),
                              tr("上传配置文件到服务器失败，详情查看日志。"));
                 return;
             }

             ui->pb_start->setEnabled(true);

             // 更新数据库里面的Masternode状态
             cmn.m_status = "UPLOADED";
             mMasternodes[cmn.m_ip].m_status = "UPLOADED";
             mydb.updateMn(current_ip, CMasternode::Serializable(cmn));
             show_masternode(cmn);
         }
         else
         {
             //@TODO show change dialog
             QMessageBox::information(this,tr("提示"),
                          tr("登录远程服务器失败，"\
                             "请确认服务器IP、用户名和密码是否正确。"));
             changedialog.exec();
         }
     }
     else
     {
         QMessageBox::information(this,tr("提示"),
                      tr("程序运行错误，未能找到当前IP"));
     }
}

void UIMnMain::on_pb_start_clicked()
{
    if (current_ip != "")
    {
        CMasternode cmn = CMasternode::DeSerializable(mydb.queryData(current_ip));
        mns.set(cmn.m_ip,cmn.m_user,cmn.m_pwd);
        mns.start();
        timer->start(10000);

        // 让start按钮不能重复响应
        ui->pb_start->setEnabled(false);
        ui->pb_rechange->setEnabled(false);
        cmn.m_status = "STARTING";
        mMasternodes[cmn.m_ip].m_status = "STARTING";
        mydb.updateMn(current_ip, CMasternode::Serializable(cmn));
        show_masternode(cmn);
    }
    else
    {
        QMessageBox::information(this,tr("提示"),
                     tr("程序运行错误，未能找到当前IP"));
    }
}

void UIMnMain::on_pb_rechange_clicked()
{
    changedialog.exec();
    changedialog.restart();
}
