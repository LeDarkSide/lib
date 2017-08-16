#ifndef TASKPOOL_H
#define TASKPOOL_H

//c'est juste une idée

#include <QObject>
#include <QList>

#include "task.h"

class TaskPool : public QObject
{
    Q_OBJECT
public:
    explicit TaskPool(QObject *parent = Q_NULLPTR);

    void start(Task *task);

signals:

public slots:

private:
    QList<Task*> m_pendingTasks;
    QList<Task*> m_runningTasks;
};

#endif // TASKPOOL_H
