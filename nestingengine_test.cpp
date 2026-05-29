#include "nestingengine.h"
#include "piecerect.h"
#include "panel.h"

#include <QTest>
#include <QDebug>

/**
 * @brief Unit tests for the AI Smart Nesting Optimizer
 */
class NestingEngineTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        qDebug() << "Initializing NestingEngine tests...";
    }

    void testPieceRect()
    {
        // Test piece creation
        PieceRect piece(1, "Test Piece", 100, 200);
        QVERIFY(piece.id() == 1);
        QVERIFY(piece.name() == "Test Piece");
        QVERIFY(piece.length() == 100);
        QVERIFY(piece.width() == 200);
        QVERIFY(piece.area() == 20000);
        QVERIFY(piece.isValid());

        // Test rotation
        QRectF originalRect = piece.toRect();
        QRectF rotatedRect = piece.rotatedRect();
        QVERIFY(originalRect.width() == rotatedRect.height());
        QVERIFY(originalRect.height() == rotatedRect.width());
    }

    void testPanel()
    {
        // Test panel creation
        Panel panel(1, "Test Panel", 2440, 1220, 18, "MDF");
        QVERIFY(panel.id() == 1);
        QVERIFY(panel.name() == "Test Panel");
        QVERIFY(panel.length() == 2440);
        QVERIFY(panel.width() == 1220);
        QVERIFY(panel.thickness() == 18);
        QVERIFY(panel.material() == "MDF");
        QVERIFY(panel.isValid());

        // Test standard panels
        Panel stdPanel = Panel::standard2440x1220(2, "Test");
        QVERIFY(stdPanel.length() == 2440);
        QVERIFY(stdPanel.width() == 1220);
    }

    void testNestingEngineCreation()
    {
        NestingEngine engine;
        QVERIFY(engine.isSimulationMode() == false);

        engine.setSimulationMode(true);
        QVERIFY(engine.isSimulationMode() == true);
    }

    void testSimpleNesting()
    {
        NestingEngine engine;

        // Create test pieces
        QVector<PieceRect*> pieces;
        pieces.append(new PieceRect(1, "Small Piece", 100, 100));
        pieces.append(new PieceRect(2, "Medium Piece", 200, 150));

        // Create test panel
        QVector<Panel*> panels;
        panels.append(new Panel(1, "Test Panel", 500, 400, 18));

        // Run optimization
        QVector<PanelLayout> layouts = engine.optimizeLayout(pieces, panels, NestingMode::Fast, 3.0);

        // Verify results
        QVERIFY(!layouts.isEmpty());
        QVERIFY(layouts.first().placedPieces.size() <= 2); // Should place 1 or 2 pieces

        // Clean up
        qDeleteAll(pieces);
        qDeleteAll(panels);
    }

    void testWasteCalculation()
    {
        // Test waste calculation with known layout
        QRectF panelRect(0, 0, 1000, 1000);
        QVector<QRectF> usedRects;
        usedRects.append(QRectF(0, 0, 400, 300));
        usedRects.append(QRectF(400, 0, 300, 400));

        double waste = NestingEngine::calculateWasteArea(panelRect, usedRects);
        double expectedWaste = 1000000 - (400*300 + 300*400); // 1000*1000 - used area

        QVERIFY(qAbs(waste - expectedWaste) < 1.0); // Allow small floating point differences
    }

    void testAIScore()
    {
        NestingEngine engine;

        // Create test layout with 80% efficiency
        PanelLayout layout;
        layout.panel = new Panel(1, "Test", 1000, 1000, 18);
        layout.wasteArea = 200000; // 20% waste
        layout.wastePercent = 20.0;
        layout.efficiency = 80.0;
        layout.placedPieces = QVector<PlacedPiece>(); // Empty for this test

        QVector<PanelLayout> layouts;
        layouts.append(layout);

        double aiScore = engine.calculateAIScore(layouts);
        QVERIFY(aiScore >= 75.0); // Should be high score for single panel with good efficiency

        // Clean up
        delete layout.panel;
    }

    void testOptimizationSuggestions()
    {
        NestingEngine engine;

        // Create test layout with high waste
        PanelLayout layout;
        layout.panel = new Panel(1, "Test", 1000, 1000, 18);
        layout.wastePercent = 50.0; // 50% waste
        layout.efficiency = 50.0;

        QVector<PanelLayout> layouts;
        layouts.append(layout);

        QStringList suggestions = engine.generateOptimizationSuggestions(layouts);
        QVERIFY(!suggestions.isEmpty());
        QVERIFY(suggestions.first().contains("déchet élevé") ||
                suggestions.first().contains("efficacité faible"));

        // Clean up
        delete layout.panel;
    }

    void cleanupTestCase()
    {
        qDebug() << "Cleaning up NestingEngine tests...";
    }
};

// QTEST_MAIN macro includes the main() function
QTEST_MAIN(NestingEngineTest)

#include "nestingengine_test.moc"</content>
<parameter name="filePath">c:\Users\SETUP\OneDrive\Bureau\New folder (2)\Nouveau dossier\Nouveau dossier\nestingengine_test.cpp