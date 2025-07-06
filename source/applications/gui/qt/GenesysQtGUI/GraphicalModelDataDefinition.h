#ifndef GRAPHICALMODELDATADEFINITION_H
#define GRAPHICALMODELDATADEFINITION_H


#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QPen>
#include <QBrush>
#include "../../../../kernel/simulator/ModelComponent.h"
#include "../../../../kernel/simulator/Plugin.h"

class GraphicalModelDataDefinition : public QGraphicsObject {
public:
	GraphicalModelDataDefinition(Plugin* plugin, ModelComponent* component, qreal x, qreal y, QColor color = Qt::blue, QGraphicsItem *parent = nullptr);
	GraphicalModelDataDefinition(const GraphicalModelDataDefinition& orig);
	virtual ~GraphicalModelDataDefinition();
public:
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	ModelComponent* getComponent() const;
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
};

#endif // GRAPHICALMODELDATADEFINITION_H
