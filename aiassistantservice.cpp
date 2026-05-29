#include "aiassistantservice.h"
#include "airesponse.h"

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcessEnvironment>
#include <QStringList>
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QRegularExpression>

const QString AIAssistantService::kApiUrl = QStringLiteral("https://models.inference.ai.azure.com/chat/completions");
const QString AIAssistantService::kGeminiApiUrlTemplate =
    QStringLiteral("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=%1");
const QString AIAssistantService::kImageApiUrl = QStringLiteral("https://models.inference.ai.azure.com/images/generate?api-version=2024-12-01");

const QString AIAssistantService::kApiKeyPlaceholder = QStringLiteral("AZURE_AI_API_KEY_HERE");

AIAssistantService::AIAssistantService(QObject *parent)
    : QObject(parent)
{
    connect(&manager, &QNetworkAccessManager::finished,
            this, &AIAssistantService::onReplyFinished);
}

void AIAssistantService::setApiKey(const QString &apiKey)
{
    overrideApiKey = apiKey.trimmed();
}

bool AIAssistantService::hasApiKey() const
{
    const QString azureKey = resolveApiKey();
    const QString geminiKey = resolveGeminiApiKey();
    const bool hasAzure = !azureKey.isEmpty()
           && azureKey != kApiKeyPlaceholder
           && azureKey.compare(QStringLiteral("your-api-key-here"), Qt::CaseInsensitive) != 0;
    const bool hasGemini = !geminiKey.isEmpty()
           && geminiKey != kApiKeyPlaceholder
           && geminiKey.compare(QStringLiteral("your-api-key-here"), Qt::CaseInsensitive) != 0;
    return hasAzure || hasGemini;
}

static bool isValidConfiguredKey(const QString &key)
{
    return !key.isEmpty()
           && key != QStringLiteral("API_KEY_HERE")
           && key.compare(QStringLiteral("your-api-key-here"), Qt::CaseInsensitive) != 0;
}

void AIAssistantService::askEmployeeAssistant(const QString &userPrompt)
{
    if (!isValidConfiguredKey(resolveApiKey())) {
        emit requestFailed(AIResponse::error(QStringLiteral("Azure key is not set. Define AZURE_AI_API_KEY for employee assistant.")));
        return;
    }

    currentMode = RequestMode::EmployeeAssistant;
    currentMessages = QJsonArray();

    currentMessages.append(QJsonObject{
        {QStringLiteral("role"), QStringLiteral("system")},
        {QStringLiteral("content"), QStringLiteral("Tu es un assistant RH intelligent. Tu as un accès direct à la base de données Oracle de l'entreprise via l'outil 'executer_requete_sql'. Le schéma inclut la table EMPLOYEE(IDEMPLOYEE, CIN, MAIL, NOM, PRENOM, TELEPHONE, DATE_NAISSANCE, DATE_EMBAUCHE, TYPE_CONTRAT, SALAIRE_BASE, STATUT, ROLE, IDFABRICATION). Réponds aux questions en interrogeant la BD si nécessaire. Ne génère pas de SQL pour insérer, modifier ou supprimer, tu es en LECTURE SEULE. Sois concis et professionnel en français.")}
    });
    currentMessages.append(QJsonObject{
        {QStringLiteral("role"), QStringLiteral("user")},
        {QStringLiteral("content"), userPrompt.trimmed()}
    });

    sendChatRequest();
}

void AIAssistantService::askFurnitureAssistant(const QString &userPrompt)
{
    if (!hasApiKey()) {
        emit requestFailed(AIResponse::error(QStringLiteral("API key is not set. Add GEMINI_API_KEY or AZURE_AI_API_KEY and retry.")));
        return;
    }

    currentMode = RequestMode::FurnitureAssistant;
    currentMessages = QJsonArray();

    currentMessages.append(QJsonObject{
        {QStringLiteral("role"), QStringLiteral("system")},
        {QStringLiteral("content"), resolveFurniturePrompt()}
    });
    currentMessages.append(QJsonObject{
        {QStringLiteral("role"), QStringLiteral("user")},
        {QStringLiteral("content"), userPrompt.trimmed()}
    });

    const QString geminiKey = resolveGeminiApiKey();
    if (isValidConfiguredKey(geminiKey)) {
        sendGeminiFurnitureRequest();
    } else {
        sendChatRequest();
    }
}

void AIAssistantService::askImageGeneration(const QString &prompt, const QString &requestId)
{
    if (!isValidConfiguredKey(resolveApiKey())) {
        emit imageRequestFailed(requestId, QStringLiteral("Azure key is not set. Define AZURE_AI_API_KEY for image generation."));
        return;
    }

    currentMode = RequestMode::ImageGeneration;
    currentPrompt = prompt.trimmed();
    currentRequestId = requestId;
    sendImageRequest();
}

void AIAssistantService::sendChatRequest()
{
    QNetworkRequest request{QUrl(kApiUrl)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    const QByteArray apiToken = resolveApiKey().toUtf8();
    request.setRawHeader("api-key", apiToken);
    request.setRawHeader("Authorization", QByteArray("Bearer ") + apiToken);

    QJsonObject payload;
    payload.insert(QStringLiteral("model"), QStringLiteral("gpt-4.1-mini"));
    payload.insert(QStringLiteral("messages"), currentMessages);
    payload.insert(QStringLiteral("temperature"), 0.2);

    if (currentMode == RequestMode::EmployeeAssistant) {
        QJsonArray tools;
        QJsonObject funcDef;
        funcDef.insert(QStringLiteral("name"), QStringLiteral("executer_requete_sql"));
        funcDef.insert(QStringLiteral("description"), QStringLiteral("Exécute une requête SQL SELECT sur la BD Oracle."));

        QJsonObject parameters, properties, queryParam;
        parameters.insert(QStringLiteral("type"), QStringLiteral("object"));
        queryParam.insert(QStringLiteral("type"), QStringLiteral("string"));
        queryParam.insert(QStringLiteral("description"), QStringLiteral("La requête SQL SELECT valide à exécuter. Ex: SELECT NOM, ROLE FROM EMPLOYEE WHERE TYPE_CONTRAT = 'CDD'"));
        properties.insert(QStringLiteral("query"), queryParam);
        parameters.insert(QStringLiteral("properties"), properties);
        parameters.insert(QStringLiteral("required"), QJsonArray() << QStringLiteral("query"));
        funcDef.insert(QStringLiteral("parameters"), parameters);

        QJsonObject tool;
        tool.insert(QStringLiteral("type"), QStringLiteral("function"));
        tool.insert(QStringLiteral("function"), funcDef);
        tools.append(tool);

        payload.insert(QStringLiteral("tools"), tools);
    }

    QNetworkReply *reply = manager.post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
    reply->setProperty("requestMode", static_cast<int>(currentMode));
}

void AIAssistantService::sendGeminiFurnitureRequest()
{
    const QString geminiKey = resolveGeminiApiKey();
    QNetworkRequest request{QUrl(kGeminiApiUrlTemplate.arg(geminiKey))};
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    QString combinedPrompt;
    for (const QJsonValue &v : currentMessages) {
        const QJsonObject m = v.toObject();
        const QString role = m.value(QStringLiteral("role")).toString();
        const QString content = m.value(QStringLiteral("content")).toString();
        if (!content.trimmed().isEmpty()) {
            combinedPrompt += QStringLiteral("[%1]\n%2\n\n").arg(role, content);
        }
    }

    QJsonObject payload;
    QJsonArray contents;
    QJsonObject userPart;
    userPart.insert(QStringLiteral("text"), combinedPrompt.trimmed());
    QJsonArray parts;
    parts.append(userPart);
    QJsonObject contentObj;
    contentObj.insert(QStringLiteral("parts"), parts);
    contents.append(contentObj);
    payload.insert(QStringLiteral("contents"), contents);

    QJsonObject generationConfig;
    generationConfig.insert(QStringLiteral("temperature"), 0.2);
    payload.insert(QStringLiteral("generationConfig"), generationConfig);

    QNetworkReply *reply = manager.post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
    reply->setProperty("requestMode", static_cast<int>(currentMode));
    reply->setProperty("provider", QStringLiteral("gemini"));
}

void AIAssistantService::sendImageRequest()
{
    QNetworkRequest request{QUrl(resolveImageApiUrl())};
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    const QByteArray apiToken = resolveApiKey().toUtf8();
    request.setRawHeader("api-key", apiToken);
    request.setRawHeader("Authorization", QByteArray("Bearer ") + apiToken);

    QJsonObject payload;
    payload.insert(QStringLiteral("model"), QStringLiteral("gpt-image-1"));
    payload.insert(QStringLiteral("prompt"), currentPrompt);
    payload.insert(QStringLiteral("size"), QStringLiteral("256x256"));

    QNetworkReply *reply = manager.post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
    reply->setProperty("requestMode", static_cast<int>(currentMode));
    reply->setProperty("requestId", currentRequestId);
    reply->setProperty("prompt", currentPrompt);
}

QString AIAssistantService::resolvePiecesAssetDirectory() const
{
    const QString preferredAbsolutePath = QDir::cleanPath(
        QStringLiteral("C:/Users/msi/Downloads/Nouveau dossier (1) (1)/Nouveau dossier (1)/Nouveau dossier/piece_images"));
    const QFileInfo preferredInfo(preferredAbsolutePath);
    if (preferredInfo.exists() && preferredInfo.isDir()) {
        return preferredInfo.absoluteFilePath();
    }

    const QString appDir = QCoreApplication::applicationDirPath();
    QDir dirPieceSearch(appDir);

    for (int depth = 0; depth < 8; ++depth) {
        const QString pieceImagesCandidate = QDir(dirPieceSearch).filePath(QStringLiteral("piece_images"));
        const QFileInfo pieceImagesInfo(pieceImagesCandidate);
        if (pieceImagesInfo.exists() && pieceImagesInfo.isDir()) {
            return pieceImagesInfo.absoluteFilePath();
        }

        if (!dirPieceSearch.cdUp()) {
            break;
        }
    }

    QDir dirAssetsSearch(appDir);
    for (int depth = 0; depth < 8; ++depth) {
        const QString assetsCandidate = QDir(dirAssetsSearch).filePath(QStringLiteral("assets/pieces"));
        const QFileInfo assetsInfo(assetsCandidate);
        if (assetsInfo.exists() && assetsInfo.isDir()) {
            return assetsInfo.absoluteFilePath();
        }
        if (!dirAssetsSearch.cdUp()) {
            break;
        }
    }

    const QString cwdPieceImages = QDir::current().filePath(QStringLiteral("piece_images"));
    const QFileInfo cwdPieceImagesInfo(cwdPieceImages);
    if (cwdPieceImagesInfo.exists() && cwdPieceImagesInfo.isDir()) {
        return cwdPieceImagesInfo.absoluteFilePath();
    }

    const QString cwdAssets = QDir::current().filePath(QStringLiteral("assets/pieces"));
    const QFileInfo cwdAssetsInfo(cwdAssets);
    if (cwdAssetsInfo.exists() && cwdAssetsInfo.isDir()) {
        return cwdAssetsInfo.absoluteFilePath();
    }

    QDir fallbackDir(preferredAbsolutePath);
    if (!fallbackDir.exists()) {
        fallbackDir.mkpath(QStringLiteral("."));
    }
    if (fallbackDir.exists()) {
        return fallbackDir.absolutePath();
    }

    fallbackDir = QDir(QDir::current().filePath(QStringLiteral("piece_images")));
    if (!fallbackDir.exists()) {
        fallbackDir.mkpath(QStringLiteral("."));
    }

    return fallbackDir.absolutePath();
}

QString AIAssistantService::resolveImageApiUrl() const
{
    const QString envUrl = QProcessEnvironment::systemEnvironment().value(QStringLiteral("AZURE_IMAGE_API_URL")).trimmed();
    return envUrl.isEmpty() ? kImageApiUrl : envUrl;
}

QString AIAssistantService::executeSafeSql(const QString &queryStr)
{
    QString trimmedQuery = queryStr.trimmed();
    QString upperQuery = trimmedQuery.toUpper();

    if (!upperQuery.startsWith(QStringLiteral("SELECT"))) {
        return QStringLiteral("ERREUR DE SÉCURITÉ : Seules les requêtes SELECT sont autorisées.");
    }

    QRegularExpression dangerousWords(QStringLiteral("\\b(UPDATE|DELETE|DROP|INSERT|ALTER|CREATE|TRUNCATE|GRANT|REVOKE)\\b"), QRegularExpression::CaseInsensitiveOption);
    if (dangerousWords.match(upperQuery).hasMatch()) {
        return QStringLiteral("ERREUR DE SÉCURITÉ : Mots-clés de modification détectés dans la requête.");
    }

    QSqlQuery q;
    if (!q.exec(trimmedQuery)) {
        return QStringLiteral("ERREUR SQL : ") + q.lastError().text();
    }

    QStringList rows;
    while (q.next()) {
        QStringList rowVals;
        for (int i = 0; i < q.record().count(); ++i) {
            rowVals << q.value(i).toString();
        }
        rows << rowVals.join(QStringLiteral(" | "));
    }

    if (rows.isEmpty()) {
        return QStringLiteral("Aucun résultat trouvé.");
    }

    QString result = rows.join(QStringLiteral("\n"));
    if (result.length() > 4000) {
        result = result.left(4000) + QStringLiteral("\n... (trop de résultats, texte tronqué)");
    }

    return result;
}

void AIAssistantService::onReplyFinished(QNetworkReply *reply)
{
    const QByteArray raw = reply->readAll();
    const int replyModeInt = reply->property("requestMode").toInt();
    const RequestMode replyMode = static_cast<RequestMode>(replyModeInt);
    const QString provider = reply->property("provider").toString();
    QString replyRequestId = reply->property("requestId").toString();
    if (replyRequestId.isEmpty()) {
        replyRequestId = currentRequestId;
    }

    if (reply->error() != QNetworkReply::NoError) {
        QString sanitized = reply->errorString();
        sanitized.replace(QRegularExpression(QStringLiteral("key=[^\\s&]+"), QRegularExpression::CaseInsensitiveOption),
                          QStringLiteral("key=***"));
        const QString message = QStringLiteral("API request failed: %1").arg(sanitized);
        reply->deleteLater();
        if (replyMode == RequestMode::ImageGeneration || replyMode == RequestMode::ImageDownload) {
            emit imageRequestFailed(replyRequestId, message);
        } else {
            emit requestFailed(AIResponse::error(message));
        }
        return;
    }

    if (replyMode == RequestMode::ImageDownload) {
        const QByteArray imageBytes = raw;
        if (imageBytes.isEmpty()) {
            reply->deleteLater();
            emit imageRequestFailed(replyRequestId, QStringLiteral("Downloaded image bytes are empty."));
            return;
        }

        const QString outputDir = QDir(resolvePiecesAssetDirectory()).filePath(QStringLiteral("generated"));
        QDir().mkpath(outputDir);
        const QString outputPath = QDir(outputDir).filePath(replyRequestId + QStringLiteral(".png"));
        QFile imageFile(outputPath);
        if (!imageFile.open(QIODevice::WriteOnly)) {
            reply->deleteLater();
            emit imageRequestFailed(replyRequestId, QStringLiteral("Unable to save downloaded image to disk."));
            return;
        }

        imageFile.write(imageBytes);
        imageFile.close();
        reply->deleteLater();
        emit imageReady(replyRequestId, outputPath);
        return;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(raw, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        reply->deleteLater();
        if (replyMode == RequestMode::ImageGeneration) {
            emit imageRequestFailed(replyRequestId, QStringLiteral("Invalid JSON response from API."));
        } else {
            emit requestFailed(AIResponse::error(QStringLiteral("Invalid JSON response from API.")));
        }
        return;
    }

    const QJsonObject root = doc.object();

    if (provider == QStringLiteral("gemini")) {
        const QJsonArray candidates = root.value(QStringLiteral("candidates")).toArray();
        if (candidates.isEmpty()) {
            reply->deleteLater();
            emit requestFailed(AIResponse::error(QStringLiteral("Gemini response has no candidates.")));
            return;
        }
        const QJsonObject contentObj = candidates.at(0).toObject().value(QStringLiteral("content")).toObject();
        const QJsonArray parts = contentObj.value(QStringLiteral("parts")).toArray();
        QString text;
        for (const QJsonValue &p : parts) {
            const QString t = p.toObject().value(QStringLiteral("text")).toString();
            if (!t.isEmpty()) {
                if (!text.isEmpty()) text += "\n";
                text += t;
            }
        }
        if (text.trimmed().isEmpty()) {
            reply->deleteLater();
            emit requestFailed(AIResponse::error(QStringLiteral("Gemini returned empty content.")));
            return;
        }
        reply->deleteLater();
        emit responseReady(AIResponse::success(text.trimmed(), QStringLiteral("Google Gemini Flash")));
        return;
    }
    const QJsonArray choices = root.value(QStringLiteral("choices")).toArray();

    if (replyMode == RequestMode::ImageGeneration) {
        QJsonArray dataArray = root.value(QStringLiteral("data")).toArray();
        if (dataArray.isEmpty() && !choices.isEmpty()) {
            dataArray = choices.at(0).toObject().value(QStringLiteral("data")).toArray();
        }

        if (dataArray.isEmpty()) {
            reply->deleteLater();
            emit imageRequestFailed(replyRequestId, QStringLiteral("No image data returned by API."));
            return;
        }

        QString base64Data = dataArray.at(0).toObject().value(QStringLiteral("b64_json")).toString();
        if (base64Data.isEmpty()) {
            const QString rawText = dataArray.at(0).toObject().value(QStringLiteral("text")).toString();
            if (rawText.startsWith(QStringLiteral("data:image/png;base64,"))) {
                base64Data = rawText.mid(QStringLiteral("data:image/png;base64,").size()).trimmed();
            }
        }

        if (base64Data.isEmpty()) {
            const QString imageUrl = dataArray.at(0).toObject().value(QStringLiteral("url")).toString();
            if (!imageUrl.isEmpty()) {
                QNetworkRequest downloadRequest{QUrl(imageUrl)};
                QNetworkReply *downloadReply = manager.get(downloadRequest);
                downloadReply->setProperty("requestMode", static_cast<int>(RequestMode::ImageDownload));
                downloadReply->setProperty("requestId", replyRequestId);
                reply->deleteLater();
                return;
            }

            const QString rawText = dataArray.at(0).toObject().value(QStringLiteral("text")).toString();
            if (rawText.startsWith(QStringLiteral("data:image/png;base64,"))) {
                base64Data = rawText.mid(QStringLiteral("data:image/png;base64,").size()).trimmed();
            }
        }

        if (base64Data.isEmpty()) {
            const QString rawText = choices.isEmpty() ? QString() : choices.at(0).toObject().value(QStringLiteral("message")).toObject().value(QStringLiteral("content")).toString();
            if (rawText.startsWith(QStringLiteral("data:image/png;base64,"))) {
                base64Data = rawText.mid(QStringLiteral("data:image/png;base64,").size()).trimmed();
            }
        }

        if (base64Data.isEmpty()) {
            reply->deleteLater();
            emit imageRequestFailed(replyRequestId, QStringLiteral("Image response did not contain base64 image data."));
            return;
        }

        const QByteArray imageBytes = QByteArray::fromBase64(base64Data.toUtf8());
        if (imageBytes.isEmpty()) {
            reply->deleteLater();
            emit imageRequestFailed(replyRequestId, QStringLiteral("Failed to decode base64 image data."));
            return;
        }

        const QString outputDir = QDir(resolvePiecesAssetDirectory()).filePath(QStringLiteral("generated"));
        QDir().mkpath(outputDir);
        const QString outputPath = QDir(outputDir).filePath(replyRequestId + QStringLiteral(".png"));
        QFile imageFile(outputPath);
        if (!imageFile.open(QIODevice::WriteOnly)) {
            reply->deleteLater();
            emit imageRequestFailed(replyRequestId, QStringLiteral("Unable to save generated image to disk."));
            return;
        }

        imageFile.write(imageBytes);
        imageFile.close();
        reply->deleteLater();
        emit imageReady(replyRequestId, outputPath);
        return;
    }

    if (choices.isEmpty()) {
        reply->deleteLater();
        emit requestFailed(AIResponse::error(QStringLiteral("No response choices returned by API.")));
        return;
    }

    const QJsonObject messageObj = choices.at(0).toObject().value(QStringLiteral("message")).toObject();

    if (messageObj.contains(QStringLiteral("tool_calls"))) {
        QJsonArray toolCalls = messageObj.value(QStringLiteral("tool_calls")).toArray();
        if (!toolCalls.isEmpty()) {
            QJsonObject firstCall = toolCalls.at(0).toObject();
            QJsonObject functionObj = firstCall.value(QStringLiteral("function")).toObject();
            QString functionName = functionObj.value(QStringLiteral("name")).toString();
            QString argsStr = functionObj.value(QStringLiteral("arguments")).toString();
            QString toolCallId = firstCall.value(QStringLiteral("id")).toString();

            currentMessages.append(messageObj);

            if (functionName == QStringLiteral("executer_requete_sql")) {
                QJsonParseError argsError;
                QJsonDocument argsDoc = QJsonDocument::fromJson(argsStr.toUtf8(), &argsError);
                QString sqlQuery = argsDoc.object().value(QStringLiteral("query")).toString();

                QString sqlResult = executeSafeSql(sqlQuery);

                QJsonObject toolMsg;
                toolMsg.insert(QStringLiteral("role"), QStringLiteral("tool"));
                toolMsg.insert(QStringLiteral("tool_call_id"), toolCallId);
                toolMsg.insert(QStringLiteral("name"), functionName);
                toolMsg.insert(QStringLiteral("content"), sqlResult);
                currentMessages.append(toolMsg);

                reply->deleteLater();
                sendChatRequest();
                return;
            } else {
                QJsonObject toolMsg;
                toolMsg.insert(QStringLiteral("role"), QStringLiteral("tool"));
                toolMsg.insert(QStringLiteral("tool_call_id"), toolCallId);
                toolMsg.insert(QStringLiteral("name"), functionName);
                toolMsg.insert(QStringLiteral("content"), QStringLiteral("Erreur: Outil non reconnu."));
                currentMessages.append(toolMsg);

                reply->deleteLater();
                sendChatRequest();
                return;
            }
        }
    }

    const QString content = messageObj.value(QStringLiteral("content")).toString().trimmed();
    if (content.isEmpty() && !messageObj.contains(QStringLiteral("tool_calls"))) {
        reply->deleteLater();
        emit requestFailed(AIResponse::error(QStringLiteral("API returned an empty response.")));
        return;
    }

    reply->deleteLater();
    emit responseReady(AIResponse::success(content, QStringLiteral("Azure AI")));
}

QString AIAssistantService::resolveApiKey() const
{
    if (!overrideApiKey.isEmpty()) {
        return overrideApiKey;
    }

    const QString envKey = QProcessEnvironment::systemEnvironment().value(QStringLiteral("AZURE_AI_API_KEY")).trimmed();
    if (!envKey.isEmpty()) {
        return envKey;
    }

    return kApiKeyPlaceholder;
}

QString AIAssistantService::resolveGeminiApiKey() const
{
    const QString envKey = QProcessEnvironment::systemEnvironment().value(QStringLiteral("GEMINI_API_KEY")).trimmed();
    if (!envKey.isEmpty()) {
        return envKey;
    }
    return QString();
}

QString AIAssistantService::resolveFurniturePrompt() const
{
    const QString localPath = QDir::current().filePath(QStringLiteral("PROMPT_ANALYSE_MEUBLE_SANS_MATERIAUX.txt"));
    QFile file(localPath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const QString loaded = QString::fromUtf8(file.readAll()).trimmed();
        file.close();
        if (!loaded.isEmpty()) {
            return loaded;
        }
    }

    return QStringLiteral(
        "ROLE\n"
        "Tu es un moteur d'analyse d'images specialise dans la detection et l'estimation de meubles.\n"
        "Reponds exclusivement en JSON valide.\n"
        "Si non meuble: {\"is_furniture\": false}\n"
        "Si meuble: retourne exactement les champs "
        "{\"is_furniture\": true, \"nom_modele\":\"\", \"categorie\":\"\", \"type\":\"\", \"dimensions\":\"\", "
        "\"prix_base\":0, \"cout_estime\":0, \"temps_realisation_estime\":0, \"description\":\"\", \"score_ia\":0.0}\n"
        "Contraintes: cout_estime < prix_base, dimensions en cm (L x l x H), pas de materiaux, valeurs realistes marche tunisien."
    );
}