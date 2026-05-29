#ifndef MODELPREVIEWWIDGET_H
#define MODELPREVIEWWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QMap>
#include <QString>

class ModelPreviewWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ModelPreviewWidget(QWidget *parent = nullptr);

    void setModelImage(const QPixmap &pixmap);
    void addHighlightRegion(const QString &pieceId, const QPolygonF &polygon, const QColor &color);
    void clearRegions();

public slots:
    void highlightRegion(const QString &pieceId);
    void resetHighlight();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsScene *m_scene;
    QGraphicsPixmapItem *m_pixmapItem;
    QMap<QString, QGraphicsPolygonItem*> m_regions;
    
    void fitImage();
};

#endif // MODELPREVIEWWIDGET_H
