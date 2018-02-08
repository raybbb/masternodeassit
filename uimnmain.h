#ifndef UIMNMAIN_H
#define UIMNMAIN_H

#include <QDialog>
#include <QTableWidgetItem>
#include "frametitlebar.h"
#include "cmasternode.h"

namespace Ui {
class UIMnMain;
}

class UIMnMain : public QDialog
{
    Q_OBJECT

public:
    explicit UIMnMain(QWidget *parent = 0);
    ~UIMnMain();

private:
    void insertTableWidgetItem(CMasternode cmn);
    void removeTableWidgetItem(CMasternode cmn);
    void removeTableWidgetItem(int nIndex);

private:
    Ui::UIMnMain *ui;
    bool max;
    QRect location;

private slots:
    void initForm();
    void initTableWidget();
    void buttonClick();

private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void on_pb_add_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);
    void on_tableWidget_clicked(const QModelIndex &index);
    void on_pb_remove_clicked();
};

#endif // UUIMAIN_H
