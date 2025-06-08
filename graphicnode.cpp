#include "graphicnode.h"
#include <QPen>
#include <QCursor>
#include <iostream>
#include <QGraphicsScene>

GraphicNode::GraphicNode(qreal x, qreal y)
{
    setRect(-25, -25, 75, 75);
    setPos(x, y);
    setBrush(QBrush(Qt::white));
    setPen(QPen(Qt::black, 2));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    booleanNode = nullptr;
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

void GraphicNode::addLine(QGraphicsLineItem *line)
{
    connectedLines.append(line);
}

void GraphicNode::removeLine(QGraphicsLineItem *line)
{
    connectedLines.removeAll(line);
}

void GraphicNode::removeAllLines(QGraphicsScene *scene)
{
    for (auto line : connectedLines) {
        if (line->scene() == scene) {
            scene->removeItem(line);
            delete line;
        }
    }
    connectedLines.clear();
}

void GraphicNode::setLogicNode(BooleanNode* node) {
    this->booleanNode = node;
}

void GraphicNode::setFunctionText(const QString& text) {
    if (!functionLabel) {
        functionLabel = new QGraphicsTextItem(text, this);
        functionLabel->setPos(rect().center().x() - 20, rect().center().y() - 10);
    } else {
        functionLabel->setPlainText(text);
    }
}

