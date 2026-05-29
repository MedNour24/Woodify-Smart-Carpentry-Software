#include "modelecostestimator.h"

#include <QRegularExpression>
#include <cmath>

namespace {

double materiauMultiplier(const QString &materiau)
{
    const QString m = materiau.toLower();
    if (m.contains(QStringLiteral("metal")) || m.contains(QStringLiteral("métal"))
        || m.contains(QStringLiteral("acier")) || m.contains(QStringLiteral("alumin"))) {
        return 0.92;
    }
    if (m.contains(QStringLiteral("melamine")) || m.contains(QStringLiteral("mélamin"))) {
        return 0.88;
    }
    if (m.contains(QStringLiteral("massif")) || m.contains(QStringLiteral("chene")) || m.contains(QStringLiteral("chêne"))) {
        return 1.32;
    }
    if (m.contains(QStringLiteral("agglom")) || m.contains(QStringLiteral("panneau"))) {
        return 0.78;
    }
    if (m.contains(QStringLiteral("contre"))) {
        return 0.92;
    }
    if (m.contains(QStringLiteral("stratif"))) {
        return 1.08;
    }
    if (m.contains(QStringLiteral("mdf"))) {
        return 0.82;
    }
    return 1.0;
}

double typeMultipliers(const QString &type, double *timeMultOut)
{
    const QString t = type.toLower();
    double m = 1.0;
    double tm = 1.0;
    if (t.contains(QStringLiteral("chaise"))) {
        m = 0.38;
        tm = 0.55;
    } else if (t.contains(QStringLiteral("armoire"))) {
        m = 2.15;
        tm = 1.75;
    } else if (t.contains(QStringLiteral("bibli"))) {
        m = 1.55;
        tm = 1.45;
    } else if (t.contains(QStringLiteral("bureau"))) {
        m = 1.12;
        tm = 1.15;
    } else if (t.contains(QStringLiteral("etag"))) {
        m = 0.62;
        tm = 0.75;
    } else if (t.contains(QStringLiteral("buffet"))) {
        m = 1.35;
        tm = 1.25;
    } else if (t.contains(QStringLiteral("table"))) {
        m = 1.0;
        tm = 1.0;
    }
    if (timeMultOut) {
        *timeMultOut = tm;
    }
    return m;
}

int complexiteIndex(const QString &complexite)
{
    const QString c = complexite.toLower();
    if (c.contains(QStringLiteral("faib"))) {
        return 0;
    }
    if (c.contains(QStringLiteral("ele")) || c.contains(QStringLiteral("élev"))) {
        return 2;
    }
    return 1;
}

void complexiteFactors(int idx, double *costMult, double *timeMult)
{
    double cm = 1.0;
    double tm = 1.0;
    if (idx == 0) {
        cm = 0.9;
        tm = 0.88;
    } else if (idx == 2) {
        cm = 1.28;
        tm = 1.38;
    }
    if (costMult) {
        *costMult = cm;
    }
    if (timeMult) {
        *timeMult = tm;
    }
}

QString esc(const QString &s)
{
    QString e = s;
    e.replace('&', "&amp;");
    e.replace('<', "&lt;");
    e.replace('>', "&gt;");
    return e;
}

QString listToHtmlUl(const QStringList &items, const QString &color)
{
    QString html = QStringLiteral("<ul style=\"margin:6px 0 0 18px;color:%1;\">").arg(color);
    for (const QString &r : items) {
        html += QStringLiteral("<li style=\"margin-bottom:4px;\">%1</li>").arg(esc(r));
    }
    html += QStringLiteral("</ul>");
    return html;
}

QVector<QPair<int, double>> projectCostsYears(double baseCostTnd, const QString &materiau)
{
    QVector<QPair<int, double>> out;
    const double matNoise = 0.004 * (materiauMultiplier(materiau) - 1.0);
    for (int year = ModeleCostEstimator::kAnneeProjectionDebut; year <= ModeleCostEstimator::kAnneeProjectionFin;
         ++year) {
        const int i = year - ModeleCostEstimator::kAnneeProjectionDebut;
        const double inflation = std::pow(1.05, static_cast<double>(i));
        const double variationMatiere = 1.0 + matNoise * std::sin(static_cast<double>(i) * 0.85);
        const double optimisation = std::max(0.92, 1.0 - 0.012 * static_cast<double>(i));
        const double cy = baseCostTnd * inflation * variationMatiere * optimisation;
        out.append(qMakePair(year, cy));
    }
    return out;
}

} // namespace

bool ModeleCostEstimator::parseDimensionsCm(const QString &text, double *outL, double *outW, double *outH,
                                            QString *errorMessage)
{
    if (!outL || !outW || !outH) {
        return false;
    }
    QString t = text.trimmed();
    if (t.isEmpty()) {
        if (errorMessage) {
            *errorMessage = QStringLiteral("Dimensions vides.");
        }
        return false;
    }
    t.replace(',', '.');
    const QStringList parts = t.split(QRegularExpression("[xX\\*\\s]+"), Qt::SkipEmptyParts);
    if (parts.size() < 3) {
        if (errorMessage) {
            *errorMessage = QStringLiteral("Utilisez trois valeurs en cm separees par x (ex. 140 x 80 x 75).");
        }
        return false;
    }
    bool okL = false, okW = false, okH = false;
    const double L = parts.at(0).toDouble(&okL);
    const double W = parts.at(1).toDouble(&okW);
    const double H = parts.at(2).toDouble(&okH);
    if (!okL || !okW || !okH || L <= 0.0 || W <= 0.0 || H <= 0.0 || L > 600.0 || W > 400.0 || H > 350.0) {
        if (errorMessage) {
            *errorMessage = QStringLiteral("Valeurs hors plage realiste (max. indicatif 600 x 400 x 350 cm).");
        }
        return false;
    }
    *outL = L;
    *outW = W;
    *outH = H;
    return true;
}

ModeleCostEstimateResult ModeleCostEstimator::estimate(const ModeleCostInput &in)
{
    ModeleCostEstimateResult r;
    const double volume_cm3 = in.longueurCm * in.largeurCm * in.hauteurCm;
    r.volumeM3 = volume_cm3 / 1'000'000.0;

    const double matMult = materiauMultiplier(in.materiau);
    double typeTimeMult = 1.0;
    const double typeMult = typeMultipliers(in.typeModele, &typeTimeMult);
    const int complexIdx = complexiteIndex(in.complexite);
    double complexCost = 1.0;
    double complexTime = 1.0;
    complexiteFactors(complexIdx, &complexCost, &complexTime);

    const int qty = qMax(1, in.quantite);
    const double qtyFactor = std::pow(static_cast<double>(qty), 0.9);

    // Coût matière + MO + charges (heuristique), par lot
    double coutBaseUnite = 95.0 + (volume_cm3 / 6500.0) * 11.5;
    coutBaseUnite *= matMult * typeMult * complexCost;
    r.coutFabricationLotTnd = qMax(85.0, coutBaseUnite * qtyFactor);

    double heuresUnite = 5.5 + std::sqrt(volume_cm3) / 42.0;
    heuresUnite *= typeTimeMult * complexTime;
    heuresUnite *= (1.0 + 0.18 * std::log1p(static_cast<double>(qty)));
    r.tempsHeures = qBound(4.0, heuresUnite, 220.0);

    int proba = 92;
    if (complexIdx >= 2) {
        proba -= 10;
    }
    if (matMult >= 1.2) {
        proba -= 4;
    }
    if (volume_cm3 > 2.2e6) {
        proba -= 7;
    }
    if (qty > 40) {
        proba -= 5;
    }
    if (qty > 80) {
        proba -= 4;
    }
    r.probabiliteReussitePct = qBound(58, proba, 98);

    if (complexIdx >= 2) {
        r.risques << QStringLiteral(
            "Assemblages complexes : risque de depassement de delai si une seule equipe.");
    }
    if (matMult >= 1.25) {
        r.risques << QStringLiteral(
            "Bois noble / massif : sensibilite aux variations d'humidite et de retrait.");
    }
    if (volume_cm3 > 1.8e6) {
        r.risques << QStringLiteral(
            "Grand volume : manutention, transport interne et planification atelier plus critiques.");
    }
    if (qty > 25) {
        r.risques << QStringLiteral(
            "Forte serie : homogeneite des finitions et controle qualite a renforcer.");
    }
    if (matMult < 0.95) {
        r.risques << QStringLiteral(
            "Materiau economique : verifier la resistance aux contraintes mecaniques prevues.");
    }
    if (r.risques.isEmpty()) {
        r.risques << QStringLiteral("Risques majeurs faibles avec les parametres actuels ; maintenir le suivi qualite.");
    }

    r.recommandations << QStringLiteral(
        "Valider le cahier des charges (finitions, quincaillerie, couleur) avant lancement fabrication.");
    r.recommandations << QStringLiteral(
        "Preparer une nomenclature pieces / decoupe pour reduire les chutes de panneaux.");
    if (complexIdx >= 2) {
        r.recommandations << QStringLiteral(
            "Decouper la fabrication en sous-ensembles avec jalons (test d'assemblage sec).");
    }
    if (qty > 15) {
        r.recommandations << QStringLiteral(
            "Envisager un lotissement par vague de livraison pour lisser la charge atelier.");
    }
    if (volume_cm3 > 1.5e6) {
        r.recommandations << QStringLiteral(
            "Optimiser les dimensions ou modulariser pour reduire les risques logistiques.");
    }
    r.recommandations << QStringLiteral(
        "Mettre a jour le cout reel en fin de chantier pour affiner les prochains devis.");

    r.prixVenteIndicatifTnd = qMax(0.0, r.coutFabricationLotTnd / 0.68);
    r.coutParAnnee = projectCostsYears(r.coutFabricationLotTnd, in.materiau);
    return r;
}

QString ModeleCostEstimator::toDashboardHtml(const ModeleCostInput &in, const ModeleCostEstimateResult &res)
{
    const QString risquesHtml = listToHtmlUl(res.risques, QStringLiteral("#fecdd3"));
    const QString recoHtml = listToHtmlUl(res.recommandations, QStringLiteral("#bbf7d0"));
    const QString dims = QStringLiteral("%1 × %2 × %3")
                             .arg(in.longueurCm, 0, 'f', 0)
                             .arg(in.largeurCm, 0, 'f', 0)
                             .arg(in.hauteurCm, 0, 'f', 0);

    return QStringLiteral(
               "<div style=\"font-family:'Segoe UI',Arial,sans-serif;color:#e2e8f0;\">"
               "<div style=\"background-color:rgba(8,13,32,0.95);border:1px solid rgba(56,189,248,0.35);"
               "border-radius:16px;overflow:hidden;\">"
               "<div style=\"padding:14px 18px;background-color:rgba(30,41,59,0.92);"
               "border-bottom:1px solid rgba(51,65,105,0.55);\">"
               "<p style=\"margin:0;font-size:11px;letter-spacing:1.2px;color:#64748b;font-weight:700;\">"
               "TABLEAU DE BORD — ESTIMATION INDUSTRIELLE</p>"
               "<p style=\"margin:4px 0 0 0;font-size:17px;font-weight:800;color:#e0f2fe;\">"
               "Cout, delai &amp; risques</p>"
               "<p style=\"margin:4px 0 0 0;font-size:12px;color:#94a3b8;\">%1 — %2 — Qté <b>%3</b></p>"
               "</div>"
               "<div style=\"padding:16px 18px;\">"
               "<table width=\"100%\" cellspacing=\"0\" cellpadding=\"0\" style=\"border-collapse:collapse;\">"
               "<tr><td style=\"padding:8px 0;color:#7dd3fc;font-weight:700;width:42%;\">"
               "Cout estime (fabrication, lot)</td>"
               "<td style=\"padding:8px 0;color:#38bdf8;font-weight:800;font-size:18px;\">%4 DT</td></tr>"
               "<tr><td style=\"padding:8px 0;color:#7dd3fc;font-weight:700;\">Temps estime</td>"
               "<td style=\"padding:8px 0;color:#f8fafc;font-weight:700;\">%5 h</td></tr>"
               "<tr><td style=\"padding:8px 0;color:#7dd3fc;font-weight:700;\">Probabilite de reussite</td>"
               "<td style=\"padding:8px 0;color:#4ade80;font-weight:800;font-size:18px;\">%6 %</td></tr>"
               "<tr><td style=\"padding:8px 0;color:#7dd3fc;font-weight:700;\">Prix de vente indicatif</td>"
               "<td style=\"padding:8px 0;color:#c4b5fd;font-weight:700;\">~ %7 DT (marge cible ~32 %)</td></tr>"
               "<tr><td style=\"padding:8px 0;color:#7dd3fc;font-weight:700;vertical-align:top;\">Risques</td>"
               "<td style=\"padding:8px 0;\">%8</td></tr>"
               "<tr><td style=\"padding:8px 0;color:#7dd3fc;font-weight:700;vertical-align:top;\">"
               "Recommandations</td>"
               "<td style=\"padding:8px 0;\">%9</td></tr>"
               "</table>"
               "<p style=\"margin:14px 0 0 0;font-size:11px;color:#64748b;line-height:1.4;\">"
               "Volume approximatif : <b>%10</b> m³ — Projection coût %11–%12 (inflation +5 %/an, "
               "variation matière, optimisation) : voir <b>graphique</b> ci-dessous.</p>"
               "</div></div></div>")
        .arg(esc(in.typeModele), esc(in.materiau))
        .arg(in.quantite)
        .arg(res.coutFabricationLotTnd, 0, 'f', 2)
        .arg(res.tempsHeures, 0, 'f', 1)
        .arg(res.probabiliteReussitePct)
        .arg(res.prixVenteIndicatifTnd, 0, 'f', 0)
        .arg(risquesHtml)
        .arg(recoHtml)
        .arg(res.volumeM3, 0, 'f', 3)
        .arg(kAnneeProjectionDebut)
        .arg(kAnneeProjectionFin)
        .arg(esc(dims));
}
