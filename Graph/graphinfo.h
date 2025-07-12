#ifndef GRAPHINFO_H
#define GRAPHINFO_H

#include "mgraph.h"
#include <QLabel>
#include <QLineEdit>
#include <QVector>
#include <QWidget>

namespace Ui {
class GraphInfo;
}

class GraphInfo : public QWidget //图参数输入页面
{
    Q_OBJECT

public:
    explicit GraphInfo(int vertexnum, int edgenum, QWidget *parent = nullptr);
    ~GraphInfo();

private slots:
    void on_finishButton_released();

private:
    Ui::GraphInfo *ui;
    int vertexnum;
    int edgenum;
    QVector<QLabel *> reminds;               //三句输入提示
    QVector<QLabel *> edgeCnt;               //所有边计数
    QVector<QVector<QLineEdit *>> inputEdit; //输入框，一维为第几条边，二维为三项输入内容
    QVector<QVector<QString>> editContent; //输入框内容
    MGraph g_temp; //暂存当前时刻每条边输入了几次，仅使用g_temp.arcs
    MGraphShow *graph;
    // QWidget *this_widget_parent;
};

#endif // GRAPHINFO_H
