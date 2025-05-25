#include "graphicnode.h"
#include <QPen>
#include <QCursor>
#include <iostream>

GraphicNode::GraphicNode(qreal x, qreal y)
{
    setRect(-25, -25, 50, 50);
    setPos(x, y);
    setBrush(QBrush(Qt::white));
    setPen(QPen(Qt::black, 2));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void GraphicNode::alternateState()
{
    state = !state;
    updateColor();
}

void GraphicNode::updateColor()
{
    if(state)
        setBrush(QBrush(Qt::green));
    else
        setBrush(QBrush(Qt::lightGray));
}

void GraphicNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    alternateState();
    QGraphicsEllipseItem::mouseDoubleClickEvent(event);
}

void GraphicNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseMoveEvent(event);
}

void GraphicNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(this);
    QGraphicsEllipseItem::mousePressEvent(event);
}

