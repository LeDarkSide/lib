#ifndef TASK_H
#define TASK_H

#include "ledarkside_global.h"
#include <QObject>
#include <QProcess>
#include <QUuid>
#include <QVector>

class LEDARKSIDESHARED_EXPORT Task : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)

public:
    explicit Task(QObject *parent = Q_NULLPTR);

    inline int progress() const { return m_progress; }

signals:
    void started();
    void finished();

    void newLogMessage(QString message);
    void newDebugMessage(QString message);
    void newWarningMessage(QString message);
    void newErrorMessage(QString message);

    void progressChanged();

protected:
    void log(const QString &log);
    void debug(const QString &debug);
    void warning(const QString &warning);
    void error(const QString &error);

    void setProgress(const int progress);

    void addSubTask(Task *subTask);
    void addSubTask(Task &subTask, const QString &name);
    void removeSubTask(Task *subTask);

private:
    QUuid m_id;
    int m_progress;
    QVector<Task*> m_subTasks;

    friend bool operator ==(const Task &l, const Task &r);
    friend bool operator !=(const Task &l, const Task &r);
};

class LEDARKSIDESHARED_EXPORT ProcessTask : public Task
{
    Q_OBJECT
public:
    explicit ProcessTask(QObject *parent = Q_NULLPTR);

signals:
    void processFinished(int exitCode);

protected:
    QProcess *m_process;
};

#endif // TASK_H
