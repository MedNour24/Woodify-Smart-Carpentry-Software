#ifndef ARDUINORANIM_H
#define ARDUINORANIM_H

#include <QObject>
#include <QString>
#include <QSerialPort>

class ArduinoRanim : public QObject
{
    Q_OBJECT

public:
    explicit ArduinoRanim(QObject *parent = nullptr);
    ~ArduinoRanim() override;

    bool start(const QString &portName, int baudRate);
    void stop();
    bool isActive() const;

signals:
    void statusMessage(const QString &message);
    void alarmReceived();
    void normalReceived();
    void serialError(const QString &message);
    void databaseError(const QString &message);

private slots:
    void onReadyRead();
    void onSerialError(QSerialPort::SerialPortError portError);

private:
    void processIncomingLine(const QString &line);
    bool updateAlarmStateInDatabase(bool alarmActive);
    bool sendCommand(const QByteArray &command);

    QSerialPort *m_serialPort;
    QString m_serialBuffer;
    bool m_active;
};

#endif // ARDUINORANIM_H
