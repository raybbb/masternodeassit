#include "uimnmain.h"
#include "ui_uimnmain.h"
#include "mnlibssh2.h"
#include "clocalsetting.h"
#include "cregistry.h"
#include "localrpcdialog.h"

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

QString ARR_MN_FILDNAME[] = {"Num","Alias","Addr","Port","Status"};

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

UIMnMain::UIMnMain(QWidget *parent):
    QDialog(parent),
    ui(new Ui::UIMnMain)
{
    ui->setupUi(this);
    this->initForm();

    S_MNSTATUS<<QString("UNLOAD")<<QString("LOADED")<<QString("DISABLE")<<QString("ENABLE");

    connect(&mnwizard, &MnWizard::sigMasternodeAdd, this, &UIMnMain::recvMnInfo);
    //connect(, &MnWizard::sigMasternodeAdd, this, &UIMnMain::recvMnInfo);

    ui->tableWidget->verticalHeader()->setStyleSheet
            ("QHeaderView::section{background-color:rgba(232,255,213,5);}");
    ui->tableWidget->horizontalHeader()->setStyleSheet
            ("QHeaderView::section{background-color:rgba(232,255,213,5);}");
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

    initTableWidget();
    initSetting();
}

void UIMnMain::initSetting()
{
    //@todo read rpc from /safe.conf
    local_setting.new_safe_conf_files_path = "./safeconf";
    local_setting.script_path = "./script";
    if(!CRegistry::readDataDir(local_setting.safe_conf_path))
    {
        //@todo safe haven't install. or show dialg to selet path of safe data.
        exit(0);
    }

    QString localSafeConfFile = local_setting.safe_conf_path + "/safe.conf";
    qDebug()<< "*Local safe file: " << localSafeConfFile;
    QFile file(localSafeConfFile);
    if(file.exists())
    {
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
                        qDebug()<< "user: "<<local_setting.local_rpc_user;
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
                        qDebug()<< "pwd: "<<local_setting.local_rpc_pwd;
                    }
                }
                safe_conf_line =  file.readLine();
            }
            file.close();
        }

        if(hasConfigRpc != 2)
        {
            if(file.open(QIODevice::ReadWrite))
            {
                /*
                file.write("rpcuser=s\r\n");
                file.write("rpcpassword=bankledger\r\n");
                */
                // @todo show input diolg to fill rpcuser and password;

                /*ConfigDialog dialog;
                dialog.exec();*/
                LocalrpcDialog llrpc;
                llrpc.exec();
                file.close();
            }
        }
    }
    else
    {
        qDebug()<<"文件不存在";
    }

    local_setting.masternode_conf_file
            = local_setting.safe_conf_path + "/masternode.conf";
    qDebug()<< "*Local masternode confi file: "
            << local_setting.masternode_conf_file;
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
                        qDebug()<< "3:"<<qList[3];
                        qDebug()<< "4:"<<qList[4];
                        local_setting.mn_old_info[qList[3]] = qList[4];
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
            qDebug()<<"open failed!";
        }
    }
    else
    {
        qDebug()<<"文件不存在";
    }

}

UIMnMain::~UIMnMain()
{
    delete ui;
}

void UIMnMain::initTableWidget()
{
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
    headText << "Num" << "Alias" << "Addr" << "Port" << "Status";
    ui->tableWidget->setHorizontalHeaderLabels(headText);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

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

    ui->labTitle->setText("Masternode管理平台");
    this->setWindowTitle("Masternode管理平台");

    QList<QToolButton *> btns = ui->widgetLeft->findChildren<QToolButton *>();
    foreach (QToolButton *btn, btns) {
        btn->setMaximumHeight(80);
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(buttonClick()));
    }
}

void UIMnMain::insertTableWidgetItem(CMasternode cmn)
{
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

}

void UIMnMain::removeTableWidgetItem(CMasternode cmn)
{
}

void UIMnMain::removeTableWidgetItem(int nIndex)
{
    ui->tableWidget->removeRow(nIndex);
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
    insertTableWidgetItem(mMasternodes[cmn.m_ip]);
    show_masternode(cmn);
    current_ip = cmn.m_ip;

    local_setting.remote_rpc_ip = cmn.m_remote_rpc_ip;
    local_setting.remote_rpc_user = cmn.m_remote_rpc_user;
    local_setting.remote_rpc_pwd = cmn.m_remote_rpc_pwd;
    local_setting.safe_conf_file = cmn.m_safe_conf_path;
    local_setting.masternode_conf_path = cmn.m_mn_conf_path;

}

void UIMnMain::show_masternode(const CMasternode &cmn)
{
    QString qsHtml = "<th>Masternoede Info</th>";
    qsHtml.append("<hr>");
    qsHtml.append("<table border='0.1' width='100%'>");

    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("alias");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(cmn.m_alias);
    qsHtml.append("</td>");qsHtml.append("</tr>");

    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("vps ip");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append(cmn.m_ip);
    qsHtml.append("</td>");qsHtml.append("</tr>");

    qsHtml.append("<tr>");qsHtml.append("<td>");qsHtml.append("status");
    qsHtml.append("</td>");
    qsHtml.append("<td>");qsHtml.append("UNLOAD");
    qsHtml.append("</td>");qsHtml.append("</tr>");

    qsHtml.append("</table>");
    //qDebug(qsHtml.toStdString().c_str());
    ui->textEdit->setHtml(qsHtml);
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
}

void UIMnMain::on_pb_remove_clicked()
{
    removeTableWidgetItem(ui->tableWidget->currentRow());

    /*
    mn_libssh2 ssh2;
    ssh2.mn_init();
    ssh2.mn_login("45.77.147.225","ray","jinxiaobai3304");

    ssh2.mn_exec("ls -l > bbb.xxx");

    ssh2.mn_scp_read("c:/abc.txt","/home/ray/tt.a");

    mn_libssh2 ssh2;
    ssh2.mn_init();
    ssh2.mn_login("45.77.147.225","ray","jinxiaobai3304");
    int nRet = ssh2.mn_scp_write("/home/ray/aaa.a","c:/b.txt");
    if(nRet!=0)
    {
        qDebug("Copy Error ...");
    }
    nRet = ssh2.mn_scp_write("/home/ray/bbb.a","c:/b.txt");
    if(nRet!=0)
    {
        qDebug("Copy Error ...");
    }
    */
}

void UIMnMain::on_pb_upload_clicked()
{
     if (current_ip != "")
     {
         mn_libssh2 ssh2;
         ssh2.mn_init();

         CMasternode cmd = mMasternodes[current_ip];
         int nRc = ssh2.mn_login(cmd.m_ip.toStdString().c_str(),
                       cmd.m_user.toStdString().c_str(),
                       cmd.m_pwd.toStdString().c_str());
         if (!nRc)
         {
             nRc = ssh2.mn_scp_write(cmd.m_safe_conf_path.toStdString().c_str(),
                               (QString("./safeconf/")+current_ip).toStdString().c_str());
             if (!nRc)
             {
                 qDebug("upload the file success.");
             }
             else
             {
                 qDebug("upload the file failed.");
             }
         }
     }
     else
     {
         qDebug("nothing to upload .");
     }
}

void UIMnMain::on_pb_start_clicked()
{
    if (current_ip != "")
    {
        mn_libssh2 ssh2;
        ssh2.mn_init();

        CMasternode cmd = mMasternodes[current_ip];
        int nRc = ssh2.mn_login(cmd.m_ip.toStdString().c_str(),
                      cmd.m_user.toStdString().c_str(),
                      cmd.m_pwd.toStdString().c_str());
        if (!nRc)
        {
            nRc = ssh2.mn_scp_write("/home/rbai/start_test.py",
                              (QString("./script/")+"start_test.py").toStdString().c_str());
            if (!nRc)
            {
                qDebug("upload the file success.");
                ssh2.mn_exec("python /home/rbai/start_test.py");
            }
            else
            {
                qDebug("upload the file failed.");
            }
        }
    }
    else
    {
        qDebug("nothing to upload .");
    }
}
