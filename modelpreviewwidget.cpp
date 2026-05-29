#include "modelpreviewwidget.h"
#include <QResizeEvent>
#include <QPen>
#include <QTimer>

ModelPreviewWidget::ModelPreviewWidget(QWidget *parent)
    : QGraphicsView(parent), m_scene(new QGraphicsScene(this)), m_pixmapItem(nullptr)
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    
    // Styling the view
    setStyleSheet("QGraphicsView { border: 1px solid #333740; border-radius: 8px; background-color: #1A1C23; }");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setInteractive(false);
}

void ModelPreviewWidget::setModelImage(const QPixmap &pixmap)
{
    if (m_pixmapItem) {
        m_scene->removeItem(m_pixmapItem);
        delete m_pixmapItem;
    }
    
    m_pixmapItem = m_scene->addPixmap(pixmap);
    m_pixmapItem->setZValue(-1); // Sent to back
    fitImage();
}

void ModelPreviewWidget::addHighlightRegion(const QString &pieceId, const QPolygonF &polygon, const QColor &color)
{
    QGraphicsPolygonItem *polyItem = new QGraphicsPolygonItem(polygon);
    
    QColor fillColor = color;
    fillColor.setAlpha(90); // Translucent fill
    polyItem->setBrush(fillColor);
    
    QPen pen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    polyItem->setPen(pen);
    polyItem->setZValue(1);
    polyItem->hide(); // Hidden by default
    
    m_scene->addItem(polyItem);
    m_regions.insert(pieceId, polyItem);
}

void ModelPreviewWidget::clearRegions()
{
    for (auto item : m_regions.values()) {
        m_scene->removeItem(item);
        delete item;
    }
    m_regions.clear();
}

void ModelPreviewWidget::highlightRegion(const QString &pieceId)
{
    // Hide all
    for (auto item : m_regions.values()) {
        item->hide();
    }
    
    // Show specific
    if (m_regions.contains(pieceId)) {
        m_regions[pieceId]->show();
    }
}

void ModelPreviewWidget::resetHighlight()
{
    for (auto item : m_regions.values()) {
        item->hide();
    }
}

void ModelPreviewWidget::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    // Use timer to post-pone fitImage until layout is complete
    QTimer::singleShot(10, this, &ModelPreviewWidget::fitImage);
}

void ModelPreviewWidget::fitImage()
{
    if (m_pixmapItem && !m_pixmapItem->pixmap().isNull()) {
        fitInView(m_pixmapItem, Qt::KeepAspectRatio);
        setSceneRect(m_pixmapItem->boundingRect());
    }
}
