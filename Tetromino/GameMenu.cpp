#include "GameMenu.h"

GameMenu::GameMenu(QWidget* parent)
    : QMenu(parent)
{
    this->setFixedSize(150, 300);
    this->move(375, 210);

    QAction* continueAction = new QAction("继续游戏", this);    // 按钮：继续游戏
    QAction* returnAction = new QAction("返回主菜单", this);    // 按钮：返回主菜单

    // 将动作添加到游戏菜单中
    this->addAction(continueAction);
    this->addAction(returnAction);

    // 连接槽函数
}
