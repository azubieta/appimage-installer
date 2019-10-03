#pragma once

// library
#include <QFile>
#include <QObject>
#include <QString>
#include <QNetworkReply>
#include <QNetworkAccessManager>

// local
#include "Download.h"

class FileDownload : public Download {
Q_OBJECT
    QFile file;

public:
    FileDownload(const QUrl& url, const QString& target_path, QObject* parent = nullptr);

    ~FileDownload() override;

public slots:

    void start() override;

protected slots:

    void handleReadyRead();

    void handleCompleted();

    void handleStopped();

private:
    void downloadAvailableBytes();
};
