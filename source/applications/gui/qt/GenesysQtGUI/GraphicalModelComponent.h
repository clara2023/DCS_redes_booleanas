/*
 * The MIT License
 *
 * Copyright 2022 rlcancian.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * File:   ModelComponentGraphicItem.h
 * Author: rlcancian
 *
 * Created on 16 de fevereiro de 2022, 11:41
 */

#ifndef MODELCOMPONENTGRAPHICITEM_H
#define MODELCOMPONENTGRAPHICITEM_H

#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QPen>
#include <QBrush>
#include "../../../../kernel/simulator/ModelComponent.h"
#include "../../../../kernel/simulator/Plugin.h"
#include "GraphicalComponentPort.h"

class GraphicalModelComponent : public QGraphicsObject { //QGraphicsItem {
public:
	GraphicalModelComponent(Plugin* plugin, ModelComponent* component, QPointF position, QColor color = Qt::blue, QGraphicsItem *parent = nullptr);
	GraphicalModelComponent(const GraphicalModelComponent& orig);
	virtual ~GraphicalModelComponent();
public:
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	ModelComponent* getComponent() const;
	QList<GraphicalComponentPort *> getGraphicalInputPorts() const;
	QList<GraphicalComponentPort *> getGraphicalOutputPorts() const;

protected: // virtual
	virtual bool sceneEvent(QEvent *event) override;
	//virtual void	hoverEnterEvent(QGraphicsSceneHoverEvent * event)
	//virtual void	hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
	//virtual void	hoverMoveEvent(QGraphicsSceneHoverEvent * event)
	//virtual void	inputMethodEvent(QInputMethodEvent * event)
	//virtual QVariant	inputMethodQuery(Qt::InputMethodQuery query) const
	//virtual QVariant	itemChange(GraphicsItemChange change, const QVariant & value)
	//virtual void	keyPressEvent(QKeyEvent * event)
	//virtual void	keyReleaseEvent(QKeyEvent * event)
	//   virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
	//   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	//   virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
	//virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
protected:
	qreal _width = 150;
	qreal _height = _width * 0.67;
	unsigned int _margin = 8;
	unsigned int _selWidth = 8;
	ModelComponent* _component;
	QColor _color;
	qreal _stretchPosTop = 0.5;
	qreal _stretchPosBottom = 0.5;
	qreal _stretchPosLeft = 0.5;
	qreal _stretchPosRigth = 0.5;
	qreal _stretchRigth = 0;
	qreal _stretchLeft = 0;
	qreal _stretchRigthMidle = 0;
	qreal _stretchLeftMidle = 0;
	qreal _stretchTop = 0;
	qreal _stretchBottom = 0;
	qreal _stretchTopMidle = 0;
	qreal _stretchBottomMidle = 0;
private:
	QList<GraphicalComponentPort*> _graphicalInputPorts = QList<GraphicalComponentPort*>();
	QList<GraphicalComponentPort*> _graphicalOutputPorts = QList<GraphicalComponentPort*>();
};

#endif /* MODELCOMPONENTGRAPHICITEM_H */

