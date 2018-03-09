#ifndef MasternodePanel_H
#define MasternodePanel_H

#include <QWidget>

namespace Ui {
class MasternodePanel;
}

class MasternodePanel : public QWidget
{
    Q_OBJECT

public:
    explicit MasternodePanel(QWidget *parent = 0);
    ~MasternodePanel();

    void setMasternodeName(const QString& deviceName);
    QString getMasternodeName();

    void setMasternodeServerIp(const QString& qsServerIp);
    QString getMasternodeServerIp();

    void setMasternodeHash(const QString& qsHash);
    QString getMasternodeHash();

    void setHeight(int height);
    int getHeight();

    void setStatus(const QString& qsStatus);
    QString setStatus();

private:
    Ui::MasternodePanel *ui;
};

#endif // MasternodePanel_H
