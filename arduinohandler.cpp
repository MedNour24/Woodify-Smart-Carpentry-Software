#include "arduinohandler.h"
#include <QDebug>
#include <QThread>

ArduinoHandler::ArduinoHandler(QObject *parent)
    : QObject(parent)
{
    serial = new QSerialPort(this);

    connect(serial, &QSerialPort::readyRead,
            this, &ArduinoHandler::readData);
}

void ArduinoHandler::connectToArduino(const QString &portName, int baudRate)
{
    if (portName.trimmed().isEmpty()) {
        emit connectionFailed(portName, "Port COM vide ou invalide");
        qDebug() << "[ArduinoHandler] Erreur: Port vide";
        return;
    }

    // Close existing connection if any
    if (serial->isOpen()) {
        serial->close();
    }

    serial->setPortName(portName);
    serial->setBaudRate(baudRate);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // Réessayer jusqu'à 5 fois en cas d'erreur de permission (port bloqué)
    const int maxRetries = 5;
    int retryCount = 0;

    while (retryCount < maxRetries) {
        // Toujours fermer avant de réessayer (même si déjà fermé)
        if (serial->isOpen()) {
            serial->close();
            QThread::msleep(300);
        }
        
        if (serial->open(QIODevice::ReadWrite)) {
            qDebug() << "[ArduinoHandler] Connecté au port" << portName << "à" << baudRate << "baud après" << retryCount << "tentative(s)";
            emit connectionSucceeded(portName);
            return;
        }

        const QSerialPort::SerialPortError errorCode = serial->error();
        
        // Si c'est une erreur de permission et qu'on n'a pas atteint le max, attendre et réessayer
        if (errorCode == QSerialPort::PermissionError && retryCount < maxRetries - 1) {
            qDebug() << "[ArduinoHandler] Port bloqué, tentative" << (retryCount + 1) << "- attente 1500ms...";
            QThread::msleep(1500);  // Attendre 1.5 secondes pour laisser Windows libérer le port
            retryCount++;
        } else {
            // Erreur différente ou dernier réessai échoué
            QString userFriendlyError = buildUserFriendlyErrorMessage(portName, errorCode, serial->errorString());
            qDebug() << "[ArduinoHandler] Erreur connexion sur" << portName << "-" << userFriendlyError;
            emit connectionFailed(portName, userFriendlyError);
            return;
        }
    }
}

QString ArduinoHandler::buildUserFriendlyErrorMessage(const QString &portName, 
                                                      QSerialPort::SerialPortError errorCode,
                                                      const QString &systemError)
{
    switch (errorCode) {
    case QSerialPort::PermissionError:
        return QString(
            "❌ Accès refusé au port %1\n\n"
            "Solutions:\n"
            "• Fermez les logiciels utilisant COM (IDE Arduino, moniteur série, etc.)\n"
            "• Vérifiez que COM%2 n'est pas utilisé par un autre programme\n"
            "• Relancez l'application en mode Administrateur\n"
            "• Débranchez et rebranchez l'Arduino"
        ).arg(portName, portName.mid(3)); // Extrait le numéro de "COM6"
        
    case QSerialPort::OpenError:
        return QString(
            "❌ Port %1 introuvable ou inaccessible\n\n"
            "Solutions:\n"
            "• Vérifiez que l'Arduino est branché (regardez les LEDs)\n"
            "• Ouvrez le Gestionnaire des périphériques:\n"
            "  - Clic droit 'Ce PC' → Gérer → Gestionnaire des périphériques\n"
            "  - Cherchez le port COM dans 'Ports (COM et LPT)'\n"
            "• Installez les drivers USB pour votre Arduino si nécessaire"
        ).arg(portName);
        
    case QSerialPort::TimeoutError:
        return QString("⏱️ Délai d'attente dépassé pour la connexion à %1").arg(portName);
        
    case QSerialPort::UnsupportedOperationError:
        return QString(
            "❌ Opération non supportée sur %1\n\n"
            "Cela peut survenir si:\n"
            "• Le port n'est pas un port série valide\n"
            "• C'est un port virtuel corrompu\n"
            "• Le format du nom est incorrect (utilisez COM6, pas /dev/ttyUSB0)"
        ).arg(portName);
        
    default:
        return QString(
            "❌ Erreur de connexion à %1\n\n"
            "Détail technique: %2\n\n"
            "Solutions générales:\n"
            "• Relancez l'application\n"
            "• Débranchez et rebranchez l'Arduino\n"
            "• Essayez un autre port COM (si plusieurs dispos)"
        ).arg(portName, systemError);
    }
}

bool ArduinoHandler::isConnected() const
{
    return serial && serial->isOpen();
}

void ArduinoHandler::disconnectArduino()
{
    if (serial->isOpen()) {
        serial->close();
        qDebug() << "[ArduinoHandler] Déconnecté du port";
        emit disconnected();
    }
}

void ArduinoHandler::readData()
{
    buffer.append(serial->readAll());
    // Bluetooth / Windows : souvent \r\n ; certains peripheriques : \r seul
    buffer.replace("\r\n", "\n");
    buffer.replace('\r', '\n');

    while (buffer.contains('\n')) {
        const int nl = buffer.indexOf('\n');
        QByteArray rawLine = buffer.left(nl);
        buffer.remove(0, nl + 1);

        const QString line = QString::fromUtf8(rawLine).trimmed();
        if (line.isEmpty()) {
            continue;
        }

        emit frameReceived(line);

        const QStringList parts = line.split(QLatin1Char('|'));
        if (parts.size() == 2) {
            const QString distance = parts[0].trimmed();
            const QString ir = parts[1].trimmed();
            emit dataReceived(distance, ir);
        }
    }

    const int kMaxSerialBuffer = 65536;
    if (buffer.size() > kMaxSerialBuffer) {
        qWarning() << "[ArduinoHandler] Tampon serie trop volumineux sans fin de ligne — purge.";
        buffer.clear();
    }
}
