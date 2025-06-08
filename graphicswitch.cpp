#include "graphicswitch.h"
#include <QStyle>
#include <QPoint>
#include <QRect>
#include <QDebug>
#include <QGraphicsView>

GraphicSwitch::GraphicSwitch(QWidget *parent)
    : QPushButton("0", parent)
{
    setFixedSize(50, 50);
    setCheckable(true);
    updateAppearance();

    connect(this, &QPushButton::clicked, this, [=]() {
        emit clicked(this);
    });
}

void GraphicSwitch::toggleState()
{
    setChecked(!isChecked());
    updateAppearance();
}

void GraphicSwitch::updateAppearance()
{
    if (isChecked()) {
        setText("1");
        setStyleSheet("background-color: green; color: white;");
        state = true;
    } else {
        setText("0");
        setStyleSheet("");
        state = false;
    }
}

void GraphicSwitch::addLine(QGraphicsLineItem *line)
{
    lines.append(line);
}

void GraphicSwitch::removeAllLines(QGraphicsScene *scene)
{
    for (QGraphicsLineItem* line : lines) {
        if (scene) {
            scene->removeItem(line);
        }
        delete line;
    }
    lines.clear();
}

QPointF GraphicSwitch::getAnchorPoint(QGraphicsView* graphicsView) const
{
    QPoint globalCenter = mapToGlobal(rect().center());
    QPoint viewCenter = graphicsView->mapFromGlobal(globalCenter);
    return graphicsView->mapToScene(viewCenter);
}


