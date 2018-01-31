#include "masternodeassit.h"
#include "ui_masternodeassit.h"
#include "mn_libssh2.h"
#include "mn_puttyscp.h"

masternodeassit::masternodeassit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::masternodeassit)
{
    ui->setupUi(this);
    /*
    mn_puttyscp pscp;
    int rc = pscp.init("45.77.147.225","ray","jinxiaobai3304");
    qDebug("init return: %d",rc);
    rc = pscp.scp("c:/b.txt","/home/ray/");
    qDebug("scp return: %d",rc);
    */
}

masternodeassit::~masternodeassit()
{
    delete ui;
}
