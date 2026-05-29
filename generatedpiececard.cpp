#include "generatedpiececard.h"

#include <QHBoxLayout>
#include <QPixmap>
#include <QStyle>
#include <QVBoxLayout>

GeneratedPieceCard::GeneratedPieceCard(const QString &pieceId,
                                       const QString &type,
                                       const QString &name,
                                       const QString &dimensions,
                                       int quantity,
                                       double confidenceScore,
                                       const QString &imagePath,
                                       QWidget *parent)
    : QFrame(parent), m_pieceId(pieceId), m_type(type), m_imagePath(imagePath)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setCursor(Qt::PointingHandCursor);
    setupUi(name, dimensions, quantity, confidenceScore);
}

void GeneratedPieceCard::setImagePath(const QString &imagePath)
{
    m_imagePath = imagePath;
    updateImageLabel();
}

QString GeneratedPieceCard::id() const
{
    return m_pieceId;
}

QString GeneratedPieceCard::pieceType() const
{
    return m_type;
}

void GeneratedPieceCard::mousePressEvent(QMouseEvent *event)
{
    emit clicked(m_pieceId);
    QFrame::mousePressEvent(event);
}

void GeneratedPieceCard::enterEvent(QEnterEvent *event)
{
    setProperty("hovered", true);
    style()->unpolish(this);
    style()->polish(this);
    QFrame::enterEvent(event);
}

void GeneratedPieceCard::leaveEvent(QEvent *event)
{
    setProperty("hovered", false);
    style()->unpolish(this);
    style()->polish(this);
    QFrame::leaveEvent(event);
}

void GeneratedPieceCard::setupUi(const QString &name, const QString &dimensions, int quantity, double confidenceScore)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(16);

    m_imgLabel = new QLabel(this);
    m_imgLabel->setFixedSize(60, 60);
    m_imgLabel->setAlignment(Qt::AlignCenter);
    m_imgLabel->setStyleSheet("background-color: rgba(8, 16, 38, 0.8); border-radius: 6px; border: 1px solid rgba(255,255,255,0.1);");

    QString color = "#FFFFFF";
    const QString typeLower = m_type.toLower();
    if (typeLower == "pieds" || typeLower == "pied") {
        color = "#FF5252";
    } else if (typeLower == "plateau") {
        color = "#448AFF";
    } else if (typeLower == "traverses" || typeLower == "traverse") {
        color = "#69F0AE";
    } else if (typeLower == "panneau") {
        color = "#FFD740";
    }

    updateImageLabel();

    QVBoxLayout *detailsLayout = new QVBoxLayout();
    detailsLayout->setSpacing(4);

    m_nameLabel = new QLabel(name, this);
    m_nameLabel->setStyleSheet("color: rgba(255,255,255,0.92); font-weight: bold; font-family: 'Segoe UI', Arial, sans-serif; font-size: 14px;");

    m_dimLabel = new QLabel(QString("%1  |  x%2").arg(dimensions).arg(quantity), this);
    m_dimLabel->setStyleSheet("color: #94A3B8; font-family: 'Segoe UI', Arial, sans-serif; font-size: 12px;");

    QLabel *typeLabel = new QLabel(m_type.toUpper(), this);
    typeLabel->setStyleSheet(QString("color: %1; font-size: 10px; font-weight: bold; letter-spacing: 1px;").arg(color));

    detailsLayout->addWidget(m_nameLabel);
    detailsLayout->addWidget(m_dimLabel);
    detailsLayout->addWidget(typeLabel);
    detailsLayout->addStretch();

    QVBoxLayout *rightLayout = new QVBoxLayout();
    m_badgeLabel = new QLabel(QString::number(confidenceScore, 'f', 0) + "%", this);
    m_badgeLabel->setAlignment(Qt::AlignCenter);

    const QString badgeColor = confidenceScore >= 90
        ? "rgba(76, 175, 80, 0.2)"
        : (confidenceScore >= 70 ? "rgba(255, 152, 0, 0.2)" : "rgba(244, 67, 54, 0.2)");
    const QString badgeText = confidenceScore >= 90 ? "#4CAF50" : (confidenceScore >= 70 ? "#FF9800" : "#F44336");
    m_badgeLabel->setStyleSheet(QString(
        "background-color: %1; color: %2; border: 1px solid %3; border-radius: 10px; padding: 3px 8px; font-weight: bold; font-size: 11px;"
    ).arg(badgeColor).arg(badgeText).arg(badgeColor));

    rightLayout->addWidget(m_badgeLabel);
    rightLayout->setAlignment(m_badgeLabel, Qt::AlignTop | Qt::AlignRight);
    rightLayout->addStretch();

    mainLayout->addWidget(m_imgLabel);
    mainLayout->addLayout(detailsLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(rightLayout);

    setStyleSheet(
        "GeneratedPieceCard {"
        "  background-color: rgba(255,255,255,0.03);"
        "  border: 1px solid rgba(99,102,241,0.15);"
        "  border-radius: 8px;"
        "}"
        "GeneratedPieceCard[hovered=\"true\"] {"
        "  background-color: rgba(99,102,241,0.1);"
        "  border: 1px solid #6366f1;"
        "}"
    );
    setFixedHeight(84);
}

void GeneratedPieceCard::updateImageLabel()
{
    QPixmap piecePixmap;
    if (!m_imagePath.isEmpty()) {
        piecePixmap.load(m_imagePath);
    }

    if (!piecePixmap.isNull()) {
        m_imgLabel->clear();
        m_imgLabel->setPixmap(piecePixmap.scaled(m_imgLabel->size(),
                                                 Qt::KeepAspectRatioByExpanding,
                                                 Qt::SmoothTransformation));
    } else {
        QString color = "#FFFFFF";
        const QString typeLower = m_type.toLower();
        if (typeLower == "pieds" || typeLower == "pied") {
            color = "#FF5252";
        } else if (typeLower == "plateau") {
            color = "#448AFF";
        } else if (typeLower == "traverses" || typeLower == "traverse") {
            color = "#69F0AE";
        } else if (typeLower == "panneau") {
            color = "#FFD740";
        }

        m_imgLabel->setText(QString("<span style='color:%1; font-size:24px;'>&#9632;</span>").arg(color));
        m_imgLabel->setPixmap(QPixmap());
    }
}
