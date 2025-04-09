#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <cmath>
#include <QtMath>
#include "graph.h"

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

class LoopArrow : public QGraphicsItem
{
public:
    LoopArrow(QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent), m_radius(20), m_angle(90) {}

    QRectF boundingRect() const
    {
        return QRectF(-m_radius - 10, -m_radius - 10,
                      2 * m_radius + 20, 2 * m_radius + 20);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    void setRadius(qreal radius) { m_radius = radius; }
    void setAngle(qreal angle) { m_angle = angle; }

private:
    qreal m_radius;
    qreal m_angle;
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