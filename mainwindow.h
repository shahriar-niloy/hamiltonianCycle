#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFile>
#include<vector>
#include<algorithm>
#include "node.h"
#include "edge.h"
#define N 100
#define iteration 500
#define inc 0.5
#define luv 1.0
#define k1 1.0
#define k2 1.0
#define tempLineAdjustment 0.64

using std::pair;
using std::list;
using std::vector;

namespace Ui {
class mainWindow;
}

class mainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();
public slots:
    void initializePosition(int n, node Nodes[]);
    void totalForce(int n ,int edges, node Nodes[], std::pair<int, int> edgeList[]);
    void setPosition(int n, node Nodes[]);
    double attractionForceX(node &n1, node &n2);
    double attractionForceY(node &n1, node &n2);
    double repulsionForceX(node &n1, node &n2);
    double repulsionForceY(node &n1, node &n2);
    void printForce(int n, node Nodes[]);
    void printPosition(int n, node Nodes[]);
    void drawNodes(int n, node Nodes[]);
    void drawEdges(int n, edge Edges[]);
    void updateEdges(int n, pair<int,int> edgeList[], edge Edges[], node Nodes[]);
    bool ham(int current,int source);
    void drawNextCycle();

private:
    Ui::mainWindow *ui;
    QGraphicsScene *scene;
    int nodeCount;
    int edgeCount;
    int n1, n2;
    node *Nodes;
    edge *Edges;
    std::pair<int,int> edgeList[100];

    //For hamiltonian Cycle
    int visit[N] = {0};
    list <int> adj[N];
    list <int> path;
    vector<list<int> > cycles;

    //for cycles drawing
    edge *temp = NULL;
    int currentCycle = 0;
};

#endif // MAINWINDOW_H
