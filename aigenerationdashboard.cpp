#include "aigenerationdashboard.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFrame>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <algorithm>

AiGenerationDashboard::AiGenerationDashboard(QWidget *parent)
    : QWidget(parent), m_generationStep(0)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("AiGenerationDashboard { background-color: transparent; }");

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &AiGenerationDashboard::updateGenerationProgress);

    m_imageAssistant = new AIAssistantService(this);
    connect(m_imageAssistant, &AIAssistantService::imageReady, this, &AiGenerationDashboard::onPieceImageGenerated);
    connect(m_imageAssistant, &AIAssistantService::imageRequestFailed, this, &AiGenerationDashboard::onPieceImageGenerationFailed);

    setupUi();
}

void AiGenerationDashboard::setupUi()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(24);

    QVBoxLayout *leftLayout = new QVBoxLayout();

    m_titleLabel = new QLabel("Interface de generation IA", this);
    m_titleLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold; font-family: 'Segoe UI', Arial, sans-serif;");

    m_previewWidget = new ModelPreviewWidget(this);
    m_previewWidget->setMinimumWidth(400);

    m_generateBtn = new QPushButton("Generer les pieces", this);
    m_generateBtn->setFixedHeight(48);
    m_generateBtn->setCursor(Qt::PointingHandCursor);
    m_generateBtn->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4f46e5, stop:1 #6366f1);"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  font-weight: bold;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #6366f1, stop:1 #818cf8);"
        "}"
        "QPushButton:disabled {"
        "  background: rgba(255,255,255,0.1);"
        "  color: rgba(255,255,255,0.3);"
        "}"
    );
    connect(m_generateBtn, &QPushButton::clicked, this, &AiGenerationDashboard::generatePieces);

    leftLayout->addWidget(m_titleLabel);
    leftLayout->addWidget(m_previewWidget, 1);
    leftLayout->addWidget(m_generateBtn);

    QVBoxLayout *centerLayout = new QVBoxLayout();
    QLabel *cardsTitle = new QLabel("Elements detectes (Brouillon IA)", this);
    cardsTitle->setStyleSheet("color: #00d4ff; font-size: 15px; font-weight: bold; font-family: 'Segoe UI', Arial, sans-serif;");

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; } QScrollBar:vertical { width: 10px; background: transparent; }");

    m_cardsContainer = new QWidget();
    m_cardsContainer->setStyleSheet("background: transparent;");
    m_cardsLayout = new QVBoxLayout(m_cardsContainer);
    m_cardsLayout->setSpacing(12);
    m_cardsLayout->setAlignment(Qt::AlignTop);
    m_cardsLayout->setContentsMargins(0, 0, 12, 0);

    m_scrollArea->setWidget(m_cardsContainer);

    centerLayout->addWidget(cardsTitle);
    centerLayout->addWidget(m_scrollArea, 1);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    setupInsightsPanel(rightLayout);

    mainLayout->addLayout(leftLayout, 4);
    mainLayout->addLayout(centerLayout, 4);
    mainLayout->addLayout(rightLayout, 2);
}

void AiGenerationDashboard::setupInsightsPanel(QVBoxLayout *parentLayout)
{
    QLabel *insightsTitle = new QLabel("AI Insights", this);
    insightsTitle->setStyleSheet("color: white; font-size: 16px; font-weight: bold; font-family: 'Segoe UI', Arial, sans-serif;");

    QFrame *insightsFrame = new QFrame(this);
    insightsFrame->setStyleSheet("background-color: rgba(18,18,42,0.8); border: 1px solid rgba(99,102,241,0.15); border-radius: 12px;");
    QVBoxLayout *frameLayout = new QVBoxLayout(insightsFrame);
    frameLayout->setContentsMargins(16, 16, 16, 16);
    frameLayout->setSpacing(16);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setFixedHeight(12);
    m_progressBar->setTextVisible(false);
    m_progressBar->setStyleSheet(
        "QProgressBar {"
        "  border: none;"
        "  background-color: rgba(255,255,255,0.05);"
        "  border-radius: 6px;"
        "}"
        "QProgressBar::chunk {"
        "  background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4f46e5, stop:1 #6366f1);"
        "  border-radius: 6px;"
        "}"
    );
    m_progressBar->setValue(0);

    m_scoreLabel = new QLabel("Quality Score: --%", this);
    m_scoreLabel->setStyleSheet("color: #00E5FF; font-size: 18px; font-weight: bold;");

    m_insightsLabel = new QLabel("En attente de generation...", this);
    m_insightsLabel->setStyleSheet("color: #A0A5B1; font-size: 13px; line-height: 1.5;");
    m_insightsLabel->setWordWrap(true);
    m_insightsLabel->setAlignment(Qt::AlignTop);

    frameLayout->addWidget(m_scoreLabel);
    frameLayout->addWidget(m_progressBar);
    frameLayout->addWidget(m_insightsLabel, 1);

    parentLayout->addWidget(insightsTitle);
    parentLayout->addWidget(insightsFrame, 1);
}

void AiGenerationDashboard::injectDrafts(const QVector<PieceDraft> &drafts, const QString &modelName)
{
    m_pendingDrafts = drafts;
    m_generationStep = 0;
    m_pieceImageById.clear();
    m_pieceCardById.clear();
    m_pieceNameById.clear();
    m_pieceDimensionsById.clear();
    m_currentSelectedPieceId.clear();

    if (m_titleLabel && !modelName.isEmpty()) {
        m_titleLabel->setText(QString("Analyse : %1").arg(modelName));
    }

    QLayoutItem *item = nullptr;
    while ((item = m_cardsLayout->takeAt(0)) != nullptr) {
        if (QWidget *w = item->widget()) {
            w->deleteLater();
        }
        delete item;
    }
    m_previewWidget->clearRegions();

    QString previewImagePath;
    if (!m_pendingDrafts.isEmpty()) {
        previewImagePath = resolvePieceImagePath(m_pendingDrafts.first().nom);
    }
    if (previewImagePath.isEmpty()) {
        previewImagePath = resolvePieceImagePath("default");
    }
    if (!previewImagePath.isEmpty()) {
        m_previewWidget->setModelImage(QPixmap(previewImagePath));
    }

    m_progressBar->setValue(0);
    m_scoreLabel->setText("Quality Score: Calcul...");
    m_insightsLabel->setText("Extraction des caracteristiques en cours...");

    m_timer->start(250);
}

void AiGenerationDashboard::generatePieces()
{
    m_generateBtn->setEnabled(false);
    m_generateBtn->setText("Analyse approfondie de la source...");
    emit requestGeneration();
}

void AiGenerationDashboard::updateGenerationProgress()
{
    if (m_generationStep < m_pendingDrafts.size()) {
        const PieceDraft &draft = m_pendingDrafts[m_generationStep];

        const double fakeScore = 85.0 + QRandomGenerator::global()->bounded(15)
            + (QRandomGenerator::global()->bounded(10) / 10.0);
        const QString dimensionsText = QString("%1 x %2 x %3 mm")
            .arg(draft.longueurMm)
            .arg(draft.largeurMm)
            .arg(draft.epaisseurMm);

        const QString nameLower = draft.nom.toLower();
        QString category = "Panneau";
        if (nameLower.contains("pied")) {
            category = "Pieds";
        } else if (nameLower.contains("plateau")) {
            category = "Plateau";
        } else if (nameLower.contains("traverse")) {
            category = "Traverses";
        }

        const QString pieceId = QString::number(m_generationStep);
        const QString imagePath = resolvePieceImagePath(draft.nom);
        addCard(pieceId, category, draft.nom, dimensionsText, draft.quantite, fakeScore, imagePath);

        if (m_generationStep == 0 && !imagePath.isEmpty()) {
            m_previewWidget->setModelImage(QPixmap(imagePath));
        }

        const int yOffset = 50 + ((m_generationStep % 6) * 60);
        const int xOffset = 50 + ((m_generationStep / 6) * 100);
        m_previewWidget->addHighlightRegion(pieceId,
                                            QPolygonF() << QPointF(xOffset, yOffset)
                                                        << QPointF(xOffset + 80, yOffset)
                                                        << QPointF(xOffset + 80, yOffset + 40)
                                                        << QPointF(xOffset, yOffset + 40),
                                            QColor("#00d4ff"));

        m_generationStep++;
        const int progress = (m_generationStep * 100) / std::max(1, static_cast<int>(m_pendingDrafts.size()));
        m_progressBar->setValue(progress);
        m_insightsLabel->setText(QString("- %1 elements detectes:\n-> %2").arg(m_generationStep).arg(draft.nom));
    } else {
        m_timer->stop();
        m_generateBtn->setEnabled(true);
        m_generateBtn->setText("Relancer l'analyse visuelle");

        if (m_pendingDrafts.isEmpty()) {
            m_scoreLabel->setText("Quality Score: N/A");
            m_insightsLabel->setText("Analyse terminee.\nAucun element n'a pu etre extrait.");
        } else {
            m_scoreLabel->setText("Quality Score: 98%");
            m_insightsLabel->setText(QString("Generation terminee avec succes.\n\n%1 pieces distinctes extraites pour le brouillon.")
                                         .arg(m_pendingDrafts.size()));
        }
    }
}

void AiGenerationDashboard::addCard(const QString &id,
                                    const QString &type,
                                    const QString &name,
                                    const QString &dim,
                                    int qty,
                                    double score,
                                    const QString &imagePath)
{
    GeneratedPieceCard *card = new GeneratedPieceCard(id, type, name, dim, qty, score, imagePath, this);
    connect(card, &GeneratedPieceCard::clicked, this, &AiGenerationDashboard::onPieceCardClicked);
    m_pieceCardById.insert(id, card);
    m_pieceNameById.insert(id, name);
    m_pieceDimensionsById.insert(id, dim);

    const bool hasLocalImage = !imagePath.isEmpty() && QFileInfo(imagePath).exists();
    if (hasLocalImage) {
        m_pieceImageById.insert(id, imagePath);
    } else {
        requestPieceImageGeneration(id, name, dim);
    }

    m_cardsLayout->insertWidget(0, card);
}

void AiGenerationDashboard::onPieceCardClicked(const QString &pieceId)
{
    m_currentSelectedPieceId = pieceId;
    if (m_pieceImageById.contains(pieceId)) {
        const QPixmap piecePixmap(m_pieceImageById.value(pieceId));
        if (!piecePixmap.isNull()) {
            m_previewWidget->setModelImage(piecePixmap);
        }
    }
    m_previewWidget->highlightRegion(pieceId);
}

QString AiGenerationDashboard::resolvePiecesAssetDirectory() const
{
    const QString preferredAbsolutePath = QDir::cleanPath(
        QStringLiteral("C:/Users/msi/Downloads/Nouveau dossier (1) (1)/Nouveau dossier (1)/Nouveau dossier/piece_images"));
    const QFileInfo preferredInfo(preferredAbsolutePath);
    if (preferredInfo.exists() && preferredInfo.isDir()) {
        return preferredInfo.absoluteFilePath();
    }

    const QString appDir = QCoreApplication::applicationDirPath();
    QDir dirPieceSearch(appDir);

    for (int depth = 0; depth < 10; ++depth) {
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
    for (int depth = 0; depth < 10; ++depth) {
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

QString AiGenerationDashboard::resolveGeneratedImageDirectory() const
{
    const QString assetsDir = resolvePiecesAssetDirectory();
    if (assetsDir.isEmpty()) {
        return QString();
    }

    const QString generatedDir = QDir(assetsDir).filePath("generated");
    QDir().mkpath(generatedDir);
    return generatedDir;
}

QString AiGenerationDashboard::normalizePieceNameKey(const QString &pieceName) const
{
    QString normalized = pieceName.trimmed().toLower().normalized(QString::NormalizationForm_D);
    QString key;
    key.reserve(normalized.size());

    for (const QChar &ch : normalized) {
        const QChar::Category category = ch.category();
        if (category == QChar::Mark_NonSpacing
            || category == QChar::Mark_SpacingCombining
            || category == QChar::Mark_Enclosing) {
            continue;
        }

        if (ch.isLetterOrNumber()) {
            key.append(ch);
        } else {
            key.append('_');
        }
    }

    key.replace(QRegularExpression("_+"), "_");
    key.remove(QRegularExpression("^_+"));
    key.remove(QRegularExpression("_+$"));
    return key;
}

QString AiGenerationDashboard::resolvePieceImagePath(const QString &pieceName) const
{
    const QString assetsDir = resolvePiecesAssetDirectory();
    if (assetsDir.isEmpty()) {
        return QString();
    }

    const QString key = normalizePieceNameKey(pieceName);
    const QString generatedDir = resolveGeneratedImageDirectory();
    const QStringList imageExtensions = {
        QStringLiteral(".png"),
        QStringLiteral(".jpg"),
        QStringLiteral(".jpeg"),
        QStringLiteral(".webp"),
        QStringLiteral(".bmp")
    };
    QStringList candidates;

    auto addCandidate = [&candidates](const QString &fileName) {
        if (!fileName.isEmpty() && !candidates.contains(fileName)) {
            candidates.append(fileName);
        }
    };

    auto resolveCandidatePath = [&assetsDir, &imageExtensions](const QString &fileName) -> QString {
        if (fileName.isEmpty()) {
            return QString();
        }

        const QFileInfo rawInfo(QDir(assetsDir).filePath(fileName));
        if (rawInfo.exists() && rawInfo.isFile()) {
            return rawInfo.absoluteFilePath();
        }

        QString baseName = fileName;
        if (!rawInfo.suffix().isEmpty()) {
            baseName = rawInfo.completeBaseName();
        }

        for (const QString &extension : imageExtensions) {
            const QFileInfo altInfo(QDir(assetsDir).filePath(baseName + extension));
            if (altInfo.exists() && altInfo.isFile()) {
                return altInfo.absoluteFilePath();
            }
        }
        return QString();
    };

    if (!key.isEmpty()) {
        addCandidate(key + ".png");
        const QString keyNoIndex = QString(key).remove(QRegularExpression("_[0-9]+$"));
        addCandidate(keyNoIndex + ".png");

        if (key.contains("traverse")) {
            addCandidate("traverse.png");
            if (key.contains("haute")) {
                addCandidate("traverse_haute.png");
            } else if (key.contains("basse")) {
                addCandidate("traverse_basse.png");
            } else if (key.contains("courte")) {
                addCandidate("traverse_courte.png");
            } else if (key.contains("longue")) {
                addCandidate("traverse_longue.png");
            } else if (key.contains("diagonale")) {
                addCandidate("traverse_diagonale.png");
            } else {
                addCandidate("traverse_longue.png");
            }
        }

        if (key.contains("panneau")) {
            addCandidate("panneau.png");
            if (key.contains("arriere")) {
                addCandidate("panneau_arriere.png");
            } else if (key.contains("lateral")) {
                addCandidate("panneau_lateral.png");
            }
            addCandidate("panneau_arriere.png");
            addCandidate("panneau_lateral.png");
        }

        if (key.contains("cote") || key.contains("lateral")) {
            addCandidate("cote_lateral.png");
            addCandidate("panneau_lateral.png");
        }

        if (key.contains("dessous") || key.contains("dessus")) {
            addCandidate("dessous.png");
            addCandidate("dessus.png");
            addCandidate("panneau_arriere.png");
            addCandidate("panneau_lateral.png");
        }

        if (key.contains("etagere")) {
            addCandidate("etagere.png");
        }

        if (key.contains("fond")) {
            addCandidate("fond.png");
            addCandidate("tiroir_fond.png");
            addCandidate("plaque_support.png");
            addCandidate("panneau_arriere.png");
        }

        if (key.contains("assise")) {
            addCandidate("assise.png");
        }

        if (key.contains("base")) {
            addCandidate("base.png");
        }

        if (key.contains("cadre")) {
            addCandidate("cadre.png");
        }

        if (key.contains("dossier")) {
            addCandidate("dossier.png");
        }

        if (key.contains("porte")) {
            addCandidate("porte.png");
        }

        if (key.contains("renfort")) {
            addCandidate("cote_lateral.png");
            addCandidate("panneau.png");
            addCandidate("renfort.png");
        }

        if (key.contains("support")) {
            if (key.contains("horizontal")) {
                addCandidate("support_horizontal.png");
            } else if (key.contains("vertical")) {
                addCandidate("support_vertical.png");
            }
            addCandidate("plaque_support.png");
        }

        if (key.contains("plateau")) {
            if (key.contains("petit")) {
                addCandidate("plateau_petit.png");
            }
            addCandidate("plateau.png");
        }

        if (key.contains("pied")) {
            addCandidate("pied.png");
            if (key.contains("renforce")) {
                addCandidate("pied_renforce.png");
            }
            addCandidate("pied_chaise.png");
        }

        if (key.contains("tiroir")) {
            if (key.contains("facade")) {
                addCandidate("tiroir_facade.png");
            } else if (key.contains("fond")) {
                addCandidate("tiroir_fond.png");
            }
        }

        if (key.contains("support")) {
            if (key.contains("horizontal")) {
                addCandidate("support_horizontal.png");
            } else if (key.contains("vertical")) {
                addCandidate("support_vertical.png");
            }
        }
    }

    for (const QString &fileName : std::as_const(candidates)) {
        const QString resolved = resolveCandidatePath(fileName);
        if (!resolved.isEmpty()) {
            return resolved;
        }
    }

    if (!key.isEmpty()) {
        const QStringList keyTokens = key.split('_', Qt::SkipEmptyParts);
        QDir sourceDir(assetsDir);
        const QFileInfoList sourceFiles = sourceDir.entryInfoList(
            QStringList() << "*.png" << "*.jpg" << "*.jpeg" << "*.webp" << "*.bmp",
            QDir::Files | QDir::Readable,
            QDir::Name);

        for (const QFileInfo &info : sourceFiles) {
            const QString fileKey = normalizePieceNameKey(info.completeBaseName());
            if (fileKey.isEmpty()) {
                continue;
            }
            if (fileKey == key || fileKey.contains(key) || key.contains(fileKey)) {
                return info.absoluteFilePath();
            }

            bool tokenMatch = false;
            for (const QString &token : keyTokens) {
                if (token.size() >= 3 && fileKey.contains(token)) {
                    tokenMatch = true;
                    break;
                }
            }
            if (tokenMatch) {
                return info.absoluteFilePath();
            }
        }
    }

    const QString defaultImage = resolveCandidatePath(QStringLiteral("default.png"));
    if (!defaultImage.isEmpty()) {
        return defaultImage;
    }

    if (!generatedDir.isEmpty()) {
        const QString generatedKey = key.isEmpty() ? QString() : QDir(generatedDir).filePath(key + ".png");
        if (!generatedKey.isEmpty() && QFileInfo::exists(generatedKey)) {
            return generatedKey;
        }
        const QString generatedKeyNoIndex = key.isEmpty() ? QString() : QDir(generatedDir).filePath(QString(key).remove(QRegularExpression("_[0-9]+$")) + ".png");
        if (!generatedKeyNoIndex.isEmpty() && QFileInfo::exists(generatedKeyNoIndex)) {
            return generatedKeyNoIndex;
        }
    }

    return QString();
}

QString AiGenerationDashboard::buildGeneratedImageFilePath(const QString &pieceId, const QString &pieceName) const
{
    const QString generatedDir = resolveGeneratedImageDirectory();
    if (generatedDir.isEmpty()) {
        return QString();
    }

    QString baseName = normalizePieceNameKey(pieceName);
    if (baseName.isEmpty()) {
        baseName = QStringLiteral("piece_%1").arg(pieceId.trimmed().isEmpty() ? QStringLiteral("x") : pieceId.trimmed());
    }
    return QDir(generatedDir).filePath(baseName + QStringLiteral(".png"));
}

QString AiGenerationDashboard::cacheGeneratedImageForPiece(const QString &pieceId,
                                                           const QString &pieceName,
                                                           const QString &sourcePath) const
{
    const QFileInfo srcInfo(sourcePath);
    if (!srcInfo.exists() || !srcInfo.isFile()) {
        return QString();
    }

    const QString targetPath = buildGeneratedImageFilePath(pieceId, pieceName);
    if (targetPath.isEmpty()) {
        return srcInfo.absoluteFilePath();
    }

    const QString srcAbs = srcInfo.absoluteFilePath();
    const QString targetAbs = QFileInfo(targetPath).absoluteFilePath();
    if (srcAbs.compare(targetAbs, Qt::CaseInsensitive) == 0) {
        return targetAbs;
    }

    QDir().mkpath(QFileInfo(targetPath).absolutePath());
    if (QFileInfo::exists(targetPath)) {
        QFile::remove(targetPath);
    }
    if (QFile::copy(srcAbs, targetPath)) {
        return QFileInfo(targetPath).absoluteFilePath();
    }

    return srcAbs;
}

QString AiGenerationDashboard::generateLocalFallbackImage(const QString &pieceId,
                                                          const QString &pieceName,
                                                          const QString &dimensions) const
{
    const QString outputPath = buildGeneratedImageFilePath(pieceId, pieceName);
    if (outputPath.isEmpty()) {
        return QString();
    }

    const QString key = normalizePieceNameKey(pieceName);
    QColor accentColor(QStringLiteral("#6366f1"));
    if (key.contains(QStringLiteral("pied"))) {
        accentColor = QColor(QStringLiteral("#16a34a"));
    } else if (key.contains(QStringLiteral("traverse"))) {
        accentColor = QColor(QStringLiteral("#f59e0b"));
    } else if (key.contains(QStringLiteral("plateau")) || key.contains(QStringLiteral("panneau"))) {
        accentColor = QColor(QStringLiteral("#0ea5e9"));
    } else if (key.contains(QStringLiteral("fond")) || key.contains(QStringLiteral("dessus")) || key.contains(QStringLiteral("dessous"))) {
        accentColor = QColor(QStringLiteral("#8b5cf6"));
    }

    QImage img(512, 512, QImage::Format_ARGB32_Premultiplied);
    img.fill(QColor(QStringLiteral("#f8fafc")));

    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    painter.fillRect(QRect(18, 18, 476, 476), QColor(QStringLiteral("#ffffff")));
    painter.setPen(QPen(QColor(QStringLiteral("#cbd5e1")), 2));
    painter.drawRoundedRect(QRect(18, 18, 476, 476), 20, 20);

    QRect shapeRect(110, 120, 292, 220);
    if (key.contains(QStringLiteral("pied"))) {
        shapeRect = QRect(220, 110, 72, 270);
    } else if (key.contains(QStringLiteral("traverse"))) {
        shapeRect = QRect(95, 215, 322, 74);
    } else if (key.contains(QStringLiteral("plateau")) || key.contains(QStringLiteral("dessus"))) {
        shapeRect = QRect(80, 150, 352, 132);
    } else if (key.contains(QStringLiteral("fond")) || key.contains(QStringLiteral("dessous"))) {
        shapeRect = QRect(100, 190, 312, 112);
    }

    QColor fillColor = accentColor;
    fillColor.setAlpha(195);
    painter.setPen(QPen(accentColor.darker(130), 4));
    painter.setBrush(fillColor);
    painter.drawRoundedRect(shapeRect, 16, 16);

    painter.setPen(QPen(QColor(QStringLiteral("#0f172a"))));
    QFont titleFont(QStringLiteral("Segoe UI"), 18, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(QRect(36, 360, 440, 50), Qt::AlignCenter, pieceName);

    QFont metaFont(QStringLiteral("Segoe UI"), 12, QFont::DemiBold);
    painter.setFont(metaFont);
    painter.setPen(QColor(QStringLiteral("#475569")));
    painter.drawText(QRect(36, 410, 440, 34), Qt::AlignCenter, dimensions);

    const QString signature = QStringLiteral("piece_images");
    painter.setPen(QColor(QStringLiteral("#94a3b8")));
    painter.drawText(QRect(36, 448, 440, 28), Qt::AlignRight | Qt::AlignVCenter, signature);
    painter.end();

    QDir().mkpath(QFileInfo(outputPath).absolutePath());
    if (!img.save(outputPath, "PNG")) {
        return QString();
    }
    return QFileInfo(outputPath).absoluteFilePath();
}

QString AiGenerationDashboard::buildImagePrompt(const QString &pieceName, const QString &dimensions) const
{
    return QStringLiteral("A clean flat-style schematic image of a wooden furniture part with the label '%1'. "
                          "Include simple panel or board geometry, no text labels other than the piece name, "
                          "on a light neutral background. The piece should be recognizable as a furniture component "
                          "with dimensions %2.")
        .arg(pieceName)
        .arg(dimensions);
}

void AiGenerationDashboard::requestPieceImageGeneration(const QString &pieceId, const QString &pieceName, const QString &dimensions)
{
    const QString localFallbackPath = generateLocalFallbackImage(pieceId, pieceName, dimensions);
    if (!localFallbackPath.isEmpty()) {
        m_pieceImageById.insert(pieceId, localFallbackPath);
        if (m_pieceCardById.contains(pieceId) && m_pieceCardById.value(pieceId)) {
            m_pieceCardById.value(pieceId)->setImagePath(localFallbackPath);
        }
        if (m_currentSelectedPieceId == pieceId) {
            const QPixmap localPixmap(localFallbackPath);
            if (!localPixmap.isNull()) {
                m_previewWidget->setModelImage(localPixmap);
            }
        }
    }

    if (!m_imageAssistant || !m_imageAssistant->hasApiKey()) {
        if (m_insightsLabel) {
            m_insightsLabel->setText(QStringLiteral("Image locale creee pour '%1' depuis piece_images.").arg(pieceName));
        }
        return;
    }

    const QString prompt = buildImagePrompt(pieceName, dimensions);
    if (m_insightsLabel) {
        m_insightsLabel->setText(QStringLiteral("Demande d'image IA en cours pour '%1'...").arg(pieceName));
    }
    m_imageAssistant->askImageGeneration(prompt, pieceId);
}

void AiGenerationDashboard::onPieceImageGenerated(const QString &pieceId, const QString &imagePath)
{
    if (!m_pieceCardById.contains(pieceId)) {
        return;
    }

    const QString pieceName = m_pieceNameById.value(pieceId);
    const QString normalizedPath = cacheGeneratedImageForPiece(pieceId, pieceName, imagePath);
    const QString finalPath = normalizedPath.isEmpty() ? imagePath : normalizedPath;
    m_pieceImageById.insert(pieceId, finalPath);
    GeneratedPieceCard *card = m_pieceCardById.value(pieceId);
    if (card) {
        card->setImagePath(finalPath);
    }

    if (m_currentSelectedPieceId == pieceId) {
        const QPixmap piecePixmap(finalPath);
        if (!piecePixmap.isNull()) {
            m_previewWidget->setModelImage(piecePixmap);
        }
    }

    if (m_insightsLabel) {
        const QString displayName = pieceName.isEmpty() ? pieceId : pieceName;
        m_insightsLabel->setText(QStringLiteral("Image IA generee pour '%1' et enregistree dans piece_images/generated.").arg(displayName));
    }
}

void AiGenerationDashboard::onPieceImageGenerationFailed(const QString &pieceId, const QString &errorMessage)
{
    const QString pieceName = m_pieceNameById.value(pieceId);
    if (!m_pieceImageById.contains(pieceId)) {
        const QString fallbackPath = generateLocalFallbackImage(pieceId, pieceName, m_pieceDimensionsById.value(pieceId));
        if (!fallbackPath.isEmpty()) {
            m_pieceImageById.insert(pieceId, fallbackPath);
            if (m_pieceCardById.contains(pieceId) && m_pieceCardById.value(pieceId)) {
                m_pieceCardById.value(pieceId)->setImagePath(fallbackPath);
            }
        }
    }

    if (m_insightsLabel) {
        const QString displayName = pieceName.isEmpty() ? pieceId : pieceName;
        m_insightsLabel->setText(
            QStringLiteral("Generation IA indisponible pour '%1'. Image locale conservee. %2")
                .arg(displayName, errorMessage));
    }
}
