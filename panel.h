#ifndef PANEL_H
#define PANEL_H

#include <QString>
#include <QRectF>
#include <QJsonObject>

/**
 * @brief Represents a wooden panel/sheet for cutting pieces
 */
class Panel
{
public:
    // Constructors
    Panel();
    Panel(int id, const QString& name, double length, double width,
          double thickness, const QString& material = QString());

    // Getters
    int id() const { return m_id; }
    QString name() const { return m_name; }
    QString code() const { return m_code; }
    double length() const { return m_length; }
    double width() const { return m_width; }
    double thickness() const { return m_thickness; }
    double area() const { return m_length * m_width; }
    QString material() const { return m_material; }
    int quantity() const { return m_quantity; }
    double cost() const { return m_cost; }

    // Setters
    void setId(int id) { m_id = id; }
    void setName(const QString& name) { m_name = name; }
    void setCode(const QString& code) { m_code = code; }
    void setLength(double length) { m_length = length; }
    void setWidth(double width) { m_width = width; }
    void setThickness(double thickness) { m_thickness = thickness; }
    void setMaterial(const QString& material) { m_material = material; }
    void setQuantity(int quantity) { m_quantity = quantity; }
    void setCost(double cost) { m_cost = cost; }

    // Utility methods
    QRectF toRect() const { return QRectF(0, 0, m_length, m_width); }
    bool isValid() const { return m_id > 0 && m_length > 0 && m_width > 0; }
    QString dimensionsString() const;

    // Comparison operators
    bool operator<(const Panel& other) const;
    bool operator==(const Panel& other) const;

    // JSON serialization
    QJsonObject toJson() const;
    static Panel fromJson(const QJsonObject& json);

    // String representation
    QString toString() const;

    // Common panel sizes
    static Panel standard2440x1220(int id = 1, const QString& material = "MDF");
    static Panel standard2500x1250(int id = 1, const QString& material = "MDF");
    static Panel standard2800x2070(int id = 1, const QString& material = "Contreplaqué");

private:
    int m_id;
    QString m_name;
    QString m_code;
    double m_length;    // in mm
    double m_width;     // in mm
    double m_thickness; // in mm
    QString m_material;
    int m_quantity;     // available quantity
    double m_cost;      // cost per panel
};

#endif // PANEL_H