#ifndef PACKAGETASK_H
#define PACKAGETASK_H

#include "task.h"
#include "package.h"

class LEDARKSIDESHARED_EXPORT ReadPackage : public Task
{
    Q_OBJECT
public:
    ReadPackage(QObject *parent = Q_NULLPTR);

public slots:
    void start(const QString &path, const QString &directDependency = "");

signals:
    void result(Package package);
};

#endif // PACKAGETASK_H
