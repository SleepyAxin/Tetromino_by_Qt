#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QStackedWidget>

#include"ArchiveWindow.h"
#include"BackgroundMusic.h"
#include"BackgroundPicture.h"
#include"GameWindow.h"
#include"InstructionsWindow.h"
#include"MenuWindow.h"
#include"PlayerWindow.h"
#include"RankingListWindow.h"
#include"SettingsWindow.h"

// 主界面类
class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);    // 构造函数
    ~MainWindow();                            // 析构函数

private:
    void adjustFontColor();    // 调整字体颜色

    QStackedWidget* stackedWidget;    // 集合部件：在同一个窗口中展示所有窗口

    MenuWindow* menuWindow;                    // 主菜单窗口
    SettingsWindow* settingsWindow;            // 设置窗口
    PlayerWindow* playerWindow;                // 玩家窗口
    GameWindow* gameWindow;                    // 游戏窗口
    ArchiveWindow* archiveWindow;              // 游戏存档窗口
    RankingListWindow* rankingListWindow;      // 排行榜窗口
    InstructionsWindow* instructionsWindow;    // 游戏介绍窗口

    BackgroundMusic* backgroundMusic;        // 背景音乐
    BackgroundPicture* backgroundPicture;    // 背景图片

private slots:
    void selectPlayer();          // 进入玩家窗口
    void playerReturnToMenu();    // 玩家窗口返回主菜单

    void enterGame();           // 进入游戏窗口 - 新游戏
    void gameReturnToMenu();    // 游戏窗口返回主菜单

    void loadArchive();            // 进入存档窗口
    void archiveReturnToMenu();    // 存档窗口返回主菜单

    void readGame();    // 进入游戏窗口 - 读取游戏

    void rankingList();                // 进入排行榜窗口
    void rankingListReturnToMenu();    // 排行榜窗口返回主菜单

    void settings();                // 进入设置窗口
    void settingsReturnToMenu();    // 设置窗口返回主菜单

    void instructions();                // 进入游戏介绍窗口
    void instructionsReturnToMenu();    // 游戏介绍窗口返回主菜单

    void setMusicVolume();    // 设置背景音乐音量
    void setMusicChoice();    // 设置背景音乐曲目

    void setPictureChoice();    // 设置背景图片选择
};

#endif    // MAINWINDOW_H
