#include "MasternodePanel.h"
#include "ui_MasternodePanel.h"
#include "qfontdatabase.h"

MasternodePanel::MasternodePanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MasternodePanel)
{
    ui->setupUi(this);

    //引入图形字体
    int fontId = QFontDatabase::addApplicationFont(":/images/fontawesome-webfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont iconFont = QFont(fontName);
    iconFont.setPixelSize(15);

    ui->labDevice->setFont(iconFont);
    ui->labDevice->setText(QChar(0xf136));

    ui->labTemp->setFont(iconFont);
    ui->labTemp->setText(QChar(0xf109));

    ui->labDamp->setFont(iconFont);
    ui->labDamp->setText(QChar(0xf1dc));
}

MasternodePanel::~MasternodePanel()
{
    delete ui;
}

void MasternodePanel::setMasternodeName(const QString &deviceName)
{
    ui->labDeviceName->setText(deviceName);
}

QString MasternodePanel::getMasternodeName()
{
    return ui->labDeviceName->text();
}

void MasternodePanel::setMasternodeServerIp(const QString &qsServerIp)
{
    ui->labCurrentTemp->setText(QString("公网 IP : %1 ").arg(qsServerIp));
}

QString MasternodePanel::getMasternodeServerIp()
{
    return ui->labCurrentTemp->text();
}

void MasternodePanel::setMasternodeHash(const QString &qsHash)
{
    QWidget* elideWidget = new QWidget(this);
    QFontMetrics elidfont(elideWidget->font());
    ui->labCurrentDamp->setText(elidfont.elidedText(
                        QString("%1").arg(qsHash),
                        Qt::ElideMiddle,
                        elideWidget->width() + 60));
}

QString MasternodePanel::getMasternodeHash()
{
    return ui->labCurrentDamp->text();
}

void MasternodePanel::setHeight(int height)
{
    ui->labCountAll->setText(QString("高度 : %1").arg(height));
}

int MasternodePanel::getHeight()
{
    return ui->labCountAll->text().toInt();
}

void MasternodePanel::setStatus(const QString &qsStatus)
{
    ui->labCountNoRead->setText(QString("状态 : %1").arg(qsStatus));
}

QString MasternodePanel::setStatus()
{
    return ui->labCountNoRead->text();
}
