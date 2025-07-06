#include "GraphicalComponentPort.h"
#include "GraphicalConnection.h"
#include "GraphicalModelComponent.h"
#include <QPainter>

GraphicalComponentPort::GraphicalComponentPort(GraphicalModelComponent* componentGraph, bool isInputPort, unsigned int portNum, QGraphicsItem *parent) : QGraphicsObject(parent) {
	//_component = component;
	//_color = color;
	//_color.setAlpha(64);
	_isInputPort = isInputPort;
	_portNum = portNum;
	_componentGraph = componentGraph;
	if (!isInputPort) {
		_width *= 1.15;
		_height *= 1.15;
	}
	//setPos(0,0);
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
	setAcceptHoverEvents(true);
	setAcceptTouchEvents(true);
	setActive(true);
	setSelected(false);
	QString text = QString::fromStdString(componentGraph->getComponent()->getName()) + "\n";
	if (isInputPort)
		text += "Input Port";
	else
		text += "Output Port";
	text += " " + QString::fromStdString(std::to_string(portNum));
	setToolTip(text);
}

//-------------------------
// PROTECTED VIRTUAL FUNCTIONS
//-------------------------

/*
void GraphicalComponentPort::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsObject::mousePressEvent(mouseEvent);
}

void GraphicalComponentPort::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	QGraphicsObject::mouseReleaseEvent(mouseEvent);
}

void GraphicalComponentPort::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	QGraphicsObject::mouseMoveEvent(mouseEvent);
}
 */

QVariant GraphicalComponentPort::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {
	QVariant res = QGraphicsObject::itemChange(change, value);
	if (change == QGraphicsItem::ItemPositionChange || QGraphicsItem::ItemScenePositionHasChanged) {
		for (GraphicalConnection* graphconnection : *_connections) {
			graphconnection->updateDimensionsAndPosition();
			//graphconnection->update();//updateDimensions();
		}
	}
	return res;
}


//----------------------
// overrides
//----------------------

QRectF GraphicalComponentPort::boundingRect() const {
	//qreal penWidth = _pen.width();
	//return QRectF(penWidth / 2, penWidth / 2, _width + penWidth, _height + penWidth);
	return QRectF(0, 0, _width, _height);
}

void GraphicalComponentPort::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	int _penWidth = 1;
	int _iwidth = _width - 2 * _margin - _penWidth;
	int _iheight = _height - 2 * _margin - _penWidth;
	QPen pen;
	QBrush brush;
	QRect rect;
	QPainterPath path;
	QPainterPath path2;
	// pen
	pen = QPen(Qt::black);
	pen.setWidth(_penWidth);
	pen.setJoinStyle(Qt::RoundJoin);
	painter->setPen(pen);
	if (_isInputPort) {
		// Path raised
		brush = QBrush(Qt::SolidPattern);
		brush.setColor(Qt::darkGray);
		painter->setBrush(brush);
		path.moveTo(_margin, _margin);
		path.lineTo(_margin, _margin + _iheight);
		path.lineTo(_margin + _raise, _margin + _iheight - _raise);
		path.lineTo(_margin + _raise, _margin + _raise);
		path.lineTo(_margin, _margin);
		path.lineTo(_margin + _iwidth, _margin);
		path.lineTo(_margin + _iwidth - _raise, _margin + _raise);
		path.lineTo(_margin + _raise, _margin + _raise);
		painter->drawPath(path);
		// path stunken
		brush = QBrush(Qt::SolidPattern);
		brush.setColor(Qt::white);
		painter->setBrush(brush);
		path2.moveTo(_margin + _iwidth, _margin + _iheight);
		path2.lineTo(_margin + _iwidth, _margin);
		path2.lineTo(_margin + _iwidth - _raise, _margin + _raise);
		path2.lineTo(_margin + _iwidth - _raise, _margin + _iheight - _raise);
		path2.lineTo(_margin + _raise, _margin + _iheight - _raise);
		path2.lineTo(_margin, _margin + _iheight);
		path2.lineTo(_margin + _iwidth, _margin + _iheight);
		path2.lineTo(_margin + _iwidth - _raise, _margin + _iheight - _raise);
		painter->drawPath(path2);
		// fill
		QColor _color = QColor(0, 0, 0, 25);
		//brush = QBrush(Qt::NoBrush);
		if (_connections->size() > 0) {
			_color = QColor(0, 0, 0, 225); //QColor(255,0,0,225);
		}
		brush = QBrush(Qt::SolidPattern);
		brush.setColor(_color);
		painter->setBrush(brush);
		rect = QRect(_margin + _raise, _margin + _raise, _margin + _iwidth - 2 * _raise - _margin, _margin + _iheight - 2 * _raise - _margin);
		painter->drawRect(rect);
	} else {
		// output port
		// Path raised
		brush = QBrush(Qt::SolidPattern);
		brush.setColor(Qt::darkGray);
		painter->setBrush(brush);
		path.moveTo(_margin, _margin);
		path.lineTo(_margin, _margin + _iheight);
		path.lineTo(_margin + _raise, _margin + _iheight - 2 * _raise);
		path.lineTo(_margin + _raise, _margin + 2 * _raise);
		path.lineTo(_margin, _margin);
		path.lineTo(_margin + _iwidth, _margin + _iheight / 2.0);
		path.lineTo(_margin + _iwidth - 2 * _raise, _margin + _iheight / 2.0);
		path.lineTo(_margin + _raise, _margin + 2 * _raise);
		painter->drawPath(path);
		// path stunken
		brush = QBrush(Qt::SolidPattern);
		brush.setColor(Qt::white);
		painter->setBrush(brush);
		path2.moveTo(_margin + _iwidth, _margin + _iheight / 2.0);
		path2.lineTo(_margin + _iwidth - 1 - _raise, _margin + _iheight / 2.0);
		path2.lineTo(_margin + _raise, _margin + _iheight - 1 - _raise);
		path2.lineTo(_margin, _margin + _iheight);
		path2.lineTo(_margin + _iwidth, _margin + _iheight / 2.0);
		painter->drawPath(path2);
		// fill
		QColor _color = QColor(0, 0, 0, 25);
		//brush = QBrush(Qt::NoBrush);
		if (_connections->size() > 0) {
			_color = QColor(0, 0, 0, 225); //QColor(255,0,0,225);
		}
		brush = QBrush(Qt::SolidPattern);
		brush.setColor(_color);
		painter->setBrush(brush);
		QPainterPath path3;
		path3.moveTo(_margin + _raise, _margin + 1 + _raise);
		path3.lineTo(_margin + _iwidth - 1 - _raise, _margin + _iheight / 2.0);
		path3.lineTo(_margin + _raise, _margin + _iheight - 1 - _raise);
		painter->drawPath(path3);
	}
	if (this->isSelected()) { //draw squares on corners
		brush = QBrush(Qt::NoBrush);
		brush.setColor(QColor(0, 0, 0, 255));
		painter->setBrush(brush);
		rect = QRect(0, 0, _width - _penWidth, _height - _penWidth);
		painter->drawRect(rect);
	}
}

qreal GraphicalComponentPort::width() const {
	return _width;
}

qreal GraphicalComponentPort::height() const {
	return _height;
}


//---------------------

void GraphicalComponentPort::addGraphicalConnection(GraphicalConnection* connection) {
	_connections->append(connection);
}

void GraphicalComponentPort::removeGraphicalConnection(GraphicalConnection* connection) {
	_connections->removeOne(connection);
}


//---------------------

unsigned int GraphicalComponentPort::portNum() const {
	return _portNum;
}

bool GraphicalComponentPort::isInputPort() const {
	return _isInputPort;
}

GraphicalModelComponent *GraphicalComponentPort::graphicalComponent() const {
	return _componentGraph;
}

QList<GraphicalConnection*>*GraphicalComponentPort::getConnections() const {
	return _connections;
}
