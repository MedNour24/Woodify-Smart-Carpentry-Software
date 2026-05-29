#include "arduinoranim.h"
#include "connsql.h"

#include <QSerialPortInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ArduinoRanim::ArduinoRanim(QObject *parent)
    : QObject(parent),
      m_serialPort(new QSerialPort(this)),
      m_active(false)
{
    connect(m_serialPort, &QSerialPort::readyRead, this, &ArduinoRanim::onReadyRead);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &ArduinoRanim::onSerialError);
}

ArduinoRanim::~ArduinoRanim()
{
    stop();
}

bool ArduinoRanim::start(const QString &portName, int baudRate) //ouvre le port série
{
    m_serialBuffer.clear();

    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }

    QString resolvedPort = portName.trimmed();
    if (resolvedPort.isEmpty()) {
        const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        if (ports.isEmpty()) {
            qDebug() << "Erreur ouverture série : aucun port disponible";
            emit serialError(QStringLiteral("Aucun port série disponible."));
            return false;
        }
        resolvedPort = ports.first().portName();
    }

    m_serialPort->setPortName(resolvedPort);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        const QString err = QStringLiteral("Impossible d'ouvrir le port %1 : %2")
                                .arg(resolvedPort, m_serialPort->errorString());
        qDebug() << "Erreur ouverture série :" << err;
        emit serialError(err);
        return false;
    }

    m_active = true;
    qDebug() << "ouverture série OK" << resolvedPort << baudRate;
    emit statusMessage(QStringLiteral("Connexion Arduino établie sur %1").arg(resolvedPort));
    return true;
}

void ArduinoRanim::stop() //ferme le port série.
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
    m_active = false;
}

bool ArduinoRanim::isActive() const
{
    return m_active && m_serialPort->isOpen();
}

void ArduinoRanim::onReadyRead() //lit les données arrivées de l'Arduino.
{
    const QByteArray data = m_serialPort->readAll();
    if (data.isEmpty()) {
        return;
    }

    m_serialBuffer.append(QString::fromUtf8(data));

    int newlineIndex = -1;
    while ((newlineIndex = m_serialBuffer.indexOf('\n')) >= 0) {
        const QString line = m_serialBuffer.left(newlineIndex).trimmed();
        m_serialBuffer.remove(0, newlineIndex + 1);
        if (!line.isEmpty()) {
            processIncomingLine(line);
        }
    }
}
void ArduinoRanim::processIncomingLine(const QString &line) //Si la ligne est ALARME :il met à jour la base avec état actif,
    //Si la ligne est NORMAL :il met à jour la base en état normal,
{
    const QString normalized = line.trimmed().toUpper();
    qDebug() << "Ligne reçue :" << QString("\"%1\"").arg(normalized);

    if (normalized == QStringLiteral("ALARME")) {
        qDebug() << "ALARME reçu";

        // Ne pas bloquer la sécurité incendie si la base échoue.
        updateAlarmStateInDatabase(true);

        qDebug() << "Envoi vers Arduino : DECLENCHE";
        // Commande principale
        sendCommand("DECLENCHE\n");
        // Commandes alternatives pour compatibilité firmware (LCD / buzzer)
        sendCommand("DANGER\n");
        sendCommand("BUZZER_ON\n");
        sendCommand("BUZZERON\n");
        sendCommand("LCD:DANGER\n");

        // Émettre le signal UI **après** l'envoi des commandes pour éviter
        // que la boîte modale bloque l'envoi série.
        emit alarmReceived();
        return;
    }

    if (normalized == QStringLiteral("NORMAL")) {
        qDebug() << "NORMAL reçu";

        // Ne pas bloquer l'arret buzzer si la base échoue.
        updateAlarmStateInDatabase(false);

        qDebug() << "Envoi vers Arduino : STOP";
        // Commande principale
        sendCommand("STOP\n");
        // Commandes alternatives pour compatibilité firmware (LCD / buzzer)
        sendCommand("BUZZER_OFF\n");
        sendCommand("BUZZEROFF\n");
        sendCommand("LCD:NORMAL\n");

        emit normalReceived();
        return;
    }

    emit statusMessage(line);
}

bool ArduinoRanim::updateAlarmStateInDatabase(bool alarmActive) //gère la table INCENDIE.
    //Vérifie la connexion Oracle.
   // Crée une ligne par défaut si la table est vide.
    //En mode alarme :
    //ALARME_ACTIVE = 1
    //DATE_ALARME = SYSDATE
    //TYPE_ALERTE = 'INCENDIE'
   // En mode normal :
    //                 ALARME_ACTIVE = 0
    //                 DATE_ALARME = NULL
     //                TYPE_ALERTE = NULL
{
    ConnSQL &conn = ConnSQL::createInstance();
    QSqlDatabase *dbPtr = conn.getDB();

    if (!dbPtr) {
        const QString message = QStringLiteral("Connexion Oracle invalide (dbPtr null).");
        qDebug() << "DB ouverte ?" << false;
        qDebug() << "ERREUR SQL :" << message;
        emit databaseError(message);
        return false;
    }

    qDebug() << "DB ouverte ?" << dbPtr->isOpen();
    if (!dbPtr->isOpen()) {
        const QString message = QStringLiteral("Connexion Oracle non ouverte.");
        qDebug() << "ERREUR SQL :" << message;
        emit databaseError(QStringLiteral("Connexion Oracle non ouverte."));
        return false;
    }

    if (!dbPtr->transaction()) {
        qDebug() << "ERREUR SQL :" << dbPtr->lastError().text();
        emit databaseError(dbPtr->lastError().text());
        return false;
    }

    QSqlQuery countQuery(*dbPtr);
    if (!countQuery.exec(QStringLiteral("SELECT COUNT(*) FROM \"CARP\".\"INCENDIE\""))) {
        qDebug() << "ERREUR SQL :" << countQuery.lastError().text();
        dbPtr->rollback();
        emit databaseError(countQuery.lastError().text());
        return false;
    }

    int incendieRowCount = 0;
    if (countQuery.next()) {
        incendieRowCount = countQuery.value(0).toInt();
    }
    qDebug() << "Nombre de lignes INCENDIE =" << incendieRowCount;

    if (incendieRowCount == 0) {
        QSqlQuery insertQuery(*dbPtr);
        const QString insertSql = QStringLiteral("INSERT INTO \"CARP\".\"INCENDIE\" (\"IDINCENDIE\", \"ALARME_ACTIVE\") VALUES (1, 0)");
        qDebug() << "SQL :" << insertSql;

        if (!insertQuery.exec(insertSql)) {
            qDebug() << "ERREUR SQL :" << insertQuery.lastError().text();
            dbPtr->rollback();
            emit databaseError(insertQuery.lastError().text());
            return false;
        }

        qDebug() << "Insertion automatique faite : idIncendie = 1";
    }

    QSqlQuery query(*dbPtr);
    const QString sql = alarmActive
        ? QStringLiteral("UPDATE \"CARP\".\"INCENDIE\" "
                         "SET \"ALARME_ACTIVE\" = 1, "
                         "    \"DATE_ALARME\" = SYSDATE, "
                         "    \"TYPE_ALERTE\" = 'INCENDIE' "
                         "WHERE \"IDINCENDIE\" = 1")
        : QStringLiteral("UPDATE \"CARP\".\"INCENDIE\" "
                         "SET \"ALARME_ACTIVE\" = 0, "
                         "    \"DATE_ALARME\" = NULL, "
                         "    \"TYPE_ALERTE\" = NULL "
                         "WHERE \"IDINCENDIE\" = 1");

    qDebug() << "SQL :" << sql;

    if (!query.exec(sql)) {
        qDebug() << "ERREUR SQL :" << query.lastError().text();
        dbPtr->rollback();
        emit databaseError(query.lastError().text());
        return false;
    }

    qDebug() << "Rows affected =" << query.numRowsAffected();
    if (query.numRowsAffected() <= 0) {
        const QString message = QStringLiteral("Aucune ligne mise à jour -> problème de base ou id inexistant");
        qDebug() << message;
        dbPtr->rollback();
        emit databaseError(message);
        return false;
    }

    qDebug() << "UPDATE réussi";

    if (!dbPtr->commit()) {
        qDebug() << "ERREUR COMMIT :" << dbPtr->lastError().text();
        dbPtr->rollback();
        emit databaseError(dbPtr->lastError().text());
        return false;
    }

    qDebug() << "COMMIT réussi";
    return true;
}

bool ArduinoRanim::sendCommand(const QByteArray &command) //envoie une commande texte à l'Arduino.
    //Il ajoute \n si besoin.
    //Il signale une erreur si le port n'est pas ouvert.
{
    if (!m_serialPort->isOpen()) {
        const QString message = QStringLiteral("Port série non ouvert.");
        qDebug() << "Erreur écriture série :" << message;
        emit serialError(message);
        return false;
    }

    QByteArray payload = command;
    if (!payload.endsWith('\n')) {
        payload.append('\n');
    }

    const qint64 bytesWritten = m_serialPort->write(payload);
    if (bytesWritten < 0) {
        const QString message = m_serialPort->errorString();
        qDebug() << "Erreur écriture série :" << message;
        emit serialError(message);
        return false;
    }

    if (!m_serialPort->waitForBytesWritten(250)) {
        const QString message = QStringLiteral("Timeout écriture série.");
        qDebug() << "Erreur écriture série :" << message;
        emit serialError(message);
        return false;
    }

    m_serialPort->flush();
    return true;
}

void ArduinoRanim::onSerialError(QSerialPort::SerialPortError portError) //remonte les erreurs série.
{
    if (portError == QSerialPort::NoError) {
        return;
    }

    const QString err = m_serialPort->errorString();
    if (!err.trimmed().isEmpty()) {
        qDebug() << "Erreur série :" << err;
        emit serialError(err);
    }
}
