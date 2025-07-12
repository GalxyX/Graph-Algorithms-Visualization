#ifndef MGRAPH_H
#define MGRAPH_H
#include "Parameter.h"
#include <QGraphicsItemGroup>
#include <QGraphicsOpacityEffect>
#include <QGraphicsScene>
#include <QVector>
class MGraph
{
public:
    //邻接矩阵类型
    int vexnum, arcnum;         //图的顶点数和弧数
    QVector<QString> vexs;      //顶点表
    QVector<QVector<int>> arcs; //邻接矩阵

    MGraph(int vertexnum, int edgenum, QVector<QVector<int>> graph);
    MGraph(int vertexnum, int edgenum);
    bool isConnected();

private:
    void BFS(int v, QVector<bool> &visited);
};

class NodeShape : public QObject
{
private:
    // int id;
    // qreal x;
    // qreal y;
    // qreal radius;
    QGraphicsOpacityEffect *opacity;

public:
    QGraphicsItemGroup node;
    NodeShape(QString name,
              qreal x,
              qreal y,
              qreal radius = NODE_RADIUS,
              QColor bkColor = NODE_BK_COLOR,
              QColor edgeColor = NODE_EDGE_COLOR,
              QColor textColor = NODE_ID_COLOR,
              QObject *parent = nullptr);
    void SetNodeVisibility(bool visible, bool animation = true);
    // ~NodeShape();
};
class ArcShape : public QObject
{
private:
    QGraphicsOpacityEffect *opacity;
public:
    int smallvex, bigvex;
    QGraphicsItemGroup arc;
    ArcShape(qreal weight,
             int smallidnode,
             int bigidnode,
             qreal startx,
             qreal starty,
             qreal endx,
             qreal endy,
             QColor lineColor = ARC_LINE_COLOR,
             qreal width = ARC_WIDTH,
             QColor textColor = ARC_WEIGHT_COLOR,
             QObject *parent = nullptr);
    void SetArcVisibility(bool visible, bool animation = true);
    // ~ArcShape();
};

class MGraphShow
{
private:
    int r_show;
    QVector<NodeShape *> nodes_shape;
    QVector<ArcShape *> arcs_shape;
    void AddNode(int index);
    void AddArc(ArcShape *addarc);
    int FindArc(int smallindex, int bigindex);

public:
    MGraph G;
    QGraphicsScene canvas;
    bool CreateNewArc(int smallindex, int bigindex, qreal weight);
    bool DeleteCreatedArc(int smallindex, int bigindex);

    bool SetNodeVisibility(int index, bool visible, bool animation = true);
    bool SetArcVisibility(int smallindex, int bigindex, bool visible, bool animation = true);
    void SetAllInvisible(bool animation = false);

    bool SetNodeColor(int index, QColor newcolor, bool animation = true);
    bool SetArcColor(int smallindex, int bigindex, QColor newcolor, bool animation = true);

    MGraphShow(MGraph graph, int width, int height);
    ~MGraphShow();
};

#endif // MGRAPH_H
