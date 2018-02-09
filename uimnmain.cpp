#include "uimnmain.h"
#include "ui_uimnmain.h"
#include "mnlibssh2.h"
#include <QDesktopWidget>
#include <QDateTime>
#include <QFile>
#include <QDebug>


QString ARR_MN_FILDNAME[] = {"Num","Alias","Addr","Port","Status"};
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
/*
    CMasternode mn;
    mn.m_alias = "money";
    mn.m_ip = "127.0.0.1";
    mn.m_port = 110;
    mn.m_status = "well well well";

    insertTableWidgetItem(mn);
    */
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
    //qDebug(qsHtml.toStdString().c_str());
    ui->textEdit->setHtml(qsHtml);
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
