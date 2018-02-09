#ifndef UIMNMAIN_H
#define UIMNMAIN_H

#include <QDialog>
#include <QTableWidgetItem>
#include <map>
#include "frametitlebar.h"
#include "cmasternode.h"
#include "mnwizard.h"
using namespace std;

namespace Ui {
class UIMnMain;
}

class UIMnMain : public QDialog
{
    Q_OBJECT

    typedef enum {UNLOAD=0,LOADED,DISABLE,ENABLE} E_MNSTATUS;

    QStringList S_MNSTATUS ;//("UNLOAD","LOADED","DISABLE");

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
    MnWizard mnwizard;
    map<QString, CMasternode> mMasternodes;

private slots:
    void initForm();
    void initTableWidget();
    void buttonClick();
    void recvMnInfo(const CMasternode &cmn);

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
