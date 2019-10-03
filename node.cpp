#include "node.h"

node::node(double X, double Y, QGraphicsItem *parent, double width, double height) : QGraphicsEllipseItem(X-width/2, -Y-height/2, width, height, parent)
{
    x = X;
    y = Y;
    force.x = 0.0;
    force.y = 0.0;
    pen.setWidth(thickness);
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    label = new QGraphicsTextItem(this);
    name = QString("");
    label->setPlainText(name);
    label->setPos(x, y);
    label->setTextWidth(20);
    label->setDefaultTextColor(Qt::black);
    this->setZValue(100);
    this->setBrush(brush);
    this->setPen(pen);
}

node::node() : QGraphicsEllipseItem(10.0, 10.0, radius, radius, 0)
{
    x = 0.0;
    y = 0.0;
    force.x = 0.0;
    force.y = 0.0;
    pen.setWidth(thickness);
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    label = new QGraphicsTextItem(this);
    name = QString("");
    label->setPlainText(name);
    label->setPos(x+radius/2, y+radius/2);
    label->setTextWidth(20);
    label->setDefaultTextColor(Qt::black);
    this->setBrush(brush);
    this->setPen(pen);
}

void node::setName(QString s)
{
    name = s;
    label->setPlainText(name);
}

void node::setPos(double x, double y)
{
    this->x = x;
    this->y = y;
    QGraphicsEllipseItem::setPos(amp * x, amp * y);
}

void node::setForce(double x, double y)
{
    force.x = x;
    force.y = y;
}

double node::distance(node &n)
{
    return sqrt(pow(this->x - n.x,2) + pow(this->y - n.y,2));
}
