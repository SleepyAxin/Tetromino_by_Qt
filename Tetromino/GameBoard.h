#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include<QColor>
#include<QDebug>
#include<QFile>
#include<QJsonDocument>
#include<QJsonObject>
#include<QPainter>
#include<QPixmap>
#include<QRect>
#include<QRectF>
#include<QGraphicsPixmapItem>

#include"SettingsWindow.h"

// 游戏棋盘格子类
class GameBoard: public QGraphicsPixmapItem
{
public:
    GameBoard();    // 构造函数

    static void setBoardWidth(const int w);    // 设置棋盘宽度
    static int getBoardWidth();                // 获取棋盘宽度

    static void setBoardHeight(const int h);   // 设置棋盘高度
    static int getBoardHeight();               // 获取棋盘高度

    static void setGridSize(const int s);    // 设置格子尺寸
    static int getGridSize();                // 获取格子尺寸

    static void setSmallBoardSize(const int s);    // 设置小棋盘尺寸
    static int getSmallBoardSize();                // 获取小棋盘尺寸

    void setGridState(const int state);    // 设置格子状态
    int getGridState() const;              // 获取格子状态

    QColor getColorFromString(const QString& s);    // 将颜色字符串转化为索引

private:
    void loadSettings();

    static int boardHeight;    // 棋盘高度
    static int boardWidth;     // 棋盘宽度
    static int gridSize;       // 格子尺寸

    static int smallBoardSize;    // 小棋盘边长

    int gridState = 0;    // 格子状态
    // 0 -- 无方块
    // 1 -- 有移动方块
    // 2 -- 有固定方块

    QPixmap backGrid;      // 空白格子背景图片
    QPixmap movingGrid;    // 移动方块背景图片
    QPixmap fixedGrid;     // 固定方块背景图片
};

#endif    // GAMEBOARD_H
