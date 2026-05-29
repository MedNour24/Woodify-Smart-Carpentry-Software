#ifndef ARDUINOMANAGER_H
#define ARDUINOMANAGER_H

#include <QObject>
#include <QSerialPort>

class ArduinoManager : public QObject
{
    Q_OBJECT
public:
    explicit ArduinoManager(QObject *parent = nullptr);
    ~ArduinoManager();

    // Tente de se connecter à l'Arduino
    bool connectToArduino();
    void disconnectFromArduino();
    
    // Status
    bool isConnected() const;

signals:
    // Signal émis quand une plaque est scannée
    void boardScanned(double length, double width, double thickness);
    void connectionStatusChanged(bool isConnected, const QString& message);

private slots:
    void onReadyRead();

private:
    QSerialPort* m_serialPort;
    QString m_arduinoPortName;
    bool m_isConnected;
    QByteArray m_buffer;

    void parseBuffer();
};

#endif // ARDUINOMANAGER_H
