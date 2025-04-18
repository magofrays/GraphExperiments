#include "headers/visualizer.h"
#include <iostream>
Arrow::Arrow(QGraphicsItem *parent) : QGraphicsLineItem(parent)
{
    setPen(QPen(Qt::black, 3));
    arrowHead.setPolygon(createArrowHead());
}

void Arrow::setLine(const QLineF &line)
{
    QGraphicsLineItem::setLine(line);
    updateArrowHead();
    arrowHead.setBrush(Qt::black);
}
QPolygonF Arrow::createArrowHead() const
{
    QPolygonF head;
    head << QPointF(0, 0)
         << QPointF(-10, -5)
         << QPointF(-10, 5);
    return head;
}

void Arrow::updateArrowHead()
{
    QLineF l = line();
    qreal angle = std::atan2(l.dy(), l.dx());

    arrowHead.setPos(l.p2());
    arrowHead.setRotation(qRadiansToDegrees(angle));
}

void LoopArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::NoBrush);

    // Рисуем дугу
    painter->drawArc(QRectF(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius),
                     m_angle * 16, 270 * 16);

    // Рисуем стрелку
    QPointF arrowPos = QPointF(m_radius * cos((m_angle + 270) * M_PI / 180),
                               -m_radius * sin((m_angle + 270) * M_PI / 180));
    QPointF arrowDir = QPointF(-sin((m_angle + 250) * M_PI / 180),
                               -cos((m_angle + 250) * M_PI / 180));

    painter->setBrush(Qt::black);
    QPolygonF arrowHead;
    arrowHead << arrowPos
              << arrowPos + QPointF(-arrowDir.y() - arrowDir.x(), arrowDir.x() - arrowDir.y()) * 5
              << arrowPos + QPointF(arrowDir.y() - arrowDir.x(), -arrowDir.x() - arrowDir.y()) * 5;
    painter->drawPolygon(arrowHead);
}

Visualizer::Visualizer(int argc, char **argv)
{
    if (QApplication::instance() == nullptr)
    {
        app = new QApplication(argc, argv);
        scene = new QGraphicsScene();
        view = new QGraphicsView(scene);
        view->setRenderHint(QPainter::Antialiasing);
        view->show();
    }
}

Visualizer *Visualizer::visualizer = nullptr;

Visualizer *Visualizer::instance(int argc, char **argv)
{
    if (visualizer == nullptr)
    {
        visualizer = new Visualizer(argc, argv);
    }
    return visualizer;
}

int Visualizer::exec()
{
    return app->exec();
}

void Visualizer::show()
{
    if (view != nullptr && scene != nullptr && app != nullptr)
        view->show();
}

void Visualizer::draw_graph(const Graph &g, int x, int y)
{
    std::vector<QGraphicsEllipseItem *> nodes;
    const int radius = 20;
    const int node_distance = 200 * g.elements / 10;

    for (size_t i = 0; i < g.elements; ++i)
    {
        auto node = new QGraphicsEllipseItem(0, 0, 2 * radius, 2 * radius);
        node->setPos(g.positions[i].first * node_distance + x,
                     g.positions[i].second * node_distance + y);
        node->setBrush(Qt::blue);

        auto text = new QGraphicsSimpleTextItem(QString::number(g.ids[i]), node);
        text->setPos(radius - 5, radius - 8);
        nodes.push_back(node);
    }

    for (size_t i = 0; i < g.elements; ++i)
    {
        for (size_t j = 0; j < g.graph[i].size(); ++j)
        {
            if (g.graph[i][j])
            {
                if (i == j)
                {
                    LoopArrow *loopArrow = new LoopArrow();
                    QPointF nodeCenter = nodes[i]->pos() + QPointF(radius, radius);
                    loopArrow->setPos(nodeCenter + QPointF(0, -radius - 4));
                    loopArrow->setRadius(radius - 5);
                    loopArrow->setAngle(300);
                    scene->addItem(loopArrow);
                }
                else
                {
                    QPointF start = nodes[i]->pos() + QPointF(radius, radius);
                    QPointF end = nodes[j]->pos() + QPointF(radius, radius);
                    QLineF line(start, end);
                    line.setLength(line.length() - radius);
                    Arrow *arrow = new Arrow();
                    arrow->setLine(line);
                    scene->addItem(arrow);
                }
            }
        }
    }

    for (auto &node : nodes)
    {
        scene->addItem(node);
    }
}
