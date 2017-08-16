#ifndef TASK_H
#define TASK_H

#include "ledarkside_global.h"
#include <QObject>
#include <QProcess>

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
    void newWarningMessage(QString message);
    void newErrorMessage(QString message);

    void progressChanged();

protected:
    void log(const QString &log);
    void warning(const QString &warning);
    void error(const QString &error);

    void setProgress(const int progress);

    QProcess *makeProcess();

private:
    int m_progress;
};

#endif // TASK_H
