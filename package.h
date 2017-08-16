#ifndef PACKAGE_H
#define PACKAGE_H

#include "ledarkside_global.h"
#include <QString>
#include <QMap>

class LEDARKSIDESHARED_EXPORT Package
{
public:
    Package(const QString &path, const QString &id, const QString &name,
            const QString &version, const QString &description,
            const QString &category,
            const QString &installedCVersion, const QString &latestCVersion,
            const QString &directDependency, const QStringList &dependence);

    inline QString path() const { return m_path; }
    inline QString id() const { return m_id; }
    inline QString name() const { return m_name; }
    inline QString version() const { return m_version; }
    inline QString description() const { return m_description; }
    inline QString category() const { return m_category; }
    inline QString installedCVersion() const { return m_installedCVersion; }
    inline QString latestCVersion() const { return m_latestCVersion; }
    inline QString directDependency() const { return m_directDependency; }
    inline QStringList dependence() const { return m_dependence; }

    inline static QString fileInfoName() { return "info.json"; }
    inline static QString fileScriptName() { return "script.sh"; }

private:
    QString m_path, m_id, m_name, m_version, m_description, m_category, m_installedCVersion, m_latestCVersion, m_directDependency;
    QStringList m_dependence;
};

#endif // PACKAGE_H
