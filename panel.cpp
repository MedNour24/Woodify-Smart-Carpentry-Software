#include "panel.h"
#include <QJsonDocument>
#include <QDebug>

// Default constructor
Panel::Panel()
    : m_id(0), m_length(0), m_width(0), m_thickness(0), m_quantity(1), m_cost(0)
{
}

// Constructor with parameters
Panel::Panel(int id, const QString& name, double length, double width,
             double thickness, const QString& material)
    : m_id(id), m_name(name), m_length(length), m_width(width),
      m_thickness(thickness), m_material(material), m_quantity(1), m_cost(0)
{
    m_code = QString("PAN%1").arg(id, 3, 10, QChar('0'));
}

// Comparison operators
bool Panel::operator<(const Panel& other) const
{
    // Sort by area (larger first)
    return area() > other.area();
}

bool Panel::operator==(const Panel& other) const
{
    return m_id == other.m_id;
}

// Utility methods
QString Panel::dimensionsString() const
{
    return QString("%1 x %2 mm")
        .arg(QString::number(m_length, 'f', 0))
        .arg(QString::number(m_width, 'f', 0));
}

// JSON serialization
QJsonObject Panel::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["name"] = m_name;
    json["code"] = m_code;
    json["length"] = m_length;
    json["width"] = m_width;
    json["thickness"] = m_thickness;
    json["material"] = m_material;
    json["quantity"] = m_quantity;
    json["cost"] = m_cost;
    return json;
}

Panel Panel::fromJson(const QJsonObject& json)
{
    Panel panel;
    panel.m_id = json["id"].toInt();
    panel.m_name = json["name"].toString();
    panel.m_code = json["code"].toString();
    panel.m_length = json["length"].toDouble();
    panel.m_width = json["width"].toDouble();
    panel.m_thickness = json["thickness"].toDouble();
    panel.m_material = json["material"].toString();
    panel.m_quantity = json["quantity"].toInt(1);
    panel.m_cost = json["cost"].toDouble();
    return panel;
}

// String representation
QString Panel::toString() const
{
    return QString("%1 (%2): %3 x %4 x %5 mm")
        .arg(m_name)
        .arg(m_code)
        .arg(QString::number(m_length, 'f', 0))
        .arg(QString::number(m_width, 'f', 0))
        .arg(QString::number(m_thickness, 'f', 1));
}

// Common panel sizes
Panel Panel::standard2440x1220(int id, const QString& material)
{
    return Panel(id, "Panel Standard 2440x1220", 2440, 1220, 18, material);
}

Panel Panel::standard2500x1250(int id, const QString& material)
{
    return Panel(id, "Panel Standard 2500x1250", 2500, 1250, 18, material);
}

Panel Panel::standard2800x2070(int id, const QString& material)
{
    return Panel(id, "Panel Standard 2800x2070", 2800, 2070, 18, material);
}