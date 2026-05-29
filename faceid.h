#ifndef FACEID_H
#define FACEID_H

#include <QDialog>

#include <QtCore/QBuffer>
#include <QDebug>
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QMovie>
#include <QPixmap>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QMediaCaptureSession>
#include <QtMultimedia/QMediaDevices>
#include <QtMultimedia/QVideoFrame>
#include <QtMultimedia/QVideoSink>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QUrlQuery>
#include <QList>

namespace Ui {
class faceid;
}

class faceid : public QDialog {
    Q_OBJECT

  public:
    explicit faceid(QWidget *parent = nullptr);
    ~faceid();
    QString getResult() const { return face_token; }
    bool isSad() const {return sadness >= 80;}

  private:
    Ui::faceid *ui;
    QImage frame;
    QTimer *timer;
    int tries = 0;
    QCamera *camera = nullptr;
    QMediaCaptureSession *captureSession = nullptr;
    QVideoSink *videoSink = nullptr;
    QString face_token = "";
    double sadness;
    QMovie *movie;

  private slots:
    void updateCamera();
    bool detectFace(const QImage &image);
    void on_pushButton_released();
};

#endif // FACEID_H
