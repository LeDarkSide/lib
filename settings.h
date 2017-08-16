#ifndef SETTINGS_H
#define SETTINGS_H

#include "ledarkside_global.h"
#include <QSettings>

class LEDARKSIDESHARED_EXPORT Settings
{
public:
    Settings() = delete;

    static QString version();
    static QString dataLocation();
    static QString repositories();
    static QString packages();
    static QString scriptHelper();

private:
    static QSettings m_settings;
};

#endif // SETTINGS_H
