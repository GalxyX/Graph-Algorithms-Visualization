#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "graphinfo.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Homepage;
}
QT_END_NAMESPACE

class Homepage : public QWidget //首页
{
    Q_OBJECT

public:
    Homepage(QWidget *parent = nullptr);
    ~Homepage();

private slots:
    void on_confirmButton_released();

private:
    Ui::Homepage *ui;
    GraphInfo *graphinfo_window; //图参数输入页面
};
#endif // HOMEPAGE_H
