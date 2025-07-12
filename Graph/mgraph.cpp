#include "mgraph.h"
#include <cmath>
#include <queue>
#include <QPen>
#include <QPropertyAnimation>
MGraph::MGraph(int vertexnum, int edgenum, QVector<QVector<int> > graph): vexnum(vertexnum) , arcnum(edgenum) , arcs(graph)
{
    vexs.resize(vertexnum, 0);
    for (int i = 0; i < vertexnum; ++i)
        vexs[i] = QString::number(i);
}
MGraph::MGraph(int vertexnum, int edgenum): vexnum(vertexnum) , arcnum(edgenum)
{
    vexs.resize(vertexnum, 0);
    for (int i = 0; i < vertexnum; ++i)
        vexs[i] = QString::number(i);
    arcs.resize(vertexnum, QVector<int>(vertexnum, INT_MAX));
}
void MGraph::BFS(int v, QVector<bool> &visited)
{
    std::queue<int> Q;
    visited[v] = 1;
    Q.emplace(v);
    while (!Q.empty()) {
        v = Q.front();
        Q.pop();
        for (int i = 0; i < vexnum; ++i) {
            if (arcs[v][i] != INT_MAX && !visited[i]) {
                visited[i] = true;
                Q.emplace(i);
            }
        }
    }
}
bool MGraph::isConnected()
{
    QVector<bool> visited(vexnum, false);
    BFS(0, visited);
    for (int v = 0; v < vexnum; ++v)
        if (!visited[v])
            return false;
    return true;
}

NodeShape::NodeShape(QString name, qreal centerx, qreal centery, qreal radius, QColor bkColor, QColor edgeColor, QColor textColor, QObject *parent): QObject(parent)
{
    //设置圆形格式
    QGraphicsEllipseItem *nodeCircle = new QGraphicsEllipseItem(centerx - radius, centery - radius, radius * 2, radius * 2, &this->node);
    nodeCircle->setBrush(bkColor);
    QPen pen(edgeColor, NODE_EDGE_WIDTH);
    nodeCircle->setPen(pen);
    //设置标号样式
    QGraphicsSimpleTextItem *nodeId = new QGraphicsSimpleTextItem(name, &this->node);
    nodeId->setBrush(textColor);
    QFont f;
    f.setPointSize(NODE_TEXT_POINTSIZE);
    nodeId->setFont(f);
    nodeId->setPos(centerx - nodeId->boundingRect().width() / 2, centery - nodeId->boundingRect().height() / 2);
    //组合
    node.addToGroup(nodeCircle);
    node.addToGroup(nodeId);

    //淡入动画
    opacity = new QGraphicsOpacityEffect(this);
    node.setGraphicsEffect(opacity);
    QPropertyAnimation *animation = new QPropertyAnimation(opacity, "opacity", this);
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

ArcShape::ArcShape(qreal weight, int smallidnode, int bigidnode, qreal startx, qreal starty, qreal endx, qreal endy,
                   QColor lineColor, qreal width, QColor textColor, QObject *parent): QObject(parent) , smallvex(smallidnode) , bigvex(bigidnode)
{
    //设置线条格式
    QGraphicsLineItem *Line = new QGraphicsLineItem(startx, starty, endx, endy, &this->arc);
    QPen pen(lineColor, width);
    Line->setPen(pen);
    //设置权值样式
    QGraphicsSimpleTextItem *arcId = new QGraphicsSimpleTextItem(QString::number(weight), &this->arc);
    arcId->setBrush(textColor);
    QFont f;
    f.setPointSize(ARC_TEXT_POINTSIZE);
    arcId->setFont(f);
    arcId->setPos((startx + endx) / 2, (starty + endy) / 2);
    //组合
    arc.addToGroup(Line);
    arc.addToGroup(arcId);
    //淡入动画
    opacity = new QGraphicsOpacityEffect(this);
    arc.setGraphicsEffect(opacity);
    QPropertyAnimation *animation = new QPropertyAnimation(opacity, "opacity", this);
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

MGraphShow::MGraphShow(MGraph graph, int width, int height) : G(graph)
{
    canvas.setSceneRect(-width / 2, -height / 2, width, height);
    r_show = std::min(width, height) / 2 - GRAPH_SPACES - NODE_RADIUS; //整张图半径
    //QVector<NodeShape*> nodes
    nodes_shape.resize(G.vexnum);
    qreal delta = 2 * M_PI / G.vexnum; //360 / G.vexnum * M_PI / 180;
    int i, j /*,smallindex, bigindex*/;
    for (i = 0; i < G.vexnum; ++i) {
        nodes_shape[i] = new NodeShape(G.vexs[i], r_show * std::sin(i * delta), -r_show * std::cos(i * delta));
        AddNode(i);
    }
    //QVector<ArcShape*> arcs
    for (i = 0; i < G.vexnum; ++i)
        for (j = i; j < G.vexnum; ++j)
            if (G.arcs[i][j] != INT_MAX) {
                // smallindex=std::min(i,j);
                // bigindex=std::max(i,j);
                // ArcShape* temp=new ArcShape(G.arcs[i][j],smallindex,bigindex, r_show * std::sin(smallindex * delta), -r_show * std::cos(smallindex * delta),r_show * std::sin(bigindex * delta), -r_show * std::cos(bigindex * delta));
                ArcShape *temp = new ArcShape(G.arcs[i][j], i, j, r_show * std::sin(i * delta), -r_show * std::cos(i * delta), r_show * std::sin(j * delta), -r_show * std::cos(j * delta));
                arcs_shape.append(temp);
                AddArc(temp);
            }
    std::sort(arcs_shape.begin(), arcs_shape.end(), [](ArcShape *const &a1, ArcShape *const &a2) {
        if (a1->smallvex < a2->smallvex)
            return true;
        else if (a1->smallvex == a2->smallvex && a1->bigvex < a2->bigvex)
            return true;
        else
            return false;
    });
}
MGraphShow::~MGraphShow()
{
    int i;
    for (i = 0; i < G.vexnum; ++i)
        delete nodes_shape[i];
    for (i = 0; i < G.arcnum; ++i)
        delete arcs_shape[i];
}

void MGraphShow::AddNode(int index)
{ //添加结点的图形化，仅在QGraphicsScene canvas上添加
    canvas.addItem(&nodes_shape[index]->node);
    nodes_shape[index]->node.setZValue(1);
}
void MGraphShow::AddArc(ArcShape *addarc)
{ //添加边的图形化，仅在QGraphicsScene canvas上添加
    canvas.addItem(&addarc->arc);
    addarc->arc.setZValue(0);
}
int MGraphShow::FindArc(int smallindex, int bigindex)
{ //查找特定边，返回索引
    if (smallindex >= G.vexnum || bigindex >= G.vexnum || smallindex == bigindex)
        return -1;
    int index, size;
    for (index = 0, size = arcs_shape.size();
         index < size && arcs_shape[index]->smallvex <= smallindex;
         ++index)
        if (arcs_shape[index]->smallvex == smallindex && arcs_shape[index]->bigvex == bigindex)
            return index;
    return -1; //找不到该边返回-1
}

bool MGraphShow::CreateNewArc(int smallindex, int bigindex, qreal weight)
{ //默认直接显示//加入新边（原在邻接矩阵中不存在）
    //该边已存在
    if (smallindex >= G.vexnum || bigindex >= G.vexnum || smallindex == bigindex
        || G.arcs[smallindex][bigindex] != INT_MAX)
        return false;
    //MGraph
    ++G.arcnum;
    G.arcs[smallindex][bigindex] = G.arcs[bigindex][smallindex] = weight;
    //arcs
    qreal delta = 2 * M_PI / G.vexnum; //360 / G.vexnum * M_PI / 180;
    ArcShape *temp = new ArcShape(weight, smallindex, bigindex, r_show * std::sin(smallindex * delta), -r_show * std::cos(smallindex * delta), r_show * std::sin(bigindex * delta), -r_show * std::cos(bigindex * delta));
    arcs_shape.append(temp);
    AddArc(temp);
    canvas.update();
    std::sort(arcs_shape.begin(), arcs_shape.end(), [](ArcShape *const &a1, ArcShape *const &a2) {
        if (a1->smallvex < a2->smallvex)
            return true;
        else if (a1->smallvex == a2->smallvex && a1->bigvex < a2->bigvex)
            return true;
        else
            return false;
    });
    return true;
}
bool MGraphShow::DeleteCreatedArc(int smallindex, int bigindex)
{
    int index = FindArc(smallindex, bigindex);
    if (index == -1)
        return false;
    else {
        //MGraph
        --G.arcnum;
        G.arcs[smallindex][bigindex] = G.arcs[bigindex][smallindex] = INT_MAX;
        //arcs
        canvas.removeItem(&arcs_shape[index]->arc);
        canvas.update();
        delete arcs_shape[index];
        arcs_shape.erase(arcs_shape.cbegin() + index);
        return true;
    }
}

void NodeShape::SetNodeVisibility(bool visible, bool animation)
{
    if (node.opacity() == visible)
        return;
    if (animation) {
        // QGraphicsOpacityEffect *opacity=new QGraphicsOpacityEffect(this);
        // node.setGraphicsEffect(opacity);
        QPropertyAnimation *animationeffect = new QPropertyAnimation(opacity, "opacity", this);
        animationeffect->setDuration(ANIMATION_DURATION);
        animationeffect->setStartValue(!visible);
        animationeffect->setEndValue(visible);
        animationeffect->start(QAbstractAnimation::DeleteWhenStopped);
    }
    node.setOpacity(visible);
    //通过更改透明度，并未实现setVisible
}
void ArcShape::SetArcVisibility(bool visible, bool animation)
{
    if (arc.opacity() == visible)
        return;
    if (animation) {
        // QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(this);
        // arc.setGraphicsEffect(opacity);
        QPropertyAnimation *animationeffect = new QPropertyAnimation(opacity, "opacity", this);
        animationeffect->setDuration(ANIMATION_DURATION);
        animationeffect->setStartValue(!visible);
        animationeffect->setEndValue(visible);
        animationeffect->start(QAbstractAnimation::DeleteWhenStopped);
    }
    arc.setOpacity(visible);
}

bool MGraphShow::SetNodeVisibility(int index, bool visible, bool animation)
{
    if (index >= G.vexnum) {
        return false;
    }
    nodes_shape[index]->SetNodeVisibility(visible, animation);
    return true;
}
bool MGraphShow::SetArcVisibility(int smallindex, int bigindex, bool visible, bool animation)
{
    int index = FindArc(smallindex, bigindex);
    if (index == -1) {
        return false;
    }
    arcs_shape[index]->SetArcVisibility(visible, animation);
    return true;
}
void MGraphShow::SetAllInvisible(bool animation)
{
    int i, j;
    for (i = 0; i < G.vexnum; ++i)
        SetNodeVisibility(i, false, animation);
    for (i = 0; i < G.vexnum; ++i)
        for (j = i; j < G.vexnum; ++j)
            SetArcVisibility(i, j, false, animation);
}
