#ifndef BUDGETGLTFVIEWER_H
#define BUDGETGLTFVIEWER_H

#include <QWidget>

#include <QNetworkRequest>

class QLabel;
class QNetworkAccessManager;
class QTemporaryFile;

/** Charge un fichier glTF/glb (HTTPS ou chemin local) et affiche la scene en Qt3D dans le widget. */
class BudgetGltfViewerWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit BudgetGltfViewerWidget(QWidget *parent = nullptr);

    /** Telecharge si besoin puis affiche le modele (URL https vers .glb / .gltf ou file://). */
    void loadModelUrl(const QString &urlString);
    void setFallbackModelType(const QString &modelType);
    void setFallbackModelSpec(const QString &modelType, const QString &dimensionsText);

    void clearViewer();

private:
    void setupLoadingUi();
    void tearDownScene();
    void buildQt3DScene(const QString &localGlbPath);
    void buildFallbackScene();

    void downloadMirror(const QStringList &urls, int index, const QString &lastError);
    static QStringList mirrorCandidatesForUrl(const QString &urlString);
    static QNetworkRequest buildAssetDownloadRequest(const QUrl &url);

    QLabel *m_statusLabel = nullptr;
    QWidget *m_sceneHost = nullptr;
    QNetworkAccessManager *m_nam = nullptr;

    QWidget *m_windowContainer = nullptr;
    QTemporaryFile *m_tempGlb = nullptr;
    QString m_fallbackModelType;
    QString m_fallbackDimensionsText;
};

#endif
