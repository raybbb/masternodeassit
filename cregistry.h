#ifndef CREGISTRY_H
#define CREGISTRY_H
#include <QString>

class CRegistry
{
public:
    CRegistry();
    static bool readDataDir(QString& path);
};


#endif // CREGISTRY_H
