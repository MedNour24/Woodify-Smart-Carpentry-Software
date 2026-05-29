#ifndef ARDUINOHANDLER_H
#define ARDUINOHANDLER_H

#include <QObject>
#include <QSerialPort>

class ArduinoHandler : public QObject
{
    Q_OBJECT

public:
    explicit ArduinoHandler(QObject *parent = nullptr);

    void connectToArduino(const QString &portName, int baudRate = 9600);
    void disconnectArduino();
    bool isConnected() const;

signals:
    void dataReceived(QString distance, QString irState);
    void frameReceived(const QString &frame);
    void connectionSucceeded(const QString &portName);
    void connectionFailed(const QString &portName, const QString &errorMessage);
    void disconnected();

private slots:
    void readData();

private:
    QSerialPort *serial;
    QByteArray buffer;
    QString buildUserFriendlyErrorMessage(const QString &portName, 
                                         QSerialPort::SerialPortError errorCode,
                                         const QString &systemError);
};

#endif // ARDUINOHANDLER_H
