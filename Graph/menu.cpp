#include "menu.h"
#include "Parameter.h"
#include "ui_menu.h"
Menu::Menu(MGraph G, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Menu)
    , graphscene(G, WINDOW_WIDTH / 2 - 30, WINDOW_HEIGHT - 40)
{
    ui->setupUi(this);
    ////////////////////////////////////////////////设置窗口////////////////////////////////////////////////
    setWindowFlags(Qt::Window);
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);                                           //窗口大小
    setWindowTitle(WINDOW_HEADLINE);                                               //程序标题
    this->setStyleSheet(QString("background-color:") + QString(BACKGROUND_COLOR)); //设置窗口背景色
    //标题初始化
    ui->infoLabel->setStyleSheet(QString("color:") + QString(TITLE_COLOR)); //设置标题标签颜色
    ui->graphicsView->setScene(&graphscene.canvas);
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_problem1Button_released()
{
    solution = new Functions(graphscene.G, 1, this);
    connect(solution, &Functions::destroyed, this, &Menu::show);
    hide();
    solution->show();
}

void Menu::on_problem2_1Button_released()
{
    if (graphscene.G.isConnected()) {
        solution = new Functions(graphscene.G, 2, this);
        connect(solution, &Functions::destroyed, this, &Menu::show);
        hide();
        solution->show();
    } else
        ui->errLabel->setText("该图不是连通图，无最小生成树");
}

void Menu::on_problem2_2Button_released()
{
    if (graphscene.G.isConnected()) {
        solution = new Functions(graphscene.G, 3, this);
        connect(solution, &Functions::destroyed, this, &Menu::show);
        hide();
        solution->show();
    } else
        ui->errLabel->setText("该图不是连通图，无最小生成树");
}

void Menu::on_problem3Button_released()
{
    if (graphscene.G.isConnected()) {
        solution = new Functions(graphscene.G, 4, this);
        connect(solution, &Functions::destroyed, this, &Menu::show);
        hide();
        solution->show();
    } else
        ui->errLabel->setText("该图不是连通图，无法求解单源到所有顶点最短路径");
}

void Menu::on_problem4Button_released()
{
    if (graphscene.G.isConnected()) {
        solution = new Functions(graphscene.G, 5, this);
        connect(solution, &Functions::destroyed, this, &Menu::show);
        hide();
        solution->show();
    } else
        ui->errLabel->setText("该图不是连通图，无法求解所有顶点之间最短路径");
}
