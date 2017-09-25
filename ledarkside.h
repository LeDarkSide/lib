#ifndef LEDARKSIDE_H
#define LEDARKSIDE_H

#include "ledarkside_global.h"
#include <QString>

class LEDARKSIDESHARED_EXPORT LeDarkSide
{

public:
    LeDarkSide() = delete;

    static QString version();
    static QString scriptHelper();

    static QString dataLocation();
    static QString repositories();
    static QString packages();
};

#endif // LEDARKSIDE_H
