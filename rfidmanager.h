#pragma once
#include <QObject>
#include <QSerialPort>
#include <QDateTime>

struct AccessLog {
    QDateTime timestamp;
    QString   uid;
    bool      granted;
    QString   employeeName;
    QString   eventType;  // "IN" or "OUT"
    int       employeeId; // 0 if unknown
};

enum class AccessType {
    IN_EVENT,
    OUT_EVENT,
    UNKNOWN
};

class RfidManager : public QObject {
    Q_OBJECT
public:
    explicit RfidManager(QObject *parent = nullptr);

    void connectToPort(const QString &portName);  // e.g. "COM3" or "/dev/ttyUSB0"
    void disconnect();
    void setAuthorizedCard(const QString &uid);
    bool isConnected() const;
    AccessType recordAttendance(const QString &cardUid, int employeeId = 0);
    AccessType getLastAccessType(const QString &cardUid);
    bool assignCardToEmployee(const QString &cardUid, int employeeId);
    QString resolveEmployeeNameForCard(const QString &cardUid, int *employeeId = nullptr);

signals:
    void statusChanged(const QString &status);   // "READY" | "OPEN" | "CLOSED"
    void accessEvent(const AccessLog &log);
    void cardUpdated(const QString &uid);
    void connectionChanged(bool connected);

private slots:
    void onDataReceived();

private:
    QSerialPort        *m_serial;
    QList<AccessLog>    m_logs;
    QString             m_pendingData;
    QString             m_pendingUid;
    QString             m_lastEmittedUid;
    bool                m_lastEmittedGranted = false;
    QDateTime           m_lastEmittedAt;
    void parseLine(const QString &line);
    void emitAccessLog(const AccessLog &log);
};
