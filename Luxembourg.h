#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Luxembourg.h"
#include "Graf.h"
#include <QMouseEvent>
#include <queue>
#include <chrono>

class Luxembourg : public QMainWindow
{
    Q_OBJECT

public:
    Luxembourg(QWidget *parent = Q_NULLPTR);
    void paintEvent(QPaintEvent* event)override;
    void mouseReleaseEvent(QMouseEvent* e)override;
    void mousePressEvent(QMouseEvent* e)override;
    void mouseMoveEvent(QMouseEvent* e)override;

    void Dijkstra();

    bool isValid(const QPointF& point);

private:
    Ui::LuxembourgClass ui;
    Graf graf;

    bool m_drawByHand, luxembourg;
    bool drawNode, drawArc, isOriented;
    Node firstNode, secondNode;
    Arc::NodePtr pressedNode;

    QPointF point;

    int xSpace = 10, ySpace = 40;
    int divider = 500;
    bool IsTree;
private:

    void DrawByHand(QPainter& p);
    void drawLuxembourg(QPainter& p);
    void on_draw_clicked();
    void on_luxembourg_clicked();
    void on_root_clicked();
    void on_Dijkstra_clicked();
};
