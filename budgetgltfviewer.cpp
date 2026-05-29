#include "budgetgltfviewer.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDir>
#include <QTemporaryFile>
#include <QUrl>
#include <QColor>
#include <QStringList>
#include <QRegularExpression>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QDirectionalLight>
#include <Qt3DRender/QSceneLoader>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPhongMaterial>

namespace {

bool dataLooksLikeGlb(const QByteArray &d)
{
    // GLB : uint32 magic 0x46546C67 → sur LE x86 : 67 6C 54 46 == "glTF"
    return d.size() >= 12 && static_cast<unsigned char>(d[0]) == 0x67 && static_cast<unsigned char>(d[1]) == 0x6C
           && static_cast<unsigned char>(d[2]) == 0x54 && static_cast<unsigned char>(d[3]) == 0x46;
}

} // namespace

BudgetGltfViewerWidget::BudgetGltfViewerWidget(QWidget *parent)
    : QWidget(parent)
{
    m_nam = new QNetworkAccessManager(this);
    setupLoadingUi();
}

void BudgetGltfViewerWidget::setupLoadingUi()
{
    m_statusLabel = new QLabel(tr("Pret."), this);
    m_sceneHost = new QWidget(this);
    auto *outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(4);
    outer->addWidget(m_statusLabel);
    outer->addWidget(m_sceneHost, 1);
}

void BudgetGltfViewerWidget::clearViewer()
{
    tearDownScene();
    m_statusLabel->setText(tr("Pret."));
}

void BudgetGltfViewerWidget::setFallbackModelType(const QString &modelType)
{
    m_fallbackModelType = modelType.trimmed().toUpper();
}

void BudgetGltfViewerWidget::setFallbackModelSpec(const QString &modelType, const QString &dimensionsText)
{
    m_fallbackModelType = modelType.trimmed().toUpper();
    m_fallbackDimensionsText = dimensionsText.trimmed();
}

void BudgetGltfViewerWidget::tearDownScene()
{
    if (m_sceneHost) {
        if (QLayout *lay = m_sceneHost->layout()) {
            while (QLayoutItem *it = lay->takeAt(0)) {
                if (QWidget *w = it->widget())
                    delete w;
                delete it;
            }
            delete lay;
        }
    }
    m_windowContainer = nullptr;
    if (m_tempGlb) {
        delete m_tempGlb;
        m_tempGlb = nullptr;
    }
}

QNetworkRequest BudgetGltfViewerWidget::buildAssetDownloadRequest(const QUrl &url)
{
    QNetworkRequest r(url);
    // GitHub raw et certains CDN refusent ou repondent mal sans User-Agent realiste.
    r.setHeader(QNetworkRequest::UserAgentHeader,
                QStringLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                               "AppleWebKit/537.36 (KHTML, like Gecko) "
                               "Chrome/120.0.0.0 Safari/537.36"));
    r.setRawHeader("Accept", QByteArrayLiteral("*/*"));
    r.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                   QNetworkRequest::NoLessSafeRedirectPolicy);
    r.setTransferTimeout(120000);
    return r;
}

QStringList BudgetGltfViewerWidget::mirrorCandidatesForUrl(const QString &urlString)
{
    const QString u = urlString.trimmed();
    const QString chairJs = QStringLiteral(
        "https://cdn.jsdelivr.net/gh/KhronosGroup/glTF-Sample-Assets@main/Models/Chair/glTF-Binary/Chair.glb");
    const QString chairRaw = QStringLiteral(
        "https://raw.githubusercontent.com/KhronosGroup/glTF-Sample-Assets/main/Models/Chair/glTF-Binary/Chair.glb");
    const QString chairLegacy = QStringLiteral(
        "https://raw.githubusercontent.com/KhronosGroup/glTF-Sample-Models/master/2.0/Chair/glTF-Binary/Chair.glb");

    const bool isChairKhronos =
        u.contains(QStringLiteral("KhronosGroup"), Qt::CaseInsensitive)
        && u.contains(QStringLiteral("Chair"), Qt::CaseInsensitive)
        && (u.endsWith(QStringLiteral("Chair.glb"), Qt::CaseInsensitive)
            || u.contains(QStringLiteral("Chair.glTF-Binary"), Qt::CaseInsensitive));

    QStringList mirrors;
    if (isChairKhronos) {
        mirrors << chairJs << chairRaw << chairLegacy;
    } else {
        mirrors << u;
    }

    QStringList out;
    for (const QString &s : mirrors) {
        if (!out.contains(s))
            out.append(s);
    }
    return out;
}

void BudgetGltfViewerWidget::downloadMirror(const QStringList &urls, int index, const QString &lastError)
{
    if (index >= urls.size()) {
        buildFallbackScene();
        m_statusLabel->setText(tr("Modele distant indisponible, affichage d'un modele local de secours. "
                                  "Derniere erreur: %1")
                                   .arg(lastError.isEmpty() ? tr("inconnue") : lastError));
        return;
    }

    const QUrl url(urls.at(index));
    if (!url.isValid()) {
        downloadMirror(urls, index + 1, lastError);
        return;
    }

    m_statusLabel->setText(tr("Telechargement du modele (%1/%2)...").arg(index + 1).arg(urls.size()));

    QNetworkReply *reply = m_nam->get(buildAssetDownloadRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, reply, urls, index]() {
        reply->deleteLater();
        const QString err = reply->errorString();

        if (reply->error() != QNetworkReply::NoError) {
            downloadMirror(urls, index + 1, err);
            return;
        }

        const QByteArray data = reply->readAll();
        const QString reqPath = QUrl(urls.at(index)).path();
        const bool expectGlb = reqPath.endsWith(QStringLiteral(".glb"), Qt::CaseInsensitive);
        if (expectGlb && !dataLooksLikeGlb(data)) {
            downloadMirror(urls,
                           index + 1,
                           tr("Reponse invalide (GLB attendu), %1 octets").arg(data.size()));
            return;
        }
        if (!expectGlb && data.size() < 30) {
            downloadMirror(urls, index + 1, tr("Fichier trop petit (%1 octets)").arg(data.size()));
            return;
        }

        m_tempGlb = new QTemporaryFile(this);
        m_tempGlb->setFileTemplate(QDir::temp().absoluteFilePath(QStringLiteral("woodify_model_XXXXXX.glb")));
        m_tempGlb->setAutoRemove(true);
        if (!m_tempGlb->open()) {
            delete m_tempGlb;
            m_tempGlb = nullptr;
            downloadMirror(urls, index + 1, tr("Impossible de creer un fichier temporaire."));
            return;
        }
        m_tempGlb->write(data);
        m_tempGlb->flush();
        const QString path = m_tempGlb->fileName();
        m_tempGlb->close();

        buildQt3DScene(path);
        m_statusLabel->setText(tr("Modele charge — souris pour tourner."));
    });
}

void BudgetGltfViewerWidget::loadModelUrl(const QString &urlString)
{
    tearDownScene();

    const QString trimmed = urlString.trimmed();
    const QUrl url(trimmed);
    if (!url.isValid()) {
        m_statusLabel->setText(tr("URL du modele invalide."));
        return;
    }

    if (url.isLocalFile()) {
        const QString path = url.toLocalFile();
        m_statusLabel->setText(tr("Chargement du fichier local..."));
        buildQt3DScene(path);
        m_statusLabel->setText(tr("Modele charge — souris pour tourner."));
        return;
    }

    if (url.scheme() != QStringLiteral("https") && url.scheme() != QStringLiteral("http")) {
        m_statusLabel->setText(
            tr("Schema d'URL non pris en charge (https, http ou fichier local)."));
        return;
    }

    const QStringList urls = mirrorCandidatesForUrl(trimmed);
    downloadMirror(urls, 0, QString());
}

void BudgetGltfViewerWidget::buildQt3DScene(const QString &localPath)
{
    auto *view3d = new Qt3DExtras::Qt3DWindow();
    if (auto *fg = view3d->defaultFrameGraph())
        fg->setClearColor(QColor(0x0b1220));

    auto *rootEntity = new Qt3DCore::QEntity();

    auto *loaderEntity = new Qt3DCore::QEntity(rootEntity);
    auto *sceneLoader = new Qt3DRender::QSceneLoader(loaderEntity);
    loaderEntity->addComponent(sceneLoader);
    sceneLoader->setSource(QUrl::fromLocalFile(localPath));

    auto *lightEntity = new Qt3DCore::QEntity(rootEntity);
    auto *light = new Qt3DRender::QDirectionalLight(lightEntity);
    light->setColor(Qt::white);
    light->setIntensity(1.0f);
    light->setWorldDirection(QVector3D(-1.f, -3.f, -1.f).normalized());
    lightEntity->addComponent(light);

    Qt3DRender::QCamera *camera = view3d->camera();
    camera->lens()->setPerspectiveProjection(45.f, 16.f / 9.f, 0.1f, 1000.f);
    camera->setPosition(QVector3D(3.f, 2.f, 5.f));
    camera->setViewCenter(QVector3D(0.f, 0.5f, 0.f));

    auto *orbit = new Qt3DExtras::QOrbitCameraController(rootEntity);
    orbit->setCamera(camera);
    orbit->setLinearSpeed(50.f);
    orbit->setLookSpeed(180.f);

    view3d->setRootEntity(rootEntity);

    m_windowContainer = QWidget::createWindowContainer(view3d, m_sceneHost);
    m_windowContainer->setMinimumSize(280, 240);
    m_windowContainer->setFocusPolicy(Qt::StrongFocus);

    auto *vl = new QVBoxLayout(m_sceneHost);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(m_windowContainer);
}

void BudgetGltfViewerWidget::buildFallbackScene()
{
    auto *view3d = new Qt3DExtras::Qt3DWindow();
    if (auto *fg = view3d->defaultFrameGraph())
        fg->setClearColor(QColor(0x0b1220));

    auto *rootEntity = new Qt3DCore::QEntity();

    QColor diffuse(166, 123, 91);
    QColor ambient(120, 85, 60);
    const QString t = m_fallbackModelType;
    QVector3D aiScale(1.0f, 1.0f, 1.0f);
    {
        // Parse simple dimensions like "120x60x30 cm" from IA output
        QRegularExpression re(QStringLiteral("(\\d+(?:[\\.,]\\d+)?)\\D+(\\d+(?:[\\.,]\\d+)?)\\D+(\\d+(?:[\\.,]\\d+)?)"));
        const QRegularExpressionMatch m = re.match(m_fallbackDimensionsText);
        if (m.hasMatch()) {
            auto toD = [](QString s) {
                s.replace(',', '.');
                bool ok = false;
                const double v = s.toDouble(&ok);
                return ok ? v : 0.0;
            };
            const double a = toD(m.captured(1));
            const double b = toD(m.captured(2));
            const double c = toD(m.captured(3));
            if (a > 0.0 && b > 0.0 && c > 0.0) {
                // Normalize around typical furniture cm ranges (120,60,75)
                aiScale = QVector3D(qBound(0.6f, float(a / 120.0), 2.6f),
                                    qBound(0.6f, float(c / 75.0), 2.6f),
                                    qBound(0.6f, float(b / 60.0), 2.6f));
            }
        }
    }
    if (t.contains(QStringLiteral("ARMOIRE")) || t.contains(QStringLiteral("DRESSING"))
        || t.contains(QStringLiteral("BIBLIOTHEQUE")) || t.contains(QStringLiteral("ETAGERE"))) {
        diffuse = QColor(126, 92, 69);
    } else if (t.contains(QStringLiteral("CHAISE")) || t.contains(QStringLiteral("TABOURET"))
               || t.contains(QStringLiteral("FAUTEUIL"))) {
        diffuse = QColor(145, 104, 76);
    } else if (t.contains(QStringLiteral("MEUBLE TV")) || t.contains(QStringLiteral("COMMODE"))
               || t.contains(QStringLiteral("CAISSON"))) {
        diffuse = QColor(158, 114, 84);
    }

    auto addPart = [&](const QVector3D &scale, const QVector3D &translation) {
        auto *part = new Qt3DCore::QEntity(rootEntity);
        auto *mesh = new Qt3DExtras::QCuboidMesh(part);
        auto *transform = new Qt3DCore::QTransform(part);
        auto *material = new Qt3DExtras::QPhongMaterial(part);
        transform->setScale3D(scale);
        transform->setTranslation(translation);
        material->setDiffuse(diffuse);
        material->setAmbient(ambient);
        part->addComponent(mesh);
        part->addComponent(transform);
        part->addComponent(material);
    };

    // Build recognizable furniture silhouettes
    if (t.contains(QStringLiteral("CHAISE")) || t.contains(QStringLiteral("TABOURET"))
        || t.contains(QStringLiteral("FAUTEUIL"))) {
        // Seat
        addPart(QVector3D(0.9f * aiScale.x(), 0.14f * aiScale.y(), 0.9f * aiScale.z()),
                QVector3D(0.0f, 0.55f * aiScale.y(), 0.0f));
        // Backrest
        addPart(QVector3D(0.9f * aiScale.x(), 0.85f * aiScale.y(), 0.14f),
                QVector3D(0.0f, 0.95f * aiScale.y(), -0.38f * aiScale.z()));
        // Legs
        addPart(QVector3D(0.12f, 0.55f * aiScale.y(), 0.12f),
                QVector3D(0.34f * aiScale.x(), 0.22f * aiScale.y(), 0.34f * aiScale.z()));
        addPart(QVector3D(0.12f, 0.55f * aiScale.y(), 0.12f),
                QVector3D(-0.34f * aiScale.x(), 0.22f * aiScale.y(), 0.34f * aiScale.z()));
        addPart(QVector3D(0.12f, 0.55f * aiScale.y(), 0.12f),
                QVector3D(0.34f * aiScale.x(), 0.22f * aiScale.y(), -0.34f * aiScale.z()));
        addPart(QVector3D(0.12f, 0.55f * aiScale.y(), 0.12f),
                QVector3D(-0.34f * aiScale.x(), 0.22f * aiScale.y(), -0.34f * aiScale.z()));
    } else if (t.contains(QStringLiteral("ARMOIRE")) || t.contains(QStringLiteral("DRESSING"))
               || t.contains(QStringLiteral("BIBLIOTHEQUE")) || t.contains(QStringLiteral("ETAGERE"))) {
        // Cabinet body
        addPart(QVector3D(1.2f * aiScale.x(), 2.2f * aiScale.y(), 0.55f * aiScale.z()),
                QVector3D(0.0f, 1.1f * aiScale.y(), 0.0f));
        // Shelf hints
        addPart(QVector3D(1.16f * aiScale.x(), 0.06f, 0.52f * aiScale.z()),
                QVector3D(0.0f, 1.65f * aiScale.y(), 0.0f));
        addPart(QVector3D(1.16f * aiScale.x(), 0.06f, 0.52f * aiScale.z()),
                QVector3D(0.0f, 1.2f * aiScale.y(), 0.0f));
        addPart(QVector3D(1.16f * aiScale.x(), 0.06f, 0.52f * aiScale.z()),
                QVector3D(0.0f, 0.75f * aiScale.y(), 0.0f));
    } else if (t.contains(QStringLiteral("MEUBLE TV")) || t.contains(QStringLiteral("COMMODE"))
               || t.contains(QStringLiteral("CAISSON"))) {
        // Low cabinet
        addPart(QVector3D(1.7f * aiScale.x(), 0.85f * aiScale.y(), 0.7f * aiScale.z()),
                QVector3D(0.0f, 0.42f * aiScale.y(), 0.0f));
        // Top board
        addPart(QVector3D(1.76f * aiScale.x(), 0.08f, 0.76f * aiScale.z()),
                QVector3D(0.0f, 0.86f * aiScale.y(), 0.0f));
        // Feet
        addPart(QVector3D(0.16f, 0.2f * aiScale.y(), 0.16f),
                QVector3D(0.7f * aiScale.x(), 0.1f * aiScale.y(), 0.24f * aiScale.z()));
        addPart(QVector3D(0.16f, 0.2f * aiScale.y(), 0.16f),
                QVector3D(-0.7f * aiScale.x(), 0.1f * aiScale.y(), 0.24f * aiScale.z()));
        addPart(QVector3D(0.16f, 0.2f * aiScale.y(), 0.16f),
                QVector3D(0.7f * aiScale.x(), 0.1f * aiScale.y(), -0.24f * aiScale.z()));
        addPart(QVector3D(0.16f, 0.2f * aiScale.y(), 0.16f),
                QVector3D(-0.7f * aiScale.x(), 0.1f * aiScale.y(), -0.24f * aiScale.z()));
    } else {
        // TABLE (default)
        addPart(QVector3D(2.0f * aiScale.x(), 0.14f, 1.2f * aiScale.z()),
                QVector3D(0.0f, 0.9f * aiScale.y(), 0.0f)); // tabletop
        addPart(QVector3D(0.16f, 0.9f * aiScale.y(), 0.16f),
                QVector3D(0.82f * aiScale.x(), 0.42f * aiScale.y(), 0.42f * aiScale.z()));
        addPart(QVector3D(0.16f, 0.9f * aiScale.y(), 0.16f),
                QVector3D(-0.82f * aiScale.x(), 0.42f * aiScale.y(), 0.42f * aiScale.z()));
        addPart(QVector3D(0.16f, 0.9f * aiScale.y(), 0.16f),
                QVector3D(0.82f * aiScale.x(), 0.42f * aiScale.y(), -0.42f * aiScale.z()));
        addPart(QVector3D(0.16f, 0.9f * aiScale.y(), 0.16f),
                QVector3D(-0.82f * aiScale.x(), 0.42f * aiScale.y(), -0.42f * aiScale.z()));
    }

    auto *lightEntity = new Qt3DCore::QEntity(rootEntity);
    auto *light = new Qt3DRender::QDirectionalLight(lightEntity);
    light->setColor(Qt::white);
    light->setIntensity(1.0f);
    light->setWorldDirection(QVector3D(-1.f, -2.f, -1.f).normalized());
    lightEntity->addComponent(light);

    Qt3DRender::QCamera *camera = view3d->camera();
    camera->lens()->setPerspectiveProjection(45.f, 16.f / 9.f, 0.1f, 1000.f);
    camera->setPosition(QVector3D(3.2f, 2.1f, 4.6f));
    camera->setViewCenter(QVector3D(0.f, 0.7f, 0.f));

    auto *orbit = new Qt3DExtras::QOrbitCameraController(rootEntity);
    orbit->setCamera(camera);
    orbit->setLinearSpeed(50.f);
    orbit->setLookSpeed(180.f);

    view3d->setRootEntity(rootEntity);

    m_windowContainer = QWidget::createWindowContainer(view3d, m_sceneHost);
    m_windowContainer->setMinimumSize(280, 240);
    m_windowContainer->setFocusPolicy(Qt::StrongFocus);

    auto *vl = new QVBoxLayout(m_sceneHost);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(m_windowContainer);
}
