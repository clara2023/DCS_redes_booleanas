#ifndef GRAPHICNODE_H
#define GRAPHICNODE_H

#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

class GraphicNode : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    GraphicNode(qreal x, qreal y);
    bool state = false;

    void alternateState();
    void updateColor();

    QString getId() const;

signals:
    void clicked(GraphicNode* node);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QString idNode;
    QGraphicsItem* text;
};


#endif // GRAPHICNODE_H
