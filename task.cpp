#include "task.h"
#include <QDebug>

Task::Task(QObject *parent)
    : QObject(parent),
      m_id(QUuid::createUuid()),
      m_progress(0)
{
}

void Task::log(const QString &log)
{
    qInfo() << log;
    emit newLogMessage(log);
}

void Task::debug(const QString &debug)
{
    qDebug() << debug;
    emit newDebugMessage(debug);
}

void Task::warning(const QString &warning)
{
    qWarning() << warning;
    emit newWarningMessage(warning);
}

void Task::error(const QString &error)
{
    qCritical() << error;
    emit newErrorMessage(error);
    emit finished();
}

void Task::setProgress(const int progress)
{
    if (m_progress != progress) {
        m_progress = progress;
        qInfo() << tr("%1%").arg(m_progress);
        emit progressChanged();
    }
}

void Task::addSubTask(Task *subTask)
{
    if (!subTask)
        return;

    for (int i = 0; i < m_subTasks.size(); ++i)
        if (*m_subTasks[i] == *subTask)
            return;

    subTask->setParent(this);
    m_subTasks.append(subTask);
    connect(subTask, &Task::finished, this, [this, subTask]() {
        removeSubTask(subTask);
    });
    connect(subTask, &Task::finished, subTask, &Task::deleteLater);
}

void Task::addSubTask(Task &subTask, const QString &name)
{
    connect(&subTask, &Task::newLogMessage, this, [this, name](QString message) {
        log(tr("[%1] %2").arg(name, message));
    });
    connect(&subTask, &Task::newDebugMessage, this, [this, name](QString message) {
        debug(tr("[%1] %2").arg(name, message));
    });
    connect(&subTask, &Task::newWarningMessage, this, [this, name](QString message) {
        warning(tr("[%1] %2").arg(name, message));
    });
    connect(&subTask, &Task::newErrorMessage, this, [this, name](QString message) {
        error(tr("[%1] %2").arg(name, message));
    });
}

void Task::removeSubTask(Task *subTask)
{
    if (!subTask)
        return;

    subTask->setParent(Q_NULLPTR);
    for (int i = 0; i < m_subTasks.size(); ++i) {
        if (*m_subTasks[i] == *subTask) {
            m_subTasks.remove(i);
            break;
        }
    }
    if (m_subTasks.isEmpty())
        emit finished();
}

bool operator ==(const Task &l, const Task &r)
{
    return l.m_id == r.m_id;
}

bool operator !=(const Task &l, const Task &r)
{
    return l.m_id != r.m_id;
}

ProcessTask::ProcessTask(QObject *parent)
    : Task(parent),
      m_process(Q_NULLPTR)
{
    m_process = new QProcess(this);
    connect(m_process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError pError) {
        error(tr("The process did not end normally; program: %1; error code: %3; error message: %2").arg(m_process->program(), m_process->errorString()).arg(pError));
    });
    connect(m_process, static_cast<void(QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitStatus != QProcess::NormalExit) {
            error(tr("The process did not end normally; exit code: %1; program error messages: %2").arg(exitCode).arg(QString(m_process->readAllStandardError())));
            return;
        }
        emit processFinished(exitCode);
    });
//    connect(&m_process, static_cast<void(QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished), proc, &QProcess::deleteLater);
}
