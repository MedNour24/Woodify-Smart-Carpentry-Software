#include "nestingwidget.h"
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QScrollArea>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QProgressBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>
#include <QDebug>
#include <QTabBar>

#include <QTimeLine>
#include <QEasingCurve>

// PieceGraphicsItem implementation
PieceGraphicsItem::PieceGraphicsItem(const PlacedPiece& placedPiece, QGraphicsItem* parent)
    : QGraphicsRectItem(parent), m_pieceData(placedPiece), m_dragging(false)
{
    setRect(0, 0, placedPiece.bounds.width(), placedPiece.bounds.height());

    // 3D Glass/Acrylic Gradient
    QColor baseColor = generateVibrantColor(placedPiece.piece->id());
    QLinearGradient gradient(0, 0, placedPiece.bounds.width(), placedPiece.bounds.height());
    gradient.setColorAt(0.0, baseColor.lighter(130));
    gradient.setColorAt(0.5, baseColor);
    gradient.setColorAt(1.0, baseColor.darker(120));
    setBrush(QBrush(gradient));

    // Refined Border
    QPen borderPen(QColor(255, 255, 255, 140), 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    borderPen.setCosmetic(true); // Keeps border thin regardless of zoom
    setPen(borderPen);

    // Stronger Drop Shadow
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(0, 0, 0, 150));
    shadow->setOffset(4, 4);
    setGraphicsEffect(shadow);

    // Name Text
    QString displayName = placedPiece.piece->name();
    if (displayName.length() > 15) displayName = displayName.left(13) + "...";
    m_textItem = new QGraphicsTextItem(displayName, this);
    m_textItem->setDefaultTextColor(Qt::white);
    m_textItem->setFont(QFont("Segoe UI", 10, QFont::Bold));

    // Center text perfectly
    QRectF textRect = m_textItem->boundingRect();
    m_textItem->setPos((placedPiece.bounds.width() - textRect.width()) / 2.0, 
                       (placedPiece.bounds.height() - textRect.height()) / 2.0);

    // Subtle text shadow
    QGraphicsDropShadowEffect* textShadow = new QGraphicsDropShadowEffect();
    textShadow->setBlurRadius(3);
    textShadow->setColor(QColor(0, 0, 0, 200));
    textShadow->setOffset(1, 1);
    m_textItem->setGraphicsEffect(textShadow);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QColor PieceGraphicsItem::generateVibrantColor(int pieceId)
{
    // Generate vibrant, distinct colors using HSV color space
    int hue = (pieceId * 137) % 360; // Use golden angle for good distribution
    int saturation = 160 + (pieceId * 7) % 40; // High saturation
    int value = 200 + (pieceId * 11) % 55; // High brightness

    return QColor::fromHsv(hue, saturation, value);
}

void PieceGraphicsItem::setPieceData(const PlacedPiece& piece)
{
    m_pieceData = piece;
    setRect(0, 0, piece.bounds.width(), piece.bounds.height());
    
    QColor baseColor = generateVibrantColor(piece.piece->id());
    QLinearGradient gradient(0, 0, piece.bounds.width(), piece.bounds.height());
    gradient.setColorAt(0.0, baseColor.lighter(130));
    gradient.setColorAt(0.5, baseColor);
    gradient.setColorAt(1.0, baseColor.darker(120));
    setBrush(QBrush(gradient));

    if (m_textItem) {
        QString displayName = piece.piece->name();
        if (displayName.length() > 15) displayName = displayName.left(13) + "...";
        m_textItem->setPlainText(displayName);
        QRectF textRect = m_textItem->boundingRect();
        m_textItem->setPos((piece.bounds.width() - textRect.width()) / 2.0, 
                           (piece.bounds.height() - textRect.height()) / 2.0);
    }
}

void PieceGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event)
    setPen(QPen(QColor(0, 212, 255), 3)); // Neon cyan border on hover
    setCursor(Qt::OpenHandCursor);
    
    if (m_pieceData.piece) {
        setToolTip(QString("<div style='background-color: #151A22; color: #E2E8F0; padding: 6px; border: 1px solid #00D4FF; border-radius: 6px;'>"
                           "<b style='color: white; font-size: 14px;'>Piece %1</b> <span style='color: #94A3B8;'>(%2)</span><hr style='border: 1px solid #404040;'/>"
                           "Dimensions: <span style='color:#00D4FF; font-weight: bold;'>%3 x %4 mm</span><br/>"
                           "Rotation: <span style='color:%5; font-weight: bold;'>%6</span>"
                           "</div>")
                  .arg(m_pieceData.piece->name())
                  .arg(m_pieceData.piece->code())
                  .arg(QString::number(m_pieceData.bounds.width(), 'f', 1))
                  .arg(QString::number(m_pieceData.bounds.height(), 'f', 1))
                  .arg(m_pieceData.rotated ? "#00FF88" : "#94A3B8")
                  .arg(m_pieceData.rotated ? "Oui (90 deg)" : "Non"));
    }
}

void PieceGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event)
    QPen borderPen(QColor(255, 255, 255, 140), 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    borderPen.setCosmetic(true);
    setPen(borderPen);
    setCursor(Qt::ArrowCursor);
}

void PieceGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragStartPos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsRectItem::mousePressEvent(event);
}

void PieceGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_dragging) {
        QPointF delta = event->pos() - m_dragStartPos;
        moveBy(delta.x(), delta.y());
        m_dragStartPos = event->pos();
    }
    QGraphicsRectItem::mouseMoveEvent(event);
}

void PieceGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_dragging && event->button() == Qt::LeftButton) {
        m_dragging = false;
        setCursor(Qt::OpenHandCursor);
    }
    QGraphicsRectItem::mouseReleaseEvent(event);
}

// PanelGraphicsItem implementation
PanelGraphicsItem::PanelGraphicsItem(const Panel* panel, int panelIndex, QGraphicsItem* parent)
    : QGraphicsRectItem(parent), m_panelIndex(panelIndex)
{
    QRectF panelRect = panel->toRect();
    setRect(0, 0, panelRect.width(), panelRect.height());

    // Industrial Grid background
    QColor baseColor(21, 26, 34);
    setBrush(QBrush(baseColor));

    // High-tech Cyan/Blue border
    QPen borderPen(QColor(0, 212, 255, 180), 3);
    borderPen.setCosmetic(true);
    setPen(borderPen);

    // Super realistic shadow
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(25);
    shadow->setColor(QColor(0, 0, 0, 200));
    shadow->setOffset(6, 6);
    setGraphicsEffect(shadow);

    // Modern Label
    m_panelLabel = new QGraphicsTextItem(QString("PANNEAU %1 | %2 x %3 mm")
                                        .arg(panelIndex + 1)
                                        .arg(QString::number(panelRect.width(), 'f', 0))
                                        .arg(QString::number(panelRect.height(), 'f', 0)), this);
    m_panelLabel->setDefaultTextColor(QColor(148, 163, 184)); // Slate 400
    m_panelLabel->setFont(QFont("Segoe UI", 12, QFont::Black));
    m_panelLabel->setPos(12, 8);

    // Text shadow
    QGraphicsDropShadowEffect* textShadow = new QGraphicsDropShadowEffect();
    textShadow->setBlurRadius(4);
    textShadow->setColor(Qt::black);
    textShadow->setOffset(2, 2);
    m_panelLabel->setGraphicsEffect(textShadow);

    // Waste overlay (initially transparent)
    m_wasteOverlay = new QGraphicsRectItem(this);
    m_wasteOverlay->setRect(rect());
    m_wasteOverlay->setBrush(QBrush(QColor(255, 100, 100, 0))); // Transparent
    m_wasteOverlay->setPen(Qt::NoPen);
    m_wasteOverlay->setZValue(1); // Above pieces
}

void PanelGraphicsItem::updateWasteVisualization(double wastePercent)
{
    if (wastePercent <= 0) {
        m_wasteOverlay->setBrush(QBrush(QColor(255, 100, 100, 0)));
        return;
    }

    // Modern diagonal hatched pattern for waste
    int alpha = qMin(160, static_cast<int>(wastePercent * 2.5));
    QColor wasteColor(255, 77, 77, alpha);

    QBrush hatchBrush(wasteColor, Qt::FDiagPattern);
    m_wasteOverlay->setBrush(hatchBrush);
}

// NestingWidget implementation
NestingWidget::NestingWidget(QWidget* parent)
    : QWidget(parent), m_engine(new NestingEngine(this)),
      m_scene(new QGraphicsScene(this)), m_isOptimizing(false), m_comparisonMode(false),
      m_realTimeEnabled(false), m_currentZoom(1.0), m_heatmapItem(nullptr)
{
    setupUI();
    setupConnections();
}

NestingWidget::~NestingWidget()
{
    clearLayout();
}

void NestingWidget::setupUI()
{
    setObjectName("nestingRoot");
    this->setStyleSheet(R"(
        QWidget#nestingRoot {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #0b1220,
                                        stop:0.55 #111b2f,
                                        stop:1 #09101c);
            color: #dbe8ff;
            font-family: 'Segoe UI', Arial, sans-serif;
        }

        QWidget {
            color: #dbe8ff;
            font-size: 12px;
        }

        QWidget#headerCard {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 rgba(20,30,48,230),
                                        stop:1 rgba(14,24,40,235));
            border: 1px solid rgba(120, 176, 255, 55);
            border-radius: 14px;
        }

        QFrame#dashboardCard, QFrame#canvasCard {
            background: rgba(15, 23, 37, 220);
            border: 1px solid rgba(128, 178, 255, 42);
            border-radius: 14px;
        }

        QFrame#sectionCard {
            background: rgba(8, 16, 28, 210);
            border: 1px solid rgba(128, 178, 255, 30);
            border-radius: 10px;
        }

        QFrame#kpiCard {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 rgba(34, 48, 72, 200),
                                        stop:1 rgba(18, 28, 46, 220));
            border: 1px solid rgba(126, 177, 255, 45);
            border-radius: 12px;
        }

        QLabel#titleLabel {
            color: #ecf4ff;
            font-size: 24px;
            font-weight: 800;
            letter-spacing: 0.4px;
        }

        QLabel#headerStatus {
            color: #9fc0ea;
            font-size: 12px;
            font-weight: 500;
        }

        QLabel#sectionTitle {
            color: #7ed8ff;
            font-size: 13px;
            font-weight: 700;
            padding-bottom: 2px;
        }

        QLabel#canvasTitle {
            color: #d9ecff;
            font-size: 13px;
            font-weight: 700;
        }

        QLabel#kpiValue {
            color: #ffffff;
            font-size: 28px;
            font-weight: 800;
        }

        QLabel#kpiLabel {
            color: #9fb8dc;
            font-size: 11px;
            font-weight: 600;
        }

        QTabWidget#leftControlTabs::pane,
        QTabWidget#analyticsTabs::pane {
            border: 1px solid rgba(126, 177, 255, 42);
            background: rgba(10, 17, 29, 222);
            border-radius: 12px;
            top: -1px;
        }

        QTabWidget#leftControlTabs QTabBar::tab,
        QTabWidget#analyticsTabs QTabBar::tab {
            background: rgba(18, 28, 46, 218);
            color: #9fb7d8;
            border: 1px solid rgba(126, 177, 255, 34);
            border-bottom: none;
            border-top-left-radius: 10px;
            border-top-right-radius: 10px;
            padding: 9px 12px;
            min-width: 120px;
            font-weight: 700;
        }

        QTabWidget#leftControlTabs QTabBar::tab:selected,
        QTabWidget#analyticsTabs QTabBar::tab:selected {
            background: rgba(30, 46, 72, 246);
            color: #f4f9ff;
            border-color: rgba(126, 177, 255, 70);
        }

        QTabWidget#leftControlTabs QTabBar::tab:hover:!selected,
        QTabWidget#analyticsTabs QTabBar::tab:hover:!selected {
            color: #cde8ff;
            background: rgba(24, 39, 62, 235);
        }

        QPushButton#primaryButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 #1a9eff,
                                        stop:1 #3677ff);
            color: #f4f9ff;
            border: none;
            border-radius: 10px;
            padding: 12px 16px;
            font-size: 13px;
            font-weight: 700;
        }

        QPushButton#primaryButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 #4ab6ff,
                                        stop:1 #578dff);
        }

        QPushButton#primaryButton:disabled {
            background: rgba(73, 105, 155, 180);
            color: rgba(230, 241, 255, 180);
        }

        QPushButton#secondaryButton, QPushButton#iconButton {
            background: rgba(26, 40, 62, 220);
            color: #d6e9ff;
            border: 1px solid rgba(126, 177, 255, 48);
            border-radius: 8px;
            padding: 8px 12px;
            font-weight: 600;
        }

        QPushButton#iconButton {
            min-width: 34px;
            max-width: 34px;
            min-height: 30px;
            max-height: 30px;
            padding: 0;
        }

        QPushButton#secondaryButton:hover, QPushButton#iconButton:hover {
            background: rgba(36, 56, 86, 240);
            border-color: rgba(126, 177, 255, 78);
        }

        QPushButton#secondaryButton:checked {
            background: rgba(66, 119, 202, 215);
            color: #ffffff;
            border-color: rgba(151, 201, 255, 180);
        }

        QComboBox, QSpinBox {
            background: rgba(8, 14, 24, 210);
            color: #e5f1ff;
            border: 1px solid rgba(126, 177, 255, 42);
            border-radius: 8px;
            padding: 7px 10px;
            min-height: 34px;
        }

        QComboBox:hover, QSpinBox:hover,
        QComboBox:focus, QSpinBox:focus {
            border-color: rgba(120, 196, 255, 135);
        }

        QComboBox::drop-down {
            border: none;
            width: 24px;
        }

        QComboBox QAbstractItemView {
            background: #101a2b;
            color: #e5f1ff;
            selection-background-color: #2e5ea9;
            border: 1px solid rgba(126, 177, 255, 50);
            border-radius: 6px;
        }

        QCheckBox {
            color: #c7dbf8;
            spacing: 10px;
            font-size: 12px;
            padding: 2px 0;
        }

        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border: 1px solid rgba(126, 177, 255, 65);
            border-radius: 4px;
            background: rgba(8, 14, 24, 220);
        }

        QCheckBox::indicator:checked {
            background: #32adff;
            border-color: #32adff;
        }

        QProgressBar {
            min-height: 20px;
            border: 1px solid rgba(126, 177, 255, 48);
            border-radius: 8px;
            background: rgba(8, 14, 24, 220);
            color: #e9f3ff;
            text-align: center;
            font-weight: 700;
        }

        QProgressBar::chunk {
            border-radius: 7px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 #3cb5ff,
                                        stop:1 #7ad5ff);
        }

        QTextEdit {
            background: rgba(8, 14, 24, 220);
            color: #d8eaff;
            border: 1px solid rgba(126, 177, 255, 34);
            border-radius: 10px;
            padding: 10px;
            line-height: 1.5;
        }

        QGraphicsView#nestingCanvas {
            background: #121a28;
            border: 1px solid rgba(126, 177, 255, 42);
            border-radius: 10px;
        }
    )");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 18, 20, 20);
    mainLayout->setSpacing(18);

    // Header with title and status
    createHeader();
    mainLayout->addWidget(m_headerWidget);

    // Main dashboard area
    QHBoxLayout* dashboardLayout = new QHBoxLayout();
    dashboardLayout->setSpacing(18);

    // Left control panel
    m_leftPanel = createLeftControlPanel();
    dashboardLayout->addWidget(m_leftPanel, 1);

    // Central visualization
    m_centerPanel = createCentralVisualization();
    dashboardLayout->addWidget(m_centerPanel, 3);

    // Right analytics panel
    m_rightPanel = createRightAnalyticsPanel();
    dashboardLayout->addWidget(m_rightPanel, 1);

    mainLayout->addLayout(dashboardLayout, 1);
}

void NestingWidget::createHeader()
{
    m_headerWidget = new QWidget();
    m_headerWidget->setObjectName("headerCard");
    m_headerWidget->setMinimumHeight(92);
    m_headerWidget->setMaximumHeight(108);

    QHBoxLayout* headerLayout = new QHBoxLayout(m_headerWidget);
    headerLayout->setContentsMargins(18, 14, 18, 14);
    headerLayout->setSpacing(14);

    // Title section
    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(2);
    m_titleLabel = new QLabel("AI Smart Nesting Optimizer");
    m_titleLabel->setObjectName("titleLabel");
    m_statusLabel = new QLabel("Pret a optimiser");
    m_statusLabel->setObjectName("headerStatus");
    titleLayout->addWidget(m_titleLabel);
    titleLayout->addWidget(m_statusLabel);
    headerLayout->addLayout(titleLayout);

    headerLayout->addStretch();

    // Progress section
    m_progressBar = new QProgressBar();
    m_progressBar->setFixedWidth(240);
    m_progressBar->setTextVisible(true);
    m_progressBar->setFormat("%p%");
    m_progressBar->setVisible(false);

    headerLayout->addWidget(m_progressBar);
}

QWidget* NestingWidget::createLeftControlPanel()
{
    QWidget* panel = new QWidget();
    panel->setObjectName("controlPanel");
    panel->setFixedWidth(306);

    QVBoxLayout* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QTabWidget* leftTabs = new QTabWidget();
    leftTabs->setObjectName("leftControlTabs");
    leftTabs->setDocumentMode(true);
    leftTabs->setUsesScrollButtons(false);
    leftTabs->tabBar()->setExpanding(true);
    leftTabs->tabBar()->setElideMode(Qt::ElideRight);

    // --- Tab 1 : Parametrage ---
    QWidget* paramTab = new QWidget();
    QVBoxLayout* paramLayout = new QVBoxLayout(paramTab);
    paramLayout->setContentsMargins(12, 12, 12, 12);
    paramLayout->setSpacing(10);

    QFrame* sizeCard = new QFrame();
    sizeCard->setObjectName("sectionCard");
    QVBoxLayout* sizeCardLayout = new QVBoxLayout(sizeCard);
    sizeCardLayout->setContentsMargins(12, 12, 12, 12);
    sizeCardLayout->setSpacing(8);

    QLabel* plaqueTitle = new QLabel("Dimensions plaque");
    plaqueTitle->setObjectName("sectionTitle");
    sizeCardLayout->addWidget(plaqueTitle);

    QFormLayout* sizeLayout = new QFormLayout();
    sizeLayout->setContentsMargins(0, 0, 0, 0);
    sizeLayout->setHorizontalSpacing(8);
    sizeLayout->setVerticalSpacing(8);

    QSpinBox* lengthSpin = new QSpinBox();
    lengthSpin->setObjectName("panelLengthSpin");
    lengthSpin->setRange(100, 10000);
    lengthSpin->setValue(2440);
    lengthSpin->setSuffix(" mm");

    QSpinBox* widthSpin = new QSpinBox();
    widthSpin->setObjectName("panelWidthSpin");
    widthSpin->setRange(100, 10000);
    widthSpin->setValue(1220);
    widthSpin->setSuffix(" mm");

    sizeLayout->addRow("Longueur:", lengthSpin);
    sizeLayout->addRow("Largeur:", widthSpin);
    sizeCardLayout->addLayout(sizeLayout);
    paramLayout->addWidget(sizeCard);

    QFrame* algoCard = new QFrame();
    algoCard->setObjectName("sectionCard");
    QVBoxLayout* algoCardLayout = new QVBoxLayout(algoCard);
    algoCardLayout->setContentsMargins(12, 12, 12, 12);
    algoCardLayout->setSpacing(8);

    QLabel* algoTitle = new QLabel("Algorithme");
    algoTitle->setObjectName("sectionTitle");
    algoCardLayout->addWidget(algoTitle);

    QFormLayout* algoLayout = new QFormLayout();
    algoLayout->setContentsMargins(0, 0, 0, 0);
    algoLayout->setHorizontalSpacing(8);
    algoLayout->setVerticalSpacing(8);

    m_modeCombo = new QComboBox();
    m_modeCombo->addItem("Rapide", static_cast<int>(NestingMode::Fast));
    m_modeCombo->addItem("Equilibre", static_cast<int>(NestingMode::Balanced));
    m_modeCombo->addItem("IA avancee", static_cast<int>(NestingMode::Advanced));
    m_modeCombo->addItem("Ajustement", static_cast<int>(NestingMode::BestFit));

    m_toleranceSpin = new QSpinBox();
    m_toleranceSpin->setRange(1, 20);
    m_toleranceSpin->setValue(3);
    m_toleranceSpin->setSuffix(" mm");

    algoLayout->addRow("Mode:", m_modeCombo);
    algoLayout->addRow("Lame:", m_toleranceSpin);
    algoCardLayout->addLayout(algoLayout);
    paramLayout->addWidget(algoCard);

    QFrame* optionsCard = new QFrame();
    optionsCard->setObjectName("sectionCard");
    QVBoxLayout* optionsLayout = new QVBoxLayout(optionsCard);
    optionsLayout->setContentsMargins(12, 12, 12, 12);
    optionsLayout->setSpacing(6);

    QLabel* optionsTitle = new QLabel("Options");
    optionsTitle->setObjectName("sectionTitle");
    optionsLayout->addWidget(optionsTitle);

    QCheckBox* allowRotationCheck = new QCheckBox("Autoriser rotation");
    allowRotationCheck->setObjectName("allowRotationCheck");
    allowRotationCheck->setChecked(true);
    optionsLayout->addWidget(allowRotationCheck);

    m_simulationCheck = new QCheckBox("Mode simulation");
    optionsLayout->addWidget(m_simulationCheck);

    m_realTimeCheck = new QCheckBox("Mode temps reel");
    optionsLayout->addWidget(m_realTimeCheck);
    optionsLayout->addStretch();

    paramLayout->addWidget(optionsCard);
    paramLayout->addStretch();
    leftTabs->addTab(paramTab, "Parametres");

    // --- Tab 2 : Execution ---
    QWidget* optTab = new QWidget();
    QVBoxLayout* optLayout = new QVBoxLayout(optTab);
    optLayout->setContentsMargins(12, 12, 12, 12);
    optLayout->setSpacing(10);

    QFrame* launchCard = new QFrame();
    launchCard->setObjectName("sectionCard");
    QVBoxLayout* launchLayout = new QVBoxLayout(launchCard);
    launchLayout->setContentsMargins(12, 12, 12, 12);
    launchLayout->setSpacing(10);

    QLabel* optTitle = new QLabel("Execution");
    optTitle->setObjectName("sectionTitle");
    launchLayout->addWidget(optTitle);

    QLabel* launchHint = new QLabel("Lancer l'optimisation et comparer les strategies.");
    launchHint->setWordWrap(true);
    launchHint->setStyleSheet("color: #9fb7d8; font-size: 11px;");
    launchLayout->addWidget(launchHint);

    m_optimizeButton = new QPushButton("Optimiser la decoupe");
    m_optimizeButton->setObjectName("primaryButton");
    m_optimizeButton->setMinimumHeight(52);
    launchLayout->addWidget(m_optimizeButton);

    m_comparisonButton = new QPushButton("Comparer avant / apres");
    m_comparisonButton->setObjectName("secondaryButton");
    m_comparisonButton->setCheckable(true);
    m_comparisonButton->setMinimumHeight(36);
    launchLayout->addWidget(m_comparisonButton);
    optLayout->addWidget(launchCard);

    QFrame* exportCard = new QFrame();
    exportCard->setObjectName("sectionCard");
    QVBoxLayout* exportLayout = new QVBoxLayout(exportCard);
    exportLayout->setContentsMargins(12, 12, 12, 12);
    exportLayout->setSpacing(10);

    QLabel* exportTitle = new QLabel("Exports");
    exportTitle->setObjectName("sectionTitle");
    exportLayout->addWidget(exportTitle);

    QHBoxLayout* exportButtonsLayout = new QHBoxLayout();
    exportButtonsLayout->setSpacing(8);

    m_exportPDFButton = new QPushButton("Exporter PDF");
    m_exportPDFButton->setObjectName("secondaryButton");

    m_exportPNGButton = new QPushButton("Exporter PNG");
    m_exportPNGButton->setObjectName("secondaryButton");

    exportButtonsLayout->addWidget(m_exportPDFButton);
    exportButtonsLayout->addWidget(m_exportPNGButton);
    exportLayout->addLayout(exportButtonsLayout);
    optLayout->addWidget(exportCard);

    optLayout->addStretch();
    leftTabs->addTab(optTab, "Execution");

    layout->addWidget(leftTabs);

    return panel;
}

QWidget* NestingWidget::createCentralVisualization()
{
    QWidget* panel = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QFrame* canvasCard = new QFrame();
    canvasCard->setObjectName("canvasCard");

    QVBoxLayout* canvasLayout = new QVBoxLayout(canvasCard);
    canvasLayout->setContentsMargins(12, 12, 12, 12);
    canvasLayout->setSpacing(10);

    QHBoxLayout* topBar = new QHBoxLayout();
    topBar->setContentsMargins(2, 0, 2, 0);
    topBar->setSpacing(8);

    QLabel* canvasTitle = new QLabel("Apercu du nesting");
    canvasTitle->setObjectName("canvasTitle");
    topBar->addWidget(canvasTitle);
    topBar->addStretch();

    m_zoomInButton = new QPushButton("+");
    m_zoomInButton->setObjectName("iconButton");

    m_zoomOutButton = new QPushButton("-");
    m_zoomOutButton->setObjectName("iconButton");

    m_fitButton = new QPushButton("Fit");
    m_fitButton->setObjectName("secondaryButton");
    m_fitButton->setMinimumWidth(56);

    topBar->addWidget(m_zoomInButton);
    topBar->addWidget(m_zoomOutButton);
    topBar->addWidget(m_fitButton);

    canvasLayout->addLayout(topBar);

    setupGraphicsView();
    m_graphicsView->setMinimumHeight(420);
    canvasLayout->addWidget(m_graphicsView, 1);

    layout->addWidget(canvasCard, 1);

    return panel;
}

QWidget* NestingWidget::createRightAnalyticsPanel()
{
    QWidget* panel = new QWidget();
    panel->setFixedWidth(360);

    QVBoxLayout* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QTabWidget* rightTabs = new QTabWidget();
    rightTabs->setObjectName("analyticsTabs");
    rightTabs->setDocumentMode(true);
    rightTabs->setUsesScrollButtons(false);
    rightTabs->tabBar()->setExpanding(true);
    rightTabs->tabBar()->setElideMode(Qt::ElideRight);

    // --- Tab 1: Metrics ---
    QWidget* kpiTab = new QWidget();
    QVBoxLayout* kpiTabLayout = new QVBoxLayout(kpiTab);
    kpiTabLayout->setContentsMargins(12, 12, 12, 12);
    kpiTabLayout->setSpacing(10);

    QLabel* kpiTitle = new QLabel("Metriques de performance");
    kpiTitle->setObjectName("sectionTitle");
    kpiTabLayout->addWidget(kpiTitle);

    QGridLayout* kpiGrid = new QGridLayout();
    kpiGrid->setSpacing(10);

    kpiGrid->addWidget(createKPICard("Pieces", "0", "normal"), 0, 0);
    kpiGrid->addWidget(createKPICard("Panneaux", "0", "normal"), 0, 1);
    kpiGrid->addWidget(createKPICard("Materiau utilise", "0%", "warning"), 1, 0);
    kpiGrid->addWidget(createKPICard("Dechet", "0%", "bad"), 1, 1);
    kpiGrid->addWidget(createKPICard("Score IA", "0%", "normal"), 2, 0, 1, 2);

    kpiTabLayout->addLayout(kpiGrid);
    kpiTabLayout->addStretch();
    rightTabs->addTab(kpiTab, "Metriques");

    // --- Tab 2: AI insights ---
    QWidget* aiTab = new QWidget();
    QVBoxLayout* aiTabLayout = new QVBoxLayout(aiTab);
    aiTabLayout->setContentsMargins(12, 12, 12, 12);
    aiTabLayout->setSpacing(10);

    QWidget* insightsPanel = createAIInsightsPanel();
    aiTabLayout->addWidget(insightsPanel);
    aiTabLayout->addStretch();

    rightTabs->addTab(aiTab, "Insights IA");

    layout->addWidget(rightTabs);

    return panel;
}

QFrame* NestingWidget::createKPICard(const QString& title, const QString& value, const QString& status)
{
    QFrame* card = new QFrame();
    card->setObjectName("kpiCard");
    card->setMinimumSize(132, 96);

    if (title == "Score IA") {
        card->setMinimumHeight(104);
    }

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(12, 10, 12, 10);
    layout->setSpacing(4);

    QLabel* valueLabel = new QLabel(value);
    valueLabel->setObjectName("kpiValue");
    valueLabel->setAlignment(Qt::AlignCenter);

    QLabel* titleLabel = new QLabel(title);
    titleLabel->setObjectName("kpiLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setWordWrap(true);

    QString colorStyle;
    QString glowColor;
    if (status == "excellent") {
        colorStyle = "color: #34d399;";
        glowColor = "#34d399";
    } else if (status == "good") {
        colorStyle = "color: #60a5fa;";
        glowColor = "#60a5fa";
    } else if (status == "warning") {
        colorStyle = "color: #fbbf24;";
        glowColor = "#fbbf24";
    } else if (status == "bad") {
        colorStyle = "color: #f87171;";
        glowColor = "#f87171";
    } else {
        colorStyle = "color: #ffffff;";
        glowColor = "#ffffff";
    }

    valueLabel->setStyleSheet(colorStyle + " font-size: 34px; font-weight: 800;");

    QGraphicsDropShadowEffect* glow = new QGraphicsDropShadowEffect();
    glow->setBlurRadius(14);
    glow->setOffset(0, 0);
    QColor c(glowColor);
    c.setAlpha(90);
    glow->setColor(c);
    valueLabel->setGraphicsEffect(glow);

    layout->addStretch();
    layout->addWidget(valueLabel);
    layout->addWidget(titleLabel);
    layout->addStretch();

    return card;
}

QWidget* NestingWidget::createAIInsightsPanel()
{
    QFrame* panel = new QFrame();
    panel->setObjectName("dashboardCard");

    QVBoxLayout* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(10);

    QLabel* insightsTitle = new QLabel("Decision IA");
    insightsTitle->setObjectName("sectionTitle");
    layout->addWidget(insightsTitle);

    m_explainableAIText = new QTextEdit();
    m_explainableAIText->setMaximumHeight(156);
    m_explainableAIText->setPlaceholderText("Les decisions d'optimisation s'affichent ici...");
    m_explainableAIText->setReadOnly(true);
    layout->addWidget(m_explainableAIText);

    QLabel* suggestionsTitle = new QLabel("Suggestions");
    suggestionsTitle->setObjectName("sectionTitle");
    layout->addWidget(suggestionsTitle);

    m_suggestionsText = new QTextEdit();
    m_suggestionsText->setMaximumHeight(120);
    m_suggestionsText->setPlaceholderText("Suggestions de placement et reduction des pertes...");
    m_suggestionsText->setReadOnly(true);
    layout->addWidget(m_suggestionsText);

    return panel;
}

void NestingWidget::setupGraphicsView()
{
    m_scene = new QGraphicsScene(this);
    m_graphicsView = new QGraphicsView(m_scene);
    m_graphicsView->setObjectName("nestingCanvas");

    // Modern graphics settings
    m_graphicsView->setRenderHint(QPainter::Antialiasing, true);
    m_graphicsView->setRenderHint(QPainter::SmoothPixmapTransform, true);
    m_graphicsView->setRenderHint(QPainter::TextAntialiasing, true);
    m_graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    m_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    m_graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // Dark background for the view
    m_graphicsView->setFrameShape(QFrame::NoFrame);
    m_graphicsView->setBackgroundBrush(QBrush(QColor(18, 26, 40)));
    m_scene->setBackgroundBrush(QBrush(QColor(18, 26, 40)));

    // Enable mouse tracking for better interaction
    m_graphicsView->setMouseTracking(true);
    m_graphicsView->viewport()->setMouseTracking(true);
}

void NestingWidget::setupControls()
{
    m_optimizeButton = new QPushButton("Optimiser découpe");
    m_optimizeButton->setMinimumHeight(40);

    m_modeCombo = new QComboBox();
    m_modeCombo->addItem("Rapide (Greedy)", static_cast<int>(NestingMode::Fast));
    m_modeCombo->addItem("Équilibré", static_cast<int>(NestingMode::Balanced));
    m_modeCombo->addItem("IA avancée", static_cast<int>(NestingMode::Advanced));
    m_modeCombo->addItem("Meilleur ajustement", static_cast<int>(NestingMode::BestFit));
    m_modeCombo->addItem("Aléatoire", static_cast<int>(NestingMode::Random));
    m_modeCombo->addItem("Génétique", static_cast<int>(NestingMode::Genetic));

    m_toleranceSpin = new QSpinBox();
    m_toleranceSpin->setRange(1, 20);
    m_toleranceSpin->setValue(3);
    m_toleranceSpin->setSuffix(" mm");

    m_simulationCheck = new QCheckBox("Mode simulation");
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
}

void NestingWidget::setupAnalytics()
{
    m_piecesCountLabel = new QLabel("Pièces: 0");
    m_panelsCountLabel = new QLabel("Panneaux: 0");
    m_wasteLabel = new QLabel("Déchet: 0 mm² (0%)");
    m_efficiencyLabel = new QLabel("Efficacité: 0%");
    m_aiScoreLabel = new QLabel("Score IA: 0%");
}

void NestingWidget::setupSuggestions()
{
    m_suggestionsText = new QTextEdit();
    m_suggestionsText->setMaximumHeight(150);
    m_suggestionsText->setReadOnly(true);
}

void NestingWidget::setupConnections()
{
    connect(m_optimizeButton, &QPushButton::clicked, this, &NestingWidget::onOptimizeButtonClicked);
    connect(m_modeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NestingWidget::onModeChanged);
    connect(m_toleranceSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &NestingWidget::onToleranceChanged);
    connect(m_simulationCheck, &QCheckBox::toggled, this, &NestingWidget::onSimulationToggled);
    connect(m_realTimeCheck, &QCheckBox::toggled, this, &NestingWidget::enableRealTimeOptimization);

    connect(m_zoomInButton, &QPushButton::clicked, this, &NestingWidget::onZoomIn);
    connect(m_zoomOutButton, &QPushButton::clicked, this, &NestingWidget::onZoomOut);
    connect(m_fitButton, &QPushButton::clicked, this, &NestingWidget::onFitToView);

    connect(m_exportPDFButton, &QPushButton::clicked, this, &NestingWidget::onExportPDF);
    connect(m_exportPNGButton, &QPushButton::clicked, this, &NestingWidget::onExportPNG);
    connect(m_comparisonButton, &QPushButton::toggled, this, &NestingWidget::onComparisonToggled);

    connect(m_engine, &NestingEngine::optimizationProgress, this, &NestingWidget::onOptimizationProgress);
    connect(m_engine, &NestingEngine::simulationStep, this, &NestingWidget::onSimulationStep);
    connect(m_engine, &NestingEngine::layoutCompleted, this, &NestingWidget::onLayoutCompleted);
}

QGroupBox* NestingWidget::createControlGroup()
{
    QGroupBox* group = new QGroupBox("Contrôles d'optimisation");
    QVBoxLayout* layout = new QVBoxLayout(group);

    layout->addWidget(m_optimizeButton);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Mode:", m_modeCombo);
    formLayout->addRow("Tolérance de coupe:", m_toleranceSpin);
    layout->addLayout(formLayout);

    layout->addWidget(m_simulationCheck);
    layout->addWidget(m_progressBar);

    return group;
}

QGroupBox* NestingWidget::createAnalyticsGroup()
{
    QGroupBox* group = new QGroupBox("Analyses de performance");
    QVBoxLayout* layout = new QVBoxLayout(group);

    layout->addWidget(m_piecesCountLabel);
    layout->addWidget(m_panelsCountLabel);
    layout->addWidget(m_wasteLabel);
    layout->addWidget(m_efficiencyLabel);
    layout->addWidget(m_aiScoreLabel);

    return group;
}

QGroupBox* NestingWidget::createSuggestionsGroup()
{
    QGroupBox* group = new QGroupBox("Suggestions d'optimisation IA");
    QVBoxLayout* layout = new QVBoxLayout(group);

    layout->addWidget(m_suggestionsText);

    return group;
}

void NestingWidget::loadPieces(const QVector<PieceRect*>& pieces)
{
    m_pieces = pieces;
    clearLayout();
    if (m_statusLabel) {
        if (m_pieces.isEmpty()) {
            m_statusLabel->setText("Aucune piece chargee. Generez ou publiez d'abord un jeu de pieces.");
            m_statusLabel->setStyleSheet("color: #fbbf24; font-size: 12px;");
        } else {
            m_statusLabel->setText(QString("%1 piece(s) chargee(s), pret a optimiser.").arg(m_pieces.size()));
            m_statusLabel->setStyleSheet("color: #93c5fd; font-size: 12px;");
        }
    }
    updateAnalytics(m_currentLayouts);
}

void NestingWidget::loadPanels(const QVector<Panel*>& panels)
{
    m_panels = panels;
    if (m_statusLabel && m_panels.isEmpty()) {
        m_statusLabel->setText("Aucun panneau charge. Verifiez la configuration des plaques.");
        m_statusLabel->setStyleSheet("color: #f87171; font-size: 12px;");
    }
}

void NestingWidget::startOptimization()
{
    if (m_panels.isEmpty()) {
        m_statusLabel->setText("Aucun panneau disponible. Ajoutez ou chargez au moins une plaque.");
        m_statusLabel->setStyleSheet("color: #ff4444; font-size: 12px;");
        return;
    }
    if (m_pieces.isEmpty()) {
        m_statusLabel->setText("Aucune piece disponible. Generez des pieces puis relancez l'optimisation.");
        m_statusLabel->setStyleSheet("color: #fbbf24; font-size: 12px;");
        return;
    }

    // Capture user panel configuration if available
    QSpinBox* lengthSpin = findChild<QSpinBox*>("panelLengthSpin");
    QSpinBox* widthSpin = findChild<QSpinBox*>("panelWidthSpin");
    if (lengthSpin && widthSpin && !m_panels.isEmpty()) {
        m_panels[0]->setLength(lengthSpin->value());
        m_panels[0]->setWidth(widthSpin->value());
    }

    // Apply rotation preferences to all pieces
    QCheckBox* rotCheck = findChild<QCheckBox*>("allowRotationCheck");
    if (rotCheck) {
        bool allowRot = rotCheck->isChecked();
        for (PieceRect* piece : m_pieces) {
            piece->setCanRotate(allowRot);
        }
    }

    m_isOptimizing = true;
    m_optimizeButton->setEnabled(false);
    m_optimizeButton->setText("Optimisation en cours...");
    m_progressBar->setVisible(true);
    m_progressBar->setValue(0);

    // Update status
    m_statusLabel->setText("Analyse IA en cours...");
    m_statusLabel->setStyleSheet("color: #00d4ff; font-size: 12px;");

    NestingMode mode = static_cast<NestingMode>(m_modeCombo->currentData().toInt());
    double tolerance = m_toleranceSpin->value();

    m_engine->setSimulationMode(m_simulationCheck->isChecked());

    // Clear previous results
    clearLayout();

    // Start optimization (completion handled via NestingEngine::layoutCompleted signal)
    m_engine->optimizeLayout(m_pieces, m_panels, mode, tolerance);
}

void NestingWidget::stopOptimization()
{
    m_isOptimizing = false;
    m_optimizeButton->setEnabled(true);
    m_optimizeButton->setText("Optimiser la decoupe");
    m_progressBar->setVisible(false);
}

void NestingWidget::clearLayout()
{
    m_scene->clear();
    m_heatmapItem = nullptr;
    m_currentLayouts.clear();
    updateAnalytics(m_currentLayouts);
    m_suggestionsText->clear();
}

void NestingWidget::updateVisualization(const QVector<PanelLayout>& layouts)
{
    clearVisualization();

    double yOffset = 0;
    for (int i = 0; i < layouts.size(); ++i) {
        const PanelLayout& layout = layouts[i];
        addPanelToScene(layout, i, 0, yOffset);
        yOffset += layout.panel->width() + 50; // Add spacing between panels
    }

    onFitToView();
}

void NestingWidget::clearVisualization()
{
    m_scene->clear();
    m_heatmapItem = nullptr;
}

void NestingWidget::addPanelToScene(const PanelLayout& layout, int panelIndex, double xOffset, double yOffset)
{
    PanelGraphicsItem* panelItem = new PanelGraphicsItem(layout.panel, panelIndex);
    panelItem->setPos(xOffset, yOffset);
    panelItem->updateWasteVisualization(layout.wastePercent);
    m_scene->addItem(panelItem);

    // Add pieces to panel
    for (const PlacedPiece& piece : layout.placedPieces) {
        PieceGraphicsItem* pieceItem = new PieceGraphicsItem(piece);
        pieceItem->setParentItem(panelItem);
        pieceItem->setPos(piece.bounds.topLeft());
    }
}

void NestingWidget::addPieceToScene(const PlacedPiece& /* piece */)
{
    // Implementation for adding individual pieces
}

void NestingWidget::updateAnalytics(const QVector<PanelLayout>& layouts)
{
    if (layouts.isEmpty()) {
        updateKPICard("Pieces", "0", "normal");
        updateKPICard("Panneaux", "0", "normal");
        updateKPICard("Materiau utilise", "0%", "warning");
        updateKPICard("Dechet", "0%", "bad");
        updateKPICard("Score IA", "0%", "normal");
        return;
    }

    int totalPieces = 0;
    double totalWaste = 0.0;
    double totalUsedArea = 0.0;
    double totalPanelArea = 0.0;

    for (const PanelLayout& layout : layouts) {
        totalPieces += layout.placedPieces.size();
        totalWaste += layout.wasteArea;
        totalUsedArea += layout.panel->area() - layout.wasteArea;
        totalPanelArea += layout.panel->area();
    }

    if (totalPanelArea <= 0.0) {
        updateKPICard("Pieces", QString::number(totalPieces), "normal");
        updateKPICard("Panneaux", QString::number(layouts.size()), "normal");
        updateKPICard("Materiau utilise", "0%", "warning");
        updateKPICard("Dechet", "0%", "bad");
        updateKPICard("Score IA", "0%", "warning");
        return;
    }

    const double usedPercent = qBound(0.0, (totalUsedArea / totalPanelArea) * 100.0, 100.0);
    const double wastePercent = qBound(0.0, (totalWaste / totalPanelArea) * 100.0, 100.0);
    const double aiScore = qBound(0.0, m_engine->calculateAIScore(layouts), 100.0);

    updateKPICard("Pieces", QString::number(totalPieces), "normal");
    updateKPICard("Panneaux", QString::number(layouts.size()), "normal");
    updateKPICard("Materiau utilise", QString("%1%").arg(QString::number(usedPercent, 'f', 1)),
                  usedPercent >= 80.0 ? "excellent" : usedPercent >= 60.0 ? "good" : "warning");
    updateKPICard("Dechet", QString("%1%").arg(QString::number(wastePercent, 'f', 1)),
                  wastePercent <= 20.0 ? "good" : wastePercent <= 35.0 ? "warning" : "bad");
    updateKPICard("Score IA", QString("%1%").arg(QString::number(aiScore, 'f', 1)),
                  aiScore >= 80.0 ? "excellent" : aiScore >= 60.0 ? "good" : aiScore >= 40.0 ? "warning" : "bad");
}

void NestingWidget::updateKPICard(const QString& title, const QString& value, const QString& status)
{
    QList<QLabel*> kpiValueLabels = findChildren<QLabel*>("kpiValue");
    QList<QLabel*> kpiTitleLabels = findChildren<QLabel*>("kpiLabel");

    for (int i = 0; i < kpiTitleLabels.size(); ++i) {
        if (kpiTitleLabels[i]->text() == title && i < kpiValueLabels.size()) {
            kpiValueLabels[i]->setText(value);

            QString colorCode;
            if (status == "excellent") {
                colorCode = "#34d399";
            } else if (status == "good") {
                colorCode = "#60a5fa";
            } else if (status == "warning") {
                colorCode = "#fbbf24";
            } else if (status == "bad") {
                colorCode = "#f87171";
            } else {
                colorCode = "#ffffff";
            }

            const int fontSize = value.size() > 5 ? 28 : 34;
            kpiValueLabels[i]->setStyleSheet(
                QString("color: %1; font-size: %2px; font-weight: 800;")
                    .arg(colorCode)
                    .arg(fontSize));

            if (QGraphicsDropShadowEffect* eff = qobject_cast<QGraphicsDropShadowEffect*>(kpiValueLabels[i]->graphicsEffect())) {
                QColor c(colorCode);
                c.setAlpha(90);
                eff->setColor(c);
            }
            break;
        }
    }
}

void NestingWidget::updateSuggestions(const QStringList& suggestions)
{
    m_suggestionsText->setPlainText(suggestions.join("\n"));
}

QString NestingWidget::formatEfficiencyScore(double score) const
{
    QString level;
    if (score >= 90) level = "Excellent";
    else if (score >= 80) level = "Tres bon";
    else if (score >= 70) level = "Bon";
    else if (score >= 60) level = "Moyen";
    else level = "A ameliorer";

    return QString("Score IA: %1% (%2)")
        .arg(QString::number(score, 'f', 1))
        .arg(level);
}

QString NestingWidget::formatWasteInfo(double wasteMm2, double wastePercent) const
{
    return QString("Dechet: %1 mm2 (%2%)")
        .arg(QString::number(wasteMm2, 'f', 0))
        .arg(QString::number(wastePercent, 'f', 1));
}

// Event handlers
void NestingWidget::onOptimizeButtonClicked()
{
    if (m_isOptimizing) {
        stopOptimization();
    } else {
        startOptimization();
    }
}

void NestingWidget::onModeChanged(int index)
{
    Q_UNUSED(index)
    // Mode changed, could update UI hints
}

void NestingWidget::onToleranceChanged(int value)
{
    Q_UNUSED(value)
    // Tolerance changed, could validate input
}

void NestingWidget::onSimulationToggled(bool enabled)
{
    m_engine->setSimulationMode(enabled);
}

void NestingWidget::onOptimizationProgress(int percent)
{
    m_progressBar->setValue(percent);
}

void NestingWidget::onSimulationStep(const PlacedPiece& piece, int step, int totalSteps)
{
    Q_UNUSED(piece)
    m_progressBar->setValue((step * 100) / totalSteps);
    QCoreApplication::processEvents(); // Allow UI updates during simulation
}

void NestingWidget::onLayoutCompleted(const QVector<PanelLayout>& layouts)
{
    m_currentLayouts = layouts;

    // Update status
    m_statusLabel->setText("Optimisation terminee");
    m_statusLabel->setStyleSheet("color: #00ff88; font-size: 12px;");

    // Animate the visualization update
    updateVisualization(layouts);
    updateAnalytics(layouts);

    // Generate and show suggestions
    QStringList suggestions = m_engine->generateOptimizationSuggestions(layouts);
    updateSuggestions(suggestions);

    // Enhanced explainable AI with more detailed decisions
    QStringList decisions;
    decisions << QString("Algorithme: %1").arg(m_modeCombo->currentText());
    decisions << QString("Pieces optimisees: %1").arg(m_pieces.size());
    decisions << QString("Panneaux utilises: %1").arg(layouts.size());

    if (!layouts.isEmpty()) {
        double totalEfficiency = 0.0;
        for (const PanelLayout& layout : layouts) {
            totalEfficiency += layout.efficiency;
        }
        const double avgEfficiency = totalEfficiency / layouts.size();
        decisions << QString("Efficacite moyenne: %1%").arg(QString::number(avgEfficiency, 'f', 1));

        if (layouts.size() == 1) {
            decisions << "Toutes les pieces tiennent sur un panneau.";
        } else {
            decisions << QString("%1 panneaux necessaires pour toutes les pieces.").arg(layouts.size());
        }

        int rotatedCount = 0;
        for (const PanelLayout& layout : layouts) {
            for (const PlacedPiece& piece : layout.placedPieces) {
                if (piece.rotated) {
                    rotatedCount++;
                }
            }
        }
        if (rotatedCount > 0) {
            decisions << QString("Pieces pivotees pour optimisation: %1").arg(rotatedCount);
        }
    }

    for (const QString& suggestion : suggestions) {
        decisions << QString("Suggestion: %1").arg(suggestion);
    }
    showExplainableAI(decisions);

    // Update heatmap for advanced visualization
    updateHeatmap();

    stopOptimization();

    // Animate KPI cards appearance
    animateKPICards();

    emit optimizationCompleted(layouts);
}

void NestingWidget::animateKPICards()
{
    // Add subtle animation to KPI cards when results are updated
    QList<QFrame*> kpiCards = findChildren<QFrame*>();
    for (QFrame* card : kpiCards) {
        if (card->objectName() == "kpiCard") {
            QPropertyAnimation* animation = new QPropertyAnimation(card, "opacity");
            animation->setDuration(300);
            animation->setStartValue(0.7);
            animation->setEndValue(1.0);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}

// Zoom controls
void NestingWidget::onZoomIn()
{
    m_currentZoom *= 1.2;
    m_graphicsView->scale(1.2, 1.2);
}

void NestingWidget::onZoomOut()
{
    m_currentZoom /= 1.2;
    m_graphicsView->scale(1 / 1.2, 1 / 1.2);
}

void NestingWidget::onFitToView()
{
    if (!m_scene->items().isEmpty()) {
        m_graphicsView->fitInView(m_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
        m_currentZoom = m_graphicsView->transform().m11(); // Get current scale
    }
}

// Export functions
void NestingWidget::exportToPDF(const QString& filePath)
{
    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);

    // Render scene to PDF
    m_scene->render(&painter);

    // Add analytics text
    painter.setFont(QFont("Arial", 10));
    int y = 50;
    painter.drawText(50, y, m_piecesCountLabel->text()); y += 20;
    painter.drawText(50, y, m_panelsCountLabel->text()); y += 20;
    painter.drawText(50, y, m_wasteLabel->text()); y += 20;
    painter.drawText(50, y, m_efficiencyLabel->text()); y += 20;
    painter.drawText(50, y, m_aiScoreLabel->text());
}

void NestingWidget::exportToPNG(const QString& filePath)
{
    QRectF sceneRect = m_scene->itemsBoundingRect();
    if (sceneRect.isEmpty()) return;

    QImage image(sceneRect.size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    m_scene->render(&painter, QRectF(QPointF(0, 0), sceneRect.size()), sceneRect);

    image.save(filePath, "PNG");
}

void NestingWidget::onExportPDF()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter en PDF",
                                                   "layout_nesting.pdf",
                                                   "PDF Files (*.pdf)");
    if (!filePath.isEmpty()) {
        exportToPDF(filePath);
    }
}

void NestingWidget::onExportPNG()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter en PNG",
                                                   "layout_nesting.png",
                                                   "PNG Files (*.png)");
    if (!filePath.isEmpty()) {
        exportToPNG(filePath);
    }
}

void NestingWidget::onComparisonToggled(bool enabled)
{
    m_comparisonMode = enabled;

    if (enabled) {
        // Generate "before" layout (simple placement without optimization)
        m_beforeLayouts = generateSimpleLayout();

        // Show side-by-side comparison
        showComparisonView();
        m_comparisonButton->setText("📊 Masquer comparaison");
    } else {
        // Show only optimized layout
        updateVisualization(m_currentLayouts);
        m_comparisonButton->setText("📊 Avant / Après");
    }
}

QVector<PanelLayout> NestingWidget::generateSimpleLayout()
{
    // Create a simple layout by placing pieces in order without optimization
    QVector<PanelLayout> layouts;

    if (m_pieces.isEmpty() || m_panels.isEmpty()) {
        return layouts;
    }

    Panel* panel = m_panels.first();
    PanelLayout layout;
    layout.panel = panel;

    // Simple row-by-row placement
    double currentX = 0;
    double currentY = 0;
    double rowHeight = 0;
    QRectF panelRect = panel->toRect();
    double panelWidth = panelRect.width();
    double panelHeight = panelRect.height();

    for (int i = 0; i < m_pieces.size(); ++i) {
        PieceRect* piece = m_pieces[i];
        QRectF pieceRect(0, 0, piece->length(), piece->width());

        // Try to place in current row
        if (currentX + pieceRect.width() > panelWidth) {
            // Move to next row
            currentX = 0;
            currentY += rowHeight;
            rowHeight = pieceRect.height();
        }

        // Check if it fits vertically
        if (currentY + pieceRect.height() > panelHeight) {
            // Would need new panel, but for comparison we'll just place it anyway
            // This shows the inefficiency
        }

        QRectF placedRect(currentX, currentY, pieceRect.width(), pieceRect.height());
        PlacedPiece placedPiece(piece, placedRect, false, QColor(150, 150, 150), 0);
        layout.placedPieces.append(placedPiece);

        currentX += pieceRect.width();
        rowHeight = qMax(rowHeight, pieceRect.height());
    }

    // Calculate waste for comparison
    QVector<QRectF> usedRects;
    for (const PlacedPiece& piece : layout.placedPieces) {
        usedRects.append(piece.bounds);
    }
    layout.wasteArea = NestingEngine::calculateWasteArea(panel->toRect(), usedRects);
    layout.wastePercent = (layout.wasteArea / panel->area()) * 100.0;
    layout.efficiency = NestingEngine::calculateLayoutEfficiency(layout);

    layouts.append(layout);
    return layouts;
}

void NestingWidget::showComparisonView()
{
    clearVisualization();

    if (m_beforeLayouts.isEmpty() || m_currentLayouts.isEmpty()) {
        return;
    }

    double spacing = 80;

    // Show "Before" layout on the left
    double yOffset = 0;
    for (int i = 0; i < m_beforeLayouts.size(); ++i) {
        const PanelLayout& layout = m_beforeLayouts[i];
        addPanelToScene(layout, i, 0, yOffset); // Left side
        yOffset += layout.panel->toRect().height() + spacing;
    }

    // Show "After" layout on the right
    yOffset = 0;
    double xOffset = m_panels.first()->toRect().width() + spacing * 2;
    for (int i = 0; i < m_currentLayouts.size(); ++i) {
        const PanelLayout& layout = m_currentLayouts[i];
        addPanelToScene(layout, i, xOffset, yOffset); // Right side
        yOffset += layout.panel->toRect().height() + spacing;
    }

    // Add comparison labels with backgrounds
    auto createLabel = [this](const QString& text, QColor color, QPointF pos) {
        QGraphicsRectItem* bg = new QGraphicsRectItem(pos.x() - 10, pos.y() - 10, 300, 50);
        bg->setBrush(QColor(21, 26, 34, 220));
        bg->setPen(QPen(color, 2));
        
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(20);
        shadow->setColor(color);
        shadow->setOffset(0, 0);
        bg->setGraphicsEffect(shadow);

        QGraphicsTextItem* label = new QGraphicsTextItem(text, bg);
        label->setDefaultTextColor(color);
        label->setFont(QFont("Segoe UI", 16, QFont::Black));
        label->setPos(pos.x() + 10, pos.y() - 2);

        m_scene->addItem(bg);
    };

    createLabel("📉 AVANT OPTIMISATION", QColor(255, 77, 77), QPointF(20, -70));
    createLabel("🚀 APRÈS OPTIMISATION", QColor(0, 212, 255), QPointF(xOffset + 20, -70));

    // Connect them with an arrow
    QGraphicsLineItem* link = new QGraphicsLineItem(m_panels.first()->toRect().width() + 20, m_panels.first()->toRect().height()/2,
                                                    xOffset - 20, m_panels.first()->toRect().height()/2);
    QPen linkPen(QColor(0, 212, 255, 100), 4, Qt::DashLine);
    link->setPen(linkPen);
    m_scene->addItem(link);

    QGraphicsTextItem* arrowHead = new QGraphicsTextItem("▶");
    arrowHead->setDefaultTextColor(QColor(0, 212, 255));
    arrowHead->setFont(QFont("Segoe UI", 24, QFont::Bold));
    arrowHead->setPos(xOffset - 40, m_panels.first()->toRect().height()/2 - 24);
    m_scene->addItem(arrowHead);

    // Fit view to show both layouts
    m_graphicsView->fitInView(m_scene->itemsBoundingRect().adjusted(-50, -100, 50, 50), Qt::KeepAspectRatio);
}

// AI Features Implementation
void NestingWidget::enableRealTimeOptimization(bool enabled)
{
    m_realTimeEnabled = enabled;
    if (enabled && !m_pieces.isEmpty()) {
        // Trigger optimization when pieces change
        startOptimization();
    }
}

bool NestingWidget::hasLearnedLayout(const QVector<PieceRect*>& pieces) const
{
    return m_engine->hasSimilarLayout(pieces);
}

QVector<PanelLayout> NestingWidget::loadBestLayouts(const QVector<PieceRect*>& pieces) const
{
    return m_engine->loadBestLayouts(pieces);
}

void NestingWidget::storeLayoutResult(const QVector<PanelLayout>& layouts)
{
    m_engine->storeLayoutResult(layouts);
}

QVector<QRectF> NestingWidget::trackWasteSpaces(const QRectF& panelRect, const QVector<QRectF>& usedRects) const
{
    return m_engine->trackWasteSpaces(panelRect, usedRects);
}

void NestingWidget::applyLayout(const QVector<PanelLayout>& layouts)
{
    onLayoutCompleted(layouts);
}

void NestingWidget::showExplainableAI(const QStringList& decisions)
{
    m_explainableAIText->clear();
    for (const QString& decision : decisions) {
        m_explainableAIText->append(decision);
    }
}

void NestingWidget::updateHeatmap()
{
    if (!m_heatmapItem) {
        m_heatmapItem = new QGraphicsPixmapItem();
        m_scene->addItem(m_heatmapItem);
        m_heatmapItem->setZValue(-1); // Behind pieces
    }

    QRectF sceneRect = m_scene->sceneRect();
    if (sceneRect.isEmpty()) return;

    QImage heatmap(sceneRect.size().toSize(), QImage::Format_ARGB32);
    heatmap.fill(QColor(255, 255, 255, 0)); // Transparent

    QPainter painter(&heatmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // Create efficiency heatmap based on piece density
    for (const PanelLayout& layout : m_currentLayouts) {
        for (const PlacedPiece& piece : layout.placedPieces) {
            // Color based on efficiency (green = good, red = poor)
            int efficiency = static_cast<int>(layout.efficiency);
            QColor color = QColor::fromHsv(120 * efficiency / 100, 100, 200, 100); // Green to red

            painter.fillRect(piece.bounds, color);
        }
    }

    m_heatmapItem->setPixmap(QPixmap::fromImage(heatmap));
    m_heatmapItem->setPos(sceneRect.topLeft());
}

