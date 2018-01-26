#include "masternodeassit.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    masternodeassit w;
    w.show();

    return a.exec();
}
