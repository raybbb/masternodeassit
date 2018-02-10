#include "cregistry.h"
#include <QSettings>
#include <QDebug>

CRegistry::CRegistry()
{

}

bool CRegistry::readDataDir(QString& path)
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Safe\\Safe-Qt",QSettings::NativeFormat);
    path = settings.value("strDataDir", "error").toString();
    qDebug()<<path<<"...";
    if(!path.compare("error"))
    {
        QSettings settingsdefault("HKEY_CURRENT_USER\\Software\\Safe\\Safe-Qt",QSettings::NativeFormat);
        path = settingsdefault.value("strDataDirDefault", "error").toString();
        if (!path.compare("error"))
        {
            return false;
        }
    }
    return true;
}
