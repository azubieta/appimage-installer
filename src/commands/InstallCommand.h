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

    void handleDownloadProgress(qint64 progress, qint64 total, const QString& message);

    void handleDownloadCompleted();

    void handleDownloadFailed(const QString& message);

    void handleAtticaProviderAdded(const Attica::Provider& provider);

    void getDownloadLink();

    void handleGetDownloadLinkJobFinished(Attica::BaseJob* job);

    void handleAtticaFailedToLoad(const QUrl& provider, QNetworkReply::NetworkError error) {
        emit Command::executionFailed("Unable to connect to " + provider.toString());
    }

private:
    void createApplicationsDir();

    QString buildTargetPath(const QString& contentId);

    Attica::ProviderManager providerManager;
    Attica::Provider provider;

    FileDownload* fileDownload;
    QString appId;
    QString targetPath;
    QTextStream out;

    void showInlineMessage(const QString& message);
};
