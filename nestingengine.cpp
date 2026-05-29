#include "nestingengine.h"
#include "piecerect.h"
#include "panel.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QStringList>
#include <QtMath>
#include <QElapsedTimer>
#include <algorithm>
#include <limits>

namespace {

double rectArea(const QRectF &rect)
{
    return qMax(0.0, rect.width()) * qMax(0.0, rect.height());
}

double overlapLength(double a1, double a2, double b1, double b2)
{
    return qMax(0.0, qMin(a2, b2) - qMax(a1, b1));
}

double contactScore(const QRectF &candidate,
                    const QVector<QRectF> &usedRects,
                    const QRectF &panelRect,
                    double tolerance)
{
    const double snap = qMax(0.5, tolerance + 0.5);
    double score = 0.0;

    for (const QRectF &used : usedRects) {
        if (qAbs(candidate.left() - used.right()) <= snap ||
            qAbs(candidate.right() - used.left()) <= snap) {
            score += overlapLength(candidate.top(), candidate.bottom(), used.top(), used.bottom());
        }

        if (qAbs(candidate.top() - used.bottom()) <= snap ||
            qAbs(candidate.bottom() - used.top()) <= snap) {
            score += overlapLength(candidate.left(), candidate.right(), used.left(), used.right());
        }
    }

    if (qAbs(candidate.left() - panelRect.left()) <= snap ||
        qAbs(candidate.right() - panelRect.right()) <= snap) {
        score += candidate.height();
    }
    if (qAbs(candidate.top() - panelRect.top()) <= snap ||
        qAbs(candidate.bottom() - panelRect.bottom()) <= snap) {
        score += candidate.width();
    }

    return score;
}

void appendUniqueCandidate(QVector<double> &values, double candidate, double minValue, double maxValue)
{
    if (candidate < minValue - 0.5 || candidate > maxValue + 0.5) {
        return;
    }
    for (double value : values) {
        if (qAbs(value - candidate) < 0.25) {
            return;
        }
    }
    values.append(candidate);
}

QVector<PieceRect*> sortPiecesForPacking(const QVector<PieceRect*> &pieces)
{
    QVector<PieceRect*> sorted = pieces;
    std::sort(sorted.begin(), sorted.end(), [](const PieceRect *a, const PieceRect *b) {
        if (a == nullptr || b == nullptr) {
            return a != nullptr;
        }
        if (a->priority() != b->priority()) {
            return a->priority() > b->priority();
        }

        const double longA = qMax(a->length(), a->width());
        const double longB = qMax(b->length(), b->width());
        if (!qFuzzyCompare(longA + 1.0, longB + 1.0)) {
            return longA > longB;
        }

        const double areaA = a->area();
        const double areaB = b->area();
        if (!qFuzzyCompare(areaA + 1.0, areaB + 1.0)) {
            return areaA > areaB;
        }
        return a->id() < b->id();
    });
    return sorted;
}

QString pieceToken(const PieceRect *piece)
{
    if (!piece) {
        return QString();
    }

    const double longSide = qMax(piece->length(), piece->width());
    const double shortSide = qMin(piece->length(), piece->width());
    return QString("%1x%2:r%3:p%4:m%5")
        .arg(QString::number(longSide, 'f', 2))
        .arg(QString::number(shortSide, 'f', 2))
        .arg(piece->canRotate() ? "1" : "0")
        .arg(piece->priority())
        .arg(piece->material().trimmed().toUpper());
}

} // namespace

// Constructor
NestingEngine::NestingEngine(QObject *parent)
    : QObject(parent), simulationMode(false), currentSimulationStep(0)
{
}

// Destructor
NestingEngine::~NestingEngine()
{
}

// Main optimization method
QVector<PanelLayout> NestingEngine::optimizeLayout(const QVector<PieceRect*>& pieces,
                                                  const QVector<Panel*>& availablePanels,
                                                  NestingMode mode,
                                                  double cutTolerance)
{
    currentSimulationStep = 0;
    QVector<PanelLayout> allLayouts = (mode == NestingMode::Advanced)
        ? runAllStrategies(pieces, availablePanels, cutTolerance)
        : optimizeForMode(pieces, availablePanels, mode, cutTolerance);
    emit layoutCompleted(allLayouts);
    return allLayouts;
}

QVector<PanelLayout> NestingEngine::optimizeForMode(const QVector<PieceRect*>& pieces,
                                                    const QVector<Panel*>& availablePanels,
                                                    NestingMode mode,
                                                    double cutTolerance)
{
    QVector<PanelLayout> allLayouts;
    if (pieces.isEmpty() || availablePanels.isEmpty()) {
        return allLayouts;
    }

    QVector<PieceRect*> remainingPieces = sortPiecesForPacking(pieces);
    const double tolerance = qMax(0.0, cutTolerance);
    const bool simulationWasEnabled = simulationMode;
    const int initialPieceCount = remainingPieces.size();
    int placedTotalCount = 0;

    int loopGuard = 0;
    while (!remainingPieces.isEmpty() && loopGuard++ < 500) {
        Panel* bestPanel = nullptr;
        QVector<PlacedPiece> bestPlaced;
        double bestPlacedArea = -1.0;
        double bestPanelArea = std::numeric_limits<double>::max();

        simulationMode = false; // do silent scoring for panel selection
        for (Panel* candidatePanel : availablePanels) {
            if (!candidatePanel || !candidatePanel->isValid()) {
                continue;
            }

            const QRectF panelRect = candidatePanel->toRect();
            QVector<PlacedPiece> candidatePlaced = runStrategyForPanel(remainingPieces, panelRect, mode, tolerance);
            if (candidatePlaced.isEmpty()) {
                continue;
            }

            double candidatePlacedArea = 0.0;
            for (const PlacedPiece& piece : candidatePlaced) {
                candidatePlacedArea += rectArea(piece.bounds);
            }

            const bool better = candidatePlaced.size() > bestPlaced.size()
                || (candidatePlaced.size() == bestPlaced.size() && candidatePlacedArea > bestPlacedArea + 0.01)
                || (candidatePlaced.size() == bestPlaced.size()
                    && qAbs(candidatePlacedArea - bestPlacedArea) <= 0.01
                    && panelRect.width() * panelRect.height() < bestPanelArea);

            if (better) {
                bestPanel = candidatePanel;
                bestPlaced = candidatePlaced;
                bestPlacedArea = candidatePlacedArea;
                bestPanelArea = panelRect.width() * panelRect.height();
            }
        }
        simulationMode = simulationWasEnabled;

        if (!bestPanel || bestPlaced.isEmpty()) {
            qDebug() << "Nesting stopped:" << remainingPieces.size()
                     << "piece(s) could not be placed with mode" << static_cast<int>(mode);
            break;
        }

        // Re-run only the selected panel with simulation feedback enabled.
        if (simulationWasEnabled) {
            bestPlaced = runStrategyForPanel(remainingPieces, bestPanel->toRect(), mode, tolerance);
            if (bestPlaced.isEmpty()) {
                qDebug() << "Simulation rerun failed on selected panel.";
                break;
            }
        }

        const int panelIndex = allLayouts.size();
        PanelLayout layout;
        layout.panel = bestPanel;
        layout.placedPieces = bestPlaced;

        QVector<QRectF> usedRects;
        usedRects.reserve(layout.placedPieces.size());

        for (PlacedPiece &placed : layout.placedPieces) {
            placed.panelIndex = panelIndex;
            usedRects.append(placed.bounds);
            const int idx = remainingPieces.indexOf(placed.piece);
            if (idx >= 0) {
                remainingPieces.removeAt(idx);
            }
        }
        placedTotalCount += layout.placedPieces.size();

        const QRectF panelRect = bestPanel->toRect();
        const double panelArea = qMax(1.0, rectArea(panelRect));
        layout.wasteArea = qMax(0.0, calculateWasteArea(panelRect, usedRects));
        layout.wastePercent = qBound(0.0, (layout.wasteArea / panelArea) * 100.0, 100.0);
        layout.efficiency = calculateLayoutEfficiency(layout);

        allLayouts.append(layout);
        emit optimizationProgress(static_cast<int>((placedTotalCount * 100.0) / qMax(1, initialPieceCount)));
    }

    return allLayouts;
}

QVector<PlacedPiece> NestingEngine::runStrategyForPanel(const QVector<PieceRect*>& pieces,
                                                        const QRectF& panelRect,
                                                        NestingMode mode,
                                                        double tolerance)
{
    switch (mode) {
    case NestingMode::Fast:
        return greedyNesting(pieces, panelRect, tolerance);
    case NestingMode::Balanced:
        return advancedNesting(pieces, panelRect, tolerance);
    case NestingMode::BestFit:
        return bestFitNesting(pieces, panelRect, tolerance);
    case NestingMode::Random:
        return randomNesting(pieces, panelRect, tolerance);
    case NestingMode::Genetic:
        return geneticNesting(pieces, panelRect, tolerance);
    case NestingMode::Advanced:
    default:
        return advancedNesting(pieces, panelRect, tolerance);
    }
}

// Greedy nesting algorithm (fast)
QVector<PlacedPiece> NestingEngine::greedyNesting(const QVector<PieceRect*>& pieces,
                                                 const QRectF& panelRect,
                                                 double tolerance)
{
    QVector<PlacedPiece> placedPieces;
    QVector<QRectF> usedRects;

    for (int i = 0; i < pieces.size(); ++i) {
        PieceRect* piece = pieces[i];
        if (!piece->isValid()) continue;

        // Try both orientations
        QVector<QRectF> possibleRects = {piece->toRect()};
        if (piece->canRotate()) {
            possibleRects.append(piece->rotatedRect());
        }
        QRectF bestPosition;
        bool bestRotated = false;
        double bestScore = std::numeric_limits<double>::max();

        for (int rot = 0; rot < possibleRects.size(); ++rot) {
            const QRectF pieceRect = possibleRects[rot];
            QRectF position = findBestPosition(pieceRect, usedRects, panelRect, tolerance);

            if (!position.isNull()) {
                QRectF footprint = position;
                for (const QRectF& existing : usedRects) {
                    footprint = footprint.united(existing);
                }
                const double score = rectArea(footprint) - (contactScore(position, usedRects, panelRect, tolerance) * 4.0);
                if (score < bestScore) {
                    bestScore = score;
                    bestPosition = position;
                    bestRotated = (rot == 1 && piece->canRotate());
                }
            }
        }

        if (!bestPosition.isNull()) {
            PlacedPiece placed(piece, bestPosition, bestRotated,
                             generatePieceColor(i), 0);
            placedPieces.append(placed);
            usedRects.append(bestPosition);

            if (simulationMode) {
                emit simulationStep(placed, currentSimulationStep++, pieces.size());
                QThread::msleep(100); // Simulate processing time
            }
        }
    }

    return placedPieces;
}

// Advanced nesting algorithm (balanced)
QVector<PlacedPiece> NestingEngine::advancedNesting(const QVector<PieceRect*>& pieces,
                                                   const QRectF& panelRect,
                                                   double tolerance)
{
    // Use bottom-left fill algorithm with look-ahead
    QVector<PlacedPiece> placedPieces;
    QVector<QRectF> usedRects;

    // Generate free rectangles
    QVector<QRectF> freeRects = {panelRect};

    for (int i = 0; i < pieces.size(); ++i) {
        PieceRect* piece = pieces[i];
        if (!piece->isValid()) continue;

        QRectF bestPosition;
        bool bestRotated = false;
        double bestEfficiency = -1;

        // Try both orientations
        QVector<QRectF> possibleRects = {piece->toRect(), piece->rotatedRect()};

        for (int rot = 0; rot < 2; ++rot) {
            if (!piece->canRotate() && rot > 0) break;

            QRectF pieceRect = possibleRects[rot];

            // Try to place in each free rectangle
            for (const QRectF& freeRect : freeRects) {
                if (pieceRect.width() <= freeRect.width() &&
                    pieceRect.height() <= freeRect.height()) {

                    // Try different positions within free rectangle
                    QVector<QPointF> positions = {
                        freeRect.topLeft(),
                        QPointF(freeRect.left(), freeRect.bottom() - pieceRect.height()),
                        QPointF(freeRect.right() - pieceRect.width(), freeRect.top()),
                        QPointF(freeRect.right() - pieceRect.width(),
                               freeRect.bottom() - pieceRect.height())
                    };

                    for (const QPointF& pos : positions) {
                        QRectF testRect(pos, pieceRect.size());
                        if (canPlacePiece(testRect, usedRects, panelRect, tolerance)) {
                            // Calculate efficiency score
                            double efficiency = (pieceRect.width() * pieceRect.height()) /
                                              (freeRect.width() * freeRect.height());
                            if (efficiency > bestEfficiency) {
                                bestEfficiency = efficiency;
                                bestPosition = testRect;
                                bestRotated = (rot == 1);
                            }
                        }
                    }
                }
            }
        }

        if (!bestPosition.isNull()) {
            PlacedPiece placed(piece, bestPosition, bestRotated,
                             generatePieceColor(i), 0);
            placedPieces.append(placed);
            usedRects.append(bestPosition);

            // Update free rectangles
            freeRects = generateFreeRectangles(panelRect, usedRects, tolerance);

            if (simulationMode) {
                emit simulationStep(placed, currentSimulationStep++, pieces.size());
                QThread::msleep(200); // Simulate processing time
            }
        }
    }

    return placedPieces;
}

// Check if piece can be placed at position
bool NestingEngine::canPlacePiece(const QRectF& pieceRect, const QVector<QRectF>& existingRects,
                                 const QRectF& panelRect, double tolerance)
{
    // Check panel bounds with slight math tolerance to avoid floating edge rejection
    if (!panelRect.adjusted(-0.1, -0.1, 0.1, 0.1).contains(pieceRect)) {
        return false;
    }

    // Expansion around piece to maintain kerf tolerance
    const double halfKerf = qMax(0.0, tolerance / 2.0);

    for (const QRectF& existing : existingRects) {
        QRectF expandedPiece = pieceRect.adjusted(-halfKerf, -halfKerf, halfKerf, halfKerf);
        QRectF expandedExisting = existing.adjusted(-halfKerf, -halfKerf, halfKerf, halfKerf);

        if (expandedPiece.adjusted(0.1, 0.1, -0.1, -0.1).intersects(expandedExisting.adjusted(0.1, 0.1, -0.1, -0.1))) {
            return false;
        }
    }

    return true;
}

// Find best position for piece (greedy)
QRectF NestingEngine::findBestPosition(const QRectF& pieceRect, const QVector<QRectF>& existingRects,
                                     const QRectF& panelRect, double tolerance)
{
    QVector<double> xCandidates;
    QVector<double> yCandidates;

    appendUniqueCandidate(xCandidates, panelRect.left(), panelRect.left(), panelRect.right());
    appendUniqueCandidate(yCandidates, panelRect.top(), panelRect.top(), panelRect.bottom());

    const double kerf = qMax(0.0, tolerance);
    for (const QRectF &existing : existingRects) {
        appendUniqueCandidate(xCandidates, existing.right() + kerf, panelRect.left(), panelRect.right());
        appendUniqueCandidate(xCandidates, existing.left() - pieceRect.width() - kerf, panelRect.left(), panelRect.right());
        appendUniqueCandidate(yCandidates, existing.bottom() + kerf, panelRect.top(), panelRect.bottom());
        appendUniqueCandidate(yCandidates, existing.top() - pieceRect.height() - kerf, panelRect.top(), panelRect.bottom());
    }

    std::sort(xCandidates.begin(), xCandidates.end());
    std::sort(yCandidates.begin(), yCandidates.end());

    QRectF bestRect;
    double bestScore = std::numeric_limits<double>::max();

    for (double y : yCandidates) {
        for (double x : xCandidates) {
            QRectF candidate(x, y, pieceRect.width(), pieceRect.height());
            if (!canPlacePiece(candidate, existingRects, panelRect, tolerance)) {
                continue;
            }

            QRectF footprint = candidate;
            for (const QRectF &existing : existingRects) {
                footprint = footprint.united(existing);
            }

            const double score = rectArea(footprint)
                - contactScore(candidate, existingRects, panelRect, tolerance) * 5.0
                + (candidate.top() * 0.001)
                + (candidate.left() * 0.0001);

            if (score < bestScore) {
                bestScore = score;
                bestRect = candidate;
            }
        }
    }

    return bestRect;
}

// Generate free rectangles after placing pieces (MaxRects approach)
QVector<QRectF> NestingEngine::generateFreeRectangles(const QRectF& panelRect,
                                                     const QVector<QRectF>& usedRects,
                                                     double tolerance)
{
    QVector<QRectF> freeRects;
    freeRects.append(panelRect);

    double halfKerf = tolerance; // Treat tolerance stringently as obstacle boundary
    
    for (const QRectF& used : usedRects) {
        QVector<QRectF> newFreeRects;
        QRectF obstacle = used.adjusted(-halfKerf, -halfKerf, halfKerf, halfKerf);

        for (const QRectF& free : freeRects) {
            if (!free.intersects(obstacle.adjusted(0.1,0.1,-0.1,-0.1))) {
                newFreeRects.append(free);
            } else {
                // Split free rectangle around the obstacle properly (MaxRects strict splits)
                if (obstacle.top() > free.top()) {
                    newFreeRects.append(QRectF(free.left(), free.top(),
                                             free.width(), obstacle.top() - free.top()));
                }
                if (obstacle.bottom() < free.bottom()) {
                    newFreeRects.append(QRectF(free.left(), obstacle.bottom(),
                                             free.width(), free.bottom() - obstacle.bottom()));
                }
                if (obstacle.left() > free.left()) {
                    newFreeRects.append(QRectF(free.left(), free.top(),
                                             obstacle.left() - free.left(), free.height()));
                }
                if (obstacle.right() < free.right()) {
                    newFreeRects.append(QRectF(obstacle.right(), free.top(),
                                             free.right() - obstacle.right(), free.height()));
                }
            }
        }
        
        freeRects = newFreeRects;
    }
    
    // Purge micro-rectangles too small for any piece
    QVector<QRectF> cleanFreeRects;
    for (const QRectF& f : freeRects) {
        if (f.width() >= 10.0 && f.height() >= 10.0) {
            cleanFreeRects.append(f);
        }
    }

    // Remove rectangles fully contained by larger ones to reduce fragmentation noise.
    QVector<QRectF> filtered;
    for (int i = 0; i < cleanFreeRects.size(); ++i) {
        const QRectF current = cleanFreeRects.at(i);
        bool contained = false;
        for (int j = 0; j < cleanFreeRects.size(); ++j) {
            if (i == j) {
                continue;
            }
            const QRectF other = cleanFreeRects.at(j);
            if (other.contains(current.adjusted(0.1, 0.1, -0.1, -0.1))) {
                contained = true;
                break;
            }
        }
        if (!contained) {
            filtered.append(current);
        }
    }

    return filtered;
}

// Calculate waste area
double NestingEngine::calculateWasteArea(const QRectF& panelRect, const QVector<QRectF>& usedRects)
{
    const double panelArea = rectArea(panelRect);
    if (panelArea <= 0.0) {
        return 0.0;
    }

    double usedArea = 0.0;
    for (const QRectF& rect : usedRects) {
        usedArea += rectArea(rect);
    }
    return qMax(0.0, panelArea - usedArea);
}

// Calculate layout efficiency
double NestingEngine::calculateLayoutEfficiency(const PanelLayout& layout)
{
    if (!layout.panel || layout.placedPieces.isEmpty()) {
        return 0.0;
    }

    const double panelArea = qMax(1.0, layout.panel->area());
    double usedArea = 0.0;
    QRectF footprint;

    for (const PlacedPiece& piece : layout.placedPieces) {
        usedArea += rectArea(piece.bounds);
        footprint = footprint.isNull() ? piece.bounds : footprint.united(piece.bounds);
    }

    const double utilization = qBound(0.0, (usedArea / panelArea) * 100.0, 100.0);
    const double footprintArea = qMax(1.0, rectArea(footprint));
    const double compactness = qBound(0.0, (usedArea / footprintArea) * 100.0, 100.0);
    const double cutPenalty = qMin(8.0, qLn(1.0 + static_cast<double>(layout.placedPieces.size())) * 2.0);

    const double efficiency = (utilization * 0.75) + (compactness * 0.25) - cutPenalty;
    return qBound(0.0, efficiency, 100.0);
}

// Generate AI score
double NestingEngine::calculateAIScore(const QVector<PanelLayout>& layouts)
{
    if (layouts.isEmpty()) {
        return 0.0;
    }

    double totalEfficiency = 0.0;
    double weightedWastePercent = 0.0;
    double totalPanelArea = 0.0;
    double varianceAccumulator = 0.0;

    for (const PanelLayout& layout : layouts) {
        totalEfficiency += layout.efficiency;
        const double area = layout.panel ? qMax(0.0, layout.panel->area()) : 0.0;
        totalPanelArea += area;
        weightedWastePercent += layout.wastePercent * area;
    }

    const double avgEfficiency = totalEfficiency / layouts.size();
    const double avgWaste = totalPanelArea > 0.0 ? (weightedWastePercent / totalPanelArea) : 100.0;

    for (const PanelLayout& layout : layouts) {
        const double delta = layout.efficiency - avgEfficiency;
        varianceAccumulator += delta * delta;
    }
    const double stdDev = qSqrt(varianceAccumulator / layouts.size());
    const double stability = qBound(0.0, 100.0 - (stdDev * 1.5), 100.0);
    const double panelPenalty = qMax(0, layouts.size() - 1) * 3.0;

    const double aiScore = (avgEfficiency * 0.65)
        + ((100.0 - avgWaste) * 0.25)
        + (stability * 0.10)
        - panelPenalty;

    return qBound(0.0, aiScore, 100.0);
}

// Generate optimization suggestions
QStringList NestingEngine::generateOptimizationSuggestions(const QVector<PanelLayout>& layouts)
{
    QStringList suggestions;

    if (layouts.isEmpty()) {
        suggestions << "Aucune disposition disponible pour l'analyse.";
        return suggestions;
    }

    int totalPieces = 0;
    double totalWastePercent = 0.0;
    double minEfficiency = std::numeric_limits<double>::max();
    double maxEfficiency = 0.0;

    for (const PanelLayout& layout : layouts) {
        totalPieces += layout.placedPieces.size();
        totalWastePercent += layout.wastePercent;
        minEfficiency = qMin(minEfficiency, layout.efficiency);
        maxEfficiency = qMax(maxEfficiency, layout.efficiency);
    }

    const double avgWastePercent = totalWastePercent / layouts.size();

    if (avgWastePercent > 30.0) {
        suggestions << QString("Dechet moyen eleve (%1%). Essayez un panneau plus adapte ou un autre mode de nesting.")
                          .arg(QString::number(avgWastePercent, 'f', 1));
    } else if (avgWastePercent > 18.0) {
        suggestions << QString("Dechet moyen correct (%1%), mais encore ameliorable avec le mode BestFit ou Advanced.")
                          .arg(QString::number(avgWastePercent, 'f', 1));
    } else {
        suggestions << QString("Excellent taux de remplissage (%1% de dechet moyen).")
                          .arg(QString::number(avgWastePercent, 'f', 1));
    }

    if (layouts.size() > 1) {
        suggestions << QString("L'optimisation utilise %1 panneaux. Essayez de reduire la tolerance pour consolider.")
                          .arg(layouts.size());
    }

    if ((maxEfficiency - minEfficiency) > 18.0) {
        suggestions << "Forte variabilite entre panneaux: regroupez les pieces de dimensions proches.";
    }

    if (totalPieces < 5) {
        suggestions << "Peu de pieces placees: verifiez les dimensions des pieces et la coherence des donnees.";
    }

    suggestions << "Conseil IA: activez la rotation pour les pieces non contraintes.";
    suggestions << "Conseil IA: comparez plusieurs strategies via le mode Advanced.";

    return suggestions;
}
// Generate piece color
QColor NestingEngine::generatePieceColor(int pieceIndex)
{
    // Generate distinct colors for pieces
    int hue = (pieceIndex * 137) % 360; // Use golden angle for good distribution
    return QColor::fromHsv(hue, 200, 220);
}

// Best-fit nesting algorithm
QVector<PlacedPiece> NestingEngine::bestFitNesting(const QVector<PieceRect*>& pieces,
                                                  const QRectF& panelRect,
                                                  double tolerance)
{
    QVector<PlacedPiece> placedPieces;
    QVector<QRectF> usedRects;
    QVector<QRectF> freeRects = {panelRect};

    for (PieceRect* piece : pieces) {
        if (!piece || !piece->isValid()) {
            continue;
        }
        QRectF bestPosition;
        double bestWaste = std::numeric_limits<double>::max();
        bool bestRotated = false;

        QVector<bool> orientations = {false};
        if (piece->canRotate()) {
            orientations.append(true);
        }

        for (bool rotated : orientations) {
            const QRectF pieceRect = rotated ? piece->rotatedRect() : piece->toRect();

            for (const QRectF& freeRect : freeRects) {
                if (pieceRect.width() <= freeRect.width() && pieceRect.height() <= freeRect.height()) {
                    const QVector<QPointF> candidates = {
                        freeRect.topLeft(),
                        QPointF(freeRect.right() - pieceRect.width(), freeRect.top()),
                        QPointF(freeRect.left(), freeRect.bottom() - pieceRect.height()),
                        QPointF(freeRect.right() - pieceRect.width(), freeRect.bottom() - pieceRect.height())
                    };

                    for (const QPointF &candidatePos : candidates) {
                        const QRectF position(candidatePos, pieceRect.size());
                        if (!canPlacePiece(position, usedRects, panelRect, tolerance)) {
                            continue;
                        }

                        const double shortSideResidual = qMin(freeRect.width() - pieceRect.width(),
                                                              freeRect.height() - pieceRect.height());
                        const double longSideResidual = qMax(freeRect.width() - pieceRect.width(),
                                                             freeRect.height() - pieceRect.height());
                        const double score = shortSideResidual * 1000.0
                            + longSideResidual
                            - contactScore(position, usedRects, panelRect, tolerance) * 2.0;
                        if (score < bestWaste) {
                            bestWaste = score;
                            bestPosition = position;
                            bestRotated = rotated;
                        }
                    }
                }
            }
        }

        if (!bestPosition.isEmpty()) {
            PlacedPiece placed(piece, bestPosition, bestRotated,
                             generatePieceColor(placedPieces.size()), 0);
            placedPieces.append(placed);
            usedRects.append(bestPosition);

            // Update free rectangles
            freeRects = generateFreeRectangles(panelRect, usedRects, tolerance);
        }
    }

    return placedPieces;
}

// Random nesting algorithm
QVector<PlacedPiece> NestingEngine::randomNesting(const QVector<PieceRect*>& pieces,
                                                 const QRectF& panelRect,
                                                 double tolerance)
{
    QVector<PlacedPiece> placedPieces;
    QVector<QRectF> usedRects;

    for (PieceRect* piece : pieces) {
        if (!piece || !piece->isValid()) {
            continue;
        }
        bool piecePlaced = false;

        // Try random positions
        for (int attempt = 0; attempt < 50 && !piecePlaced; ++attempt) {
            const bool rotated = piece->canRotate() && (QRandomGenerator::global()->bounded(2) == 1);
            QRectF pieceRect = rotated ? piece->rotatedRect() : piece->toRect();

            const int maxX = qMax(0, static_cast<int>(qFloor(panelRect.width() - pieceRect.width())));
            const int maxY = qMax(0, static_cast<int>(qFloor(panelRect.height() - pieceRect.height())));
            if (pieceRect.width() > panelRect.width() || pieceRect.height() > panelRect.height()) {
                continue;
            }

            const double x = (maxX == 0) ? 0.0 : static_cast<double>(QRandomGenerator::global()->bounded(maxX + 1));
            const double y = (maxY == 0) ? 0.0 : static_cast<double>(QRandomGenerator::global()->bounded(maxY + 1));
            QRectF position(x, y, pieceRect.width(), pieceRect.height());

            if (canPlacePiece(position, usedRects, panelRect, tolerance)) {
                PlacedPiece placed(piece, position, rotated,
                                 generatePieceColor(placedPieces.size()), 0);
                placedPieces.append(placed);
                usedRects.append(position);
                piecePlaced = true;
            }
        }
    }

    return placedPieces;
}

// Genetic nesting algorithm (simplified)
QVector<PlacedPiece> NestingEngine::geneticNesting(const QVector<PieceRect*>& pieces,
                                                  const QRectF& panelRect,
                                                  double tolerance)
{
    // Simplified genetic algorithm - create multiple random layouts and pick best
    QVector<QVector<PlacedPiece>> candidates;

    for (int i = 0; i < 10; ++i) {
        candidates.append(randomNesting(pieces, panelRect, tolerance));
    }

    // Find best candidate
    QVector<PlacedPiece> best;
    double bestEfficiency = 0;

    for (const QVector<PlacedPiece>& candidate : candidates) {
        QVector<QRectF> usedRects;
        for (const PlacedPiece& p : candidate) {
            usedRects.append(p.bounds);
        }
        double waste = calculateWasteArea(panelRect, usedRects);
        double efficiency = 100.0 - (waste / (panelRect.width() * panelRect.height())) * 100.0;

        if (efficiency > bestEfficiency) {
            bestEfficiency = efficiency;
            best = candidate;
        }
    }

    return best;
}

// AI Decision System: Run all strategies and select best
QVector<PanelLayout> NestingEngine::runAllStrategies(const QVector<PieceRect*>& pieces,
                                                    const QVector<Panel*>& availablePanels,
                                                    double tolerance)
{
    QVector<QVector<PanelLayout>> allLayouts;

    // Run each algorithm
    QVector<NestingMode> modes = {NestingMode::Fast, NestingMode::Balanced,
                                  NestingMode::BestFit, NestingMode::Random, NestingMode::Genetic};

    for (NestingMode mode : modes) {
        const int attempts = (mode == NestingMode::Random || mode == NestingMode::Genetic) ? 3 : 1;
        QVector<PanelLayout> bestAttempt;
        double bestAttemptScore = std::numeric_limits<double>::max();

        for (int i = 0; i < attempts; ++i) {
            const QVector<PanelLayout> layouts = optimizeForMode(pieces, availablePanels, mode, tolerance);
            if (layouts.isEmpty()) {
                continue;
            }
            const double score = scoreLayoutSet(layouts);
            if (score < bestAttemptScore) {
                bestAttempt = layouts;
                bestAttemptScore = score;
            }
        }

        if (!bestAttempt.isEmpty()) {
            allLayouts.append(bestAttempt);
        }
    }

    return selectBestLayout(allLayouts);
}

// Select best layout based on waste and panel count
QVector<PanelLayout> NestingEngine::selectBestLayout(const QVector<QVector<PanelLayout>>& allLayouts)
{
    QVector<PanelLayout> bestLayout;
    double bestScore = std::numeric_limits<double>::max();

    for (const QVector<PanelLayout>& layouts : allLayouts) {
        const double score = scoreLayoutSet(layouts);
        if (score < bestScore) {
            bestScore = score;
            bestLayout = layouts;
        }
    }

    return bestLayout;
}

QString NestingEngine::buildPieceSignature(const QVector<PieceRect*>& pieces) const
{
    QStringList tokens;
    tokens.reserve(pieces.size());
    for (const PieceRect* piece : pieces) {
        if (!piece || !piece->isValid()) {
            continue;
        }
        tokens.append(pieceToken(piece));
    }
    std::sort(tokens.begin(), tokens.end());
    return tokens.join('|');
}

double NestingEngine::scoreLayoutSet(const QVector<PanelLayout>& layouts) const
{
    if (layouts.isEmpty()) {
        return std::numeric_limits<double>::max();
    }

    double totalPanelArea = 0.0;
    double totalUsedArea = 0.0;
    double totalWaste = 0.0;
    int totalPieces = 0;

    for (const PanelLayout& layout : layouts) {
        const double panelArea = layout.panel ? qMax(0.0, layout.panel->area()) : 0.0;
        totalPanelArea += panelArea;
        totalWaste += qMax(0.0, layout.wasteArea);
        totalUsedArea += qMax(0.0, panelArea - layout.wasteArea);
        totalPieces += layout.placedPieces.size();
    }

    if (totalPanelArea <= 0.0 || totalPieces <= 0) {
        return std::numeric_limits<double>::max();
    }

    const double utilization = qBound(0.0, totalUsedArea / totalPanelArea, 1.0);
    const double wasteRatio = qBound(0.0, totalWaste / totalPanelArea, 1.0);
    const double panelPenalty = qMax(0, layouts.size() - 1) * 0.10;

    // Lower score is better.
    return (1.0 - utilization) * 1000.0
        + wasteRatio * 500.0
        + panelPenalty * 100.0;
}

// Learning System: Store layout result
void NestingEngine::storeLayoutResult(const QVector<PanelLayout>& layout)
{
    if (layout.isEmpty()) {
        return;
    }

    QVector<PieceRect*> placedPieces;
    for (const PanelLayout& panelLayout : layout) {
        for (const PlacedPiece& placed : panelLayout.placedPieces) {
            if (placed.piece) {
                placedPieces.append(placed.piece);
            }
        }
    }

    const QString signature = buildPieceSignature(placedPieces);
    if (!signature.isEmpty()) {
        if (!layoutCache.contains(signature)
            || scoreLayoutSet(layout) < scoreLayoutSet(layoutCache.value(signature))) {
            layoutCache.insert(signature, layout);
        }
    }

    learnedLayouts.append(layout);

    // Keep only recent results
    if (learnedLayouts.size() > 50) {
        learnedLayouts.removeFirst();
    }
}

// Load best layouts for similar piece sets
QVector<PanelLayout> NestingEngine::loadBestLayouts(const QVector<PieceRect*>& pieces)
{
    const QString signature = buildPieceSignature(pieces);
    if (layoutCache.contains(signature)) {
        return layoutCache.value(signature);
    }

    // Fallback for old in-memory records.
    QVector<PanelLayout> bestMatch;
    double bestMatchScore = std::numeric_limits<double>::max();
    for (const QVector<PanelLayout>& layout : learnedLayouts) {
        QVector<PieceRect*> placedPieces;
        for (const PanelLayout& panel : layout) {
            for (const PlacedPiece& placed : panel.placedPieces) {
                if (placed.piece) {
                    placedPieces.append(placed.piece);
                }
            }
        }

        if (buildPieceSignature(placedPieces) == signature) {
            const double score = scoreLayoutSet(layout);
            if (score < bestMatchScore) {
                bestMatch = layout;
                bestMatchScore = score;
            }
        }
    }

    return bestMatch;
}

// Check if we have similar layout
bool NestingEngine::hasSimilarLayout(const QVector<PieceRect*>& pieces)
{
    return !loadBestLayouts(pieces).isEmpty();
}

// Waste Reuse System: Track leftover spaces
QVector<QRectF> NestingEngine::trackWasteSpaces(const QRectF& panelRect,
                                              const QVector<QRectF>& usedRects)
{
    return generateFreeRectangles(panelRect, usedRects, 0);
}

// Reuse waste spaces for remaining pieces
QVector<PlacedPiece> NestingEngine::reuseWasteSpaces(const QVector<QRectF>& wasteSpaces,
                                                    const QVector<PieceRect*>& remainingPieces,
                                                    double tolerance)
{
    QVector<PlacedPiece> additionalPieces;
    QVector<QRectF> occupiedWasteRects;

    for (const QRectF &wasteSpace : wasteSpaces) {
        for (PieceRect* piece : remainingPieces) {
            if (!piece || !piece->isValid()) {
                continue;
            }
            if (piece->area() > wasteSpace.width() * wasteSpace.height()) {
                continue;
            }

            QVector<QRectF> options = {piece->toRect()};
            if (piece->canRotate()) {
                options.append(piece->rotatedRect());
            }

            bool placed = false;
            for (int optionIndex = 0; optionIndex < options.size() && !placed; ++optionIndex) {
                const QRectF option = options.at(optionIndex);
                if (option.width() <= wasteSpace.width() && option.height() <= wasteSpace.height()) {
                    const QRectF position(wasteSpace.topLeft(), option.size());
                    if (canPlacePiece(position, occupiedWasteRects, wasteSpace, tolerance)) {
                        additionalPieces.append(PlacedPiece(piece, position, optionIndex == 1,
                                                            generatePieceColor(additionalPieces.size()), 0));
                        occupiedWasteRects.append(position);
                        placed = true;
                    }
                }
            }

            if (placed) {
                break;
            }
        }
    }

    return additionalPieces;
}

