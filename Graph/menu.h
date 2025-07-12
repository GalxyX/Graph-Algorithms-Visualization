#ifndef MENU_H
#define MENU_H

#include "functions.h"
#include "mgraph.h"
#include <QWidget>

namespace Ui {
class Menu;
}

class Menu : public QWidget //选择演示内容页面
{
    Q_OBJECT

public:
    explicit Menu(MGraph G, QWidget *parent = nullptr);
    ~Menu();

private slots:
    void on_problem1Button_released();

    void on_problem2_1Button_released();

    void on_problem2_2Button_released();

    void on_problem3Button_released();

    void on_problem4Button_released();

private:
    Ui::Menu *ui;
    MGraphShow graphscene;
    Functions *solution;
};

#endif // MENU_H
