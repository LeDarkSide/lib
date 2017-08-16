#include "packagetask.h"
#include "settings.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>
#include <QProcess>
#include <QDebug>

ReadPackage::ReadPackage(QObject *parent)
    : Task(parent)
{}

void ReadPackage::start(const QString &path, const QString &directDependency)
{
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
    if (!directDependency.isEmpty())
        dependence.push_front(directDependency);

    QProcess *proc = makeProcess();
    proc->setProgram(Settings::scriptHelper());
    proc->setArguments({path, "info"});
    connect(proc, static_cast<void(QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished),
            this, [this, path, directDependency, infoObject, dependence, proc](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitStatus != QProcess::NormalExit) {
            error(tr("The process did not end normally; exit code: %1; program error messages: %2").arg(exitCode).arg(QString(proc->readAllStandardError())));
            return;
        }

        QMap<QString, QString> variables;
        while (proc->canReadLine()) {
            QString line(proc->readLine().trimmed());
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
                        directDependency,
                        dependence);

        emit result(package);
        emit finished();
    });
    proc->start();
}
