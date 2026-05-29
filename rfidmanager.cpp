#include "rfidmanager.h"
#include "connsql.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>

RfidManager::RfidManager(QObject *parent)
    : QObject(parent)
    , m_serial(new QSerialPort(this))
{
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);

    connect(m_serial, &QSerialPort::readyRead, this, &RfidManager::onDataReceived);
}

void RfidManager::connectToPort(const QString &portName) {
    m_serial->setPortName(portName);
    if (m_serial->open(QIODevice::ReadWrite)) {
        m_pendingData.clear();
        m_pendingUid.clear();
        emit connectionChanged(true);
    }
}

void RfidManager::disconnect() {
    m_serial->close();
    m_pendingData.clear();
    m_pendingUid.clear();
    emit connectionChanged(false);
}

void RfidManager::setAuthorizedCard(const QString &uid) {
    if (!m_serial->isOpen()) return;
    QString cmd = "SET_CARD:" + uid.trimmed().toUpper() + "\n";
    m_serial->write(cmd.toUtf8());
}

bool RfidManager::isConnected() const {
    return m_serial->isOpen();
}

void RfidManager::onDataReceived() {
    const QByteArray rawBytes = m_serial->readAll();
    if (!rawBytes.isEmpty()) {
        qDebug().noquote() << "[RFID RAW BYTES]" << rawBytes.toHex(' ');
    }

    m_pendingData += QString::fromUtf8(rawBytes);

    int separatorIndex = -1;
    while ((separatorIndex = m_pendingData.indexOf(QRegularExpression("[\r\n]"))) >= 0) {
        QString line = m_pendingData.left(separatorIndex).trimmed();
        m_pendingData.remove(0, separatorIndex + 1);

        if (!line.isEmpty()) {
            qDebug().noquote() << "[RFID RAW LINE]" << line;
            parseLine(line);
        }
    }
}

void RfidManager::parseLine(const QString &line) {
    const QString trimmedLine = line.trimmed();
    const QString normalizedLine = trimmedLine.toUpper();
    const QString asciiLine = trimmedLine.normalized(QString::NormalizationForm_D)
                                   .remove(QRegularExpression(QStringLiteral("\\p{Mn}+")))
                                   .toUpper();

    if (trimmedLine.startsWith("STATUS:")) {
        emit statusChanged(trimmedLine.mid(7));  // "READY", "OPEN", "CLOSED"

    } else if (trimmedLine.startsWith("UID", Qt::CaseInsensitive)) {
        const int separatorIndex = trimmedLine.indexOf(':');
        if (separatorIndex >= 0) {
            m_pendingUid = trimmedLine.mid(separatorIndex + 1).trimmed().toUpper();
        }

    } else if (trimmedLine.startsWith("ACCESS:")) {
        QStringList parts = trimmedLine.split(":");
        // parts[0]=ACCESS  parts[1]=GRANTED/DENIED  parts[2..]=UID bytes
        if (parts.size() >= 3) {
            AccessLog log;
            log.timestamp = QDateTime::currentDateTime();
            log.granted   = (parts[1].trimmed().compare("GRANTED", Qt::CaseInsensitive) == 0);
            // UID may contain ":" so rejoin from index 2
            log.uid = parts.mid(2).join(":").trimmed().toUpper();
            emitAccessLog(log);
        }

    } else if (asciiLine.contains("ACCES ACCORDE")
               || asciiLine.contains("ACCESS GRANTED")
               || asciiLine.contains("ACCORD")
               || asciiLine.contains("GRANT")
               || asciiLine.contains("OUVERT")) {
        AccessLog log;
        log.timestamp = QDateTime::currentDateTime();
        log.granted = true;
        log.uid = m_pendingUid;
        emitAccessLog(log);
        qDebug().noquote() << "[RFID GRANTED]" << "Line:" << line << "UID:" << m_pendingUid;
        m_pendingUid.clear();

    } else if (asciiLine.contains("ACCES REFUSE")
               || asciiLine.contains("CARTE INCONNUE")
               || asciiLine.contains("ACCESS DENIED")
               || asciiLine.contains("REFUSE")
               || asciiLine.contains("INCONNUE")
               || asciiLine.contains("DENIED")) {
        AccessLog log;
        log.timestamp = QDateTime::currentDateTime();
        log.granted = false;
        log.uid = m_pendingUid;
        emitAccessLog(log);
        qDebug().noquote() << "[RFID DENIED]" << "Line:" << line << "UID:" << m_pendingUid;
        m_pendingUid.clear();

    } else if (trimmedLine.startsWith("CMD:CARD_UPDATED:")) {
        emit cardUpdated(trimmedLine.mid(17));
    } else {
        static const QRegularExpression uidPattern("^(?:[0-9A-Fa-f]{2}:){3,7}[0-9A-Fa-f]{2}$");
        if (uidPattern.match(trimmedLine).hasMatch()) {
            qDebug().noquote() << "[RFID UID DIRECT]" << trimmedLine;
            AccessLog log;
            log.timestamp = QDateTime::currentDateTime();
            log.granted = true;
            log.uid = trimmedLine.toUpper();
            emitAccessLog(log);
        }
    }
}

void RfidManager::emitAccessLog(const AccessLog &log) {
    const QString normalizedUid = log.uid.trimmed().toUpper();
    if (!normalizedUid.isEmpty()
        && normalizedUid == m_lastEmittedUid
        && log.granted == m_lastEmittedGranted
        && m_lastEmittedAt.isValid()
        && m_lastEmittedAt.msecsTo(log.timestamp) >= 0
        && m_lastEmittedAt.msecsTo(log.timestamp) < 1500) {
        qDebug().noquote() << "[RFID DUPLICATE IGNORED]" << normalizedUid;
        return;
    }

    AccessLog normalizedLog = log;
    normalizedLog.uid = normalizedUid;
    m_lastEmittedUid = normalizedUid;
    m_lastEmittedGranted = log.granted;
    m_lastEmittedAt = log.timestamp;
    m_logs.append(normalizedLog);
    emit accessEvent(normalizedLog);
}

AccessType RfidManager::recordAttendance(const QString &cardUid, int employeeId) {
    ConnSQL &db = ConnSQL::createInstance();
    if (!db.getDB()->isOpen()) {
        qWarning() << "Database not connected for attendance recording";
        return AccessType::UNKNOWN;
    }

    const QString normalizedUid = cardUid.trimmed().toUpper();
    if (normalizedUid.isEmpty()) {
        qWarning() << "Cannot record attendance without a card UID";
        return AccessType::UNKNOWN;
    }

    QSqlDatabase *database = db.getDB();
    if (!database->transaction()) {
        qWarning() << "Failed to start RFID attendance transaction:" << database->lastError().text();
        return AccessType::UNKNOWN;
    }

    QSqlQuery stateQuery(*database);
    stateQuery.prepare("SELECT NVL(ISINSIDE, 0), NVL(ID_EMPLOYEE, 0) "
                       "FROM RFID_CARDS "
                       "WHERE CARD_UID = :uid AND NVL(STATUS, 'ACTIVE') = 'ACTIVE' "
                       "FOR UPDATE");
    stateQuery.addBindValue(normalizedUid);

    if (!stateQuery.exec() || !stateQuery.next()) {
        qWarning() << "Cannot find active RFID card state:" << stateQuery.lastError().text();
        database->rollback();
        return AccessType::UNKNOWN;
    }

    const bool isInside = (stateQuery.value(0).toInt() == 1);
    const int cardEmployeeId = stateQuery.value(1).toInt();
    const int resolvedEmployeeId = employeeId > 0 ? employeeId : cardEmployeeId;
    const AccessType newType = isInside ? AccessType::OUT_EVENT : AccessType::IN_EVENT;
    const QString eventType = (newType == AccessType::IN_EVENT) ? "IN" : "OUT";
    const int newInsideValue = (newType == AccessType::IN_EVENT) ? 1 : 0;

    QSqlQuery query(*database);
    query.prepare("INSERT INTO RFID_ACCESS_LOGS (ID_LOG, CARD_UID, ID_EMPLOYEE, ACCESS_STATUS, ACCESS_TIME) "
                  "SELECT NVL(MAX(ID_LOG), 0) + 1, :uid, :emp, :type, SYSTIMESTAMP "
                  "FROM RFID_ACCESS_LOGS");
    query.addBindValue(normalizedUid);
    query.addBindValue(resolvedEmployeeId > 0 ? resolvedEmployeeId : QVariant());
    query.addBindValue(eventType);

    if (!query.exec()) {
        qWarning() << "Failed to record attendance:" << query.lastError().text();
        database->rollback();
        return AccessType::UNKNOWN;
    }

    QSqlQuery updateQuery(*database);
    updateQuery.prepare("UPDATE RFID_CARDS SET ISINSIDE = :inside WHERE CARD_UID = :uid");
    updateQuery.addBindValue(newInsideValue);
    updateQuery.addBindValue(normalizedUid);

    if (!updateQuery.exec()) {
        qWarning() << "Failed to update RFID inside state:" << updateQuery.lastError().text();
        database->rollback();
        return AccessType::UNKNOWN;
    }

    if (!database->commit()) {
        qWarning() << "Failed to commit RFID attendance transaction:" << database->lastError().text();
        database->rollback();
        return AccessType::UNKNOWN;
    }

    return newType;
}

AccessType RfidManager::getLastAccessType(const QString &cardUid) {
    ConnSQL &db = ConnSQL::createInstance();
    if (!db.getDB()->isOpen()) {
        qWarning() << "Database not connected for access type query";
        return AccessType::UNKNOWN;
    }

    QSqlQuery query(*db.getDB());
    query.prepare("SELECT NVL(ISINSIDE, 0) FROM RFID_CARDS WHERE CARD_UID = :uid");
    query.addBindValue(cardUid.trimmed().toUpper());

    if (query.exec() && query.next()) {
        return (query.value(0).toInt() == 1) ? AccessType::IN_EVENT : AccessType::OUT_EVENT;
    }

    return AccessType::UNKNOWN;
}

bool RfidManager::assignCardToEmployee(const QString &cardUid, int employeeId) {
    ConnSQL &db = ConnSQL::createInstance();
    if (!db.getDB()->isOpen()) {
        qWarning() << "Database not connected for card assignment";
        return false;
    }

    // Try UPDATE first
    QSqlQuery updateQuery(*db.getDB());
    updateQuery.prepare("UPDATE RFID_CARDS SET ID_EMPLOYEE = :emp, STATUS = 'ACTIVE' WHERE CARD_UID = :uid");
    updateQuery.addBindValue(employeeId);
    updateQuery.addBindValue(cardUid);

    if (updateQuery.exec()) {
        if (updateQuery.numRowsAffected() > 0) {
            qDebug() << "Card" << cardUid << "assigned to employee" << employeeId;
            return true;
        }

        // If no rows affected, INSERT new card
        QSqlQuery insertQuery(*db.getDB());
        insertQuery.prepare("INSERT INTO RFID_CARDS (ID_CARD, CARD_UID, ID_EMPLOYEE, STATUS, ISSUE_DATE) "
                           "SELECT NVL(MAX(ID_CARD), 0) + 1, :uid, :emp, 'ACTIVE', TRUNC(SYSDATE) "
                           "FROM RFID_CARDS");
        insertQuery.addBindValue(cardUid.trimmed().toUpper());
        insertQuery.addBindValue(employeeId);

        if (insertQuery.exec()) {
            qDebug() << "New card" << cardUid << "created for employee" << employeeId;
            return true;
        } else {
            qWarning() << "Failed to insert new card:" << insertQuery.lastError().text();
            return false;
        }
    } else {
        qWarning() << "Failed to update card:" << updateQuery.lastError().text();
        return false;
    }
}

QString RfidManager::resolveEmployeeNameForCard(const QString &cardUid, int *employeeId) {
    ConnSQL &db = ConnSQL::createInstance();
    if (!db.getDB()->isOpen()) {
        qWarning() << "Database not connected for employee lookup";
        if (employeeId) {
            *employeeId = 0;
        }
        return QString();
    }

    QSqlQuery query(*db.getDB());
    query.prepare(
        "SELECT c.ID_EMPLOYEE, NVL(e.NOM, ''), NVL(e.PRENOM, '') "
        "FROM RFID_CARDS c "
        "LEFT JOIN EMPLOYEE e ON e.IDEMPLOYEE = c.ID_EMPLOYEE "
        "WHERE c.CARD_UID = :uid");
    query.addBindValue(cardUid.trimmed().toUpper());

    if (!query.exec() || !query.next()) {
        if (employeeId) {
            *employeeId = 0;
        }
        return QString();
    }

    const int resolvedEmployeeId = query.value(0).toInt();
    if (employeeId) {
        *employeeId = resolvedEmployeeId;
    }

    const QString nom = query.value(1).toString().trimmed();
    const QString prenom = query.value(2).toString().trimmed();
    const QString fullName = QString("%1 %2").arg(prenom, nom).trimmed();
    if (!fullName.isEmpty()) {
        return fullName;
    }

    if (resolvedEmployeeId > 0) {
        return QString("Employé #%1").arg(resolvedEmployeeId);
    }

    return QString();
}
