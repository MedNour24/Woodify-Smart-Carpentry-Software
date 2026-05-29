#include "piecerect.h"
#include <QJsonDocument>
#include <QDebug>

// Default constructor
PieceRect::PieceRect()
    : m_id(0), m_length(0), m_width(0), m_priority(0), m_canRotate(true)
{
}

// Constructor with parameters
PieceRect::PieceRect(int id, const QString& name, double length, double width,
                     const QString& material, int priority)
    : m_id(id), m_name(name), m_length(length), m_width(width),
      m_material(material), m_priority(priority), m_canRotate(true)
{
    m_code = QString("P%1").arg(id, 3, 10, QChar('0'));
}

// Comparison operators
bool PieceRect::operator<(const PieceRect& other) const
{
    // Sort by priority (higher first), then by area (larger first)
    if (m_priority != other.m_priority) {
        return m_priority > other.m_priority;
    }
    return area() > other.area();
}

bool PieceRect::operator==(const PieceRect& other) const
{
    return m_id == other.m_id;
}

// JSON serialization
QJsonObject PieceRect::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["name"] = m_name;
    json["code"] = m_code;
    json["length"] = m_length;
    json["width"] = m_width;
    json["material"] = m_material;
    json["priority"] = m_priority;
    json["canRotate"] = m_canRotate;
    return json;
}

PieceRect PieceRect::fromJson(const QJsonObject& json)
{
    PieceRect piece;
    piece.m_id = json["id"].toInt();
    piece.m_name = json["name"].toString();
    piece.m_code = json["code"].toString();
    piece.m_length = json["length"].toDouble();
    piece.m_width = json["width"].toDouble();
    piece.m_material = json["material"].toString();
    piece.m_priority = json["priority"].toInt();
    piece.m_canRotate = json["canRotate"].toBool(true);
    return piece;
}

// String representation
QString PieceRect::toString() const
{
    return QString("%1 (%2): %3 x %4 mm")
        .arg(m_name)
        .arg(m_code)
        .arg(QString::number(m_length, 'f', 1))
        .arg(QString::number(m_width, 'f', 1));
}