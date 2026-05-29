#ifndef AIGENERATIONDASHBOARD_H
#define AIGENERATIONDASHBOARD_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QHash>
#include <QTimer>

#include "modelpreviewwidget.h"
#include "generatedpiececard.h"
#include "aiassistantservice.h"
#include "piece.h"

class AiGenerationDashboard : public QWidget
{
    Q_OBJECT
public:
    explicit AiGenerationDashboard(QWidget *parent = nullptr);
    void injectDrafts(const QVector<PieceDraft> &drafts, const QString &modelName);

signals:
    void requestGeneration();

private slots:
    void generatePieces();
    void onPieceCardClicked(const QString &pieceId);
    void updateGenerationProgress();

private:
    ModelPreviewWidget *m_previewWidget;
    QWidget *m_cardsContainer;
    QVBoxLayout *m_cardsLayout;
    QScrollArea *m_scrollArea;
    QPushButton *m_generateBtn;
    QLabel *m_titleLabel;
    
    // AI Insights Panel Widgets
    QLabel *m_scoreLabel;
    QLabel *m_insightsLabel;
    QProgressBar *m_progressBar;
    
    void setupUi();
    void setupInsightsPanel(QVBoxLayout *parentLayout);
    void addCard(const QString &id, const QString &type, const QString &name, const QString &dim, int qty, double score, const QString &imagePath);
    QString resolvePiecesAssetDirectory() const;
    QString resolveGeneratedImageDirectory() const;
    QString normalizePieceNameKey(const QString &pieceName) const;
    QString resolvePieceImagePath(const QString &pieceName) const;
    QString buildGeneratedImageFilePath(const QString &pieceId, const QString &pieceName) const;
    QString cacheGeneratedImageForPiece(const QString &pieceId, const QString &pieceName, const QString &sourcePath) const;
    QString generateLocalFallbackImage(const QString &pieceId, const QString &pieceName, const QString &dimensions) const;
    QString buildImagePrompt(const QString &pieceName, const QString &dimensions) const;
    void requestPieceImageGeneration(const QString &pieceId, const QString &pieceName, const QString &dimensions);
    void onPieceImageGenerated(const QString &pieceId, const QString &imagePath);
    void onPieceImageGenerationFailed(const QString &pieceId, const QString &errorMessage);

    // Generation State
    int m_generationStep;
    QTimer *m_timer;
    QVector<PieceDraft> m_pendingDrafts;
    QHash<QString, QString> m_pieceImageById;
    QHash<QString, GeneratedPieceCard*> m_pieceCardById;
    QHash<QString, QString> m_pieceNameById;
    QHash<QString, QString> m_pieceDimensionsById;
    QString m_currentSelectedPieceId;
    AIAssistantService *m_imageAssistant;
};

#endif // AIGENERATIONDASHBOARD_H
