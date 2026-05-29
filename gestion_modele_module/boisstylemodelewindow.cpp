#include "boisstylemodelewindow.h"

#include <QAbstractItemView>
#include <QComboBox>
#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QtGlobal>
#include <QVBoxLayout>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <algorithm>
#include <numeric>

BoisStyleModeleWindow::BoisStyleModeleWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    seedData();
    refreshFiltersAndSort();
}

void BoisStyleModeleWindow::setupUi()
{
    resize(1300, 780);
    setWindowTitle("Gestion des modeles - Style gestion du bois");

    m_central = new QWidget(this);
    setCentralWidget(m_central);
    m_central->setStyleSheet(
        "QLabel{color:rgba(255,255,255,0.92);font-weight:500;font-family:'Segoe UI',sans-serif;}"
        "QWidget{background-color:#11112a;color:rgba(255,255,255,0.9);font-family:'Segoe UI',sans-serif;font-size:14px;}"
        "QLineEdit,QTextEdit{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-radius:8px;padding:8px 12px;color:white;}"
        "QLineEdit:focus,QTextEdit:focus{border:1px solid #6366f1;background-color:rgba(99,102,241,0.08);}"
        "QComboBox{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-radius:8px;padding:7px 10px;color:white;}"
        "QComboBox:hover{border:1px solid #6366f1;}"
        "QComboBox::drop-down{border:none;}"
        "QPushButton{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1);color:white;border-radius:10px;padding:10px 20px;font-weight:600;border:none;}"
        "QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #6366f1,stop:1 #818cf8);}"
        "QPushButton:pressed{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #3730a3,stop:1 #4f46e5);}"
        "QTableWidget{border:none;font-size:14px;background-color:#13132b;gridline-color:rgba(99,102,241,0.08);}"
        "QTableWidget::item:selected{background-color:rgba(99,102,241,0.25);color:#FFFFFF;}"
        "QHeaderView{font-size:14px;background-color:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #3730a3,stop:0.5 #4f46e5,stop:1 #6366f1);color:#FFFFFF;border:none;}"
        "QHeaderView::section:horizontal{padding:6px;border:none;background-color:transparent;}");

    auto *root = new QVBoxLayout(m_central);
    root->setContentsMargins(20, 20, 20, 20);
    root->setSpacing(10);

    auto *title = new QLabel("Gestion des modeles", m_central);
    title->setStyleSheet("color:rgba(255,255,255,0.95);font-size:20px;font-weight:700;background:transparent;");
    root->addWidget(title);

    m_tabWidget = new QTabWidget(m_central);
    m_tabWidget->setStyleSheet(
        "QTabWidget::pane{border:1px solid rgba(99,102,241,0.2);border-radius:10px;background-color:rgba(18,18,42,0.9);top:-1px;}"
        "QTabBar::tab{background:rgba(30,30,60,0.6);color:#ffffff;border:1px solid rgba(99,102,241,0.15);border-bottom:none;border-top-left-radius:12px;border-top-right-radius:12px;padding:6px 15px;margin-right:4px;font-weight:600;}"
        "QTabBar::tab:selected{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1);color:white;}"
        "QTabBar::tab:hover:!selected{background:rgba(99,102,241,0.15);color:white;}");
    root->addWidget(m_tabWidget, 1);

    QWidget *listTab = new QWidget(m_tabWidget);
    auto *listLayout = new QVBoxLayout(listTab);
    listLayout->setContentsMargins(16, 16, 16, 16);
    listLayout->setSpacing(10);

    auto *topRow = new QHBoxLayout();
    m_searchEdit = new QLineEdit(listTab);
    m_searchEdit->setPlaceholderText("Rechercher un modele...");
    topRow->addWidget(m_searchEdit, 2);

    auto *filterLabel = new QLabel("Statut :", listTab);
    topRow->addWidget(filterLabel);

    m_filterCombo = new QComboBox(listTab);
    m_filterCombo->addItems({"Tous", "en fabrication", "en stock", "livre"});
    topRow->addWidget(m_filterCombo, 1);
    listLayout->addLayout(topRow);

    auto *sortRow = new QHBoxLayout();
    auto *sortLabel = new QLabel("Trier par :", listTab);
    sortLabel->setStyleSheet("color:#94a3b8;font-weight:bold;font-size:11px;");
    sortRow->addWidget(sortLabel);

    m_sortFieldCombo = new QComboBox(listTab);
    m_sortFieldCombo->addItems({"ID", "Categorie", "Prix", "Ventes"});
    sortRow->addWidget(m_sortFieldCombo);

    m_sortOrderCombo = new QComboBox(listTab);
    m_sortOrderCombo->addItems({"Croissant", "Decroissant"});
    sortRow->addWidget(m_sortOrderCombo);
    sortRow->addStretch();
    listLayout->addLayout(sortRow);

    m_table = new QTableWidget(listTab);
    m_table->setColumnCount(10);
    m_table->setHorizontalHeaderLabels({
        "ID", "Categorie", "Dimensions", "Type", "Prix (DT)",
        "Description", "Statut", "Disponibilite", "Ventes", "Action"
    });
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_table->setAlternatingRowColors(true);
    listLayout->addWidget(m_table, 1);

    auto *paginationRow = new QHBoxLayout();
    paginationRow->addStretch();
    m_prevButton = new QPushButton("‹", listTab);
    m_prevButton->setFixedSize(28, 28);
    m_prevButton->setStyleSheet(
        "QPushButton{background:rgba(99,102,241,0.15);color:#818cf8;border:1px solid rgba(99,102,241,0.3);border-radius:14px;font-size:11px;font-weight:bold;padding:0;}"
        "QPushButton:hover{background:rgba(99,102,241,0.35);color:white;}"
        "QPushButton:disabled{color:rgba(255,255,255,0.2);border-color:rgba(255,255,255,0.05);}");
    paginationRow->addWidget(m_prevButton);

    m_pageNumbersContainer = new QWidget(listTab);
    auto *numbersLayout = new QHBoxLayout(m_pageNumbersContainer);
    numbersLayout->setContentsMargins(6, 0, 6, 0);
    numbersLayout->setSpacing(6);
    paginationRow->addWidget(m_pageNumbersContainer);

    m_nextButton = new QPushButton("›", listTab);
    m_nextButton->setFixedSize(28, 28);
    m_nextButton->setStyleSheet(m_prevButton->styleSheet());
    paginationRow->addWidget(m_nextButton);
    paginationRow->addStretch();
    listLayout->addLayout(paginationRow);

    auto *actionsRow = new QHBoxLayout();
    actionsRow->addStretch();
    m_addButton = new QPushButton("Ajouter", listTab);
    m_addButton->setStyleSheet("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #2563eb,stop:1 #3b82f6);padding:10px 30px;");
    actionsRow->addWidget(m_addButton);
    m_editButton = new QPushButton("Modifier", listTab);
    actionsRow->addWidget(m_editButton);
    m_deleteButton = new QPushButton("Supprimer", listTab);
    m_deleteButton->setStyleSheet("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #dc2626,stop:1 #ef4444);padding:10px 30px;");
    actionsRow->addWidget(m_deleteButton);
    m_exportButton = new QPushButton("Export PDF", listTab);
    m_exportButton->setStyleSheet("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #b45309,stop:1 #f59e0b);padding:10px 30px;");
    actionsRow->addWidget(m_exportButton);
    listLayout->addLayout(actionsRow);

    m_tabWidget->addTab(listTab, "Liste des modeles");

    QWidget *statsTab = new QWidget(m_tabWidget);
    auto *statsLayout = new QVBoxLayout(statsTab);
    statsLayout->setContentsMargins(16, 16, 16, 16);
    statsLayout->setSpacing(10);
    auto *statsTitle = new QLabel("Statistiques modernes", statsTab);
    statsTitle->setStyleSheet("font-size:18px;font-weight:700;color:#c7d2fe;background:transparent;");
    statsLayout->addWidget(statsTitle);

    m_statsChartContainer = new QWidget(statsTab);
    m_statsChartContainer->setMinimumHeight(260);
    m_statsChartContainer->setStyleSheet("background-color:rgba(255,255,255,0.03);border:1px solid rgba(99,102,241,0.2);border-radius:12px;");
    statsLayout->addWidget(m_statsChartContainer);

    auto *cardsLayout = new QHBoxLayout();
    m_statsTotalLabel = new QLabel("Total modeles : --", statsTab);
    m_statsAvailableLabel = new QLabel("Disponibles : --", statsTab);
    m_statsAvgPriceLabel = new QLabel("Prix moyen : --", statsTab);
    m_statsBestSellerLabel = new QLabel("Top ventes : --", statsTab);
    const QString cardStyle = "background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.25);border-radius:10px;padding:10px 12px;color:#e2e8f0;font-weight:600;";
    m_statsTotalLabel->setStyleSheet(cardStyle);
    m_statsAvailableLabel->setStyleSheet(cardStyle);
    m_statsAvgPriceLabel->setStyleSheet(cardStyle);
    m_statsBestSellerLabel->setStyleSheet(cardStyle);
    cardsLayout->addWidget(m_statsTotalLabel);
    cardsLayout->addWidget(m_statsAvailableLabel);
    cardsLayout->addWidget(m_statsAvgPriceLabel);
    cardsLayout->addWidget(m_statsBestSellerLabel);
    statsLayout->addLayout(cardsLayout);

    auto *statsBtnRow = new QHBoxLayout();
    statsBtnRow->addStretch();
    auto *statsRefreshBtn = new QPushButton("Rafraichir", statsTab);
    statsBtnRow->addWidget(statsRefreshBtn);
    statsLayout->addLayout(statsBtnRow);
    m_tabWidget->addTab(statsTab, "Statistiques");

    QWidget *analysisTab = new QWidget(m_tabWidget);
    auto *analysisLayout = new QVBoxLayout(analysisTab);
    analysisLayout->setContentsMargins(16, 16, 16, 16);
    analysisLayout->setSpacing(10);
    auto *analysisTitle = new QLabel("Analyse d'image (estimation)", analysisTab);
    analysisTitle->setStyleSheet("font-size:18px;font-weight:700;color:#c7d2fe;background:transparent;");
    analysisLayout->addWidget(analysisTitle);
    auto *imageRow = new QHBoxLayout();
    m_imagePathEdit = new QLineEdit(analysisTab);
    m_imagePathEdit->setReadOnly(true);
    m_imagePathEdit->setPlaceholderText("Aucune image selectionnee...");
    imageRow->addWidget(m_imagePathEdit, 1);
    auto *chooseImageBtn = new QPushButton("Choisir image", analysisTab);
    imageRow->addWidget(chooseImageBtn);
    analysisLayout->addLayout(imageRow);
    m_imageResultLabel = new QLabel("Resultat : --", analysisTab);
    m_imageResultLabel->setWordWrap(true);
    m_imageResultLabel->setMinimumHeight(140);
    m_imageResultLabel->setStyleSheet("background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.25);border-radius:12px;padding:12px;");
    analysisLayout->addWidget(m_imageResultLabel);
    analysisLayout->addStretch();
    m_tabWidget->addTab(analysisTab, "Analyse d'image");

    QWidget *advisorTab = new QWidget(m_tabWidget);
    auto *advisorLayout = new QVBoxLayout(advisorTab);
    advisorLayout->setContentsMargins(16, 16, 16, 16);
    advisorLayout->setSpacing(10);
    auto *advisorTitle = new QLabel("Conseiller modele", advisorTab);
    advisorTitle->setStyleSheet("font-size:18px;font-weight:700;color:#c7d2fe;background:transparent;");
    advisorLayout->addWidget(advisorTitle);
    auto *budgetRow = new QHBoxLayout();
    auto *budgetLabel = new QLabel("Budget client (DT):", advisorTab);
    budgetRow->addWidget(budgetLabel);
    m_budgetSpin = new QDoubleSpinBox(advisorTab);
    m_budgetSpin->setRange(100.0, 50000.0);
    m_budgetSpin->setDecimals(2);
    m_budgetSpin->setValue(1200.0);
    budgetRow->addWidget(m_budgetSpin);
    budgetRow->addStretch();
    advisorLayout->addLayout(budgetRow);
    auto *advisorBtnRow = new QHBoxLayout();
    advisorBtnRow->addStretch();
    auto *advisorBtn = new QPushButton("Generer recommandation", advisorTab);
    advisorBtnRow->addWidget(advisorBtn);
    advisorLayout->addLayout(advisorBtnRow);
    m_advisorResultLabel = new QLabel("Suggestion : --", advisorTab);
    m_advisorResultLabel->setWordWrap(true);
    m_advisorResultLabel->setMinimumHeight(160);
    m_advisorResultLabel->setStyleSheet("background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.25);border-radius:12px;padding:12px;");
    advisorLayout->addWidget(m_advisorResultLabel);
    advisorLayout->addStretch();
    m_tabWidget->addTab(advisorTab, "Conseiller");

    connect(statsRefreshBtn, &QPushButton::clicked, this, &BoisStyleModeleWindow::onStatsRefresh);
    connect(chooseImageBtn, &QPushButton::clicked, this, &BoisStyleModeleWindow::onChooseImage);
    connect(advisorBtn, &QPushButton::clicked, this, &BoisStyleModeleWindow::onAdvisorGenerate);

    connect(m_addButton, &QPushButton::clicked, this, &BoisStyleModeleWindow::onAddClicked);
    connect(m_editButton, &QPushButton::clicked, this, &BoisStyleModeleWindow::onEditClicked);
    connect(m_deleteButton, &QPushButton::clicked, this, &BoisStyleModeleWindow::onDeleteClicked);
    connect(m_exportButton, &QPushButton::clicked, this, &BoisStyleModeleWindow::onExportPdf);
    connect(m_searchEdit, &QLineEdit::textChanged, this, &BoisStyleModeleWindow::onSearchChanged);
    connect(m_filterCombo, &QComboBox::currentTextChanged, this, &BoisStyleModeleWindow::onFilterChanged);
    connect(m_sortFieldCombo, &QComboBox::currentTextChanged, this, &BoisStyleModeleWindow::onSortChanged);
    connect(m_sortOrderCombo, &QComboBox::currentTextChanged, this, &BoisStyleModeleWindow::onSortChanged);
    connect(m_prevButton, &QPushButton::clicked, this, &BoisStyleModeleWindow::onPrevPage);
    connect(m_nextButton, &QPushButton::clicked, this, &BoisStyleModeleWindow::onNextPage);

    setupStatsChart();
}

void BoisStyleModeleWindow::seedData()
{
    m_allModeles = {
        {"001", "Salon", "200x90x80", "Canape", 950.0, "Canape en bois massif", "en stock", "Disponible", 18},
        {"002", "Chambre", "160x200x110", "Lit", 1200.0, "Lit double avec tete integree", "en fabrication", "Indisponible", 12},
        {"003", "Bureau", "140x70x75", "Bureau", 560.0, "Bureau compact", "en stock", "Disponible", 25},
        {"004", "Cuisine", "90x60x90", "Ilot", 1300.0, "Ilot central", "livre", "Indisponible", 31},
        {"005", "Exterieur", "180x80x90", "Table", 870.0, "Table en teck", "en stock", "Disponible", 9},
        {"006", "Salon", "220x95x82", "Canape d'angle", 1800.0, "Canape 5 places", "en fabrication", "Indisponible", 5},
        {"007", "Bureau", "120x60x75", "Bibliotheque", 490.0, "Bibliotheque murale", "en stock", "Disponible", 14},
        {"008", "Chambre", "60x45x50", "Table de nuit", 220.0, "2 tiroirs", "en stock", "Disponible", 40},
        {"009", "Cuisine", "250x60x90", "Meuble bas", 760.0, "Module cuisine", "livre", "Indisponible", 20},
        {"010", "Salle a manger", "200x100x76", "Table", 1450.0, "Table 8 places", "en fabrication", "Indisponible", 7}
    };
}

void BoisStyleModeleWindow::onAddClicked()
{
    Modele modele;
    modele.id = QString("%1").arg(static_cast<int>(m_allModeles.size()) + 1, 3, 10, QChar('0'));
    if (!showEditorDialog(modele, false)) {
        return;
    }
    m_allModeles.push_back(modele);
    refreshFiltersAndSort();
}

void BoisStyleModeleWindow::onEditClicked()
{
    const int idx = selectedGlobalIndex();
    if (idx < 0 || idx >= static_cast<int>(m_allModeles.size())) {
        QMessageBox::information(this, "Selection requise", "Veuillez selectionner une ligne a modifier.");
        return;
    }
    Modele edited = m_allModeles[idx];
    if (!showEditorDialog(edited, true)) {
        return;
    }
    m_allModeles[idx] = edited;
    refreshFiltersAndSort();
}

void BoisStyleModeleWindow::onDeleteClicked()
{
    const int idx = selectedGlobalIndex();
    if (idx < 0 || idx >= static_cast<int>(m_allModeles.size())) {
        QMessageBox::information(this, "Selection requise", "Veuillez selectionner une ligne a supprimer.");
        return;
    }
    if (QMessageBox::question(this, "Confirmation", "Supprimer le modele selectionne ?") != QMessageBox::Yes) {
        return;
    }
    m_allModeles.removeAt(idx);
    refreshFiltersAndSort();
}

void BoisStyleModeleWindow::onSearchChanged(const QString &)
{
    refreshFiltersAndSort();
}

void BoisStyleModeleWindow::onFilterChanged(const QString &)
{
    refreshFiltersAndSort();
}

void BoisStyleModeleWindow::onSortChanged()
{
    refreshFiltersAndSort();
}

void BoisStyleModeleWindow::onPrevPage()
{
    if (m_currentPage > 0) {
        --m_currentPage;
        renderCurrentPage();
    }
}

void BoisStyleModeleWindow::onNextPage()
{
    const int filteredCount = static_cast<int>(m_filteredIndices.size());
    const int totalPages = (filteredCount + m_pageSize - 1) / m_pageSize;
    if (m_currentPage + 1 < totalPages) {
        ++m_currentPage;
        renderCurrentPage();
    }
}

void BoisStyleModeleWindow::refreshFiltersAndSort()
{
    const QString search = m_searchEdit->text().trimmed().toLower();
    const QString status = m_filterCombo->currentText();

    m_filteredIndices.clear();
    const int allCount = static_cast<int>(m_allModeles.size());
    for (int i = 0; i < allCount; ++i) {
        const Modele &m = m_allModeles[i];
        if (status != "Tous" && m.statut != status) {
            continue;
        }

        if (!search.isEmpty()) {
            const QString haystack = (m.id + " " + m.categorie + " " + m.dimensions + " " + m.type + " "
                                      + QString::number(m.prix) + " " + m.description + " " + m.statut + " "
                                      + m.disponibilite + " " + QString::number(m.ventes)).toLower();
            if (!haystack.contains(search)) {
                continue;
            }
        }
        m_filteredIndices.push_back(i);
    }

    const QString field = m_sortFieldCombo->currentText();
    const bool asc = (m_sortOrderCombo->currentText() == "Croissant");
    std::sort(m_filteredIndices.begin(), m_filteredIndices.end(), [&](int a, int b) {
        const Modele &ma = m_allModeles[a];
        const Modele &mb = m_allModeles[b];
        if (field == "ID") return asc ? (ma.id < mb.id) : (ma.id > mb.id);
        if (field == "Categorie") return asc ? (ma.categorie < mb.categorie) : (ma.categorie > mb.categorie);
        if (field == "Prix") return asc ? (ma.prix < mb.prix) : (ma.prix > mb.prix);
        return asc ? (ma.ventes < mb.ventes) : (ma.ventes > mb.ventes);
    });

    const int filteredCount = static_cast<int>(m_filteredIndices.size());
    const int totalPages = qMax(1, (filteredCount + m_pageSize - 1) / m_pageSize);
    if (m_currentPage >= totalPages) {
        m_currentPage = totalPages - 1;
    }
    if (m_currentPage < 0) {
        m_currentPage = 0;
    }

    renderCurrentPage();
    refreshStatsCards();
}

void BoisStyleModeleWindow::renderCurrentPage()
{
    m_table->setRowCount(0);

    const int start = m_currentPage * m_pageSize;
    const int filteredCount = static_cast<int>(m_filteredIndices.size());
    const int end = qMin(start + m_pageSize, filteredCount);

    for (int i = start; i < end; ++i) {
        const Modele &m = m_allModeles[m_filteredIndices[i]];
        const int row = m_table->rowCount();
        m_table->insertRow(row);

        m_table->setItem(row, 0, new QTableWidgetItem(m.id));
        m_table->setItem(row, 1, new QTableWidgetItem(m.categorie));
        m_table->setItem(row, 2, new QTableWidgetItem(m.dimensions));
        m_table->setItem(row, 3, new QTableWidgetItem(m.type));
        m_table->setItem(row, 4, new QTableWidgetItem(QString::number(m.prix, 'f', 2)));
        m_table->setItem(row, 5, new QTableWidgetItem(m.description));
        m_table->setItem(row, 6, new QTableWidgetItem(m.statut));
        m_table->setItem(row, 7, new QTableWidgetItem(m.disponibilite));
        m_table->setItem(row, 8, new QTableWidgetItem(QString::number(m.ventes)));

        auto *actions = new QWidget(m_table);
        auto *layout = new QHBoxLayout(actions);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(6);

        auto *btnEdit = new QPushButton("Modifier", actions);
        btnEdit->setFixedHeight(28);
        btnEdit->setProperty("globalIndex", m_filteredIndices[i]);
        btnEdit->setStyleSheet("QPushButton{background:#2563eb;color:white;border-radius:6px;padding:2px 10px;}QPushButton:hover{background:#3b82f6;}");
        auto *btnDel = new QPushButton("Supprimer", actions);
        btnDel->setFixedHeight(28);
        btnDel->setProperty("globalIndex", m_filteredIndices[i]);
        btnDel->setStyleSheet("QPushButton{background:#dc2626;color:white;border-radius:6px;padding:2px 10px;}QPushButton:hover{background:#ef4444;}");

        layout->addWidget(btnEdit);
        layout->addWidget(btnDel);
        layout->setAlignment(Qt::AlignCenter);
        m_table->setCellWidget(row, 9, actions);

        connect(btnEdit, &QPushButton::clicked, this, [this, btnEdit]() {
            const int idx = btnEdit->property("globalIndex").toInt();
            if (idx < 0 || idx >= static_cast<int>(m_allModeles.size())) return;
            Modele edited = m_allModeles[idx];
            if (showEditorDialog(edited, true)) {
                m_allModeles[idx] = edited;
                refreshFiltersAndSort();
            }
        });
        connect(btnDel, &QPushButton::clicked, this, [this, btnDel]() {
            const int idx = btnDel->property("globalIndex").toInt();
            if (idx < 0 || idx >= static_cast<int>(m_allModeles.size())) return;
            if (QMessageBox::question(this, "Confirmation", "Supprimer le modele selectionne ?") == QMessageBox::Yes) {
                m_allModeles.removeAt(idx);
                refreshFiltersAndSort();
            }
        });
    }

    m_prevButton->setEnabled(m_currentPage > 0);
    const int totalPages = qMax(1, (filteredCount + m_pageSize - 1) / m_pageSize);
    m_nextButton->setEnabled(m_currentPage + 1 < totalPages);
    rebuildPageButtons();
}

void BoisStyleModeleWindow::rebuildPageButtons()
{
    auto *layout = qobject_cast<QHBoxLayout *>(m_pageNumbersContainer->layout());
    if (!layout) {
        layout = new QHBoxLayout(m_pageNumbersContainer);
        layout->setContentsMargins(6, 0, 6, 0);
        layout->setSpacing(6);
    }
    while (QLayoutItem *item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    const int filteredCount = static_cast<int>(m_filteredIndices.size());
    const int totalPages = qMax(1, (filteredCount + m_pageSize - 1) / m_pageSize);
    const int firstPage = qMax(0, m_currentPage - 2);
    const int lastPage = qMin(totalPages - 1, firstPage + 4);

    for (int p = firstPage; p <= lastPage; ++p) {
        auto *pageButton = new QPushButton(QString::number(p + 1), m_pageNumbersContainer);
        pageButton->setFixedSize(30, 28);
        if (p == m_currentPage) {
            pageButton->setStyleSheet("QPushButton{background:#6366f1;color:white;border:1px solid #818cf8;border-radius:14px;font-weight:bold;padding:0;}");
        } else {
            pageButton->setStyleSheet(
                "QPushButton{background:rgba(99,102,241,0.15);color:#818cf8;border:1px solid rgba(99,102,241,0.3);border-radius:14px;font-weight:bold;padding:0;}"
                "QPushButton:hover{background:rgba(99,102,241,0.35);color:white;}");
        }
        connect(pageButton, &QPushButton::clicked, this, [this, p]() {
            m_currentPage = p;
            renderCurrentPage();
        });
        layout->addWidget(pageButton);
    }
}

int BoisStyleModeleWindow::selectedGlobalIndex() const
{
    const int row = m_table->currentRow();
    if (row < 0) return -1;

    const int globalRow = m_currentPage * m_pageSize + row;
    if (globalRow < 0 || globalRow >= static_cast<int>(m_filteredIndices.size())) return -1;

    return m_filteredIndices[globalRow];
}

bool BoisStyleModeleWindow::showEditorDialog(Modele &modele, bool editMode)
{
    QDialog dialog(this);
    dialog.setWindowTitle(editMode ? "Modifier le modele" : "Ajouter un modele");
    dialog.setStyleSheet("QDialog{background-color:#11112a;color:white;} QLabel{color:white;}");

    QFormLayout form(&dialog);
    QLineEdit idEdit(modele.id);
    idEdit.setReadOnly(editMode);
    QLineEdit catEdit(modele.categorie);
    QLineEdit dimEdit(modele.dimensions);
    QLineEdit typeEdit(modele.type);
    QDoubleSpinBox prixEdit;
    prixEdit.setRange(0.0, 1000000.0);
    prixEdit.setDecimals(2);
    prixEdit.setValue(modele.prix);
    QTextEdit descEdit(modele.description);
    descEdit.setFixedHeight(70);
    QComboBox statutEdit;
    statutEdit.addItems({"en fabrication", "en stock", "livre"});
    statutEdit.setCurrentText(modele.statut.isEmpty() ? "en fabrication" : modele.statut);
    QComboBox dispoEdit;
    dispoEdit.addItems({"Disponible", "Indisponible"});
    dispoEdit.setCurrentText(modele.disponibilite.isEmpty() ? "Disponible" : modele.disponibilite);
    QSpinBox ventesEdit;
    ventesEdit.setRange(0, 100000);
    ventesEdit.setValue(modele.ventes);

    form.addRow("ID", &idEdit);
    form.addRow("Categorie", &catEdit);
    form.addRow("Dimensions", &dimEdit);
    form.addRow("Type", &typeEdit);
    form.addRow("Prix", &prixEdit);
    form.addRow("Description", &descEdit);
    form.addRow("Statut", &statutEdit);
    form.addRow("Disponibilite", &dispoEdit);
    form.addRow("Ventes", &ventesEdit);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(&buttons);
    QObject::connect(&buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
        if (idEdit.text().trimmed().isEmpty() || catEdit.text().trimmed().isEmpty()
            || dimEdit.text().trimmed().isEmpty() || typeEdit.text().trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Champs manquants", "Veuillez remplir les champs obligatoires.");
            return;
        }
        dialog.accept();
    });
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    modele.id = idEdit.text().trimmed();
    modele.categorie = catEdit.text().trimmed();
    modele.dimensions = dimEdit.text().trimmed();
    modele.type = typeEdit.text().trimmed();
    modele.prix = prixEdit.value();
    modele.description = descEdit.toPlainText().trimmed();
    modele.statut = statutEdit.currentText();
    modele.disponibilite = dispoEdit.currentText();
    modele.ventes = ventesEdit.value();
    return true;
}

void BoisStyleModeleWindow::setupStatsChart()
{
    if (!m_statsChartContainer) {
        return;
    }
    auto *chart = new QChart();
    chart->setTitle("Tendance des ventes");
    chart->setBackgroundVisible(false);
    chart->setPlotAreaBackgroundVisible(true);
    chart->setPlotAreaBackgroundBrush(QBrush(QColor(0x17, 0x23, 0x40)));
    chart->legend()->hide();

    auto *series = new QLineSeries(chart);
    series->setColor(QColor(0x63, 0x66, 0xf1));
    series->setPointsVisible(true);
    chart->addSeries(series);

    auto *axisX = new QCategoryAxis(chart);
    const QStringList months = {"Jan", "Fev", "Mar", "Avr", "Mai", "Juin", "Juil", "Aou", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < months.size(); ++i) {
        axisX->append(months[i], i + 1);
    }
    axisX->setRange(1, 12);
    axisX->setLabelsColor(QColor(0xf8, 0xfa, 0xfc));
    axisX->setTitleText("Mois");
    axisX->setTitleBrush(QBrush(QColor(0xf8, 0xfa, 0xfc)));

    auto *axisY = new QValueAxis(chart);
    axisY->setRange(0, 100);
    axisY->setLabelFormat("%d");
    axisY->setLabelsColor(QColor(0xf8, 0xfa, 0xfc));
    axisY->setTitleText("Ventes");
    axisY->setTitleBrush(QBrush(QColor(0xf8, 0xfa, 0xfc)));

    chart->setAxisX(axisX, series);
    chart->setAxisY(axisY, series);

    auto *layout = new QVBoxLayout(m_statsChartContainer);
    layout->setContentsMargins(8, 8, 8, 8);

    m_statsChartView = new QChartView(chart, m_statsChartContainer);
    m_statsChartView->setRenderHint(QPainter::Antialiasing);
    m_statsChartView->setStyleSheet("background:transparent;");
    layout->addWidget(m_statsChartView);
}

void BoisStyleModeleWindow::refreshStatsCards()
{
    if (!m_statsTotalLabel || !m_statsAvailableLabel || !m_statsAvgPriceLabel || !m_statsBestSellerLabel) {
        return;
    }

    const int total = static_cast<int>(m_allModeles.size());
    int available = 0;
    double totalPrice = 0.0;
    int bestSales = -1;
    QString bestName = "--";

    QVector<int> salesByMonth(12, 0);
    for (int i = 0; i < total; ++i) {
        const Modele &m = m_allModeles[i];
        if (m.disponibilite == "Disponible") {
            ++available;
        }
        totalPrice += m.prix;
        if (m.ventes > bestSales) {
            bestSales = m.ventes;
            bestName = m.type;
        }
        salesByMonth[i % 12] += m.ventes;
    }

    m_statsTotalLabel->setText(QString("Total modeles : %1").arg(total));
    m_statsAvailableLabel->setText(QString("Disponibles : %1").arg(available));
    m_statsAvgPriceLabel->setText(total > 0
                                      ? QString("Prix moyen : %1 DT").arg(totalPrice / total, 0, 'f', 2)
                                      : QString("Prix moyen : --"));
    m_statsBestSellerLabel->setText(QString("Top ventes : %1 (%2)").arg(bestName).arg(qMax(0, bestSales)));

    if (!m_statsChartView || !m_statsChartView->chart() || m_statsChartView->chart()->series().isEmpty()) {
        return;
    }
    auto *series = qobject_cast<QLineSeries *>(m_statsChartView->chart()->series().first());
    auto *axisY = qobject_cast<QValueAxis *>(m_statsChartView->chart()->axisY());
    if (!series || !axisY) {
        return;
    }
    series->clear();
    int maxValue = 10;
    for (int i = 0; i < salesByMonth.size(); ++i) {
        series->append(i + 1, salesByMonth[i]);
        maxValue = qMax(maxValue, salesByMonth[i] + 5);
    }
    axisY->setRange(0, maxValue);
}

void BoisStyleModeleWindow::onStatsRefresh()
{
    refreshStatsCards();
}

void BoisStyleModeleWindow::onChooseImage()
{
    const QString imagePath = QFileDialog::getOpenFileName(
        this,
        "Choisir image",
        QString(),
        "Images (*.png *.jpg *.jpeg *.bmp)");
    if (imagePath.isEmpty()) {
        return;
    }

    if (m_imagePathEdit) {
        m_imagePathEdit->setText(imagePath);
    }

    QFileInfo info(imagePath);
    const double sizeKb = info.size() / 1024.0;
    const double estimatedPrice = 350.0 + (sizeKb * 0.45);
    const int estimatedDays = qMax(2, static_cast<int>(sizeKb / 180.0) + 2);

    if (m_imageResultLabel) {
        m_imageResultLabel->setText(
            QString("Resultat :\nImage : %1\nTaille : %2 KB\nEstimation prix : %3 DT\nTemps estime : %4 jours")
                .arg(info.fileName())
                .arg(sizeKb, 0, 'f', 1)
                .arg(estimatedPrice, 0, 'f', 2)
                .arg(estimatedDays));
    }
}

void BoisStyleModeleWindow::onAdvisorGenerate()
{
    if (!m_budgetSpin || !m_advisorResultLabel) {
        return;
    }
    const double budget = m_budgetSpin->value();
    QString suggestionType;
    QString suggestionCategorie;
    QString suggestionDimensions;

    if (budget < 600.0) {
        suggestionCategorie = "Entree";
        suggestionType = "Table de nuit";
        suggestionDimensions = "60x45x50";
    } else if (budget < 1300.0) {
        suggestionCategorie = "Salon";
        suggestionType = "Table basse";
        suggestionDimensions = "110x60x45";
    } else {
        suggestionCategorie = "Salle a manger";
        suggestionType = "Table";
        suggestionDimensions = "200x100x76";
    }

    m_advisorResultLabel->setText(
        QString("Suggestion :\nCategorie : %1\nType : %2\nDimensions : %3\nBudget : %4 DT\n\nTu peux cliquer sur 'Ajouter' dans l'onglet liste pour creer ce modele.")
            .arg(suggestionCategorie)
            .arg(suggestionType)
            .arg(suggestionDimensions)
            .arg(budget, 0, 'f', 2));
}

void BoisStyleModeleWindow::onExportPdf()
{
    const QString path = QFileDialog::getSaveFileName(
        this,
        "Exporter PDF",
        "modeles_style_bois_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".pdf",
        "PDF (*.pdf)");
    if (path.isEmpty()) {
        return;
    }

    QPdfWriter writer(path);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageMargins(QMarginsF(12, 12, 12, 12));
    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);

    int y = 60;
    painter.setFont(QFont("Segoe UI", 16, QFont::Bold));
    painter.drawText(40, y, "Liste des modeles");
    y += 30;
    painter.setFont(QFont("Segoe UI", 9));
    painter.drawText(40, y, "Genere le " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));
    y += 30;

    for (const Modele &m : m_allModeles) {
        const QString line = QString("%1 | %2 | %3 DT | %4 | %5")
                                 .arg(m.id, m.categorie)
                                 .arg(m.prix, 0, 'f', 2)
                                 .arg(m.statut, m.disponibilite);
        painter.drawText(40, y, line);
        y += 18;
        if (y > writer.height() - 60) {
            writer.newPage();
            y = 60;
        }
    }
    painter.end();
    QMessageBox::information(this, "Export PDF", "Export termine.");
}
