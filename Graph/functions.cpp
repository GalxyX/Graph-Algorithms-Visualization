#include "functions.h"
#include "ui_functions.h"
#include <QInputDialog>
#include <QPropertyAnimation>
#include <QTimer>
Functions::Functions(MGraph &Graph, int func, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Functions)
    , func(func)
    , completeGraph(Graph, WINDOW_WIDTH / 4.0 - 25, WINDOW_HEIGHT - 80)
{
    ui->setupUi(this);

    ////////////////////////////////////////////////设置窗口////////////////////////////////////////////////
    setWindowFlags(Qt::Window);
    resize(WINDOW_WIDTH, WINDOW_HEIGHT); //窗口大小
    setWindowTitle(WINDOW_HEADLINE);     //程序标题
    setStyleSheet(QString("background-color:") + QString(BACKGROUND_COLOR));
    setAttribute(Qt::WA_DeleteOnClose);
    ui->CompletegraphicsView->setScene(&completeGraph.canvas);
    if (func == 1) {
        adjacencyList = new AdjacencyList(ui, completeGraph, this);
        adjacencyList->ShowAdjacencyList();
    }
    else if (func == 2) {
        primTree = new PrimTree(ui, completeGraph, "0", this);
        primTree->MiniSpanTree_PRIM();
    }
    else if (func == 3) {
        kruskalTree = new KruskalTree(ui, completeGraph, this);
        kruskalTree->MiniSpanTree_KRUSKAL();
    }
    else if (func == 4) {
        QInputDialog startpoint(this);
        // startpoint.getInt(this, "", "请输入迪杰斯特拉算法求解最短路径的起始节点：", 0, 0, completeGraph.G.vexnum-1, 1);
        dijkstra = new Dijkstra(ui, completeGraph, startpoint.getInt(this, "", "请输入迪杰斯特拉算法求解最短路径的起始节点：", 0, 0, completeGraph.G.vexnum - 1, 1), this);
        dijkstra->ShortestPath_DIJ();
    }
    else if (func == 5) {
        floyd = new Floyd(ui, completeGraph, this);
        floyd->ShortestPath_FLOYD();
    }
}
Functions::~Functions()
{
    delete ui;
}

AdjacencyList::AdjacencyList(const Ui::Functions *ui, const MGraphShow &completeGraph, QWidget *parent)
    : QObject(parent)
    , ui(ui)
    , completeGraph(completeGraph)
{
    workingscene = new QGraphicsScene();
}
AdjacencyList::~AdjacencyList()
{
    delete workingscene;
}
void AdjacencyList::ShowAdjacencyList()
{
    int i, j, h_painted; //在水平及竖直方向已绘制多少结点
    //设置表格格式
    QPen pen(QColor(CELL_EDGE_COLOR), NODE_EDGE_WIDTH);
    QBrush brush_CELL_BK((QColor(CELL_BK_COLOR)));
    QBrush brush_CELL_TEXT((QColor(CELL_TEXT_COLOR)));
    //设置标号样式
    QFont f;
    f.setPointSize(NODE_TEXT_POINTSIZE);

    QGraphicsSimpleTextItem *nodeId;
    for (i = 0; i < completeGraph.G.vexnum; ++i) {
        workingscene->addRect(TEXT_WIDTH, i * (CELL_LENGTH + V_CELL_GAP), CELL_LENGTH, CELL_LENGTH, pen, brush_CELL_BK);
        workingscene->addRect(TEXT_WIDTH + CELL_LENGTH, i * (CELL_LENGTH + V_CELL_GAP), CELL_LENGTH, CELL_LENGTH, pen, brush_CELL_BK);
        nodeId = workingscene->addSimpleText(QString::number(i), f);
        nodeId->setBrush(brush_CELL_TEXT);
        nodeId->setPos(-TEXT_WIDTH, i * (CELL_LENGTH + V_CELL_GAP) + (CELL_LENGTH - nodeId->boundingRect().height()) / 2);
    }

    for (i = 0; i < completeGraph.G.vexnum; ++i) {
        h_painted = 0;
        for (j = 0; j < completeGraph.G.vexnum; ++j) {
            if (completeGraph.G.arcs[i][j] != INT_MAX) {
                //表格
                workingscene->addRect((TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP) + h_painted * (SMALL_CELL_LENGTH * 2 + H_CELL_GAP),
                                      i * (CELL_LENGTH + V_CELL_GAP) + (CELL_LENGTH - SMALL_CELL_LENGTH) / 2,
                                      SMALL_CELL_LENGTH, SMALL_CELL_LENGTH, pen, brush_CELL_BK);
                workingscene->addRect((TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP) + h_painted * (SMALL_CELL_LENGTH * 2 + H_CELL_GAP) + SMALL_CELL_LENGTH,
                                      i * (CELL_LENGTH + V_CELL_GAP) + (CELL_LENGTH - SMALL_CELL_LENGTH) / 2,
                                      SMALL_CELL_LENGTH, SMALL_CELL_LENGTH, pen, brush_CELL_BK);

                //直线
                if (h_painted) {
                    workingscene->addLine((TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP) + (h_painted - 1) * (SMALL_CELL_LENGTH * 2 + H_CELL_GAP) + (SMALL_CELL_LENGTH + SMALL_CELL_LENGTH / 2),
                                          i * (CELL_LENGTH + V_CELL_GAP) + CELL_LENGTH / 2,
                                          (TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP) + h_painted * (SMALL_CELL_LENGTH * 2 + H_CELL_GAP),
                                          i * (CELL_LENGTH + V_CELL_GAP) + CELL_LENGTH / 2, pen);
                    workingscene->addLine((TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP) + h_painted * (SMALL_CELL_LENGTH * 2 + H_CELL_GAP) - H_ARROW_LENGTH / 5 * 0.707,
                                          i * (CELL_LENGTH + V_CELL_GAP) - H_ARROW_LENGTH / 5 * 0.707 + CELL_LENGTH / 2,
                                          (TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP) + h_painted * (SMALL_CELL_LENGTH * 2 + H_CELL_GAP),
                                          i * (CELL_LENGTH + V_CELL_GAP) + CELL_LENGTH / 2, pen);
                    workingscene->addLine((TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP) + h_painted * (SMALL_CELL_LENGTH * 2 + H_CELL_GAP) - H_ARROW_LENGTH / 5 * 0.707,
                                          i * (CELL_LENGTH + V_CELL_GAP) + H_ARROW_LENGTH / 5 * 0.707 + CELL_LENGTH / 2,
                                          (TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP) + h_painted * (SMALL_CELL_LENGTH * 2 + H_CELL_GAP),
                                          i * (CELL_LENGTH + V_CELL_GAP) + CELL_LENGTH / 2, pen);

                } else {
                    workingscene->addLine(TEXT_WIDTH + CELL_LENGTH * 3 / 2, i * (CELL_LENGTH + V_CELL_GAP) + CELL_LENGTH / 2,
                                          TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP, i * (CELL_LENGTH + V_CELL_GAP) + CELL_LENGTH / 2, pen);
                    workingscene->addLine(TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP - H_ARROW_LENGTH / 5 * 0.707,
                                          i * (CELL_LENGTH + V_CELL_GAP) - H_ARROW_LENGTH / 5 * 0.707 + CELL_LENGTH / 2,
                                          TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP, i * (CELL_LENGTH + V_CELL_GAP) + CELL_LENGTH / 2, pen);
                    workingscene->addLine(TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP - H_ARROW_LENGTH / 5 * 0.707,
                                          i * (CELL_LENGTH + V_CELL_GAP) + H_ARROW_LENGTH / 5 * 0.707 + CELL_LENGTH / 2,
                                          TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP, i * (CELL_LENGTH + V_CELL_GAP) + CELL_LENGTH / 2, pen);
                }

                //数字
                nodeId = workingscene->addSimpleText(QString::number(j), f);
                nodeId->setBrush(brush_CELL_TEXT);
                nodeId->setPos((TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP) + h_painted * (SMALL_CELL_LENGTH * 2 + H_CELL_GAP) + (SMALL_CELL_LENGTH - nodeId->boundingRect().width()) / 2,
                               i * (CELL_LENGTH + V_CELL_GAP) + (CELL_LENGTH - nodeId->boundingRect().height()) / 2);
                ++h_painted;
            }
        }
        //^
        nodeId = workingscene->addSimpleText("^", f);
        nodeId->setBrush(brush_CELL_TEXT);
        nodeId->setPos((TEXT_WIDTH + CELL_LENGTH * 2 + H_CELL_GAP) + (h_painted - 1) * (SMALL_CELL_LENGTH * 2 + H_CELL_GAP) + SMALL_CELL_LENGTH + (SMALL_CELL_LENGTH - nodeId->boundingRect().width()) / 2,
                       i * (CELL_LENGTH + V_CELL_GAP) + (CELL_LENGTH - nodeId->boundingRect().height()) / 2);
    }
    workingscene->update();
    ui->WorkinggraphicsView->setScene(workingscene);
}

PrimTree::PrimTree(const Ui::Functions *ui,
                   const MGraphShow &completeGraph,
                   const QString u,
                   QWidget *parent)
    : QWidget(parent)
    , ui(ui)
    , completeGraph(completeGraph)
{
    workingGraph = new MGraphShow(completeGraph.G, WINDOW_WIDTH * 3 / 4.0 - 25, WINDOW_HEIGHT - 100);
    workingGraph->SetAllInvisible();
    closedge_form.resize(workingGraph->G.vexnum + 1, QVector<QLabel *>(3, NULL)); //包括第一列表头
    closedge_form_effect.resize(workingGraph->G.vexnum, QVector<QGraphicsColorizeEffect *>(2, NULL)); //不包括第一列表头
    ui->WorkinggraphicsView->setScene(&workingGraph->canvas);
    closedge.resize(workingGraph->G.vexnum);

    //用普里姆算法从第u个顶点出发构造最小生成树T
    int k, j;
    k = u.toInt();                               //k=LocateVex(G,u);
    for (j = 0; j < workingGraph->G.vexnum; ++j) //辅助数组初始化
        if (j != k)
            closedge[j] = {k, workingGraph->G.arcs[k][j]}; //{adjvex, lowcost}
    closedge[k].lowcost = 0;                               //初始，U={u}
    workingGraph->SetNodeVisibility(k, true);

    //初始化第一列QLabel
    closedge_form[0][0] = new QLabel("closeedge", this);
    closedge_form[0][1] = new QLabel("adjvex", this);
    closedge_form[0][2] = new QLabel("lowcost", this);
    ui->sheettable->addWidget(closedge_form[0][0], 0, 0, 1, 1, Qt::AlignCenter);
    ui->sheettable->addWidget(closedge_form[0][1], 1, 0, 1, 1, Qt::AlignCenter);
    ui->sheettable->addWidget(closedge_form[0][2], 2, 0, 1, 1, Qt::AlignCenter);

    //初始化输出QLabel内容及格式
    for (i = 0; i < workingGraph->G.vexnum; ++i) {
        closedge_form[i + 1][0] = new QLabel("V" + workingGraph->G.vexs[i], this);
        closedge_form[i + 1][1] = new QLabel(closedge[i].lowcost == INT_MAX ? "" : workingGraph->G.vexs[closedge[i].adjvex], this);
        closedge_form[i + 1][2] = new QLabel(closedge[i].lowcost == INT_MAX ? "0" : QString::number(closedge[i].lowcost), this);
        ui->sheettable->addWidget(closedge_form[i + 1][0], 0, i + 1, 1, 1, Qt::AlignCenter);
        ui->sheettable->addWidget(closedge_form[i + 1][1], 1, i + 1, 1, 1, Qt::AlignCenter);
        ui->sheettable->addWidget(closedge_form[i + 1][2], 2, i + 1, 1, 1, Qt::AlignCenter);
    }
    for (i = 0; i < workingGraph->G.vexnum; ++i) {
        closedge_form_effect[i][0] = new QGraphicsColorizeEffect(closedge_form[i + 1][1]);
        closedge_form_effect[i][1] = new QGraphicsColorizeEffect(closedge_form[i + 1][2]);
        closedge_form_effect[i][0]->setColor(Qt::black);
        closedge_form_effect[i][1]->setColor(Qt::black);
        closedge_form[i + 1][1]->setGraphicsEffect(closedge_form_effect[i][0]);
        closedge_form[i + 1][2]->setGraphicsEffect(closedge_form_effect[i][1]);
    }
}
PrimTree::~PrimTree()
{
    for (i = 0; i < workingGraph->G.vexnum; ++i) {
        delete closedge_form_effect[i][0];
        delete closedge_form_effect[i][1];
    }
    delete workingGraph;
}
void PrimTree::MiniSpanTree_PRIM()
{
    timer_round = new QTimer(this);
    timer_round->start(UPDATE_INTERVAL);
    i = 1;
    connect(timer_round, &QTimer::timeout, this, [this]() {
        if (i < workingGraph->G.vexnum) {
            int j, k = 0, minCost;
            //求出T的下一个结点：第k结点
            // k=minimum(closedge);
            minCost = INT_MAX;
            for (j = 0; j < workingGraph->G.vexnum; ++j) {
                if (closedge[j].lowcost < minCost && closedge[j].lowcost != 0) {
                    minCost = closedge[j].lowcost;
                    k = j;
                }
            }
            closedge[k].lowcost = 0;
            for (j = 0; j < workingGraph->G.vexnum; ++j)
                if (workingGraph->G.arcs[k][j] < closedge[j].lowcost) {
                    closedge[j].adjvex = k;
                    closedge[j].lowcost = workingGraph->G.arcs[k][j];
                }

            //本轮被选中（要加入）的结点closedge表格高亮显示
            // QTimer::singleShot(COMPONENT_UPDATE_INTERVAL, this, [k,this]() {
            QPropertyAnimation *animation1 = new QPropertyAnimation(closedge_form_effect[k][0], "color");
            animation1->setDuration(ANIMATION_DURATION);
            animation1->setStartValue(QColor(Qt::black));
            animation1->setEndValue(QColor(CLOSEDGE_HIGHLIGHT_TEXT_COLOR));
            animation1->start(QAbstractAnimation::DeleteWhenStopped);

            QPropertyAnimation *animation2 = new QPropertyAnimation(closedge_form_effect[k][1], "color");
            animation2->setDuration(ANIMATION_DURATION);
            animation2->setStartValue(QColor(Qt::black));
            animation2->setEndValue(QColor(CLOSEDGE_HIGHLIGHT_TEXT_COLOR));
            animation2->start(QAbstractAnimation::DeleteWhenStopped);
            // });
            //图中加入新结点与新边
            QTimer::singleShot(COMPONENT_UPDATE_INTERVAL, this, [k, this]() {
                workingGraph->SetNodeVisibility(k, true);
                workingGraph->SetArcVisibility(std::min(closedge[k].adjvex, k), std::max(closedge[k].adjvex, k), true);
            });
            // //颜色恢复正常并更新closedge表格
            QTimer::singleShot(COMPONENT_UPDATE_INTERVAL * 2, this, [k, this]() {
                QPropertyAnimation *animation1 = new QPropertyAnimation(closedge_form_effect[k][0], "color");
                animation1->setDuration(ANIMATION_DURATION);
                animation1->setStartValue(QColor(CLOSEDGE_HIGHLIGHT_TEXT_COLOR));
                animation1->setEndValue(QColor(Qt::black));
                animation1->start(QAbstractAnimation::DeleteWhenStopped);

                QPropertyAnimation *animation2 = new QPropertyAnimation(closedge_form_effect[k][1], "color");
                animation2->setDuration(ANIMATION_DURATION);
                animation2->setStartValue(QColor(CLOSEDGE_HIGHLIGHT_TEXT_COLOR));
                animation2->setEndValue(QColor(Qt::black));
                animation2->start(QAbstractAnimation::DeleteWhenStopped);

                for (int j = 0; j < workingGraph->G.vexnum; ++j) { //辅助数组初始化
                    closedge_form[j + 1][1]->setText(
                        closedge[j].lowcost == INT_MAX ? "" : workingGraph->G.vexs[closedge[j].adjvex]);
                    closedge_form[j + 1][2]->setText(closedge[j].lowcost == INT_MAX ? "0" : QString::number(closedge[j].lowcost));
                }
            });
            ++i;
            workingGraph->canvas.update();
        } else
            timer_round->stop();
    });
}

KruskalTree::KruskalTree(const Ui::Functions *ui, const MGraphShow &completeGraph, QObject *parent)
    : QObject(parent)
    , ui(ui)
    , completeGraph(completeGraph)
    , nodes(completeGraph.G.vexnum)
{
    for (int i = 0; i < completeGraph.G.vexnum; ++i)
        for (int j = i; j < completeGraph.G.vexnum; ++j)
            if (completeGraph.G.arcs[i][j] != INT_MAX)
                edges.emplace(EdgeNode<int>(i, j, completeGraph.G.arcs[i][j]));
    workingGraph = new MGraphShow(completeGraph.G, WINDOW_WIDTH * 3 / 4.0 - 25, WINDOW_HEIGHT - 100);
    workingGraph->SetAllInvisible();
    ui->WorkinggraphicsView->setScene(&workingGraph->canvas);
}
KruskalTree::~KruskalTree()
{
    delete workingGraph;
}
void KruskalTree::MiniSpanTree_KRUSKAL()
{
    timer_round = new QTimer(this);
    timer_round->start(UPDATE_INTERVAL);
    selectednodes = 1;
    connect(timer_round, &QTimer::timeout, this, [this]() {
        while (selectednodes < completeGraph.G.vexnum && !edges.empty()) {
            EdgeNode<int> temp = edges.top();
            edges.pop();
            int a = nodes.Find(temp.u);
            int b = nodes.Find(temp.v);
            if (a != b) { //找到一条
                workingGraph->SetNodeVisibility(temp.u, true);
                workingGraph->SetNodeVisibility(temp.v, true);
                workingGraph->SetArcVisibility(std::min(temp.u, temp.v), std::max(temp.u, temp.v), true);
                //weights += temp;
                nodes.Union(a, b);
                ++selectednodes;
                break;
            }
            workingGraph->canvas.update();
        }
        if (selectednodes >= completeGraph.G.vexnum || edges.empty())
            timer_round->stop();
    });
}

Dijkstra::Dijkstra(const Ui::Functions *ui, const MGraphShow &completeGraph, int v0, QWidget *parent)
    : QWidget(parent)
    , ui(ui)
    , completeGraph(completeGraph)
    , pre(completeGraph.G.vexnum)
    , D(completeGraph.G.vexnum)
    , final(completeGraph.G.vexnum)
{ //v0为起点
    //workingGraph
    workingGraph = new MGraphShow(completeGraph.G, WINDOW_WIDTH * 3 / 4.0 - 25, WINDOW_HEIGHT - 100);
    workingGraph->SetAllInvisible();
    ui->WorkinggraphicsView->setScene(&workingGraph->canvas);
    workingGraph->SetNodeVisibility(v0, true);
    //form    //初始化第一列QLabel
    form.resize(workingGraph->G.vexnum + 1, QVector<QLabel *>(3, NULL));
    form[0][0] = new QLabel("终点", this);
    form[0][1] = new QLabel("从V" + workingGraph->G.vexs[v0] + "到各顶点的D值", this);
    form[0][2] = new QLabel("从V" + workingGraph->G.vexs[v0] + "到各顶点的最短路径", this);
    ui->sheettable->addWidget(form[0][0], 0, 0, 1, 1, Qt::AlignCenter);
    ui->sheettable->addWidget(form[0][1], 1, 0, 1, 1, Qt::AlignCenter);
    ui->sheettable->addWidget(form[0][2], 2, 0, 1, 1, Qt::AlignCenter);

    //初始化
    pre.resize(workingGraph->G.vexnum, v0);      //设空路径//包括第一列表头
    D.resize(workingGraph->G.vexnum, 0);         //不包括第一列表头
    final.resize(workingGraph->G.vexnum, false); //不包括第一列表头
    for (int v = 0; v < workingGraph->G.vexnum; ++v) {
        pre[v] = v0;
        D[v] = workingGraph->G.arcs[v0][v];
    }
    D[v0] = 0;
    final[v0] = true;

    //form_effect    //初始化输出QLabel内容及格式
    form_effect.resize(workingGraph->G.vexnum, QVector<QGraphicsColorizeEffect *>(3, NULL));
    for (i = 0; i < workingGraph->G.vexnum; ++i) {
        form[i + 1][0] = new QLabel("V" + workingGraph->G.vexs[i], this);
        form[i + 1][1] = new QLabel(D[i] == INT_MAX ? "∞" : QString::number(D[i]), this);
        form[i + 1][2] = new QLabel(D[i] == INT_MAX ? "∞" : GetPath(i), this);
        ui->sheettable->addWidget(form[i + 1][0], 0, i + 1, 1, 1, Qt::AlignCenter);
        ui->sheettable->addWidget(form[i + 1][1], 1, i + 1, 1, 1, Qt::AlignCenter);
        ui->sheettable->addWidget(form[i + 1][2], 2, i + 1, 1, 1, Qt::AlignCenter);
        form_effect[i][0] = new QGraphicsColorizeEffect(form[i + 1][1]);
        form_effect[i][1] = new QGraphicsColorizeEffect(form[i + 1][2]);
        form_effect[i][0]->setColor(Qt::black);
        form_effect[i][1]->setColor(Qt::black);
        form[i + 1][1]->setGraphicsEffect(form_effect[i][0]);
        form[i + 1][2]->setGraphicsEffect(form_effect[i][1]);
    }
}
Dijkstra::~Dijkstra()
{
    for (i = 0; i < workingGraph->G.vexnum; ++i) {
        delete form_effect[i][0];
        delete form_effect[i][1];
    }
    delete workingGraph;
}
void Dijkstra::ShortestPath_DIJ()
{
    timer_round = new QTimer(this);
    timer_round->start(UPDATE_INTERVAL);
    i = 1;
    connect(timer_round, &QTimer::timeout, this, [this]() {
        if (i < workingGraph->G.vexnum) {
            int w, k = 0;
            int min_d = INT_MAX;
            for (w = 0; w < workingGraph->G.vexnum; ++w)
                if (!final[w] && D[w] < min_d) {
                    k = w;
                    min_d = D[w];
                }
            final[k] = true;                             //离v0顶点最近的v加入S集合
            for (w = 0; w < workingGraph->G.vexnum; ++w) //更新当前最短路径及距离
                if (!final[w] && (workingGraph->G.arcs[k][w] < INT_MAX && min_d + workingGraph->G.arcs[k][w] < D[w])) {
                    D[w] = min_d + workingGraph->G.arcs[k][w];
                    pre[w] = k;
                }
            //以上找到k了

            //可视化部分
            //本轮被选中（要加入）的结点表格高亮显示
            // QTimer::singleShot(COMPONENT_UPDATE_INTERVAL, this, [k,this]() {
            QPropertyAnimation *animation1 = new QPropertyAnimation(form_effect[k][0], "color");
            animation1->setDuration(ANIMATION_DURATION);
            animation1->setStartValue(QColor(Qt::black));
            animation1->setEndValue(QColor(CLOSEDGE_HIGHLIGHT_TEXT_COLOR));
            animation1->start(QAbstractAnimation::DeleteWhenStopped);

            QPropertyAnimation *animation2 = new QPropertyAnimation(form_effect[k][1], "color");
            animation2->setDuration(ANIMATION_DURATION);
            animation2->setStartValue(QColor(Qt::black));
            animation2->setEndValue(QColor(CLOSEDGE_HIGHLIGHT_TEXT_COLOR));
            animation2->start(QAbstractAnimation::DeleteWhenStopped);
            // });
            //图中加入新结点与新边
            QTimer::singleShot(COMPONENT_UPDATE_INTERVAL, this, [k, this]() {
                workingGraph->SetNodeVisibility(k, true);
                workingGraph->SetArcVisibility(std::min(pre[k], k), std::max(pre[k], k), true);
            });
            // //颜色恢复正常并更新closedge表格
            QTimer::singleShot(COMPONENT_UPDATE_INTERVAL * 2, this, [k, this]() {
                QPropertyAnimation *animation1 = new QPropertyAnimation(form_effect[k][0], "color");
                animation1->setDuration(ANIMATION_DURATION);
                animation1->setStartValue(QColor(CLOSEDGE_HIGHLIGHT_TEXT_COLOR));
                animation1->setEndValue(QColor(Qt::black));
                animation1->start(QAbstractAnimation::DeleteWhenStopped);

                QPropertyAnimation *animation2 = new QPropertyAnimation(form_effect[k][1], "color");
                animation2->setDuration(ANIMATION_DURATION);
                animation2->setStartValue(QColor(CLOSEDGE_HIGHLIGHT_TEXT_COLOR));
                animation2->setEndValue(QColor(Qt::black));
                animation2->start(QAbstractAnimation::DeleteWhenStopped);

                for (int j = 0; j < workingGraph->G.vexnum; ++j) { //辅助数组初始化
                    form[j + 1][1]->setText(D[j] == INT_MAX ? "∞" : QString::number(D[j]));
                    form[j + 1][2]->setText(D[j] == INT_MAX ? "∞" : GetPath(j));
                }
            });
            ++i;
        } else
            timer_round->stop();
    });
}
QString Dijkstra::GetPath(int v)
{
    QString result = "V" + workingGraph->G.vexs[v] + ")";
    while (pre[v] != v) {
        v = pre[v];
        result = "V" + workingGraph->G.vexs[v] + ", " + result;
    }
    result = "(" + result;
    return result;
}

Floyd::Floyd(const Ui::Functions *ui, const MGraphShow &completeGraph, QWidget *parent)
    : QWidget(parent)
    , ui(ui)
    , completeGraph(completeGraph)
{
    workingGraph = new MGraphShow(completeGraph.G, WINDOW_WIDTH * 3 / 4.0 - 25, WINDOW_HEIGHT - 80 - 20 * completeGraph.G.vexnum);
    form.resize(workingGraph->G.vexnum + 1, QVector<QLabel *>(2 * (workingGraph->G.vexnum + 1), NULL)); //包括第一列表头
    form_effect.resize(workingGraph->G.vexnum, QVector<QGraphicsColorizeEffect *>(workingGraph->G.vexnum, NULL)); //不包括第一列表头
    ui->WorkinggraphicsView->setScene(&workingGraph->canvas);
    D.resize(workingGraph->G.vexnum, std::vector<int>(workingGraph->G.vexnum, 0));
    P.resize(workingGraph->G.vexnum, std::vector<int>(workingGraph->G.vexnum, 0));

    //初始化第一列QLabel
    form[0][0] = new QLabel("D", this);
    form[0][workingGraph->G.vexnum + 1] = new QLabel("P", this);
    ui->sheettable->addWidget(form[0][0], 0, 0, 1, 1, Qt::AlignCenter);
    ui->sheettable->addWidget(form[0][workingGraph->G.vexnum + 1], 0, workingGraph->G.vexnum + 1, 1, 1, Qt::AlignCenter);
    for (k = 1; k <= workingGraph->G.vexnum; ++k) {
        form[k][0] = new QLabel("V" + workingGraph->G.vexs[k - 1], this);
        form[0][k] = new QLabel("V" + workingGraph->G.vexs[k - 1], this);
        form[k][workingGraph->G.vexnum + 1] = new QLabel("V" + workingGraph->G.vexs[k - 1], this);
        form[0][workingGraph->G.vexnum + 1 + k] = new QLabel("V" + workingGraph->G.vexs[k - 1], this);
        ui->sheettable->addWidget(form[k][0], k, 0, 1, 1, Qt::AlignCenter);
        ui->sheettable->addWidget(form[0][k], 0, k, 1, 1, Qt::AlignCenter);
        ui->sheettable->addWidget(form[k][workingGraph->G.vexnum + 1], k, workingGraph->G.vexnum + 1, 1, 1, Qt::AlignCenter);
        ui->sheettable->addWidget(form[0][workingGraph->G.vexnum + 1 + k], 0, workingGraph->G.vexnum + 1 + k, 1, 1, Qt::AlignCenter);
    }

    //初始化输出QLabel内容及格式
    int v, w;
    for (v = 0; v < workingGraph->G.vexnum; ++v)
        for (w = 0; w < workingGraph->G.vexnum; ++w) {
            D[v][w] = v == w ? 0 : workingGraph->G.arcs[v][w];
            P[v][w] = w;

            form[v + 1][w + 1] = new QLabel(D[v][w] == INT_MAX ? "∞" : QString::number(D[v][w]), this);
            form[v + 1][workingGraph->G.vexnum + 1 + w + 1] = new QLabel(D[v][w] == INT_MAX || v == w ? "" : GetPath(v, w), this);
            ui->sheettable->addWidget(form[v + 1][w + 1], v + 1, w + 1, 1, 1, Qt::AlignCenter);
            ui->sheettable->addWidget(form[v + 1][workingGraph->G.vexnum + 1 + w + 1], v + 1, workingGraph->G.vexnum + 1 + w + 1, 1, 1, Qt::AlignCenter);
            form_effect[v][w] = new QGraphicsColorizeEffect(form[v + 1][w + 1]);
            form_effect[v][w]->setColor(Qt::black);
            form[v + 1][w + 1]->setGraphicsEffect(form_effect[v][w]);
            form[v + 1][workingGraph->G.vexnum + 1 + w + 1]->setGraphicsEffect(form_effect[v][w]);
        }
}
Floyd::~Floyd()
{
    int v, w;
    for (v = 0; v < workingGraph->G.vexnum; ++v)
        for (w = 0; w < workingGraph->G.vexnum; ++w)
            delete form_effect[v][w];
    delete workingGraph;
}
void Floyd::ShortestPath_FLOYD()
{ //用Floyd算法求有向网络G中各对顶点v和w之间的最短路径P[v][w]及其带权长度D[v][w]
    k = 0;
    timer_round = new QTimer(this);
    timer_round->start(UPDATE_INTERVAL);
    connect(timer_round, &QTimer::timeout, this, [this]() {
        if (k < workingGraph->G.vexnum) {
            int v, w, i;

            //图形闪烁
            workingGraph->SetNodeVisibility(k, false, false);
            workingGraph->SetNodeVisibility(k, true);
            for (i = 0; i < workingGraph->G.vexnum; ++i)
                if (workingGraph->G.arcs[k][i] != INT_MAX) {
                    workingGraph->SetArcVisibility(std::min(k, i), std::max(k, i), false, false);
                    workingGraph->SetArcVisibility(std::min(k, i), std::max(k, i), true);
                }

            for (v = 0; v < workingGraph->G.vexnum; ++v)
                for (w = 0; w < workingGraph->G.vexnum; ++w)
                    if (D[v][k] != INT_MAX && D[k][w] != INT_MAX && D[v][k] + D[k][w] < D[v][w]) {
                        D[v][w] = D[v][k] + D[k][w];
                        P[v][w] = P[v][k];

                        //可视化
                        //文字变色
                        QPropertyAnimation *animation1 = new QPropertyAnimation(form_effect[v][w], "color");
                        animation1->setDuration(ANIMATION_DURATION);
                        animation1->setStartValue(QColor(Qt::black));
                        animation1->setEndValue(QColor(CLOSEDGE_HIGHLIGHT_TEXT_COLOR));
                        animation1->start(QAbstractAnimation::DeleteWhenStopped);
                        //更新QLabel显示内容并文字颜色恢复
                        QTimer::singleShot(COMPONENT_UPDATE_INTERVAL * 2, this, [v, w, this]() {
                            form[v + 1][w + 1]->setText(
                                D[v][w] == INT_MAX ? "∞" : QString::number(D[v][w]));
                            form[v + 1][workingGraph->G.vexnum + 1 + w + 1]->setText(
                                D[v][w] == INT_MAX ? "" : GetPath(v, w));
                            QPropertyAnimation *animation1
                                = new QPropertyAnimation(form_effect[v][w], "color");
                            animation1->setDuration(ANIMATION_DURATION);
                            animation1->setStartValue(QColor(CLOSEDGE_HIGHLIGHT_TEXT_COLOR));
                            animation1->setEndValue(QColor(Qt::black));
                            animation1->start(QAbstractAnimation::DeleteWhenStopped);
                        });
                    }
            ++k;
        } else
            timer_round->stop();
    });
}
QString Floyd::GetPath(int start, int end)
{
    QString result = "V" + completeGraph.G.vexs[start];
    while (P[start][end] != end) {
        result += "V" + completeGraph.G.vexs[P[start][end]];
        start = P[start][end];
    }
    result += "V" + completeGraph.G.vexs[end];
    return result;
}

void Functions::on_pushButton_released()
{
    close();
}
