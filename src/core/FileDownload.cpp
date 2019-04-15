#include "FileDownload.h"

FileDownload::FileDownload(const QUrl& url, const QString& target_path, QObject* parent)
        : Download(url, parent), file(target_path) {

}

FileDownload::~FileDownload() = default;

void FileDownload::start() {
    FileDownload::file.open(QIODevice::WriteOnly);

    QObject::connect(this, &Download::completed, this, &FileDownload::handleCompleted);
    QObject::connect(this, &Download::stopped, this, &FileDownload::handleStopped);

    Download::start();
    QObject::connect(reply.data(), &QIODevice::readyRead, this,
                     &FileDownload::handleReadyRead);
}

void FileDownload::handleReadyRead() {
    downloadAvailableBytes();
}

void FileDownload::downloadAvailableBytes() {
    if (file.isOpen()) {
        file.write(reply->readAll());
        file.flush();
    }
}

void FileDownload::handleCompleted() {
    downloadAvailableBytes();

    FileDownload::file.close();
}

void FileDownload::handleStopped() {
    FileDownload::file.remove();
}

