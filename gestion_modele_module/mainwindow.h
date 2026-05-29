#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QTabWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddModele();
    void onEditModele();
    void onDeleteModele();
    void onEstimateFromImage();
    void onGeneratePdf();
    void onBudgetGenerate();
    void onStatsRefresh();
    void applyFilters();
    void onSaveAjout();
    void onSaveModif();
    void onCancelAjout();
    void onCancelModif();

private:
    struct Modele {
        QString id;
        QString typeModele;
        QString dimensions;
        QString type;
        double prixBase = 0.0;
        QString description;
        QString statut;
        bool disponible = false;
        int ventes = 0;
    };

    Ui::MainWindow *ui;

    QVector<Modele> modeles;

    void configureTable();
    void seedData();
    void rebuildFilters();
    void refreshTable(const QVector<Modele> &data);
    int findIndexById(const QString &id) const;
    bool idExists(const QString &id, const QString &ignoreId = QString()) const;
    QString nextId() const;
    bool showModeleDialog(Modele &modele, bool isEdit);
    void gotoPageByObjectName(QTabWidget *tw, const QString &name);
    void clearAjoutForm();
    void loadModeleToEdit(const Modele &modele);
    Modele buildModeleFromAjout() const;
    Modele buildModeleFromModif() const;
    Modele buildModeleFromBudget(double budget) const;
    void refreshStats();
    void setupStatsChart();
};
#endif // MAINWINDOW_H
