#include "repository.h"

Repository::Repository(const QString &name, const QString &description, const QString &url, const Protocol protocol)
    : m_name(name),
      m_description(description),
      m_url(url),
      m_protocol(protocol)
{
}
