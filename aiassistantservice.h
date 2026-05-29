#ifndef AIASSISTANTSERVICE_H
#define AIASSISTANTSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include "airesponse.h"

class QNetworkReply;

class AIAssistantService : public QObject
{
    Q_OBJECT

public:
    explicit AIAssistantService(QObject *parent = nullptr);

    void setApiKey(const QString &apiKey);
    bool hasApiKey() const;

    void askEmployeeAssistant(const QString &userPrompt);
    void askFurnitureAssistant(const QString &userPrompt);
    void askImageGeneration(const QString &prompt, const QString &requestId);

signals:
    void responseReady(const AIResponse &response);
    void requestFailed(const AIResponse &errorResponse);
    void imageReady(const QString &requestId, const QString &imagePath);
    void imageRequestFailed(const QString &requestId, const QString &errorMessage);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    enum class RequestMode {
        EmployeeAssistant,
        FurnitureAssistant,
        ImageGeneration,
        ImageDownload
    };

    QString resolveApiKey() const;
    QString resolveGeminiApiKey() const;
    QString resolveFurniturePrompt() const;
    QString resolvePiecesAssetDirectory() const;
    void sendChatRequest();
    void sendGeminiFurnitureRequest();
    QString executeSafeSql(const QString &queryStr);

    QNetworkAccessManager manager;
    QString overrideApiKey;
    QJsonArray currentMessages;
    QString currentPrompt;
    QString currentRequestId;
    RequestMode currentMode = RequestMode::EmployeeAssistant;

    static const QString kApiUrl;
    static const QString kGeminiApiUrlTemplate;
    static const QString kImageApiUrl;
    static const QString kApiKeyPlaceholder;

    void sendImageRequest();
    QString resolveImageApiUrl() const;
};

#endif // AIASSISTANTSERVICE_H
