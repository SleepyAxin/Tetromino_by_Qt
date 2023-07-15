#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include<QInputDialog>
#include<QMainWindow>
#include<QMessageBox>

#include"Player.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PlayerWindow; }
QT_END_NAMESPACE

// 玩家界面类
class PlayerWindow: public QMainWindow, public Player
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget* parent = nullptr);    // 构造函数
    ~PlayerWindow();                                     // 析构函数

    void adjustFontColor(const QPixmap& backgroundPic);    // 根据背景颜色调整字体颜色

private:
    Ui::PlayerWindow* ui;

    void addNameToComboBox();    // 加载所有玩家到玩家选择框

private slots:
    void selectPlayer();       // 选择玩家
    void createNewPlayer();    // 创建新玩家

    void changePlayerName();    // 修改玩家名字

    void deletePlayer();        // 删除玩家
    void deleteAllPlayers();    // 删除全部玩家

    void ensurePlayer();    // 确认玩家

    void reset();    // 重置界面

signals:
    void enterGameSignal();       // 信号：进入游戏
    void returnToMenuSignal();    // 信号：返回主菜单
};

#endif    // PLAYERWINDOW_H
