#include "cregistry.h"
#include <QSettings>
#include <QDebug>
#include <QMessageBox>

CRegistry::CRegistry()
{

}

bool CRegistry::readDataDir(QString& path)
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Safe\\Safe-Qt",QSettings::NativeFormat);
    path = settings.value("strDataDir", "error").toString();

    if(!path.compare("error"))
    {
        QSettings settingsdefault("HKEY_CURRENT_USER\\Software\\Safe\\Safe-Qt",QSettings::NativeFormat);
        path = settingsdefault.value("strDataDirDefault", "error").toString();
        if (!path.compare("error"))
        {
            // 提示用户未安装钱包程序，否则程序会退出，且无任何提示。
            QMessageBox message(QMessageBox::Warning, "SAFE ERROR",
                                "本机未检测到钱包程序，请安装SAFE钱包\n然后再启动本程序。");
            message.setIconPixmap(QPixmap(":/images/query.png"));
            message.exec();
            return false;
        }
    }
    return true;
}
