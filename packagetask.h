#ifndef PACKAGETASK_H
#define PACKAGETASK_H

#include "task.h"
#include "package.h"

class LEDARKSIDESHARED_EXPORT ReadPackage : public ProcessTask
{
    Q_OBJECT
public:
    ReadPackage(QObject *parent = Q_NULLPTR);

public slots:
    void start(const QString &path);

signals:
    void result(Package package);
};

class LEDARKSIDESHARED_EXPORT ListPackages : public Task
{
    Q_OBJECT
public:
    ListPackages(QObject *parent = Q_NULLPTR);

public slots:
    void start(const QString &path);

signals:
    void result(Package package);
};

class LEDARKSIDESHARED_EXPORT InstallPackage : public ProcessTask
{
    Q_OBJECT
public:
    InstallPackage(QObject *parent = Q_NULLPTR);

public slots:
    void start(const Package &package);
};

#endif // PACKAGETASK_H
