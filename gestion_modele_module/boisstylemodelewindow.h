#ifndef BOISSTYLEMODELEWINDOW_H
#define BOISSTYLEMODELEWINDOW_H

#include <QMainWindow>
#include <QVector>

class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTabWidget;
class QTableWidget;
class QWidget;
class QChartView;

class BoisStyleModeleWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BoisStyleModeleWindow(QWidget *parent = nullptr);

private slots:
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();
    void onSearchChanged(const QString &text);
    void onFilterChanged(const QString &value);
    void onSortChanged();
    void onPrevPage();
    void onNextPage();
    void onExportPdf();
    void onStatsRefresh();
    void onChooseImage();
    void onAdvisorGenerate();

private:
    struct Modele
    {
        QString id;
        QString categorie;
        QString dimensions;
        QString type;
        double prix = 0.0;
        QString description;
        QString statut;
        QString disponibilite;
        int ventes = 0;
    };

    QVector<Modele> m_allModeles;
    QVector<int> m_filteredIndices;
    int m_currentPage = 0;
    int m_pageSize = 8;

    QWidget *m_central = nullptr;
    QTabWidget *m_tabWidget = nullptr;
    QLineEdit *m_searchEdit = nullptr;
    QComboBox *m_filterCombo = nullptr;
    QComboBox *m_sortFieldCombo = nullptr;
    QComboBox *m_sortOrderCombo = nullptr;
    QTableWidget *m_table = nullptr;
    QPushButton *m_prevButton = nullptr;
    QPushButton *m_nextButton = nullptr;
    QWidget *m_pageNumbersContainer = nullptr;
    QPushButton *m_addButton = nullptr;
    QPushButton *m_editButton = nullptr;
    QPushButton *m_deleteButton = nullptr;
    QPushButton *m_exportButton = nullptr;

    QLabel *m_statsTotalLabel = nullptr;
    QLabel *m_statsAvailableLabel = nullptr;
    QLabel *m_statsAvgPriceLabel = nullptr;
    QLabel *m_statsBestSellerLabel = nullptr;
    QWidget *m_statsChartContainer = nullptr;
    QChartView *m_statsChartView = nullptr;

    QLineEdit *m_imagePathEdit = nullptr;
    QLabel *m_imageResultLabel = nullptr;

    QDoubleSpinBox *m_budgetSpin = nullptr;
    QLabel *m_advisorResultLabel = nullptr;

    void setupUi();
    void seedData();
    void refreshFiltersAndSort();
    void renderCurrentPage();
    void rebuildPageButtons();
    int selectedGlobalIndex() const;
    bool showEditorDialog(Modele &modele, bool editMode);
    void setupStatsChart();
    void refreshStatsCards();
};

#endif // BOISSTYLEMODELEWINDOW_H
