#ifndef NESTINGWIDGET_H
#define NESTINGWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QProgressBar>
#include <QTextEdit>
#include <QGroupBox>
#include <QTimer>
#include <QSplitter>
#include <QCheckBox>
#include <QFormLayout>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QCoreApplication>

#include "nestingengine.h"
#include "piecerect.h"
#include "panel.h"

/**
 * @brief Custom graphics item representing a placed piece
 */
class PieceGraphicsItem : public QGraphicsRectItem
{
public:
    PieceGraphicsItem(const PlacedPiece& placedPiece, QGraphicsItem* parent = nullptr);

    void setPieceData(const PlacedPiece& piece);
    const PlacedPiece& pieceData() const { return m_pieceData; }

    static QColor generateVibrantColor(int pieceId);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    PlacedPiece m_pieceData;
    QGraphicsTextItem* m_textItem;
    bool m_dragging;
    QPointF m_dragStartPos;
};

/**
 * @brief Custom graphics item representing a panel
 */
class PanelGraphicsItem : public QGraphicsRectItem
{
public:
    PanelGraphicsItem(const Panel* panel, int panelIndex, QGraphicsItem* parent = nullptr);

    void updateWasteVisualization(double wastePercent);
    int panelIndex() const { return m_panelIndex; }

private:
    int m_panelIndex;
    QGraphicsRectItem* m_wasteOverlay;
    QGraphicsTextItem* m_panelLabel;
};

/**
 * @brief Main widget for AI Smart Nesting Optimizer
 *
 * Provides interactive visualization and control for nesting optimization
 */
class NestingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NestingWidget(QWidget* parent = nullptr);
    ~NestingWidget();

    // Data loading
    void loadPieces(const QVector<PieceRect*>& pieces);
    void loadPanels(const QVector<Panel*>& panels);

    // Control methods
    void startOptimization();
    void stopOptimization();
    void clearLayout();
    void enableRealTimeOptimization(bool enabled);
    bool hasLearnedLayout(const QVector<PieceRect*>& pieces) const;
    QVector<PanelLayout> loadBestLayouts(const QVector<PieceRect*>& pieces) const;
    void storeLayoutResult(const QVector<PanelLayout>& layouts);
    QVector<QRectF> trackWasteSpaces(const QRectF& panelRect, const QVector<QRectF>& usedRects) const;
    void applyLayout(const QVector<PanelLayout>& layouts);

    // AI Features
    void showExplainableAI(const QStringList& decisions);
    void updateHeatmap();

    // Export methods
    void exportToPDF(const QString& filePath);
    void exportToPNG(const QString& filePath);

    // Animation and effects
    void animateKPICards();

signals:
    void optimizationCompleted(const QVector<PanelLayout>& layouts);
    void pieceMoved(const PlacedPiece& piece, const QPointF& newPos);
    void layoutModified();

public slots:
    void onOptimizationProgress(int percent);
    void onSimulationStep(const PlacedPiece& piece, int step, int totalSteps);
    void onLayoutCompleted(const QVector<PanelLayout>& layouts);

private slots:
    void onOptimizeButtonClicked();
    void onModeChanged(int index);
    void onToleranceChanged(int value);
    void onSimulationToggled(bool enabled);
    void onZoomIn();
    void onZoomOut();
    void onFitToView();
    void onExportPDF();
    void onExportPNG();
    void onComparisonToggled(bool enabled);

private:
    // UI setup
    void setupUI();
    void setupGraphicsView();
    void setupControls();
    void setupAnalytics();
    void setupSuggestions();
    void setupConnections();

    // New UI creation methods
    void createHeader();
    QWidget* createLeftControlPanel();
    QWidget* createCentralVisualization();
    QWidget* createRightAnalyticsPanel();
    QFrame* createKPICard(const QString& title, const QString& value, const QString& status = "");
    QWidget* createAIInsightsPanel();

    // UI creation helpers
    QGroupBox* createControlGroup();
    QGroupBox* createAnalyticsGroup();
    QGroupBox* createSuggestionsGroup();

    // Visualization
    void updateVisualization(const QVector<PanelLayout>& layouts);
    void clearVisualization();
    void addPanelToScene(const PanelLayout& layout, int panelIndex, double xOffset = 0, double yOffset = 0);
    void addPieceToScene(const PlacedPiece& piece);

    // Analytics
    void updateAnalytics(const QVector<PanelLayout>& layouts);
    void updateKPICard(const QString& title, const QString& value, const QString& status);
    void updateSuggestions(const QStringList& suggestions);

    // Comparison mode
    QVector<PanelLayout> generateSimpleLayout();
    void showComparisonView();

    // Utility
    QString formatEfficiencyScore(double score) const;
    QString formatWasteInfo(double wasteMm2, double wastePercent) const;

    // Member variables
    NestingEngine* m_engine;

    // UI components
    QGraphicsView* m_graphicsView;
    QGraphicsScene* m_scene;

    // New dashboard components
    QWidget* m_headerWidget;
    QWidget* m_leftPanel;
    QWidget* m_centerPanel;
    QWidget* m_rightPanel;

    // Header components
    QLabel* m_titleLabel;
    QLabel* m_statusLabel;

    // Control panel
    QPushButton* m_optimizeButton;
    QComboBox* m_modeCombo;
    QSpinBox* m_toleranceSpin;
    QCheckBox* m_simulationCheck;
    QCheckBox* m_realTimeCheck;
    QProgressBar* m_progressBar;

    // Analytics panel
    QLabel* m_piecesCountLabel;
    QLabel* m_panelsCountLabel;
    QLabel* m_wasteLabel;
    QLabel* m_efficiencyLabel;
    QLabel* m_aiScoreLabel;

    // Suggestions panel
    QTextEdit* m_suggestionsText;
    QTextEdit* m_explainableAIText;
    QGraphicsPixmapItem* m_heatmapItem;

    // Zoom controls
    QPushButton* m_zoomInButton;
    QPushButton* m_zoomOutButton;
    QPushButton* m_fitButton;

    // Export controls
    QPushButton* m_exportPDFButton;
    QPushButton* m_exportPNGButton;

    // Comparison mode
    QPushButton* m_comparisonButton;
    QVector<PanelLayout> m_beforeLayouts; // Store unoptimized layout

    // Data
    QVector<PieceRect*> m_pieces;
    QVector<Panel*> m_panels;
    QVector<PanelLayout> m_currentLayouts;

    // State
    bool m_isOptimizing;
    bool m_comparisonMode;
    bool m_realTimeEnabled;
    double m_currentZoom;
};

#endif // NESTINGWIDGET_H