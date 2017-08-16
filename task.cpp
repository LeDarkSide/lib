#include "task.h"
#include <QDebug>

Task::Task(QObject *parent)
    : QObject(parent)
{
}

void  Task::log(const QString &log)
{
    qInfo() << log;
    emit newLogMessage(log);
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

QProcess *Task::makeProcess()
{
    QProcess *proc = new QProcess(this);
    connect(proc, &QProcess::errorOccurred, this, [this, proc](QProcess::ProcessError pError) {
        error(tr("The process did not end normally; program: %1; error code: %3; error message: %2").arg(proc->program(), proc->errorString()).arg(pError));
    });
    connect(proc, static_cast<void(QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished), proc, &QProcess::deleteLater);
    return proc;
}
