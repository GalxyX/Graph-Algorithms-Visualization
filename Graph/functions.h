#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "mgraph.h"
#include <queue>
#include <vector>
#include <QLabel>
#include <QWidget>

class AdjacencyList;
class PrimTree;
class KruskalTree;
class Dijkstra;
class Floyd;
namespace Ui {
class Functions;
}

class Functions : public QWidget
{
    Q_OBJECT

public:
    explicit Functions(MGraph &Graph, int func, QWidget *parent = nullptr);
    ~Functions();

private slots:
    void on_pushButton_released();

private:
    Ui::Functions *ui;
    int func;
    MGraphShow completeGraph;
    AdjacencyList *adjacencyList;
    PrimTree *primTree;
    KruskalTree *kruskalTree;
    Dijkstra *dijkstra;
    Floyd *floyd;
};

class AdjacencyList : public QObject
{ //邻接链表页面
public:
    AdjacencyList(const Ui::Functions *ui, const MGraphShow &completeGraph, QWidget *parent = nullptr);
    void ShowAdjacencyList();
    ~AdjacencyList();

private:
    const Ui::Functions *ui;
    const MGraphShow &completeGraph;
    QGraphicsScene *workingscene;
};

struct closedge_unit
{
    int adjvex;
    int lowcost;
};
class PrimTree : public QWidget
{ //普利姆算法页面
public:
    PrimTree(const Ui::Functions *ui, const MGraphShow &completeGraph, const QString u, QWidget *parent = nullptr);
    void MiniSpanTree_PRIM();
    ~PrimTree();

private:
    const Ui::Functions *ui;
    const MGraphShow &completeGraph;
    int i;
    QTimer *timer_round;                      //每一轮计时器
    QVector<QVector<QLabel *>> closedge_form; //三行closedge表的QLabel
    QVector<QVector<QGraphicsColorizeEffect *>> closedge_form_effect;        //三行closedge表的QGraphicsColorizeEffect
    QVector<closedge_unit> closedge; //cloedge表
    MGraphShow *workingGraph;
};

template<class Type>
class EdgeNode
{
public:
    int u, v;
    operator Type() const { return weight; }
    bool operator>(EdgeNode<Type> &arc) { return Type(*this) > Type(arc); }
    EdgeNode(int i, int j, Type w): u(i), v(j), weight(w)
    {

    }

private:
    Type weight;
};
class Set
{
public:
    Set(int n) { members.resize(n + 1, -1); }
    int Find(int x)
    {
        while (members[x] != -1)
            x = members[x];
        return x;
    }
    bool Union(int a, int b)
    {
        if (a == b)
            return false;
        else
            members[b] = a;
        return true;
    }

private:
    std::vector<int> members;
};
class KruskalTree : public QObject
{ //克鲁斯卡尔页面
public:
    KruskalTree(const Ui::Functions *ui, const MGraphShow &completeGraph, QObject *parent = nullptr);
    void MiniSpanTree_KRUSKAL();
    ~KruskalTree();

private:
    const Ui::Functions *ui;
    const MGraphShow &completeGraph;

    int selectednodes;
    QTimer *timer_round; //每一轮计时器
    MGraphShow *workingGraph;
    Set nodes;

    std::priority_queue<EdgeNode<int>, std::vector<EdgeNode<int>>, std::greater<EdgeNode<int>>> edges;
};

class Dijkstra : public QWidget
{ //单源最短路径页面
public:
    Dijkstra(const Ui::Functions *ui, const MGraphShow &completeGraph, int v0, QWidget *parent = nullptr);
    void ShortestPath_DIJ();
    ~Dijkstra();

private:
    const Ui::Functions *ui;
    const MGraphShow &completeGraph;

    QTimer *timer_round; //每一轮计时器
    MGraphShow *workingGraph;
    int i;
    std::vector<int> pre; //ShortPathDistance//最短路径数组（存储上一个结点下标）
    std::vector<int> D;   //ShortPathTable//最短路径长度
    std::vector<bool> final;                                 //S
    QVector<QVector<QLabel *>> form;                         //三行ShortPath表的QLabel
    QVector<QVector<QGraphicsColorizeEffect *>> form_effect; //三行ShortPath表的QGraphicsColorizeEffect
    QString GetPath(int v);
};

class Floyd : public QWidget
{ //各顶点间最短路径页面
public:
    Floyd(const Ui::Functions *ui, const MGraphShow &completeGraph, QWidget *parent = nullptr);
    void ShortestPath_FLOYD();
    ~Floyd();

private:
    const Ui::Functions *ui;
    const MGraphShow &completeGraph;

    QTimer *timer_round; //每一轮计时器
    MGraphShow *workingGraph;
    int k;
    std::vector<std::vector<int>> P; //PathMatrix//最短路径
    std::vector<std::vector<int>> D; //DistanceMatrix//带权长度
    QVector<QVector<QLabel *>> form; //顶点数行ShortPath表的QLabel
    QVector<QVector<QGraphicsColorizeEffect *>> form_effect; //顶点数行ShortPath表的QGraphicsColorizeEffect
    QString GetPath(int start, int end);
};

#endif // FUNCTIONS_H
