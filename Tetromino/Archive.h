#ifndef ARCHIVE_H
#define ARCHIVE_H

#include<QDir>
#include<QFile>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QMainWindow>
#include<QPoint>
#include<QString>
#include<QVector>

#include"Block/Block.h"
#include"GameBoard.h"
#include"Player.h"

// 存档类
class Archive
{
public:
    Archive();    // 构造函数

    static void saveScreenshot(QMainWindow* window);    // 截取当前窗口并保存为图片

    void setArchiveID(const int id);    // 设置存档序号
    int getArchiveID() const;           // 获取存档序号

    void setPlayerInfo(const Player& player);    // 设置玩家信息
    Player getPlayerInfo() const;                // 获取玩家信息

    void setBoardInfo(const int width, const int height);    // 设置棋盘宽高
    int getBoardWidth() const;                               // 获取棋盘宽度
    int getBoardHeight() const;                              // 获取棋盘高度

    void setGameScore(const int score);    // 设置游戏得分
    int getGameScore() const;              // 获取游戏得分

    void setBlockInfo(const Block& currentBlock, const Block& nextBlock);    // 设置方块信息
    int getCurrentBlockShape() const;                                        // 获取当前方块形状索引号
    int getCurrentBlockStatus() const;                                       // 获取当前方块形态索引号
    int getNextBlockShape() const;                                           // 获取下一个方块形状索引号

    void setFilledGridPoint(const QVector<QPoint>& filledGridPoints);    // 设置已填充格子坐标
    QVector<QPoint> getFilledGridPoints() const;                         // 获取已填充格子坐标

    void setBlockPosition(const QPoint& p);    // 设置方块坐标
    QPoint getBlockPosition() const;           // 获取方块坐标

    bool isArchiveValid() const;    // 存档是否有效（玩家是否存在）

    void createArchive();    // 创建存档
    void readArchive();      // 读取存档
    void updateArchive();    // 更新存档信息（玩家名字修改）

protected:
    static void saveArchives(const QVector<QJsonObject>& archives);    // 保存存档到文件
    static QVector<QJsonObject> loadArchives();                       // 从文件加载存档

    static int getNewestID();    // 获取最新存档序号

    bool archiveValidity = false;    // 存档有效性（玩家是否存在）

private:
    static QJsonArray convertPointVectorToJsonArray(const QVector<QPoint>& points);    // 转化点向量为 Json 数组
    static QVector<QPoint> convertJsonArrayToPointVector(const QJsonArray& points);    // 转化 Json 数组为点向量

    int archiveID = 0;   // 存档序号

    Player player;    // 当前玩家

    int boardWidth;     // 当前棋盘宽度
    int boardHeight;    // 当前棋盘高度

    int gameScore;    // 当前游戏得分

    int currentBlockShape = 0;     // 当前方块形状索引号
    int currentBlockStatus = 0;    // 当前方块形态索引号
    int nextBlockShape = 0;        // 下一个方块形状索引号

    QVector<QPoint> filledGridPoints;    // 已固定方块的坐标数组

    QPoint blockPosition;    // 正在移动方块的坐标
};

#endif    // ARCHIVE_H
