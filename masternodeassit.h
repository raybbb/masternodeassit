#ifndef MASTERNODEASSIT_H
#define MASTERNODEASSIT_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class masternodeassit;
}

class masternodeassit : public QMainWindow
{
    Q_OBJECT

public:
    explicit masternodeassit(QWidget *parent = 0);
    ~masternodeassit();
    QSqlDatabase database;
    void initDatabase();

private slots:
    void on_pb_add_clicked();
    void getRemoteServers();
    void addRemoteServers();

    void on_pb_remove_clicked();

private:
    Ui::masternodeassit *ui;
};

#endif // MASTERNODEASSIT_H
