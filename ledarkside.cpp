#include "ledarkside.h"
#include <QStandardPaths>

QString LeDarkSide::version()
{
    return VERSION;
}

QString LeDarkSide::scriptHelper()
{
    return SCRIPT_HELPER_DIR;
}

QString LeDarkSide::dataLocation()
{
    static QString location = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/LeDarkSide";
    return location;
}

QString LeDarkSide::repositories()
{
    return dataLocation() + "/repositories";
}

QString LeDarkSide::packages()
{
    return dataLocation() + "/packages";
}
