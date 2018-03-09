#-------------------------------------------------
#
# Project created by QtCreator 2018-01-26T19:19:11
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = masternodeassit
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        masternodeassit.cpp \
    configdialog.cpp \
    pages.cpp \
    uimnmain.cpp \
    mnwizard.cpp \
    frametitlebar.cpp \
    mnlibssh2.cpp \
    cmasternode.cpp \
    walletrpc.cpp \
    cregistry.cpp \
    clocalsetting.cpp \
    localrpcdialog.cpp \
    cstartmasternode.cpp \
    database.cpp \
    changedialog.cpp \
    form/MasternodePanel.cpp



HEADERS += \
        masternodeassit.h \
    include/libssh2.h \
    include/libssh2_publickey.h \
    include/libssh2_sftp.h \
    include/scp.h \
    configdialog.h \
    pages.h \
    uimnmain.h \
    mnwizard.h \
    frametitlebar.h \
    mnlibssh2.h \
    cmasternode.h \
    walletrpc.h \
    cregistry.h \
    clocalsetting.h \
    localrpcdialog.h \
    cstartmasternode.h \
    database.h \
    changedialog.h \
    form/MasternodePanel.h

FORMS += \
    uimnmain.ui \
    form/masternodepanel.ui \
    form/cmdsend.ui


LIBS += -lWs2_32

RESOURCES += \
    images.qrc


