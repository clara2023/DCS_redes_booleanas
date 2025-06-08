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

    void alternateState();
    void updateColor();

    void setLogicNode(BooleanNode* booleanNode);
    void setFunctionText(const QString& text);

    BooleanNode* getBooleanNode() const { return booleanNode; }
    QString getId() const;

    void addLine(QGraphicsLineItem* line);
    void removeLine(QGraphicsLineItem* line);
    void removeAllLines(QGraphicsScene* line);

    QList<QGraphicsLineItem*> connectedLines;
    bool state = false;

signals:
    void clicked(GraphicNode* node);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QString idNode;
    QGraphicsTextItem* functionLabel = nullptr;
    BooleanNode* booleanNode;
};


#endif // GRAPHICNODE_H
