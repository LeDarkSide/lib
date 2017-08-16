#-------------------------------------------------
#
# Project created by QtCreator 2017-08-13T14:23:55
#
#-------------------------------------------------

#utiliser CMAKE parce que le script de s'installe pas comme il faut

QT       -= gui

TARGET = LeDarkSide
TEMPLATE = lib

VERSION = 1.0.0-alpha
DEFINES += VERSION=\\\"$$VERSION\\\"

DEFINES += LEDARKSIDE_LIBRARY

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
    ledarkside.cpp \
    task.cpp \
    packagetask.cpp \
    package.cpp \
    repository.cpp \
    repositorytask.cpp \
    settings.cpp \
    taskpool.cpp

HEADERS += \
    ledarkside.h \
    ledarkside_global.h \
    task.h \
    packagetask.h \
    package.h \
    repository.h \
    repositorytask.h \
    settings.h \
    taskpool.h

isEmpty(PREFIX) {
    unix:PREFIX=/usr/local
    else:PREFIX=$$OUT_PWD
}

unix {
    target.path = $$PREFIX/lib
    INSTALLS += target
}

DISTFILES += \
    scripthelper.sh

scripthelper.target = scripthelper.sh
scripthelper.commands = cp -u $$PWD/$$scripthelper.target $$OUT_PWD/$$scripthelper.target
QMAKE_EXTRA_TARGETS += scripthelper
POST_TARGETDEPS += scripthelper

scripthelper.files = scripthelper.sh
unix:scripthelper.path = $$PREFIX/share/LeDarkSide
else:scripthelper.path = $$PREFIX
#unix:scripthelper.extra = chmod a+x $$scripthelper.files

#unix {
#    scripthelper_exec.commands = chmod a+x $$scripthelper.path
#    scripthelper_exec.depends = scripthelper
#    install.depends = scripthelper_exec
#    QMAKE_EXTRA_TARGETS += install scripthelper_exec
#}

INSTALLS += scripthelper
DEFINES += SCRIPT_HELPER_DIR=\\\"$$scripthelper.path/$$scripthelper.files\\\"
