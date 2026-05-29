#include "arduinomanager.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QRegularExpression>

ArduinoManager::ArduinoManager(QObject *parent)
    : QObject(parent), m_serialPort(new QSerialPort(this)), m_isConnected(false)
{
    connect(m_serialPort, &QSerialPort::readyRead, this, &ArduinoManager::onReadyRead);
}

ArduinoManager::~ArduinoManager()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
}

bool ArduinoManager::connectToArduino()
{
    // Recherche automatique du port COM de l'Arduino
    m_arduinoPortName = "";
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        // En général l'Arduino UNO a l'un de ces vendor/product ID
        // On connectera le premier port disponible trouvé s'il n'y a pas de filtrage.
        if (info.hasVendorIdentifier() && info.hasProductIdentifier()) {
            if (info.vendorIdentifier() == 9025 || info.vendorIdentifier() == 6790) { // PID standard Arduino Uno
                m_arduinoPortName = info.portName();
                break;
            }
        }
    }

    if (m_arduinoPortName.isEmpty()) {
        // Si non trouvé, on tente de se connecter au premier port pour tester/simuler
        if (!infos.isEmpty()) {
            m_arduinoPortName = infos.first().portName();
        } else {
            emit connectionStatusChanged(false, "Aucun Arduino/Port Série détecté");
            return false;
        }
    }

    m_serialPort->setPortName(m_arduinoPortName);
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serialPort->open(QSerialPort::ReadOnly)) {
        m_isConnected = true;
        emit connectionStatusChanged(true, "Connecté à Arduino sur " + m_arduinoPortName);
        return true;
    } else {
        m_isConnected = false;
        emit connectionStatusChanged(false, "Échec connexion sur " + m_arduinoPortName);
        return false;
    }
}

void ArduinoManager::disconnectFromArduino()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        m_isConnected = false;
        emit connectionStatusChanged(false, "Déconnecté de l'Arduino");
    }
}

bool ArduinoManager::isConnected() const
{
    return m_isConnected;
}

void ArduinoManager::onReadyRead()
{
    m_buffer.append(m_serialPort->readAll());
    parseBuffer();
}

void ArduinoManager::parseBuffer()
{
    // On cherche une trame complète entourée de < et > (ex: <150,80,2>)
    int startIdx = m_buffer.indexOf('<');
    int endIdx = m_buffer.indexOf('>');

    while (startIdx != -1 && endIdx != -1) {
        if (endIdx > startIdx) {
            // Extraire la chaîne interne
            QByteArray payload = m_buffer.mid(startIdx + 1, endIdx - startIdx - 1);
            QString dataStr = QString::fromLatin1(payload).trimmed();
            
            // Parser les valeurs (ex: "150,80,2")
            QStringList parts = dataStr.split(',');
            if (parts.size() == 3) {
                bool okL, okW, okT;
                double length = parts[0].toDouble(&okL);
                double width = parts[1].toDouble(&okW);
                double thickness = parts[2].toDouble(&okT);

                if (okL && okW && okT) {
                    emit boardScanned(length, width, thickness);
                } else {
                    qDebug() << "[ArduinoManager] Erreur parsing données :" << dataStr;
                }
            }
            
            // Retirer la trame traitée du buffer
            m_buffer.remove(0, endIdx + 1);
        } else {
            // Trames entremêlées, nettoyer jusqu'au endIdx
            m_buffer.remove(0, endIdx + 1);
        }
        
        // Chercher la suivante
        startIdx = m_buffer.indexOf('<');
        endIdx = m_buffer.indexOf('>');
    }
}
