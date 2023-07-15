#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include<QDebug>
#include<QFile>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QGraphicsRectItem>
#include<QJsonDocument>
#include<QJsonObject>
#include<QKeyEvent>
#include<QMainWindow>
#include<QMessageBox>
#include<QPushButton>
#include<QTime>

#include"Archive.h"
#include"Block/Block.h"
#include"Block/BlockI.h"
#include"Block/BlockJ.h"
#include"Block/BlockL.h"
#include"Block/BlockO.h"
#include"Block/BlockS.h"
#include"Block/BlockT.h"
#include"Block/BlockZ.h"
#include"Game.h"
#include"GameBoard.h"
#include"Player.h"
#include"SettingsWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

// 游戏界面类
class GameWindow: public QMainWindow, public Game
{
    Q_OBJECT

public:
    GameWindow(QWidget* parent = nullptr, const Archive* archive = nullptr);    // 构造函数
    ~GameWindow();                                                              // 析构函数

    void showPlayerInfo(const Player& player);    // 展示玩家信息

    void adjustFontColor(const QPixmap& backgroundPic);    // 根据背景颜色调整字体颜色

    static int getIntFromQKeyQuence(const QKeySequence& key);    // 将键位转化为整型数值

protected:
    void timerEvent(QTimerEvent* event) override;       // 事件：计时器
    void keyPressEvent(QKeyEvent *event) override;      // 事件：按压按键
    void keyReleaseEvent(QKeyEvent *event) override;    // 事件：释放按键

private:
    void loadSettings();    // 加载设置

    void drawUI();            // 绘制界面
    void drawMainBoard();     // 绘制主棋盘
    void drawSmallBoard();    // 绘制小棋盘

    void appendBlocks();     // 载入所有形状的方块
    void rangeBlock();       // 产生随机方块
    void fillNextBlock();    // 在小棋盘中填充下一个方块

    void blockReach(bool& isNew);             // 方块到达底部或与其他方块堆叠
    void blocksFillLine(const bool isNew);    // 消去填满的行

    void readGame(const Archive& archive);    // 读取游戏

    Ui::GameWindow* ui;

    QRect mainRect;    // 游戏主棋盘矩形

    QGraphicsScene* mainScene;     // 游戏主棋盘场景
    QGraphicsScene* smallScene;    // 游戏小棋盘场景

    QVector<QVector<GameBoard*>> mainBoard;     // 游戏主棋盘
    QVector<QVector<GameBoard*>> smallBoard;    // 游戏小棋盘：显示下一个图形

    double blockSpeed = 1.0;    // 方块下落速度

    Block currentBlock;    // 当前方块
    Block nextBlock;       // 下一个方块

    QList<Block*> allBlocks;    // 所有七种方块的列表

    QKeySequence moveLeftKey = QKeySequence(Qt::Key_A);     // 键位：左移
    QKeySequence moveRightKey = QKeySequence(Qt::Key_D);    // 键位：右移
    QKeySequence quickenKey = QKeySequence(Qt::Key_S);      // 键位：加速
    QKeySequence rorateKey = QKeySequence(Qt::Key_W);       // 键位：旋转
    QKeySequence pauseKey = QKeySequence(Qt::Key_P);        // 键位：暂停游戏

    Player player;    // 当前游戏玩家

    Archive newArchive;    // 新存档

private slots:
    void startGame();               // 槽函数：开始游戏
    void endGame();                 // 槽函数：结束游戏
    void continueAndPauseGame();    // 槽函数：继续游戏和暂停游戏
    void resetGame();               // 槽函数：恢复游戏至初始状态
    void saveGame();                // 槽函数：保存进度

signals:
    void returnToMenuSignal();    // 信号：返回主菜单
};

#endif    // GAMEWINDOW_H
