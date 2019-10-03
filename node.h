#ifndef NODE_H
#define NODE_H
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QString>
#include <list>
#include <cmath>
#define radius 25
#define amp 35

struct Force{
    double x;
    double y;
};

class node : public QGraphicsEllipseItem{
private:

public:
    double x;
    double y;
    int thickness = 2;
    struct Force force;
    std::list<int> adj;
    QPen pen;
    QBrush brush;
    QString name;
    QGraphicsTextItem *label;

    node(double X, double Y, QGraphicsItem *parent = 0, double width = radius, double height = radius);
    node();
    void setName(QString s);

    //from spring embedder
    void setPos(double x, double y);
    void setForce(double x, double y);
    double distance(node &n);
};
#endif // NODE_H
