#include "masternodeassit.h"
#include "ui_masternodeassit.h"

masternodeassit::masternodeassit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::masternodeassit)
{
    ui->setupUi(this);
}

masternodeassit::~masternodeassit()
{
    delete ui;
}
