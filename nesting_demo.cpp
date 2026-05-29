#include "nestingengine.h"
#include "nestingwidget.h"
#include "piecerect.h"
#include "panel.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QDebug>

/**
 * @brief Demonstration of the AI Smart Nesting Optimizer
 *
 * This example shows how to use the nesting optimizer
 * with sample pieces and panels.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create main window
    QWidget window;
    window.setWindowTitle("AI Smart Nesting Optimizer - Demo");
    window.setMinimumSize(1200, 800);

    // Create nesting widget
    NestingWidget* nestingWidget = new NestingWidget(&window);

    // Create sample pieces
    QVector<PieceRect*> pieces;
    pieces.append(new PieceRect(1, "Plateau Table", 1200, 800));
    pieces.append(new PieceRect(2, "Tiroir Face", 400, 200));
    pieces.append(new PieceRect(3, "Porte Armoire", 600, 300));
    pieces.append(new PieceRect(4, "Etagere", 800, 250));
    pieces.append(new PieceRect(5, "Pied Table", 80, 80));
    pieces.append(new PieceRect(6, "Pied Table", 80, 80));
    pieces.append(new PieceRect(7, "Pied Table", 80, 80));
    pieces.append(new PieceRect(8, "Pied Table", 80, 80));

    // Create sample panels
    QVector<Panel*> panels;
    panels.append(Panel::standard2440x1220(1, "MDF 18mm"));
    panels.append(Panel::standard2500x1250(2, "Contreplaqué 15mm"));

    // Load data into widget
    nestingWidget->loadPieces(pieces);
    nestingWidget->loadPanels(panels);

    // Create control buttons
    QPushButton* optimizeButton = new QPushButton("🚀 Démarrer l'optimisation");
    optimizeButton->setMinimumHeight(40);
    optimizeButton->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1); "
        "color: white; border-radius: 10px; padding: 10px 20px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background: qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #6366f1,stop:1 #818cf8); }"
    );

    QPushButton* clearButton = new QPushButton("🗑️ Effacer");
    clearButton->setStyleSheet(
        "QPushButton { background: #dc2626; color: white; border-radius: 8px; padding: 8px 16px; }"
        "QPushButton:hover { background: #ef4444; }"
    );

    // Connect buttons
    QObject::connect(optimizeButton, &QPushButton::clicked, nestingWidget, &NestingWidget::startOptimization);
    QObject::connect(clearButton, &QPushButton::clicked, nestingWidget, &NestingWidget::clearLayout);

    // Create info label
    QLabel* infoLabel = new QLabel(
        "<h2>🧠 AI Smart Nesting Optimizer</h2>"
        "<p><b>Démonstration avec 8 pièces d'exemple</b></p>"
        "<ul>"
        "<li>1 Plateau de table (1200×800mm)</li>"
        "<li>1 Face de tiroir (400×200mm)</li>"
        "<li>1 Porte d'armoire (600×300mm)</li>"
        "<li>1 Étagère (800×250mm)</li>"
        "<li>4 Pieds de table (80×80mm chacun)</li>"
        "</ul>"
        "<p><b>Panneaux disponibles :</b></p>"
        "<ul>"
        "<li>MDF 2440×1220mm</li>"
        "<li>Contreplaqué 2500×1250mm</li>"
        "</ul>"
        "<p>Cliquez sur <b>'Optimiser découpe'</b> pour voir l'algorithme en action !</p>"
    );
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("QLabel { padding: 20px; background: #f8fafc; border-radius: 10px; }");

    // Layout
    QVBoxLayout* mainLayout = new QVBoxLayout(&window);

    // Top controls
    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->addWidget(optimizeButton);
    controlLayout->addWidget(clearButton);
    controlLayout->addStretch();
    mainLayout->addLayout(controlLayout);

    // Info panel
    mainLayout->addWidget(infoLabel);

    // Main nesting widget
    mainLayout->addWidget(nestingWidget);

    // Show window
    window.show();

    // Start optimization automatically after a short delay
    QTimer::singleShot(2000, [nestingWidget]() {
        qDebug() << "Starting automatic optimization demo...";
        nestingWidget->startOptimization();
    });

    return app.exec();
}

#include "main.moc"</content>
<parameter name="filePath">c:\Users\SETUP\OneDrive\Bureau\New folder (2)\Nouveau dossier\Nouveau dossier\nesting_demo.cpp