#pragma once

// libraries
#include <QObject>
#include <QString>
#include <Attica/ProviderManager>
#include <Attica/ListJob>
#include <Attica/DownloadDescription>

// local
#include <Command.h>

class FileDownload;

class InstallCommand : public Command {
Q_OBJECT
public:
    explicit InstallCommand(const QString& appId, QObject* parent = nullptr);

    void setOcsProvidersList(QStringList list);

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

    QString buildTargetPath(const Attica::Content& content, const Attica::DownloadDescription& download);

    Attica::ProviderManager providerManager;
    Attica::Provider provider;
    FileDownload* fileDownload;
    QString appId;

    QString targetPath;

    QTextStream out;
    QStringList ocsProvidersList;

    void startFileDownload(const QUrl& downloadLink);

    int askWhichFileDownload(const QList<Attica::DownloadDescription>& compatibleDownloads);

    int libappimage_install() const;
};
