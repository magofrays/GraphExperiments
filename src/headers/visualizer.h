#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include "graph.h"
#include <QGraphicsPolygonItem>
#include <cmath>
#include <QtMath>

class Arrow : public QGraphicsLineItem
{
public:
    Arrow(QGraphicsItem *parent = nullptr);

    void setLine(const QLineF &line);

private:
    QGraphicsPolygonItem arrowHead{this};

    QPolygonF createArrowHead() const;

    void updateArrowHead();
};

class Visualizer
{
public:
    Visualizer(const Visualizer &) = delete;
    Visualizer &operator=(const Visualizer &) = delete;

    QApplication *app;
    QGraphicsScene *scene;
    QGraphicsView *view;

    int exec();
    void show();
    static Visualizer *instance(int argc = 0, char **argv = nullptr);
    void draw_graph(const Graph &g, int x = 0, int y = 0);

protected:
    static Visualizer *visualizer;
    Visualizer(int argc, char **argv);
};

#endif