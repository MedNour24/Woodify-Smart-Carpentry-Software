#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractItemView>
#include <algorithm>
#include <QComboBox>
#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QPushButton>
#include <QSet>
#include <QSpinBox>
#include <QTableWidgetItem>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QRandomGenerator>
#include <QValueAxis>
#include <QCategoryAxis>


namespace {
const QString kStatusFabrication = "en fabrication";
const QString kStatusStock = "en stock";
const QString kStatusLivre = "livré";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    configureTable();
    seedData();
    rebuildFilters();
    applyFilters();

    ui->lineEdit->setPlaceholderText(
        "Rechercher par catégorie, prix, statut ou disponibilité...");

    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::applyFilters);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &MainWindow::applyFilters);
    connect(ui->comboBox_2, &QComboBox::currentTextChanged, this, &MainWindow::applyFilters);

    if (ui->btnBudgetGenerate) {
        connect(ui->btnBudgetGenerate, &QPushButton::clicked, this, &MainWindow::onBudgetGenerate);
    }
    if (ui->btnEstimationChoose) {
        connect(ui->btnEstimationChoose, &QPushButton::clicked, this, &MainWindow::onEstimateFromImage);
    }
    if (ui->btnStatsRefresh) {
        connect(ui->btnStatsRefresh, &QPushButton::clicked, this, &MainWindow::onStatsRefresh);
    }
    if (ui->btnExportModels) {
        connect(ui->btnExportModels, &QPushButton::clicked, this, &MainWindow::onGeneratePdf);
    }
    if (ui->btnNewModel) {
        connect(ui->btnNewModel, &QPushButton::clicked, this, &MainWindow::onAddModele);
    }
    if (ui->cbAjoutStatut) {
        ui->cbAjoutStatut->addItems({kStatusFabrication, kStatusStock, kStatusLivre});
    }
    if (ui->cbModifStatut) {
        ui->cbModifStatut->addItems({kStatusFabrication, kStatusStock, kStatusLivre});
    }
    if (ui->cbAjoutDispo) {
        ui->cbAjoutDispo->addItems({"Disponible", "Indisponible"});
    }
    if (ui->cbModifDispo) {
        ui->cbModifDispo->addItems({"Disponible", "Indisponible"});
    }

    if (ui->btnValiderAjout) {
        connect(ui->btnValiderAjout, &QPushButton::clicked, this, &MainWindow::onSaveAjout);
    }
    if (ui->btnAnnulerAjout) {
        connect(ui->btnAnnulerAjout, &QPushButton::clicked, this, &MainWindow::onCancelAjout);
    }
    if (ui->btnValiderModif) {
        connect(ui->btnValiderModif, &QPushButton::clicked, this, &MainWindow::onSaveModif);
    }
    if (ui->btnAnnulerModif) {
        connect(ui->btnAnnulerModif, &QPushButton::clicked, this, &MainWindow::onCancelModif);
    }

    setupStatsChart();
    refreshStats();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configureTable()
{
    ui->tableWidget->setFrameShape(QFrame::NoFrame);
    ui->tableWidget->setContentsMargins(0, 0, 0, 0);
    ui->tableWidget->viewport()->setContentsMargins(0, 0, 0, 0);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setHorizontalHeaderLabels({
        "ID modèle",
        "Type modèle",
        "Dimensions",
        "Type",
        "Prix base (DT)",
        "Description",
        "Statut",
        "Disponibilité",
        "Ventes",
        "Action"
    });

    auto *header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    header->setSectionResizeMode(9, QHeaderView::Fixed);
    ui->tableWidget->setColumnWidth(9, 210);
    header->setStretchLastSection(true);
    header->setFixedHeight(78);
    header->setDefaultAlignment(Qt::AlignCenter);
    header->setStyleSheet("QHeaderView::section { color: #ffffff; }");

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSortingEnabled(true);
}

void MainWindow::gotoPageByObjectName(QTabWidget *tw, const QString &name)
{
    if (!tw) return;

    for (int i = 0; i < tw->count(); ++i) {
        QWidget *w = tw->widget(i);
        if (w && w->objectName() == name) {
            tw->setCurrentIndex(i);
            return;
        }
    }
}

void MainWindow::clearAjoutForm()
{
    ui->leAjoutId->setText(nextId());
    ui->leAjoutCategorie->clear();
    ui->leAjoutDimensions->clear();
    ui->leAjoutType->clear();
    ui->dsAjoutPrix->setValue(0.0);
    ui->teAjoutDescription->clear();
    ui->cbAjoutStatut->setCurrentText(kStatusFabrication);
    ui->cbAjoutDispo->setCurrentIndex(1);
    ui->spAjoutVentes->setValue(0);
}

void MainWindow::loadModeleToEdit(const Modele &modele)
{
    ui->leModifId->setText(modele.id);
    ui->leModifCategorie->setText(modele.typeModele);
    ui->leModifDimensions->setText(modele.dimensions);
    ui->leModifType->setText(modele.type);
    ui->dsModifPrix->setValue(modele.prixBase);
    ui->teModifDescription->setPlainText(modele.description);
    ui->cbModifStatut->setCurrentText(modele.statut);
    ui->cbModifDispo->setCurrentIndex(modele.disponible ? 0 : 1);
    ui->spModifVentes->setValue(modele.ventes);
}

MainWindow::Modele MainWindow::buildModeleFromAjout() const
{
    Modele modele;
    modele.id = ui->leAjoutId->text().trimmed();
    modele.typeModele = ui->leAjoutCategorie->text().trimmed();
    modele.dimensions = ui->leAjoutDimensions->text().trimmed();
    modele.type = ui->leAjoutType->text().trimmed();
    modele.prixBase = ui->dsAjoutPrix->value();
    modele.description = ui->teAjoutDescription->toPlainText().trimmed();
    modele.statut = ui->cbAjoutStatut->currentText().trimmed();
    modele.disponible = (ui->cbAjoutDispo->currentIndex() == 0);
    modele.ventes = ui->spAjoutVentes->value();
    return modele;
}

MainWindow::Modele MainWindow::buildModeleFromModif() const
{
    Modele modele;
    modele.id = ui->leModifId->text().trimmed();
    modele.typeModele = ui->leModifCategorie->text().trimmed();
    modele.dimensions = ui->leModifDimensions->text().trimmed();
    modele.type = ui->leModifType->text().trimmed();
    modele.prixBase = ui->dsModifPrix->value();
    modele.description = ui->teModifDescription->toPlainText().trimmed();
    modele.statut = ui->cbModifStatut->currentText().trimmed();
    modele.disponible = (ui->cbModifDispo->currentIndex() == 0);
    modele.ventes = ui->spModifVentes->value();
    return modele;
}

void MainWindow::seedData()
{
    modeles = {
        {"001", "Salon", "200x90x80", "Canapé", 950.0,
         "Canapé en bois massif, finition naturelle.", kStatusStock, true, 18},
        {"002", "Chambre", "160x200x110", "Lit", 1200.0,
         "Lit double avec tête de lit intégrée.", kStatusFabrication, false, 12},
        {"003", "Bureau", "140x70x75", "Bureau", 560.0,
         "Bureau compact avec rangements latéraux.", kStatusStock, true, 25},
        {"004", "Cuisine", "90x60x90", "Îlot", 1300.0,
         "Îlot central avec rangement et plateau en chêne.", kStatusLivre, false, 31}
    };
}

void MainWindow::rebuildFilters()
{
    QString currentCategory = ui->comboBox->currentText();
    QString currentStatus = ui->comboBox_2->currentText();

    QSet<QString> categories;
    QSet<QString> statuts;
    for (const auto &modele : modeles) {
        categories.insert(modele.typeModele);
        statuts.insert(modele.statut);
    }

    ui->comboBox->blockSignals(true);
    ui->comboBox->clear();
    ui->comboBox->addItem("Tous");
    for (const auto &cat : categories) {
        ui->comboBox->addItem(cat);
    }
    if (!currentCategory.isEmpty()) {
        int idx = ui->comboBox->findText(currentCategory);
        if (idx >= 0) ui->comboBox->setCurrentIndex(idx);
    }
    ui->comboBox->blockSignals(false);

    ui->comboBox_2->blockSignals(true);
    ui->comboBox_2->clear();
    ui->comboBox_2->addItem("Tous");
    for (const auto &st : statuts) {
        ui->comboBox_2->addItem(st);
    }
    if (!currentStatus.isEmpty()) {
        int idx = ui->comboBox_2->findText(currentStatus);
        if (idx >= 0) ui->comboBox_2->setCurrentIndex(idx);
    }
    ui->comboBox_2->blockSignals(false);
}

void MainWindow::refreshTable(const QVector<Modele> &data)
{
    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->setRowCount(0);

    for (const auto &modele : data) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        auto *idItem = new QTableWidgetItem(modele.id);
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, 0, idItem);

        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(modele.typeModele));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(modele.dimensions));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(modele.type));

        auto *prixItem = new QTableWidgetItem();
        prixItem->setData(Qt::EditRole, modele.prixBase);
        prixItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, 4, prixItem);

        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(modele.description));

        auto *statutItem = new QTableWidgetItem(modele.statut);
        statutItem->setTextAlignment(Qt::AlignCenter);
        if (modele.statut == kStatusStock) {
            statutItem->setForeground(QColor(0x2e, 0x7d, 0x32));
        } else if (modele.statut == kStatusFabrication) {
            statutItem->setForeground(QColor(0xf5, 0x7c, 0x00));
        } else {
            statutItem->setForeground(QColor(0x6d, 0x4c, 0x41));
        }
        ui->tableWidget->setItem(row, 6, statutItem);

        auto *dispItem = new QTableWidgetItem(modele.disponible ? "Disponible" : "Indisponible");
        dispItem->setTextAlignment(Qt::AlignCenter);
        dispItem->setForeground(modele.disponible ? QColor(0x2e, 0x7d, 0x32)
                                                  : QColor(0xd8, 0x43, 0x15));
        ui->tableWidget->setItem(row, 7, dispItem);

        auto *ventesItem = new QTableWidgetItem();
        ventesItem->setData(Qt::EditRole, modele.ventes);
        ventesItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, 8, ventesItem);

        QPushButton *btnEdit = new QPushButton("Modifier");
        QPushButton *btnDel = new QPushButton("Supprimer");
        btnEdit->setCursor(Qt::PointingHandCursor);
        btnDel->setCursor(Qt::PointingHandCursor);
        btnEdit->setFocusPolicy(Qt::NoFocus);
        btnDel->setFocusPolicy(Qt::NoFocus);
        btnEdit->setProperty("id", modele.id);
        btnDel->setProperty("id", modele.id);
        btnEdit->setFixedSize(90, 28);
        btnDel->setFixedSize(90, 28);
        btnEdit->setStyleSheet(R"(
QPushButton {
    background-color: #1e5a8b;
    color: white;
    font-size: 11px;
    font-weight: bold;
    border-radius: 6px;
    padding: 1px 6px;
}
QPushButton:hover { background-color: #2a6aa3; }
)");
        btnDel->setStyleSheet(R"(
QPushButton {
    background-color: #8b1e1e;
    color: white;
    font-size: 11px;
    font-weight: bold;
    border-radius: 6px;
    padding: 1px 6px;
}
QPushButton:hover { background-color: #a62828; }
)");
        QWidget *cellWidget = new QWidget();
        cellWidget->setStyleSheet("background: transparent; border: none;");
        cellWidget->setFocusPolicy(Qt::NoFocus);
        QHBoxLayout *layout = new QHBoxLayout(cellWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(8);
        layout->addWidget(btnEdit);
        layout->addWidget(btnDel);
        layout->setAlignment(Qt::AlignCenter);
        ui->tableWidget->setCellWidget(row, 9, cellWidget);
        ui->tableWidget->setRowHeight(row, 46);

        connect(btnDel, &QPushButton::clicked, this, [=]() {
            QString idToDelete = btnDel->property("id").toString();
            if (QMessageBox::question(this, "Confirmation",
                                      "Supprimer le modèle ID " + idToDelete + " ?")
                != QMessageBox::Yes) {
                return;
            }
            int index = findIndexById(idToDelete);
            if (index >= 0) {
                modeles.removeAt(index);
                rebuildFilters();
                applyFilters();
            }
        });

        connect(btnEdit, &QPushButton::clicked, this, [=]() {
            QString idToEdit = btnEdit->property("id").toString();
            int index = findIndexById(idToEdit);
            if (index < 0) return;
            loadModeleToEdit(modeles[index]);
            gotoPageByObjectName(ui->tabWidgetModele, "PModele_Modif");
        });
    }

    ui->tableWidget->setSortingEnabled(true);
}

int MainWindow::findIndexById(const QString &id) const
{
    for (int i = 0; i < modeles.size(); ++i) {
        if (modeles[i].id == id) return i;
    }
    return -1;
}

bool MainWindow::idExists(const QString &id, const QString &ignoreId) const
{
    if (id.isEmpty()) return false;
    for (const auto &modele : modeles) {
        if (modele.id == id && modele.id != ignoreId) {
            return true;
        }
    }
    return false;
}

QString MainWindow::nextId() const
{
    int maxId = 0;
    for (const auto &modele : modeles) {
        bool ok = false;
        int value = modele.id.toInt(&ok);
        if (ok) maxId = std::max(maxId, value);
    }
    return QString("%1").arg(maxId + 1, 3, 10, QChar('0'));
}

bool MainWindow::showModeleDialog(Modele &modele, bool isEdit)
{
    QDialog dialog(this);
    dialog.setWindowTitle(isEdit ? "Modifier modèle" : "Nouveau modèle");
    QFormLayout form(&dialog);

    QLineEdit idEdit(modele.id.isEmpty() ? nextId() : modele.id);
    QLineEdit typeModeleEdit(modele.typeModele);
    QLineEdit dimensionsEdit(modele.dimensions);
    QLineEdit typeEdit(modele.type);
    QDoubleSpinBox prixEdit;
    prixEdit.setRange(0.0, 1000000.0);
    prixEdit.setDecimals(2);
    prixEdit.setValue(modele.prixBase);
    QLineEdit descriptionEdit(modele.description);

    QComboBox statutEdit;
    statutEdit.addItems({kStatusFabrication, kStatusStock, kStatusLivre});
    int statutIndex = statutEdit.findText(modele.statut);
    if (statutIndex >= 0) statutEdit.setCurrentIndex(statutIndex);

    QComboBox dispoEdit;
    dispoEdit.addItems({"Disponible", "Indisponible"});
    dispoEdit.setCurrentIndex(modele.disponible ? 0 : 1);

    QSpinBox ventesEdit;
    ventesEdit.setRange(0, 100000);
    ventesEdit.setValue(modele.ventes);

    if (isEdit) {
        idEdit.setReadOnly(true);
    }

    form.addRow("ID modèle", &idEdit);
    form.addRow("Catégorie", &typeModeleEdit);
    form.addRow("Dimensions", &dimensionsEdit);
    form.addRow("Type", &typeEdit);
    form.addRow("Prix de base (DT)", &prixEdit);
    form.addRow("Description", &descriptionEdit);
    form.addRow("Statut", &statutEdit);
    form.addRow("Disponibilité", &dispoEdit);
    form.addRow("Ventes", &ventesEdit);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(&buttons);

    connect(&buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
        QString id = idEdit.text().trimmed();
        if (id.isEmpty() || typeModeleEdit.text().trimmed().isEmpty()
            || dimensionsEdit.text().trimmed().isEmpty()
            || typeEdit.text().trimmed().isEmpty()
            || descriptionEdit.text().trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Champs manquants",
                                 "Veuillez remplir tous les champs obligatoires.");
            return;
        }
        if (idExists(id, modele.id)) {
            QMessageBox::warning(&dialog, "ID déjà utilisé",
                                 "Un modèle avec cet ID existe déjà.");
            return;
        }
        dialog.accept();
    });
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    modele.id = idEdit.text().trimmed();
    modele.typeModele = typeModeleEdit.text().trimmed();
    modele.dimensions = dimensionsEdit.text().trimmed();
    modele.type = typeEdit.text().trimmed();
    modele.prixBase = prixEdit.value();
    modele.description = descriptionEdit.text().trimmed();
    modele.statut = statutEdit.currentText();
    modele.disponible = (dispoEdit.currentIndex() == 0);
    modele.ventes = ventesEdit.value();
    return true;
}

void MainWindow::applyFilters()
{
    const QString search = ui->lineEdit->text().trimmed().toLower();
    const QString category = ui->comboBox->currentText();
    const QString statut = ui->comboBox_2->currentText();

    QVector<Modele> filtered;
    for (const auto &modele : modeles) {
        if (category != "Tous" && modele.typeModele != category) {
            continue;
        }
        if (statut != "Tous" && modele.statut != statut) {
            continue;
        }

        bool matches = true;
        if (!search.isEmpty()) {
            const QString prixText = QString::number(modele.prixBase, 'f', 2);
            const QString ventesText = QString::number(modele.ventes);
            const QString dispoText = modele.disponible ? "disponible" : "indisponible";

            matches = modele.id.toLower().contains(search)
                || modele.typeModele.toLower().contains(search)
                || modele.dimensions.toLower().contains(search)
                || modele.type.toLower().contains(search)
                || modele.description.toLower().contains(search)
                || modele.statut.toLower().contains(search)
                || prixText.contains(search)
                || ventesText.contains(search)
                || dispoText.contains(search);
        }

        if (matches) {
            filtered.push_back(modele);
        }
    }

    refreshTable(filtered);
    refreshStats();
}

void MainWindow::onAddModele()
{
    clearAjoutForm();
    gotoPageByObjectName(ui->tabWidgetModele, "PModele_Ajout");
}

void MainWindow::onEditModele()
{
    auto *item = ui->tableWidget->currentItem();
    if (!item) {
        QMessageBox::information(this, "Sélection requise",
                                 "Veuillez sélectionner un modèle à modifier.");
        return;
    }

    QString id = ui->tableWidget->item(item->row(), 0)->text();
    int index = findIndexById(id);
    if (index < 0) return;

    loadModeleToEdit(modeles[index]);
    gotoPageByObjectName(ui->tabWidgetModele, "PModele_Modif");
}

void MainWindow::onDeleteModele()
{
    auto *item = ui->tableWidget->currentItem();
    if (!item) {
        QMessageBox::information(this, "Sélection requise",
                                 "Veuillez sélectionner un modèle à supprimer.");
        return;
    }

    QString id = ui->tableWidget->item(item->row(), 0)->text();
    int index = findIndexById(id);
    if (index < 0) return;

    if (QMessageBox::question(this, "Supprimer",
                              "Voulez-vous vraiment supprimer ce modèle ?")
        != QMessageBox::Yes) {
        return;
    }

    modeles.removeAt(index);
    rebuildFilters();
    applyFilters();
}

void MainWindow::onEstimateFromImage()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Choisir une image",
        QString(),
        "Images (*.png *.jpg *.jpeg *.bmp)");
    if (fileName.isEmpty()) return;

    QFileInfo info(fileName);
    double sizeKb = info.size() / 1024.0;
    double estimatedCost = 200.0 + sizeKb * 0.6;
    double estimatedDays = 2.0 + sizeKb / 120.0;

    QString message = QString(
        "Image analysée : %1\n"
        "Taille : %2 KB\n\n"
        "Estimation du coût : %3 DT\n"
        "Temps de réalisation : %4 jours")
        .arg(info.fileName())
        .arg(sizeKb, 0, 'f', 1)
        .arg(estimatedCost, 0, 'f', 2)
        .arg(estimatedDays, 0, 'f', 1);

    if (ui->leEstimationPath) {
        ui->leEstimationPath->setText(fileName);
    }
    if (ui->lblEstimationResult) {
        ui->lblEstimationResult->setText(message);
    } else {
        QMessageBox::information(this, "Estimation image", message);
    }
}

MainWindow::Modele MainWindow::buildModeleFromBudget(double budget) const
{
    Modele modele;
    modele.id = nextId();
    modele.prixBase = budget;
    modele.disponible = false;
    modele.ventes = 0;

    if (budget < 500) {
        modele.typeModele = "Entrée";
        modele.type = "Console";
        modele.dimensions = "90x35x80";
        modele.statut = kStatusFabrication;
    } else if (budget < 1200) {
        modele.typeModele = "Salon";
        modele.type = "Table basse";
        modele.dimensions = "110x60x45";
        modele.statut = kStatusFabrication;
    } else {
        modele.typeModele = "Salle à manger";
        modele.type = "Table";
        modele.dimensions = "200x90x75";
        modele.statut = kStatusFabrication;
    }

    modele.description = "Modèle généré automatiquement selon le budget.";
    return modele;
}

void MainWindow::onBudgetGenerate()
{
    if (!ui->dsBudgetValue) return;
    double budget = ui->dsBudgetValue->value();

    Modele modele = buildModeleFromBudget(budget);
    modeles.push_back(modele);
    rebuildFilters();
    applyFilters();

    if (ui->lblBudgetResult) {
        ui->lblBudgetResult->setText(QString(
            "Résultat : %1 | %2 | %3 | %4 DT")
            .arg(modele.typeModele)
            .arg(modele.type)
            .arg(modele.dimensions)
            .arg(modele.prixBase, 0, 'f', 2));
    }
}

void MainWindow::onSaveAjout()
{
    Modele modele = buildModeleFromAjout();
    if (modele.id.isEmpty()) {
        modele.id = nextId();
        ui->leAjoutId->setText(modele.id);
    }

    if (modele.id.isEmpty() || modele.typeModele.isEmpty()
        || modele.dimensions.isEmpty() || modele.type.isEmpty()
        || modele.description.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants",
                             "Veuillez remplir tous les champs obligatoires.");
        return;
    }
    if (idExists(modele.id)) {
        QMessageBox::warning(this, "ID déjà utilisé",
                             "Un modèle avec cet ID existe déjà.");
        return;
    }

    modeles.push_back(modele);
    rebuildFilters();
    applyFilters();
    gotoPageByObjectName(ui->tabWidgetModele, "PModele_Liste");
    QMessageBox::information(this, "Succès", "Modèle ajouté avec succès.");
}

void MainWindow::onSaveModif()
{
    Modele modele = buildModeleFromModif();
    if (modele.id.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun modèle à modifier.");
        return;
    }

    if (modele.typeModele.isEmpty() || modele.dimensions.isEmpty()
        || modele.type.isEmpty() || modele.description.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants",
                             "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    int index = findIndexById(modele.id);
    if (index < 0) {
        QMessageBox::warning(this, "Erreur", "Modèle introuvable.");
        return;
    }

    modeles[index] = modele;
    rebuildFilters();
    applyFilters();
    gotoPageByObjectName(ui->tabWidgetModele, "PModele_Liste");
    QMessageBox::information(this, "Succès", "Modèle modifié avec succès.");
}

void MainWindow::onCancelAjout()
{
    gotoPageByObjectName(ui->tabWidgetModele, "PModele_Liste");
}

void MainWindow::onCancelModif()
{
    gotoPageByObjectName(ui->tabWidgetModele, "PModele_Liste");
}

void MainWindow::onStatsRefresh()
{
    refreshStats();
}

void MainWindow::refreshStats()
{
    if (!ui->lblStatsTotal || !ui->lblStatsDisponible || !ui->lblStatsPrixMoyen || !ui->lblStatsBest) {
        return;
    }

    if (modeles.isEmpty()) {
        ui->lblStatsTotal->setText("Total modèles : 0");
        ui->lblStatsDisponible->setText("Disponibles : 0");
        ui->lblStatsPrixMoyen->setText("Prix moyen : --");
        ui->lblStatsBest->setText("Meuble le plus vendu : --");
        return;
    }

    int disponibles = 0;
    double totalPrix = 0.0;
    const Modele *bestSeller = nullptr;

    for (const auto &modele : modeles) {
        if (modele.disponible) disponibles++;
        totalPrix += modele.prixBase;
        if (!bestSeller || modele.ventes > bestSeller->ventes) {
            bestSeller = &modele;
        }
    }

    ui->lblStatsTotal->setText(QString("Total modèles : %1").arg(modeles.size()));
    ui->lblStatsDisponible->setText(QString("Disponibles : %1").arg(disponibles));
    ui->lblStatsPrixMoyen->setText(QString("Prix moyen : %1 DT").arg(totalPrix / modeles.size(), 0, 'f', 2));
    ui->lblStatsBest->setText(QString("Meuble le plus vendu : %1 (%2 ventes)")
                              .arg(bestSeller ? bestSeller->type : "-")
                              .arg(bestSeller ? bestSeller->ventes : 0));
}

void MainWindow::setupStatsChart()
{
    if (!ui->statsChartContainer) return;

    auto *chart = new QChart();
    chart->setTheme(QChart::ChartThemeLight);
    chart->setBackgroundVisible(false);
    chart->setBackgroundBrush(Qt::transparent);
    chart->setPlotAreaBackgroundVisible(true);
    chart->setPlotAreaBackgroundBrush(QBrush(QColor(0xf7, 0xef, 0xe4)));
    chart->legend()->setVisible(false);
    const QColor kTextColor(Qt::white);
    const QColor kGridColor(0x9c, 0x9c, 0x9c);
    chart->setTitle("Ventes :");
    chart->setTitleBrush(QBrush(kTextColor));

    auto *seriesSales = new QLineSeries();
    seriesSales->setName("Ventes");
    seriesSales->setColor(QColor(0x2f, 0x85, 0x5f));
    seriesSales->setPointsVisible(true);

    QVector<int> monthSales(12, 0);
    for (const auto &m : modeles) {
        int monthIndex = QRandomGenerator::global()->bounded(12);
        monthSales[monthIndex] += m.ventes;
    }

    for (int i = 0; i < 12; ++i) {
        seriesSales->append(i + 1, monthSales[i]);
    }

    chart->addSeries(seriesSales);

    auto *axisX = new QCategoryAxis();
    const QStringList months = {"Jan", "Fev", "Mar", "Avr", "Mai", "Juin",
                                "Juil", "Aou", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < months.size(); ++i) {
        axisX->append(months[i], i + 1);
    }
    axisX->setRange(1, 12);
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    axisX->setLabelsColor(kTextColor);
    axisX->setTitleText("Mois");
    axisX->setTitleBrush(QBrush(kTextColor));

    auto *axisY = new QValueAxis();
    axisY->setRange(0, qMax(10, *std::max_element(monthSales.begin(), monthSales.end()) + 5));
    axisY->setLabelFormat("%d");
    axisY->setLabelsColor(kTextColor);
    axisY->setTitleBrush(QBrush(kTextColor));
    axisY->setLinePenColor(kTextColor);
    axisX->setLinePenColor(kTextColor);
    axisX->setGridLinePen(QPen(kGridColor));
    axisY->setGridLinePen(QPen(kGridColor));

    chart->setAxisX(axisX, seriesSales);
    chart->setAxisY(axisY, seriesSales);

    auto *chartView = new QChartView(chart, ui->statsChartContainer);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("background: transparent;");

    auto *layout = new QHBoxLayout(ui->statsChartContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
}

void MainWindow::onGeneratePdf()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "Enregistrer le rapport PDF", "rapport_modeles.pdf", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageMargins(QMarginsF(20, 20, 20, 20));
    writer.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&writer);
    painter.setFont(QFont("Arial", 12));

    int y = 40;
    painter.drawText(0, y, "Rapport des modèles");
    y += 30;
    painter.setFont(QFont("Arial", 9));
    painter.drawText(0, y, QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));
    y += 30;

    for (const auto &modele : modeles) {
        QString line = QString("%1 | %2 | %3 | %4 DT | %5 | %6")
            .arg(modele.id, modele.typeModele, modele.type)
            .arg(modele.prixBase, 0, 'f', 2)
            .arg(modele.statut)
            .arg(modele.disponible ? "Disponible" : "Indisponible");
        painter.drawText(0, y, line);
        y += 18;
        if (y > writer.height() - 40) {
            writer.newPage();
            y = 40;
        }
    }

    painter.end();
    QMessageBox::information(this, "Rapport PDF", "Rapport PDF généré avec succès.");
}
