#include "airesponse.h"

AIResponse::AIResponse()
    : responseType(Error),
      createdAt(QDateTime::currentDateTime()),
      databaseRowCount(0)
{
}

AIResponse::AIResponse(ResponseType type)
    : responseType(type),
      createdAt(QDateTime::currentDateTime()),
      databaseRowCount(0)
{
}

AIResponse AIResponse::success(const QString &text, const QString &source)
{
    AIResponse response(Success);
    response.responseText = text.trimmed();
    response.dataSource = source;
    return response;
}

AIResponse AIResponse::fromDatabase(const QString &data, const QString &query)
{
    AIResponse response(DataFromDatabase);
    response.responseText = data;
    response.executedQuery = query;
    response.dataSource = QStringLiteral("DATABASE");
    response.databaseRowCount = data.isEmpty() ? 0 : data.count('\n') + 1;
    return response;
}

AIResponse AIResponse::error(const QString &message)
{
    AIResponse response(Error);
    response.responseText = message;
    response.dataSource = QStringLiteral("ERROR");
    return response;
}

AIResponse AIResponse::partialResult(const QString &text)
{
    AIResponse response(PartialResult);
    response.responseText = text;
    response.dataSource = QStringLiteral("LLM");
    return response;
}

QJsonObject AIResponse::toJson() const
{
    QJsonObject obj;

    QString typeName;
    switch (responseType) {
    case Success:
        typeName = QStringLiteral("SUCCESS");
        break;
    case Error:
        typeName = QStringLiteral("ERROR");
        break;
    case PartialResult:
        typeName = QStringLiteral("PARTIAL");
        break;
    case DataFromDatabase:
        typeName = QStringLiteral("DATABASE");
        break;
    case InvalidQuery:
        typeName = QStringLiteral("INVALID_QUERY");
        break;
    }

    obj.insert(QStringLiteral("type"), typeName);
    obj.insert(QStringLiteral("text"), responseText);
    obj.insert(QStringLiteral("source"), dataSource);
    obj.insert(QStringLiteral("timestamp"), createdAt.toString(Qt::ISODate));

    if (!executedQuery.isEmpty()) {
        obj.insert(QStringLiteral("query"), executedQuery);
    }

    if (databaseRowCount > 0) {
        obj.insert(QStringLiteral("rowCount"), databaseRowCount);
    }

    return obj;
}

QString AIResponse::toDisplayString() const
{
    QString display = responseText;

    if (responseType == DataFromDatabase && databaseRowCount > 0) {
        display.prepend(QStringLiteral("DATABASE (%1 rows)\n").arg(databaseRowCount));
    }

    return display;
}