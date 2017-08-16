#include "settings.h"
#include <QStandardPaths>
#include <QCoreApplication>

QSettings Settings::m_settings("LeDarkSide", "lib");

QString Settings::version()
{
    return VERSION;
}

QString Settings::dataLocation()
{
    static QString location = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/LeDarkSide";
    return location;
}

QString Settings::repositories()
{
    return m_settings.value("repositories", dataLocation() + "/repositories").toString();
}

QString Settings::packages()
{
    return m_settings.value("packages", dataLocation() + "/packages").toString();
}

QString Settings::scriptHelper()
{
    return SCRIPT_HELPER_DIR;
}
