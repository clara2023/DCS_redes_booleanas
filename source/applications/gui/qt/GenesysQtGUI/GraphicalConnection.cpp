#include "GraphicalConnection.h"
#include "GraphicalModelComponent.h"
#include <QPainter>

GraphicalConnection::GraphicalConnection(GraphicalComponentPort* sourceGraphicalPort, GraphicalComponentPort* destinationGraphicalPort, QColor color, QGraphicsItem *parent) : QGraphicsObject(parent) {
	//// connect in the model
	//ModelComponent* sourceComponent = sourceGraphicalPort->graphicalComponent()->getComponent();
	//ModelComponent* destComponent=destinationGraphicalPort->graphicalComponent()->getComponent();
	//_sourceConnection = new Connection({sourceComponent,sourceGraphicalPort->portNum()});
	//_destinationConnection = new Connection({destComponent,destinationGraphicalPort->portNum()});
	//sourceComponent->getConnections()->insertAtRank(sourceGraphicalPort->portNum(), _destinationConnection);
	// connect graphically
	_sourceGraphicalPort = sourceGraphicalPort;
	_destinationGraphicalPort = destinationGraphicalPort;
	_sourceConnection = new Connection({sourceGraphicalPort->graphicalComponent()->getComponent(), sourceGraphicalPort->portNum()});
	_destinationConnection = new Connection({_destinationGraphicalPort->graphicalComponent()->getComponent(), _destinationGraphicalPort->portNum()});
	_color = color;
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
	setAcceptHoverEvents(true);
	setAcceptTouchEvents(true);
	setActive(true);
	setSelected(false);
	updateDimensionsAndPosition();
	//update source and dest PORTS
	sourceGraphicalPort->addGraphicalConnection(this); // to update connection on port position change
	destinationGraphicalPort->addGraphicalConnection(this);
}

GraphicalConnection::GraphicalConnection(const GraphicalConnection& orig) {

}

GraphicalConnection::~GraphicalConnection() {
	_sourceConnection->component->getConnections()->remove(_destinationConnection);
	_sourceGraphicalPort->removeGraphicalConnection(this);
	_destinationGraphicalPort->removeGraphicalConnection(this);
}

void GraphicalConnection::updateDimensionsAndPosition() {
	qreal x1, x2, y1, y2, w1, w2, h1, h2;
	x1 = _sourceGraphicalPort->scenePos().x();
	x2 = _destinationGraphicalPort->scenePos().x();
	y1 = _sourceGraphicalPort->scenePos().y();
	y2 = _destinationGraphicalPort->scenePos().y();
	w1 = _sourceGraphicalPort->width();
	h1 = _sourceGraphicalPort->height();
	w2 = _destinationGraphicalPort->width();
	h2 = _destinationGraphicalPort->height();
	setPos((x1 < x2 ? x1 + w1 : x2 + w2) - 2/*penwidth*/, y1 < y2 ? y1 : y2);
	_width = abs(x2 - x1)-(x1 < x2 ? w2 : w1);
	_height = abs(y2 - y1)+ (y1 < y2 ? h2 : h1);
	update(); //@TODO SHould not call it here
}

QRectF GraphicalConnection::boundingRect() const {
	return QRectF(0, 0, _width, _height);
}

void GraphicalConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	updateDimensionsAndPosition();
	QPen pen = QPen(_color);
	pen.setWidth(2);
	painter->setPen(pen);
	QPainterPath path;
	QPointF inipos;
	QPointF endpos;

	qreal x1, x2, y1, y2;
	if (_sourceGraphicalPort->scenePos().x() < _destinationGraphicalPort->scenePos().x()) {
		x1 = 0; //_sourceGraphicalPort->width();
		x2 = _width; //-_sourceGraphicalPort->width();
	} else {
		x2 = 0; //_sourceGraphicalPort->width();
		x1 = _width; //-_sourceGraphicalPort->width();
	}
	if (_sourceGraphicalPort->scenePos().y() < _destinationGraphicalPort->scenePos().y()) {
		y1 = _sourceGraphicalPort->height() / 2.0;
		y2 = _height - _sourceGraphicalPort->height() / 2.0;
	} else {
		y2 = _sourceGraphicalPort->height() / 2.0;
		y1 = _height - _sourceGraphicalPort->height() / 2.0;
	}
	inipos = QPointF(x1, y1); //QPointF(_sourceGraphicalPort->pos());//_sourceGraphicalPort->pos().x()+_sourceGraphicalPort->width()/2.0, _sourceGraphicalPort->pos().y()+_sourceGraphicalPort->height()/2.0
	endpos = QPointF(x2, y2); //QPointF(_destinationGraphicalPort->pos());// _destinationGraphicalPort->pos().x()+_destinationGraphicalPort->width()/2.0, _destinationGraphicalPort->pos().y()+_destinationGraphicalPort->height()/2.0
	path.moveTo(inipos);
	path.lineTo((x1 + x2) / 2, y1);
	path.lineTo((x1 + x2) / 2, y2);
	path.lineTo(endpos);
	painter->drawPath(path);
	//
	if (isSelected()) {
		pen = QPen(Qt::black);
		pen.setWidth(1);
		painter->setPen(pen);
		QBrush brush = QBrush(Qt::SolidPattern);
		brush.setColor(Qt::black);
		painter->setBrush(brush);
		painter->drawRect(QRectF(x1 < x2 ? x1 : x1 - _selWidth, y1 - _selWidth / 2, _selWidth, _selWidth));
		painter->drawRect(QRectF(x2 < x1 ? x2 : x2 - _selWidth, y2 - _selWidth / 2, _selWidth, _selWidth));
		painter->drawRect(QRectF((x1 + x2) / 2 - _selWidth / 2, y1 - _selWidth / 2, _selWidth, _selWidth));
		painter->drawRect(QRectF((x1 + x2) / 2 - _selWidth / 2, y2 - _selWidth / 2, _selWidth, _selWidth));
	}
	//
	//pen = QPen(Qt::yellow);
	//pen.setWidth(1);
	//painter->setPen(pen);
	//painter->drawRect(QRectF(0,0,_width-1,_height-1));
}

Connection* GraphicalConnection::getSource() const {
	return _sourceConnection;
}

Connection* GraphicalConnection::getDestination() const {
	return _destinationConnection;
}

bool GraphicalConnection::sceneEvent(QEvent *event) {
	QGraphicsObject::sceneEvent(event);
}
