#include <QApplication>

#include "uimnmain.h"
#include "mnwizard.h"
#include "masternodeassit.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UIMnMain mn;
    mn.show();

    return a.exec();
}
