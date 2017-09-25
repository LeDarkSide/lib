#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "ledarkside.h"
#include <QObject>
#include <QString>

class LEDARKSIDESHARED_EXPORT Repository
{
    Q_GADGET
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString url READ url CONSTANT)
    Q_PROPERTY(Protocol protocol READ protocol CONSTANT)
    Q_PROPERTY(QString path READ path CONSTANT)

public:
    enum Protocol {
        LocalDir,
        Git
    };
    Q_ENUM(Protocol)

    Repository(const QString &name, const QString &description,
               const QString &url, const Protocol protocol);

    inline QString name() const { return m_name; }
    inline QString description() const { return m_description; }
    inline QString url() const { return m_url; }
    inline Protocol protocol() const { return m_protocol; }
    inline QString path() const { return LeDarkSide::repositories() + '/' + name(); }

private:
    QString m_name, m_description, m_url;
    Protocol m_protocol;
};

#endif // REPOSITORY_H
