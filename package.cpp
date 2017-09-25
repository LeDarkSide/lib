#include "package.h"

Package::Package(const QString &path, const QString &id, const QString &name,
                 const QString &version, const QString &description, const QString &category,
                 const QString &installedCVersion, const QString &latestCVersion, const QStringList &dependence)
    : m_path(path),
      m_id(id),
      m_name(name),
      m_version(version),
      m_description(description),
      m_category(category),
      m_installedCVersion(installedCVersion),
      m_latestCVersion(latestCVersion),
      m_dependence(dependence)
{
}
