#ifndef GRAPHICSWITCH_H
#define GRAPHICSWITCH_H

#include <QPushButton>
#include <QGraphicsLineItem>
#include <QList>
#include <QGraphicsScene>
#include <QPointF>

class GraphicSwitch : public QPushButton
{
    Q_OBJECT

public:
    explicit GraphicSwitch(QWidget *parent = nullptr);

    void toggleState();
    void updateAppearance();

    void addLine(QGraphicsLineItem* line);
    void removeAllLines(QGraphicsScene* scene);

    QPointF getAnchorPoint(QGraphicsView* graphicsView) const;

signals:
    void clicked(GraphicSwitch*);

private:
    QList<QGraphicsLineItem*> lines;
};

#endif // GRAPHICSWITCH_H
