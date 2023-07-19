#ifndef BLOCK_H
#define BLOCK_H

#include"GameBoard.h"

#include<QDebug>
#include<QList>
#include<QPoint>
#include<QVector>

// 方块基类
class Block
{
public:
    Block();    // 构造函数

    int getCurrentShape() const;     // 获取当前形状
    int getCurrentStatus() const;    // 获取当前形态

    void previousStatus();    // 改变至上一个形态
    bool nextStatus();        // 判断是否可以改变至下一个形态

    QList<QPoint> getIndexPoints() const;             // 获取当前方块的位置占用情况
    QList<QPoint> getIndexPoints(int index) const;    // 获取当前方块指定形态的位置占用情况

    void setPosition(const QPoint& p);    // 设置坐标位置
    bool movePosition(int x, int y);      // 判断是否可以移动坐标位置

protected:
    int blockShapeIndex = 0;     // 当前方块形状的索引数

    QList<QList<QPoint>> totalBlocks;    // 全部该方块坐标组

private:
    int blockStatusIndex = 0;    // 当前方块形态的索引数

    QPoint currentPosition;     // 当前方块位置
};

#endif    // BLOCK_H
