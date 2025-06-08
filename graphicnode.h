#ifndef GRAPHICNODE_H
#define GRAPHICNODE_H

#include "graphicswitch.h"

#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

class BooleanNode;

class GraphicNode : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    GraphicNode(qreal x, qreal y);
    void setLogicNode(BooleanNode* booleanNode);
    QList<QGraphicsLineItem*> connectedLines;
    bool state = false;

    BooleanNode* getBooleanNode() const { return booleanNode; }
    void alternateState();
    void updateColor();
    QString getId() const;

    void addLine(QGraphicsLineItem* line);
    void removeLine(QGraphicsLineItem* line);
    void removeAllLines(QGraphicsScene* line);

signals:
    void clicked(GraphicNode* node);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QString idNode;
    QGraphicsItem* text;
    BooleanNode* booleanNode;
};


#endif // GRAPHICNODE_H
