#ifndef UIMNMAIN_H
#define UIMNMAIN_H

#include <QDialog>
#include <QTableWidgetItem>
#include <map>
#include "frametitlebar.h"
#include "cmasternode.h"
#include "mnwizard.h"
#include "changedialog.h"
#include "cstartmasternode.h"
#include "database.h"

using namespace std;

namespace Ui {
class UIMnMain;
}

class UIMnMain : public QDialog
{
    Q_OBJECT
    //typedef enum {UNLOAD=0,LOADED,DISABLE,ENABLE} E_MNSTATUS;
    typedef enum {E_INFO=0, E_MESSAGE, E_ERROR} E_MESSAGE_LEVEL;
protected:
    // 进行鼠界面的拖动
    virtual void mousePressEvent(QMouseEvent *event);
public:
    explicit UIMnMain(QWidget *parent = 0);
    ~UIMnMain();
signals:
    void sigMasternodeSynOver();

private:
    void insertTableWidgetItem(CMasternode cmn);
    void removeTableWidgetItem(CMasternode cmn);
    void removeTableWidgetItem(int nIndex);
    void initSetting();
    void initDatabase();

private:
    Ui::UIMnMain *ui;
    bool max;
    QRect location;
    MnWizard mnwizard;
    ChangeDialog changedialog;
    QString current_ip;
    CMasternode current_mn;
    QTimer *timer;
    QTimer *hlth_timer;
    CStartMasternode mns;
    Database mydb;
    bool isGenLocalSafeByProgram;
    int nTotalBlock;
    int nCurrentBlock;

private slots:
    void initForm();
    void initTableWidget();
    void buttonClick();
    void recvMnInfo(const CMasternode &cmn);
    void recvChangeMnInfo(const CMasternode &cmn);
    void show_masternode(const CMasternode &cmn);
    void GetMasternodeInfo();
    void CheckMasternodeFunctioning();
    void ShowMasternodeStatus();
    void mnSetupComplete();
    void doSynOver();

    void showProcessMessage(const QString &msg, E_MESSAGE_LEVEL ml=E_INFO);
    void showLocalSetting();
    void ShowMnToMessageBox(const CMasternode &cmn);

private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void on_pb_add_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);
    void on_tableWidget_clicked(const QModelIndex &index);
    void on_pb_remove_clicked();
    void on_pb_upload_clicked();
    void on_pb_start_clicked();
    void on_pb_rechange_clicked();
    void on_pb_stop_clicked();
};

#endif // UUIMAIN_H
