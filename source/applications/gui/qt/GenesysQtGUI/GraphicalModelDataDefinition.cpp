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
 * File:   ModelComponentGraphicItem.cpp
 * Author: rlcancian
 *
 * Created on 16 de fevereiro de 2022, 11:41
 */

#include "GraphicalModelDataDefinition.h"
#include "GraphicalComponentPort.h"
#include <QPainter>

GraphicalModelDataDefinition::GraphicalModelDataDefinition(Plugin* plugin, ModelComponent* component, qreal x, qreal y, QColor color, QGraphicsItem *parent) : QGraphicsObject(parent) {
	// parent =
	_component = component;
	_color = color;
	_color.setAlpha(128);
	setPos(x, y);
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
	setAcceptHoverEvents(true);
	setAcceptTouchEvents(true);
	setActive(true);
	setSelected(false);
	setToolTip(QString::fromStdString(component->getName()));
	GraphicalComponentPort* port;
	qreal px, py = 0;
	qreal step = (double) _height / (double) (plugin->getPluginInfo()->getMinimumInputs() + 1);
	/*
	for (unsigned int i=0; i< plugin->getPluginInfo()->getMinimumInputs(); i++) {
	port= new GraphicalComponentPort(this, true, i, parent);
	port->setX(0);
	py += step;
	port->setY(py-port->height()/2);
	port->setParentItem(this);
	}
	py = 0;
	step = (double)_height / (double)(plugin->getPluginInfo()->getMinimumOutputs()+1);
	for (unsigned int i=0; i< plugin->getPluginInfo()->getMinimumOutputs(); i++) {
	port= new GraphicalComponentPort(this, false, i, parent);
	port->setX(this->_width - port->width());
	py += step ;
	port->setY(py-port->height()/2);
	port->setParentItem(this);
	}
	 */
}

GraphicalModelDataDefinition::GraphicalModelDataDefinition(const GraphicalModelDataDefinition& orig) {
}

GraphicalModelDataDefinition::~GraphicalModelDataDefinition() {
	_component->~ModelComponent();
}

QRectF GraphicalModelDataDefinition::boundingRect() const {
	//qreal penWidth = _pen.width();
	//return QRectF(penWidth / 2, penWidth / 2, _width + penWidth, _height + penWidth);
	return QRectF(0, 0, _width, _height);
}

void GraphicalModelDataDefinition::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	/*
	int _penWidth =1;
	int _raise = 5;
	int _iwidth = _width-2*_margin-_penWidth;
	int _iheight = _height-2*_margin-_penWidth;
	qreal pw = _penWidth+0.5;
	QPen pen;
	QBrush brush;
	QRect rect;
	// pen
	pen = QPen(Qt::black);
	pen.setWidth(_penWidth);
	painter->setPen(pen);
	// Path raised
	brush = QBrush(Qt::SolidPattern);
	brush.setColor(Qt::white);
	painter->setBrush(brush);
	QPainterPath path;
	path.moveTo(_margin,_margin);
	path.lineTo(_margin, _margin+_iheight);
	path.lineTo(_margin+_raise, _margin+_iheight-_raise);
	path.lineTo(_margin+_raise, _margin+_raise);
	path.lineTo(_margin,_margin);
	path.lineTo(_margin+_iwidth,_margin);
	path.lineTo(_margin+_iwidth-_raise,_margin+_raise);
	path.lineTo(_margin+_raise,_margin+_raise);
	painter->drawPath(path);
	// path stunken
	brush = QBrush(Qt::SolidPattern);
	brush.setColor(Qt::darkGray);
	painter->setBrush(brush);
	QPainterPath path2;
	path2.moveTo(_margin+_iwidth,_margin+_iheight);
	path2.lineTo(_margin+_iwidth,_margin);
	path2.lineTo(_margin+_iwidth-_raise,_margin+_raise);
	path2.lineTo(_margin+_iwidth-_raise,_margin+_iheight-_raise);
	path2.lineTo(_margin+_raise,_margin+_iheight-_raise);
	path2.lineTo(_margin,_margin+_iheight);
	path2.lineTo(_margin+_iwidth,_margin+_iheight);
	path2.lineTo(_margin+_iwidth-_raise,_margin+_iheight-_raise);
	painter->drawPath(path2);
	// fill
	brush = QBrush(Qt::SolidPattern);
	brush.setColor(_color);
	painter->setBrush(brush);
	rect = QRect(_margin+_raise, _margin+_raise, _margin+_iwidth-2*_raise-_margin, _margin+_iheight-2*_raise-_margin);
	painter->drawRect(rect);
	// text
	brush = QBrush(Qt::NoBrush);
	painter->setBrush(brush);
	pen = QPen(Qt::white);
	painter->setPen(pen);
	QString text = QString::fromStdString(_component->getName());
	QRect rect2 = QRect(_margin+_raise+1, _margin+_raise+1, _margin+_iwidth-2*_raise-_margin, _margin+_iheight-2*_raise-_margin);
	painter->drawText(rect2, Qt::AlignCenter, text);
	pen = QPen(Qt::black);
	painter->setPen(pen);
	painter->drawText(rect, Qt::AlignCenter, text);
	//
	if (isSelected()) { //draw squares on corners
	brush = QBrush(Qt::SolidPattern);
	brush.setColor(QColor(0, 0, 0, 255));
	painter->setBrush(brush);
	rect = QRect(0,0,_selWidth,_selWidth);
	painter->drawRect(rect);
	rect = QRect(_width -_selWidth,0,_selWidth,_selWidth);
	painter->drawRect(rect);
	rect = QRect(0,_height -_selWidth,_selWidth,_selWidth);
	painter->drawRect(rect);
	rect = QRect(_width -_selWidth, _height-_selWidth,_selWidth,_selWidth);
	painter->drawRect(rect);
	}
	 */
}

ModelComponent* GraphicalModelDataDefinition::getComponent() const {
	return _component;
}

bool GraphicalModelDataDefinition::sceneEvent(QEvent *event) {
	QGraphicsObject::sceneEvent(event); // Unnecessary
}

/*
void ModelComponentGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event){

}
void ModelComponentGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event){

}

void ModelComponentGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent * event){

}
 */
