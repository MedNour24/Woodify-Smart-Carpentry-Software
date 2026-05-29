#ifndef GENERATEDPIECECARD_H
#define GENERATEDPIECECARD_H

#include <QFrame>
#include <QLabel>
#include <QString>
#include <QMouseEvent>
#include <QEnterEvent>

class GeneratedPieceCard : public QFrame
{
    Q_OBJECT
public:
    explicit GeneratedPieceCard(const QString &pieceId,
                                const QString &type,
                                const QString &name,
                                const QString &dimensions,
                                int quantity,
                                double confidenceScore,
                                const QString &imagePath = QString(),
                                QWidget *parent = nullptr);

    QString id() const;
    QString pieceType() const;
    void setImagePath(const QString &imagePath);

signals:
    void clicked(const QString &pieceId);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QString m_pieceId;
    QString m_type;
    QString m_imagePath;
    
    QLabel *m_imgLabel;
    QLabel *m_nameLabel;
    QLabel *m_dimLabel;
    QLabel *m_badgeLabel;
    
    void setupUi(const QString &name, const QString &dimensions, int quantity, double confidenceScore);
    void updateImageLabel();
};

#endif // GENERATEDPIECECARD_H
