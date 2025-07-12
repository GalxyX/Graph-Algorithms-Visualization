#include "graphinfo.h"
#include "Parameter.h"
#include "menu.h"
#include "ui_graphinfo.h"
#include <QRegularExpressionValidator>
GraphInfo::GraphInfo(int vertexnum, int edgenum, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GraphInfo)
    , vertexnum(vertexnum)
    , edgenum(edgenum)
    , g_temp(vertexnum, edgenum, QVector<QVector<int>>(vertexnum, QVector<int>(vertexnum, 0)))
{
    ui->setupUi(this);

    ////////////////////////////////////////////////设置窗口////////////////////////////////////////////////
    setWindowFlags(Qt::Window);
    resize(WINDOW_WIDTH, WINDOW_HEIGHT); //窗口大小
    setWindowTitle(WINDOW_HEADLINE);     //程序标题
    setStyleSheet(QString("background-color:") + QString(BACKGROUND_COLOR));
    setAttribute(Qt::WA_DeleteOnClose); //close时销毁
    ////////////////////////////////////////////////设置提示语句的label////////////////////////////////////////////////
    QString temp_s;
    //图的顶点数边数的提示语句
    temp_s = "顶点数 " + QString::number(this->vertexnum) + "    边数 " + QString::number(this->edgenum);
    ui->numNotice->setText(temp_s);
    //输入数字的提示语句
    temp_s = "请建立 " + QString::number(this->edgenum) + " 条边的联系（顶点范围为 0 - " + QString::number(this->vertexnum - 1) + "  边的权值为非负值）：";
    ui->inputNotice->setText(temp_s);
    ////////////////////////////////////////////////设置输入部分////////////////////////////////////////////////
    //表头（第一行）
    int i, j;
    reminds.resize(3);
    for (i = 0; i <= 2; ++i) {
        reminds[i] = new QLabel(this);
        ui->inputGrid->addWidget(reminds[i], 0, i + 1, 1, 1);
    }
    reminds[0]->setText("顶点 1");
    reminds[0]->setAlignment(Qt::AlignCenter);
    reminds[1]->setText("顶点 2");
    reminds[1]->setAlignment(Qt::AlignCenter);
    reminds[2]->setText("边的权值");
    reminds[2]->setAlignment(Qt::AlignCenter);
    //第一列
    edgeCnt.resize(this->edgenum);
    for (i = 0; i < this->edgenum; ++i) {
        edgeCnt[i] = new QLabel(this);
        temp_s = "边 " + QString::number(i + 1);
        edgeCnt[i]->setText(temp_s);
        edgeCnt[i]->setAlignment(Qt::AlignCenter);
        ui->inputGrid->addWidget(edgeCnt[i], i + 1, 0, 1, 1);
        edgeCnt[i]->setMaximumWidth(INPUTGRID_WIDTH);
    }

    //中间输入部分（QLineEdit）
    inputEdit.resize(this->edgenum, QVector<QLineEdit *>(3, NULL));
    //初始化QGraphicScene
    graph = new MGraphShow(MGraph(this->vertexnum, 0), this->width() * 2 / 5 - 20, this->height() - 30);
    //记录输入框修改前数据的vector初始化
    editContent.resize(this->edgenum, QVector<QString>(3, QString()));
    //输入范围正则表达式
    int temp_i = vertexnum - 1;
    QRegularExpression exp_node = QRegularExpression(
        temp_i < 10 ? "^([0-" + QString::number(temp_i) + "])$"
            : "^([0-9]|" + (temp_i >= 20 ? "[1-" + QString::number(temp_i / 10 - 1) + "][0-9]|" : "") + QString::number(temp_i / 10) + "[0-" + QString::number(temp_i % 10) + "]" + ")$");
    QRegularExpressionValidator *validator_node = new QRegularExpressionValidator(this);
    validator_node->setRegularExpression(exp_node);
    QRegularExpression exp_arc = QRegularExpression("^[1-9]\\d{0,6}$");
    QRegularExpressionValidator *validator_arc = new QRegularExpressionValidator(this);
    validator_arc->setRegularExpression(exp_arc);

    //逐个初始化
    for (i = 0; i < this->edgenum; ++i) {
        for (j = 0; j < 3; ++j) {
            //在布局中添加输入框并设置输入框格式
            inputEdit[i][j] = new QLineEdit(this);
            inputEdit[i][j]->setStyleSheet("background:transparent;"); //输入框样式
            inputEdit[i][j]->setMaximumWidth(INPUTGRID_WIDTH);
            inputEdit[i][j]->setMinimumHeight(INPUTGRID_HEIGHT);
            if (j < 2)
                inputEdit[i][j]->setValidator(validator_node); //输入范围
            else
                inputEdit[i][j]->setValidator(validator_arc); //输入范围
            ui->inputGrid->addWidget(inputEdit[i][j], i + 1, j + 1, 1, 1);
            //设置实时更新图像的信号与槽
            connect(inputEdit[i][j], &QLineEdit::textChanged, this, [i, j, this]() {
                //获取修改后的输入框值
                QString now = inputEdit[i][j]->text();
                if (editContent[i][j] == now) //若值未改变则直接返回不做更改
                    return;
                else { //更新图
                    //获取修改前后该行输入框内的值
                    QString vex1 = inputEdit[i][0]->text();
                    QString vex2 = inputEdit[i][1]->text();
                    QString arc = inputEdit[i][2]->text();
                    int vex1_old_num = editContent[i][0].toInt();
                    int vex2_old_num = editContent[i][1].toInt();
                    int vex1_new_num = vex1.toInt();
                    int vex2_new_num = vex2.toInt();
                    int arc_new_num = arc.toInt();
                    bool no_now = vex1.isEmpty() || vex2.isEmpty() || arc.isEmpty()
                                  || vex1_new_num == vex2_new_num;
                    bool no_previous = editContent[i][0].isEmpty() || editContent[i][1].isEmpty() || editContent[i][2].isEmpty()
                                       || vex1_old_num == vex2_old_num;
                    if (!no_previous && !no_now) { //修改前后均构成完整输入
                        --g_temp.arcs[vex1_old_num][vex2_old_num];
                        --g_temp.arcs[vex2_old_num][vex1_old_num];
                        if (!g_temp.arcs[vex1_old_num][vex2_old_num])
                            graph->DeleteCreatedArc(std::min(vex1_old_num, vex2_old_num), std::max(vex1_old_num, vex2_old_num));
                        graph->CreateNewArc(std::min(vex1_new_num, vex2_new_num), std::max(vex1_new_num, vex2_new_num), arc_new_num);
                        ++g_temp.arcs[vex1_new_num][vex2_new_num];
                        ++g_temp.arcs[vex2_new_num][vex1_new_num];
                    } else if (no_previous && !no_now) { //修改前输入不完整，修改后输入完整
                        graph->CreateNewArc(std::min(vex1_new_num, vex2_new_num), std::max(vex1_new_num, vex2_new_num), arc_new_num);
                        ++g_temp.arcs[vex1_new_num][vex2_new_num];
                        ++g_temp.arcs[vex2_new_num][vex1_new_num];
                    } else if (!no_previous && no_now) { //修改前输入完整，修改后输入不完整
                        --g_temp.arcs[vex1_old_num][vex2_old_num];
                        --g_temp.arcs[vex2_old_num][vex1_old_num];
                        if (!g_temp.arcs[vex1_old_num][vex2_old_num])
                            graph->DeleteCreatedArc(std::min(vex1_old_num, vex2_old_num), std::max(vex1_old_num, vex2_old_num));
                    }
                    editContent[i][j] = now;
                }
            });
        }
    }
    ui->graphicsView->setScene(&graph->canvas);
    graph->canvas.setItemIndexMethod(QGraphicsScene::NoIndex);
}

GraphInfo::~GraphInfo()
{
    delete ui;
    delete graph;
}

void GraphInfo::on_finishButton_released()
{
    QVector<QVector<int>> graph(this->vertexnum, QVector<int>(this->vertexnum, INT_MAX)); //邻接矩阵
    int i, v1, v2;
    for (i = 0; i < this->edgenum; ++i) {
        if (inputEdit[i][0]->text().isEmpty() || inputEdit[i][1]->text().isEmpty() || inputEdit[i][2]->text().isEmpty()) {
            ui->errRemind->setText("请输入所有参数");
            return;
        }
        v1 = inputEdit[i][0]->text().toInt();
        v2 = inputEdit[i][1]->text().toInt();
        if (v1 >= this->vertexnum || v2 >= this->vertexnum) {
            ui->errRemind->setText("请确保顶点范围正确");
            return;
        } else if (v1 == v2) {
            ui->errRemind->setText("边的两个端点相同");
            return;
        }
        if (graph[v1][v2] != INT_MAX) {
            ui->errRemind->setText("存在重复边");
            return;
        } else {
            graph[v1][v2] = graph[v2][v1] = inputEdit[i][2]->text().toInt();
        }
    }
    ui->errRemind->clear();
    Menu *menu_window = new Menu(MGraph(this->vertexnum, this->edgenum, graph));
    menu_window->show();
    this->close();
}
