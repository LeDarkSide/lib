#include "repositorytask.h"
#include "ledarkside.h"
#include <QProcess>
#include <QRegularExpression>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>

InstallRepository::InstallRepository(QObject *parent)
    : ProcessTask(parent)
{
}

void InstallRepository::start(const Repository &repo)
{
    switch (repo.protocol()) {
    case Repository::LocalDir:
        startLocalDir(repo);
        break;
    case Repository::Git:
        startGit(repo);
        break;
    default:
        error(tr("The repository protocol is not supported; repository name: %1").arg(repo.name()));
        return;
        break;
    }
}
//PAS FINI
void InstallRepository::startLocalDir(const Repository &repo)
{
    if (!QDir(repo.url()).exists()) {
        error(tr("The repository directory does not exist: %1").arg(repo.url()));
        return;
    }

    QString repoDir = repo.path();
    if (!QFile::exists(repoDir)) {
        bool result = QFile::link(repo.url(), repoDir);
        if(!result) {
            error(tr("Can't link repo dir: %1 to %2").arg(repo.url(), repoDir));
            return;
        }
    }
}

void InstallRepository::startGit(const Repository &repo)
{
    m_process->setProgram("git");
    m_process->setProcessChannelMode(QProcess::MergedChannels);
    connect(this, &InstallRepository::processFinished, this, [this](int exitCode) {
        emit finished();
    });
    connect(m_process, &QProcess::readyRead, this, [this]() {
        QString line(m_process->readAll().trimmed());
        QRegularExpression exp("\\d+(?=%)");
        auto it = exp.globalMatch(line);
        if (!it.hasNext())
            log(line);
        while (it.hasNext()) {
            auto match = it.next();
            bool ok;
            int nb = match.captured(0).toInt(&ok);
            if (ok)
                setProgress(nb);
        }
    });

    QString repoDir = repo.path();
    m_process->setArguments(QStringList() << "clone" << "--progress" << repo.url() << repoDir);

    QDir dir(repoDir + "/.git");
    if (dir.exists()) {
        QProcess p;
        p.setProgram("git");
        p.setArguments(QStringList() << "-C" << repoDir << "config" << "--get" << "remote.origin.url");
        p.setProcessChannelMode(QProcess::MergedChannels);
        p.start();
        p.waitForFinished(3000);
        QString origin = p.readAll().trimmed();
        if (origin == repo.url()) {
            m_process->setArguments(QStringList() << "-C" << repoDir << "pull");
        } else {
            warning(tr("The current repository is corrupted, it will be deleted"));
            QDir d(repoDir);
            d.removeRecursively();
        }
    }

    m_process->start();
}

ReadRepository::ReadRepository(QObject *parent)
    : Task(parent),
      m_taskCounter(0)
{}

void ReadRepository::start(const Repository &repo)
{
    readDir(repo.path());
}

void ReadRepository::readDir(const QString &path)
{
    log(tr("List packages in %1").arg(path));
    auto dirs = QDir(path).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    auto canonicalPath = QDir(path).canonicalPath() + "/";
    for (const QString &dir : dirs) {
        ++m_taskCounter;
        ReadPackage *task = new ReadPackage(this);
        connect(task, &ReadPackage::result, this, &ReadRepository::result);
        connect(task, &ReadPackage::finished, this, [this]() {
            --m_taskCounter;
            if (m_taskCounter == 0)
                emit finished();
        });
        connect(task, &ReadPackage::finished, task, &ReadPackage::deleteLater);
        task->start(canonicalPath + dir);
        readDir(canonicalPath + dir);
    }
}
