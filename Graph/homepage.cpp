#include "homepage.h"
#include "Parameter.h"
#include "ui_homepage.h"
Homepage::Homepage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Homepage)
{
    ui->setupUi(this);
    //窗口初始化
    setWindowFlags(Qt::Window);
    resize(WINDOW_WIDTH, WINDOW_HEIGHT); //窗口大小
    setWindowTitle(WINDOW_HEADLINE);     //程序标题
    this->setStyleSheet(QString("background-color:") + QString(BACKGROUND_COLOR)); //设置窗口背景色
    //标题初始化
    ui->ProgramTitle->setStyleSheet(QString("color:") + QString(TITLE_COLOR)); //设置标题标签颜色
    ui->vertexInput->setClearButtonEnabled(true);
    ui->edgeInput->setClearButtonEnabled(true);
    setAttribute(Qt::WA_DeleteOnClose); //close时销毁
}

Homepage::~Homepage()
{
    delete ui;
}

void Homepage::on_confirmButton_released()
{
    bool legal1, legal2; //输入数据是否为数字（合法）
    int vertexnum = ui->vertexInput->text().toInt(&legal1);
    int edgenum = ui->edgeInput->text().toInt(&legal2);
    //错误处理，判断顶点及边数量是否合法
    if (!legal1 || !legal2)
        ui->errorReminder->setText("请输入合法范围的数字");
    else if (vertexnum <= 0 && edgenum < 0)
        ui->errorReminder->setText("顶点数须为正值，边数需为非负值");
    else if (vertexnum <= 0)
        ui->errorReminder->setText("顶点数须为正值");
    else if (edgenum < 0)
        ui->errorReminder->setText("边数需为非负值");
    else if (vertexnum >= 100)
        ui->errorReminder->setText("顶点数过多，建议小于100");
    else if (edgenum > vertexnum * (vertexnum - 1) / 2) {
        QString errortext = "边数大于" + QString::number(vertexnum) + "无向简单图最大边数量" + QString::number(vertexnum * (vertexnum - 1) / 2);
        ui->errorReminder->setText(errortext);
    }
    else {
        //切换新界面
        ui->errorReminder->clear();
        ui->vertexInput->clear();
        ui->edgeInput->clear();
        graphinfo_window = new GraphInfo(vertexnum, edgenum);
        graphinfo_window->show();
        this->close();
    }
}
