#ifndef GRAPHICNODE_H
#define GRAPHICNODE_H

#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

class GraphicNode : public QGraphicsEllipseItem
{
public:
    GraphicNode(qreal x, qreal y, qreal width, qreal height) : QGraphicsEllipseItem(x, y, width, height)
    {
        setBrush(QBrush(Qt::cyan));
        setPen(QPen(Qt::black));
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    }

    void updateColor(bool state)
    {
        if (state){
            setBrush(QBrush(Qt::green));
        } else {
            setBrush(QBrush(Qt::cyan));
        }
    }

    QString getId() const
    {
        return idNode;
    }

private:
    QString idNode;
    QGraphicsItem* text;
};


#endif // GRAPHICNODE_H
