#ifndef MASTERNODEASSIT_H
#define MASTERNODEASSIT_H

#include <QMainWindow>

namespace Ui {
class masternodeassit;
}

class masternodeassit : public QMainWindow
{
    Q_OBJECT

public:
    explicit masternodeassit(QWidget *parent = 0);
    ~masternodeassit();

private:
    Ui::masternodeassit *ui;
};

#endif // MASTERNODEASSIT_H
