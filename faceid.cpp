#include "faceid.h"
#include "ui_faceid.h"

#include <QProcessEnvironment>

namespace {
QString resolveFaceppApiKey()
{
    return QProcessEnvironment::systemEnvironment().value(QStringLiteral("FACEPP_API_KEY")).trimmed();
}

QString resolveFaceppApiSecret()
{
    return QProcessEnvironment::systemEnvironment().value(QStringLiteral("FACEPP_API_SECRET")).trimmed();
}
}

faceid::faceid(QWidget *parent) : QDialog(parent), ui(new Ui::faceid)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    sadness = 0;

    movie = new QMovie(this);

    timer = new QTimer(this);

    captureSession = new QMediaCaptureSession(this);
    videoSink = new QVideoSink(this);
    camera = new QCamera(QMediaDevices::defaultVideoInput(), this);
    captureSession->setCamera(camera);
    captureSession->setVideoSink(videoSink);

    connect(videoSink, &QVideoSink::videoFrameChanged, this, [this](const QVideoFrame &videoFrame) {
        if (videoFrame.isValid()) {
            frame = videoFrame.toImage();
        }
    });

    connect(timer, &QTimer::timeout, this, [this]() {
        updateCamera();
        ++tries;
        if (frame.isNull()) {
            if (tries >= 3) {
                QMessageBox::warning(this, "Error", "No face detected! Please contact your administrator...");
                reject();
            }
            return;
        }

        if (detectFace(frame)) {
            if (movie) {
                movie->stop();
            }
            if (camera) {
                camera->stop();
            }
            accept();
        } else if (tries >= 3) {
            if (movie) {
                movie->stop();
            }
            if (camera) {
                camera->stop();
            }
            QMessageBox::warning(this, "Error", "No face detected! Please contact your administrator...");
            reject();
        }
    });

    camera->start();
    timer->start(2000);

    if (ui->label) {
        ui->label->setText("Starting camera...");
        ui->label->setAlignment(Qt::AlignCenter);
    }
    if (ui->pushButton) {
        ui->pushButton->setText("Close");
    }

    if (ui->pushButton) {
        connect(ui->pushButton, &QPushButton::released, this, &faceid::on_pushButton_released);
    }
}

faceid::~faceid()
{
    if (camera) {
        camera->stop();
    }
    delete ui;
}

void faceid::updateCamera()
{
    if (ui->label && !frame.isNull()) {
        ui->label->setPixmap(QPixmap::fromImage(frame).scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

bool faceid::detectFace(const QImage &image)
{
    const QString apiKey = resolveFaceppApiKey();
    const QString apiSecret = resolveFaceppApiSecret();
    if (apiKey.isEmpty() || apiSecret.isEmpty()) {
        return false;
    }

    QByteArray imageBytes;
    QBuffer buffer(&imageBytes);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    QUrl url("https://api-us.faceplusplus.com/facepp/v3/detect");
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("form-data; name=\"image_file\"; filename=\"image.png\""));
    imagePart.setBody(imageBytes);

    QHttpPart apiKeyPart;
    apiKeyPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                         QVariant("form-data; name=\"api_key\""));
    apiKeyPart.setBody(apiKey.toUtf8());

    QHttpPart apiSecretPart;
    apiSecretPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                            QVariant("form-data; name=\"api_secret\""));
    apiSecretPart.setBody(apiSecret.toUtf8());

    QHttpPart apiRetAtt;
    apiRetAtt.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"return_attributes\""));
    apiRetAtt.setBody("emotion");

    multiPart->append(imagePart);
    multiPart->append(apiKeyPart);
    multiPart->append(apiSecretPart);
    multiPart->append(apiRetAtt);

    QNetworkRequest request(url);
    QNetworkAccessManager manager(this);
    QNetworkReply *reply = manager.post(request, multiPart);
    multiPart->setParent(reply);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    const QByteArray response = reply->readAll();
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    if (jsonDoc.isEmpty()) {
        qWarning() << "Error: Empty JSON response";
        return false;
    }

    const QJsonObject jsonObj = jsonDoc.object();
    const QJsonArray facesArr = jsonObj.value("faces").toArray();
    if (facesArr.isEmpty()) {
        qWarning() << "Error: No faces detected";
        return false;
    }

    const QJsonObject faceObj = facesArr.first().toObject();
    face_token = faceObj.value("face_token").toString();
    const QJsonObject attributes = faceObj.value("attributes").toObject();
    const QJsonObject emotions = attributes.value("emotion").toObject();
    sadness = emotions.value("sadness").toDouble();
    return true;
}

void faceid::on_pushButton_released()
{
    if (timer) {
        timer->stop();
    }
    if (camera) {
        camera->stop();
    }
    reject();
}
