#pragma once

// library
#include <QTimer>
#include <QNetworkReply>
#include <QSharedPointer>

class Download : public QObject {
Q_OBJECT
    QString source_url;
    bool running;
    bool progressNotificationsEnabled;

public:
    explicit Download(QString url, QObject *parent = nullptr);

    virtual ~Download();

    void setProgressNotificationsEnabled(bool progressNotificationsEnabled);

    virtual void start();

    virtual void stop();

    bool isRunning();

signals:

    void completed();

    void stopped(const QString &message);

    void progress(qint64 progress, qint64 total, const QString &message);

protected slots:

    virtual void handleDownloadProgress(qint64 bytesRead, qint64 totalBytes);

    virtual void handleTimerTick();

    virtual void handleFinished();

protected:
    bool isStopRequested;
    QNetworkAccessManager *networkAccessManager;
    QSharedPointer<QNetworkReply> reply;

    QTimer timer;
    float speed;
    qint64 totalBytes;
    qint64 bytesRead;
    qint64 bytesReadLastTick;

    static QString formatMemoryValue(float num);

    void createNetworkAccessManagerIfNotExist();

    void reportProgress();

    void updateDownloadSpeed();

    bool wasCompletedProperly() const;

    QNetworkRequest createRequest() const;
};
