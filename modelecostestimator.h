#ifndef MODELECOSTESTIMATOR_H
#define MODELECOSTESTIMATOR_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QPair>

/**
 * Entrées du formulaire « Métier 3 » — estimation coût & performance (sans chat).
 */
struct ModeleCostInput {
    QString typeModele;
    double longueurCm = 0.0;
    double largeurCm = 0.0;
    double hauteurCm = 0.0;
    QString materiau;
    QString complexite;
    int quantite = 1;
};

/**
 * Sortie métier + série pour graphique (coût projeté sur plusieurs années).
 */
struct ModeleCostEstimateResult {
    double coutFabricationLotTnd = 0.0;
    double tempsHeures = 0.0;
    int probabiliteReussitePct = 0;
    QStringList risques;
    QStringList recommandations;
    double volumeM3 = 0.0;
    double prixVenteIndicatifTnd = 0.0;
    /** Année → coût estimé (TND), pour courbe d’évolution. */
    QVector<QPair<int, double>> coutParAnnee;
};

/**
 * Moteur d’estimation « IA simulée » : coût, délai, probabilité, risques, recommandations, projection.
 * Logique pure (pas de widgets) — exploitable depuis l’UI ou des tests.
 */
class ModeleCostEstimator
{
public:
    /** Année de départ fixe pour la projection affichée (2026 → …). */
    static constexpr int kAnneeProjectionDebut = 2026;
    static constexpr int kAnneeProjectionFin = 2030;

    /** Analyse complète à partir des entrées formulaire. */
    static ModeleCostEstimateResult estimate(const ModeleCostInput &in);

    /** HTML tableau de bord (résultats + listes). Le graphique est séparé (Qt Charts). */
    static QString toDashboardHtml(const ModeleCostInput &in, const ModeleCostEstimateResult &res);

    /** Parse « L x l x h » avec séparateurs x, X, *, espaces. Retourne false si invalide. */
    static bool parseDimensionsCm(const QString &text, double *outL, double *outW, double *outH, QString *errorMessage);
};

#endif // MODELECOSTESTIMATOR_H
