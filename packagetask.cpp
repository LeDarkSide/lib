#include "packagetask.h"
#include "ledarkside.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>
#include <QProcess>
#include <QDebug>
#include <QDir>

ReadPackage::ReadPackage(QObject *parent)
    : ProcessTask(parent)
{
    m_process->setProgram(LeDarkSide::scriptHelper());
}

void ReadPackage::start(const QString &path)
{
    log(tr("Read package in %1").arg(path));

    QFile fileInfo(path + "/" + Package::fileInfoName());
    if (!fileInfo.open(QIODevice::ReadOnly)) {
        error(tr("Can't open: %1; error: %2").arg(fileInfo.fileName(), fileInfo.errorString()));
        return;
    }

    QJsonParseError jsonError;
    QJsonDocument infoDoc = QJsonDocument::fromJson(fileInfo.readAll(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        error(tr("Can't parse json file: %1; error: %2").arg(fileInfo.fileName(), jsonError.errorString()));
        return;
    }

    if (!infoDoc.isObject()) {
        error(tr("The JSON file doesn't contain an object: %1").arg(fileInfo.fileName()));
        return;
    }
    QJsonObject infoObject = infoDoc.object();

    for (const QString &key : {"id", "name", "version", "description", "category", "dependence"}) {
        if (!infoObject.contains(key)) {
            error(tr("Can't find the key: %1; in the JSON object: %2").arg(key, fileInfo.fileName()));
            return;
        }
    }

    QStringList dependence = infoObject["dependence"].toVariant().toStringList();

    m_process->setArguments({path, "info"});
    connect(this, &ReadPackage::processFinished,
            this, [this, path, infoObject, dependence](int exitCode) {

        QMap<QString, QString> variables;
        while (m_process->canReadLine()) {
            QString line(m_process->readLine().trimmed());
            int eqPos = line.indexOf('=');
            if (eqPos > 1)
                variables[line.left(eqPos)] = line.mid(eqPos + 1);
        }

        for (const QString &key : {"INSTALLED_VERSION", "LAST_VERSION"}) {
            if (!variables.contains(key)) {
                QString strVariables;
                QDebug debug(&strVariables);
                debug << variables;
                error(tr("Can't find the key: %1; in the variables return by the program; variables: %2").arg(key, strVariables));
                return;
            }
        }

        Package package(path,
                        infoObject["id"].toString(),
                        infoObject["name"].toString(),
                        infoObject["version"].toString(),
                        infoObject["description"].toString(),
                        infoObject["category"].toString(),
                        variables["INSTALLED_VERSION"],
                        variables["LAST_VERSION"],
                        dependence);

        emit result(package);
        emit finished();
    });
    m_process->start();
}

ListPackages::ListPackages(QObject *parent)
    : Task(parent)
{}

void ListPackages::start(const QString &path)
{
    QDir dir(path);
    log(tr("List packages in %1").arg(dir.absolutePath()));

    if (!dir.isReadable()) {
        error(tr("%1 is not readable").arg(dir.absolutePath()));
        return;
    }

    auto dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    auto canonicalPath = dir.canonicalPath() + "/";

    QVector<ReadPackage*> subTask(dirs.size());
    for (int i = 0; i < dirs.size(); ++i) {
        ReadPackage *task = new ReadPackage(this);
        addSubTask(task);
        connect(task, &ReadPackage::result, this, &ListPackages::result);
        subTask[i] = task;
    }

    for (int i = 0; i < dirs.size(); ++i) {
        addSubTask(*subTask[i], canonicalPath + dirs[i]);
        subTask[i]->start(canonicalPath + dirs[i]);
    }
}

InstallPackage::InstallPackage(QObject *parent)
    : ProcessTask(parent)
{
    m_process->setProgram(LeDarkSide::scriptHelper());
}

void InstallPackage::start(const Package &package)
{
    log(tr("Install %1").arg(package.name()));

    QDir pkgDest(LeDarkSide::packages() + package.id());
    pkgDest.mkpath(".");
    pkgDest.refresh();

    if (!pkgDest.isReadable()) {
        error(tr("%1 is not readable").arg(pkgDest.absolutePath()));
        return;
    }

    if (!pkgDest.isEmpty()) {
        error(tr("The directory is not empty : %1").arg(pkgDest.absolutePath()));
        return;
    }

    int cp = QProcess::execute("cp", {"-r", package.path()+"/.", pkgDest.absolutePath()});
    if (cp != 0) {
        error(tr("The copy of %1 to %2 did not end properly; exit code : %1").arg(package.path(), pkgDest.absolutePath()).arg(cp));
        return;
    }

    m_process->setArguments({pkgDest.absolutePath(), "install"});
    connect(this, &InstallPackage::processFinished, this, [this](int exitCode) {
        emit finished();
    });
    m_process->start();
}
