#include "homepage.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //程序对象
    // QVector<QVector<int>> g={{INT_MAX,INT_MAX,INT_MAX,123,5},{INT_MAX,INT_MAX,57,INT_MAX,4},{INT_MAX,57,INT_MAX,69,INT_MAX},{123,INT_MAX,69,INT_MAX,32},{5,4,INT_MAX,32,INT_MAX}};
    // MGraph g1(5,6,g);
    // Menu w(g1);
    Homepage* w=new Homepage;
    w->show();
    return a.exec(); //程序永远执行，死循环
}
