#pragma once

// libraries
#include <QObject>
#include <QString>
#include <Attica/ProviderManager>
#include <Attica/ListJob>

// local
#include <Command.h>

class FileDownload;

class InstallCommand : public Command {
Q_OBJECT
public:
    InstallCommand(const QString& appId, QObject* parent = nullptr);

public slots:

    void execute() override;

protected slots:

// Add appimagehub.com as attica provider
    void handleAtticaProviderAdded(const Attica::Provider& provider);

    void handleAtticaFailedToLoad(const QUrl& provider, QNetworkReply::NetworkError error);

// Resolve AppImage file download link
    void getDownloadLink();

    void handleGetDownloadLinkJobFinished(Attica::BaseJob* job);

// Download AppImage file
    void handleDownloadProgress(qint64 progress, qint64 total, const QString& message);

    void handleDownloadCompleted();

    void handleDownloadFailed(const QString& message);

private:
    void installAppImage();

    void createApplicationsDir();

    void showInlineMessage(const QString& message);

    QString buildTargetPath(Attica::Content content);

    Attica::ProviderManager providerManager;
    Attica::Provider provider;
    FileDownload* fileDownload;
    QString appId;

    QString targetPath;

    QTextStream out;
};
