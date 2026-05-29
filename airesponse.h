#ifndef AIRESPONSE_H
#define AIRESPONSE_H

#include <QDateTime>
#include <QJsonObject>
#include <QString>

class AIResponse
{
public:
    enum ResponseType {
        Success,
        Error,
        PartialResult,
        DataFromDatabase,
        InvalidQuery
    };

    AIResponse();

    static AIResponse success(const QString &text, const QString &source = QStringLiteral("LLM"));
    static AIResponse fromDatabase(const QString &data, const QString &query);
    static AIResponse error(const QString &message);
    static AIResponse partialResult(const QString &text);

    ResponseType type() const { return responseType; }
    QString text() const { return responseText; }
    QString source() const { return dataSource; }
    QString query() const { return executedQuery; }
    QDateTime timestamp() const { return createdAt; }
    bool isSuccess() const { return responseType == Success || responseType == DataFromDatabase; }
    int rowCount() const { return databaseRowCount; }

    QJsonObject toJson() const;
    QString toDisplayString() const;

private:
    ResponseType responseType;
    QString responseText;
    QString dataSource;
    QString executedQuery;
    QDateTime createdAt;
    int databaseRowCount;

    AIResponse(ResponseType type);
};

#endif // AIRESPONSE_H