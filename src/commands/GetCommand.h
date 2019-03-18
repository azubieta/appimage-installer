#pragma once

// libraries
#include <QObject>
#include <QString>

// local
#include <Command.h>
#include <entities/Explorer.h>
#include <entities/Application.h>

class FileDownload;

class GetCommand : public Command {
Q_OBJECT
public:
    GetCommand(const QString& appId, QObject* parent = nullptr);

public slots:
    void execute() override;

protected slots:
    void handleGetApplicationCompleted(QVariantMap app);

    void handleDownloadProgress(qint64 progress, qint64 total, const QString &message);

    void handleDownloadCompleted();

    void handleDownloadFailed(const QString& message);

private:
    void createApplicationsDir();

    QString buildTargetPath(const Application& a);

    void downloadFile(const Application& application);

    bool isCompatibleBinary(QString arch);

    FileDownload *fileDownload;
    QString appId;
    QString targetPath;
    Explorer explorer;
    QTextStream out;
};
