#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include<QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MenuWindow; }
QT_END_NAMESPACE

// 主菜单界面类
class MenuWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit MenuWindow(QWidget *parent = nullptr);    // 构造函数
    ~MenuWindow();                                     // 析构函数

    void adjustFontColor(const QPixmap& backgroundPic);    // 根据背景颜色调整字体颜色

private:
    Ui::MenuWindow* ui;

signals:
    void newGameSignal();         // 信号：打开游戏界面 - 新游戏
    void loadArchiveSignal();     // 信号：打开存档界面
    void rankingListSignal();     // 信号：进入排行榜界面
    void settingsSignal();        // 信号：进入设置界面
    void instructionsSignal();    // 信号：进入游戏介绍界面
    void exitSignal();            // 信号：退出游戏
};

#endif    // MENUWINDOW_H
