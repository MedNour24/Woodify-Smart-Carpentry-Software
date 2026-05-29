#ifndef NESTINGENGINE_H
#define NESTINGENGINE_H

#include <QObject>
#include <QVector>
#include <QRectF>
#include <QPointF>
#include <QColor>
#include <QJsonObject>
#include <QThread>
#include <QRandomGenerator>

// Forward declarations
class PieceRect;
class Panel;

/**
 * @brief Represents a placed piece with position and rotation
 */
struct PlacedPiece {
    PieceRect* piece;
    QRectF bounds;  // Position and size on panel
    bool rotated;   // Whether piece was rotated 90 degrees
    QColor color;   // Visual color for display
    int panelIndex; // Which panel this piece is on

    PlacedPiece() : piece(nullptr), rotated(false), panelIndex(0) {}
    PlacedPiece(PieceRect* p, const QRectF& b, bool r, const QColor& c, int idx)
        : piece(p), bounds(b), rotated(r), color(c), panelIndex(idx) {}
};

/**
 * @brief Represents a panel with placed pieces
 */
struct PanelLayout {
    Panel* panel;
    QVector<PlacedPiece> placedPieces;
    double wasteArea;      // Unused area in mm²
    double wastePercent;   // Waste as percentage
    double efficiency;     // 0-100 efficiency score

    PanelLayout() : panel(nullptr), wasteArea(0), wastePercent(0), efficiency(0) {}
};

/**
 * @brief Optimization mode for nesting algorithm
 */
enum class NestingMode {
    Fast,       // Quick greedy algorithm
    Balanced,   // Balanced optimization
    Advanced,   // Try multiple layouts (AI)
    BestFit,    // Best-fit algorithm
    Random,     // Random placement
    Genetic     // Genetic algorithm
};

/**
 * @brief AI Smart Nesting Optimizer Engine
 *
 * Implements intelligent rectangle packing algorithms to minimize waste
 * when arranging wooden pieces on panels for CNC cutting.
 */
class NestingEngine : public QObject
{
    Q_OBJECT

public:
    explicit NestingEngine(QObject *parent = nullptr);
    ~NestingEngine();

    // Core optimization methods
    QVector<PanelLayout> optimizeLayout(const QVector<PieceRect*>& pieces,
                                       const QVector<Panel*>& availablePanels,
                                       NestingMode mode = NestingMode::Balanced,
                                       double cutTolerance = 3.0);

    // AI analysis methods
    QStringList generateOptimizationSuggestions(const QVector<PanelLayout>& layouts);
    double calculateAIScore(const QVector<PanelLayout>& layouts);

    // Utility methods
    static double calculateWasteArea(const QRectF& panelRect, const QVector<QRectF>& usedRects);
    static double calculateLayoutEfficiency(const PanelLayout& layout);
    static QColor generatePieceColor(int pieceIndex);

    // Simulation methods
    void setSimulationMode(bool enabled) { simulationMode = enabled; }
    bool isSimulationMode() const { return simulationMode; }

    // Learning System
    void storeLayoutResult(const QVector<PanelLayout>& layout);
    QVector<PanelLayout> loadBestLayouts(const QVector<PieceRect*>& pieces);
    bool hasSimilarLayout(const QVector<PieceRect*>& pieces);

    // Waste Reuse System
    QVector<QRectF> trackWasteSpaces(const QRectF& panelRect,
                                    const QVector<QRectF>& usedRects);
    QVector<PlacedPiece> reuseWasteSpaces(const QVector<QRectF>& wasteSpaces,
                                         const QVector<PieceRect*>& remainingPieces,
                                         double tolerance);

signals:
    void simulationStep(const PlacedPiece& piece, int step, int totalSteps);
    void optimizationProgress(int percent);
    void layoutCompleted(const QVector<PanelLayout>& layouts);

private:
    QVector<PanelLayout> optimizeForMode(const QVector<PieceRect*>& pieces,
                                         const QVector<Panel*>& availablePanels,
                                         NestingMode mode,
                                         double cutTolerance);

    QVector<PlacedPiece> runStrategyForPanel(const QVector<PieceRect*>& pieces,
                                             const QRectF& panelRect,
                                             NestingMode mode,
                                             double tolerance);

    // Internal algorithms
    QVector<PlacedPiece> greedyNesting(const QVector<PieceRect*>& pieces,
                                      const QRectF& panelRect,
                                      double tolerance);

    QVector<PlacedPiece> advancedNesting(const QVector<PieceRect*>& pieces,
                                        const QRectF& panelRect,
                                        double tolerance);

    QVector<PlacedPiece> bestFitNesting(const QVector<PieceRect*>& pieces,
                                       const QRectF& panelRect,
                                       double tolerance);

    QVector<PlacedPiece> randomNesting(const QVector<PieceRect*>& pieces,
                                      const QRectF& panelRect,
                                      double tolerance);

    QVector<PlacedPiece> geneticNesting(const QVector<PieceRect*>& pieces,
                                       const QRectF& panelRect,
                                       double tolerance);

    // AI Decision System
    QVector<PanelLayout> runAllStrategies(const QVector<PieceRect*>& pieces,
                                         const QVector<Panel*>& availablePanels,
                                         double tolerance);
    QVector<PanelLayout> selectBestLayout(const QVector<QVector<PanelLayout>>& allLayouts);
    QString buildPieceSignature(const QVector<PieceRect*>& pieces) const;
    double scoreLayoutSet(const QVector<PanelLayout>& layouts) const;

    // Helper methods
    bool canPlacePiece(const QRectF& pieceRect, const QVector<QRectF>& existingRects,
                       const QRectF& panelRect, double tolerance);
    QRectF findBestPosition(const QRectF& pieceRect, const QVector<QRectF>& existingRects,
                           const QRectF& panelRect, double tolerance);
    QVector<QRectF> generateFreeRectangles(const QRectF& panelRect,
                                          const QVector<QRectF>& usedRects,
                                          double tolerance);

    // AI analysis
    QStringList analyzeLayoutEfficiency(const PanelLayout& layout);
    QStringList suggestPieceModifications(const QVector<PieceRect*>& pieces,
                                         const QVector<PanelLayout>& layouts);

    // Simulation state
    bool simulationMode;
    int currentSimulationStep;

    // Learning data
    QVector<QVector<PanelLayout>> learnedLayouts;
    QMap<QString, QVector<PanelLayout>> layoutCache;
};

#endif // NESTINGENGINE_H
