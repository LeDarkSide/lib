#ifndef REPOSITORYTASK_H
#define REPOSITORYTASK_H

#include "ledarkside_global.h"
#include "task.h"
#include "repository.h"
#include "packagetask.h"

class LEDARKSIDESHARED_EXPORT InstallRepository : public Task
{
    Q_OBJECT
public:
    InstallRepository(QObject *parent = Q_NULLPTR);

public slots:
    void start(const Repository &repo);

private:
    void startLocalDir(const Repository &repo);
    void startGit(const Repository &repo);
};

class LEDARKSIDESHARED_EXPORT ReadRepository : public Task
{
    Q_OBJECT
public:
    ReadRepository(QObject *parent = Q_NULLPTR);

public slots:
    void start(const Repository &repo);

signals:
    void result(Package package);

private:
    void readDir(const QString &path);

private:
    size_t m_taskCounter;
};

#endif // REPOSITORYTASK_H
