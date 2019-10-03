#include "edge.h"

edge::edge(double x1, double y1, double x2, double y2) : QGraphicsLineItem(x1, y1, x2, y2, 0)
{
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
    stroke = new QPen;
    //stroke->setBrush(*color);
    stroke->setWidth(2);
    this->setZValue(-1);
    this->setPen(*stroke);
}

edge::edge()
{
    stroke = new QPen;
    stroke->setWidth(2);
    this->setZValue(-10);
    this->setPen(*stroke);
}

void edge::setPos(double x1, double y1, double x2, double y2)
{
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
}

QPen *edge::getPen()
{
    return stroke;
}
