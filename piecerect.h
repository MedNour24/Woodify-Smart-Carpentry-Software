#ifndef PIECERECT_H
#define PIECERECT_H

#include <QString>
#include <QRectF>
#include <QJsonObject>

/**
 * @brief Represents a wooden piece to be placed on a panel
 */
class PieceRect
{
public:
    // Constructors
    PieceRect();
    PieceRect(int id, const QString& name, double length, double width,
              const QString& material = QString(), int priority = 0);

    // Getters
    int id() const { return m_id; }
    QString name() const { return m_name; }
    QString code() const { return m_code; }
    double length() const { return m_length; }
    double width() const { return m_width; }
    double area() const { return m_length * m_width; }
    QString material() const { return m_material; }
    int priority() const { return m_priority; }
    bool canRotate() const { return m_canRotate; }

    // Setters
    void setId(int id) { m_id = id; }
    void setName(const QString& name) { m_name = name; }
    void setCode(const QString& code) { m_code = code; }
    void setLength(double length) { m_length = length; }
    void setWidth(double width) { m_width = width; }
    void setMaterial(const QString& material) { m_material = material; }
    void setPriority(int priority) { m_priority = priority; }
    void setCanRotate(bool canRotate) { m_canRotate = canRotate; }

    // Utility methods
    QRectF toRect() const { return QRectF(0, 0, m_length, m_width); }
    QRectF rotatedRect() const { return QRectF(0, 0, m_width, m_length); }
    bool isValid() const { return m_id > 0 && m_length > 0 && m_width > 0; }

    // Comparison operators for sorting
    bool operator<(const PieceRect& other) const;
    bool operator==(const PieceRect& other) const;

    // JSON serialization
    QJsonObject toJson() const;
    static PieceRect fromJson(const QJsonObject& json);

    // String representation
    QString toString() const;

private:
    int m_id;
    QString m_name;
    QString m_code;
    double m_length;    // in mm
    double m_width;     // in mm
    QString m_material;
    int m_priority;     // 0 = normal, higher = more important
    bool m_canRotate;   // whether piece can be rotated 90 degrees
};

#endif // PIECERECT_H