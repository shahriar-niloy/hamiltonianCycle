#ifndef EDGE_H
#define EDGE_H
#include<QGraphicsLineItem>
#include<QPen>
#include<QBrush>
#define radius 25

class edge : public QGraphicsLineItem
{
private:
    QBrush *color;
    QPen *stroke;
public:
    double x1, x2, y1, y2;

    edge(double x1, double y1, double x2, double y2);
    edge();
    void setPos(double x1, double y1, double x2, double y2);
    QPen* getPen();
};

#endif // EDGE_H
