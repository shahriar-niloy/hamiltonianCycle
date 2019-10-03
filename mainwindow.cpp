#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QDir>

mainWindow::mainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Hamiltonian Cycle Finder");
    scene = new QGraphicsScene(this);
    //canvas work
    ui->canvas->setRenderHints(QPainter::Antialiasing);
    ui->canvas->setScene(scene);
    ui->canvas->fitInView(scene->sceneRect());
    //ui->canvas->scale(5, 5);
    //Read from file
    QFile file("input.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "No file";
    }
    QTextStream in(&file);
    in >> nodeCount >> edgeCount;
    //Setting up Nodes and Edges arrray
    Nodes = new node[nodeCount];
    Edges = new edge[edgeCount];
    //Randomizing Position of the nodes
    initializePosition(nodeCount, Nodes);
    //Creating adjacency list
    for(int i = 0; i < edgeCount; ++i){
        //fscanf(in, "%d%d", &n1, &n1);
        in >> n1 >> n2;
        edgeList[i].first = n1;
        edgeList[i].second = n2;
        Nodes[n1].adj.push_back(n2);
        Nodes[n2].adj.push_back(n1);
        adj[n1].push_back(n2);
        adj[n2].push_back(n1);
    }
    //spring embedder algorithm
    for(int i = 0; i < iteration; ++i){
        //scene->clear();
        //Force adjustments
        totalForce(nodeCount, edgeCount, Nodes, edgeList);
        setPosition(nodeCount, Nodes);
        updateEdges(edgeCount, edgeList, Edges, Nodes);
        //printPosition(nodeCount, Nodes);
        //printForce(nodeCount, Nodes);
        //refresh scene/graphicsView
        //scene->update();
        //ui->canvas->viewport()->update();
    }
    //drawing nodes
    drawNodes(nodeCount, Nodes);
    drawEdges(edgeCount, Edges);
    //Hamiltonian Cycle
    if(ham(0,0)){
        qDebug() << "Hamiltonian Cycle Found!" << "How many? - " << cycles.size();
        //Draw next Cycle
        //drawNextCycle();
    }else{
        qDebug() << "Hamiltonian Cycle Not Found";
        ui->next->setEnabled(false);
    }
    //connect functions
    connect(ui->exit, SIGNAL(pressed()), this, (SLOT(close())));
    connect(ui->next, SIGNAL(clicked(bool)), this, SLOT(drawNextCycle()));
}

void mainWindow::initializePosition(int n, node Nodes[])
{
    //generate two vectors containing numbers from 1 to 100
    std::vector<int> a, b;
    for(int i = 1; i <= 100; ++i){
        a.push_back(i);
        b.push_back(i);
    }
    //shuffle those two vectors
    random_shuffle(a.begin(), a.end());
    random_shuffle(b.begin(), b.end());
    //pop back from those vectors and assign x and y values
    for(int i = 0; i < n; ++i){
        Nodes[i].x = (double)a.back(); a.pop_back();
        Nodes[i].y = (double)b.back(); b.pop_back();
    }
}

void mainWindow::totalForce(int n, int edges, node Nodes[], pair<int, int> edgeList[])
{
    double tempX, tempY;
    /// reseting force to 0
    for(int i = 0; i < n ; i++){
        Nodes[i].setForce(0.0,0.0);
    }
    ///calculating attraction force
    for(int i =  0; i < edges; i++){
        tempX = attractionForceX(Nodes[edgeList[i].first], Nodes[edgeList[i].second]);
        tempY = attractionForceY(Nodes[edgeList[i].first], Nodes[edgeList[i].second]);
        Nodes[edgeList[i].first].force.x += tempX;
        Nodes[edgeList[i].first].force.y += tempY;
        Nodes[edgeList[i].second].force.x += (-1)*tempX;
        Nodes[edgeList[i].second].force.y += (-1)*tempY;
    }
    ///calculating repulsive forces
    for(int i = 0; i < n; ++i){
        for(int j = i+1; j < n; ++j){
            tempX = repulsionForceX(Nodes[i], Nodes[j]);
            tempY = repulsionForceY(Nodes[i], Nodes[j]);
            Nodes[i].force.x += tempX;
            Nodes[i].force.y += tempY;
            Nodes[j].force.x += (-1.0)*tempX;
            Nodes[j].force.y += (-1.0)*tempY;
        }
    }
}

void mainWindow::setPosition(int n, node Nodes[])
{
    for(int i = 0;i<n;i++){
        Nodes[i].x += (Nodes[i].force.x)*(double)inc;
        Nodes[i].y += (Nodes[i].force.y)*(double)inc;
    }
}

double mainWindow::attractionForceX(node &n1, node &n2)
{
    return k1 * log10(n1.distance(n2)/luv) * (n2.x-n1.x) / n1.distance(n2);
}

double mainWindow::attractionForceY(node &n1, node &n2)
{
    return k1 * log10(n1.distance(n2)/luv) * (n2.y-n1.y) / n1.distance(n2);
}

double mainWindow::repulsionForceX(node &n1, node &n2)
{
    return k2 * (n1.x-n2.x) / pow(n1.distance(n2), 3);
}

double mainWindow::repulsionForceY(node &n1, node &n2)
{
    return k2 * (n1.y-n2.y) / pow(n1.distance(n2), 3);
}

void mainWindow::printForce(int n, node Nodes[])
{
    qDebug() << "---------------------------";
    for(int i = 0; i < n; i++){
        qDebug() << Nodes[i].force.x << "  " << Nodes[i].force.y;
    }
}

void mainWindow::printPosition(int n, node Nodes[])
{
    qDebug() << "---------------------------" << endl;
    for(int i = 0; i < n; i++){
        qDebug() << Nodes[i].x << "  " << Nodes[i].y;
    }
}

void mainWindow::drawNodes(int n, node Nodes[])
{
    for(int i = 0; i < n; ++i){
        Nodes[i].setPos((Nodes[i].x - radius/2), (-Nodes[i].y-radius/2));
        Nodes[i].setName(QString::number(i));
        scene->addItem(&Nodes[i]);
        //qDebug() << Nodes[i].x << " " << Nodes[i].y;
    }
}

void mainWindow::drawEdges(int n, edge Edges[])
{
    for(int i = 0; i < n; ++i){
        scene->addItem(&Edges[i]);
    }
}

void mainWindow::updateEdges(int n, pair<int, int> edgeList[], edge Edges[], node Nodes[])
{
    for(int i = 0; i < n; ++i){
        Edges[i].setPos(Nodes[edgeList[i].first].x, Nodes[edgeList[i].first].y, Nodes[edgeList[i].second].x, Nodes[edgeList[i].second].y);
        Edges[i].setLine((Edges[i].x1-radius/2.2) * amp, (-Edges[i].y1-radius/2.2) * amp, (Edges[i].x2-radius/2.2) * amp, (-Edges[i].y2-radius/2.2) * amp);
        //qDebug() << "*******************************";
        //qDebug() << Edges[i].x1 << " " << Edges[i].y1 << " " << Edges[i].x2 << " " << Edges[i].y2;
    }
}

bool mainWindow::ham(int current, int source)
{
    static bool ex = 0;
    path.push_back(current);
    visit[current] = 1;
    list <int>::iterator it;
    for(it=adj[current].begin(); it!=adj[current].end(); it++)
    {
        if(source == *it && path.size() == nodeCount){
            cycles.push_back(path);
            ex = true;
        }
        if(visit[*it] == 0){
            ham(*it, source);
        }
    }
    visit[current] = 0;
    path.pop_back();
    return (ex == true) ? ex : false;
}

void mainWindow::drawNextCycle()
{
    if(temp != NULL){
        for(int i = 0; i < edgeCount; ++i){
            scene->removeItem(&temp[i]);
        }
        delete temp;
    }
    //qDebug() << "Level 1" << " " << currentCycle;
    temp = new edge[edgeCount];
    //add edges from current ham cycle
    int prev = -1, i = 0;
    for(list<int>::iterator it = cycles[currentCycle].begin(); it != cycles[currentCycle].end(); ++it){
        //qDebug() << "Level 1.5";
        if(prev == -1){
            prev = *it;
            continue;
        }
        //qDebug() << "Level 2";
        temp[i].setPos(Nodes[prev].x, Nodes[prev].y, Nodes[*it].x, Nodes[*it].y);
        temp[i].setLine((temp[i].x1+tempLineAdjustment) * amp, (temp[i].y1+tempLineAdjustment) * amp, (temp[i].x2+tempLineAdjustment) * amp, (temp[i].y2+tempLineAdjustment) * amp);
        temp[i].getPen()->setColor(Qt::red);
        temp[i].getPen()->setWidth(2.0);
        temp[i].setZValue(-1);
        temp[i].setPen(*temp[i].getPen());
        scene->addItem(&temp[i]);
        prev = *it;
        ++i;
    }
    //for last path
    temp[i].setPos(temp[i-1].x2, temp[i-1].y2, temp[0].x1, temp[0].y1);
    temp[i].setLine((temp[i].x1+tempLineAdjustment) * amp, (temp[i].y1+tempLineAdjustment) * amp, (temp[i].x2+tempLineAdjustment) * amp, (temp[i].y2+tempLineAdjustment) * amp);
    temp[i].getPen()->setColor(Qt::red);
    temp[i].getPen()->setWidth(2.5);
    temp[i].setZValue(-1);
    temp[i].setPen(*temp[i].getPen());
    scene->addItem(&temp[i]);
    //qDebug() << "Level 3";
    currentCycle++;
    if(currentCycle == cycles.size())
        ui->next->setEnabled(false);
}

mainWindow::~mainWindow()
{
    delete ui;
}
