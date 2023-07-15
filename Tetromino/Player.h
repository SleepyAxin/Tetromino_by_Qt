#ifndef PLAYER_H
#define PLAYER_H

#include<QDebug>
#include<QDir>
#include<QFile>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QString>
#include<QVector>

// 玩家类
class Player
{
public:
    void setPlayerID(const int id);    // 设置玩家序号
    int getPlayerID() const;           // 获取玩家序号

    void setPlayerName(const QString& name);    // 设置玩家名字
    QString getPlayerName() const;              // 获取玩家名字

    void setPlayerHighestScore(const int score);    // 设置玩家最高得分
    int getPlayerHighestScore() const;              // 获取玩家最高得分

    void setBoardWidth(const int w);    // 设置达成最高分时棋盘宽度
    int getBoardWidth() const;          // 获取达成最高分时棋盘宽度

    void setBoardHeight(const int h);    // 设置达成最高分时棋盘高度
    int getBoardHeight() const;          // 获取达成最高分时棋盘高度

    void setBlockSpeed(const int v);    // 设置达成最高分时方块下落速度
    int getBlockSpeed() const;          // 设置达成最高分时方块下落速度

    void updatePlayerInfo();    // 更新玩家信息到文件
    void createPlayerInfo();    // 创建玩家信息到文件

    static int getNewestID();    // 获取最新玩家序号

    static void setUpdatedPlayerInfo(const QStringList& names);    // 设置玩家信息更新
    static QStringList getUpdatedPlayerInfo();                     // 读取玩家信息更新

    static QVector<QJsonObject> loadPlayersInfo();    // 从文件中加载多个玩家信息

protected:
    static void savePlayersInfo(const QVector<QJsonObject>& players);    // 保存多个玩家信息到文件中

private:
    int playerID = 10000;          // 玩家序号
    QString playerName;            // 玩家名字
    int playerHighestScore = 0;    // 玩家最高得分

    int boardWidth;       // 达成最高分时棋盘宽度
    int boardHeight;      // 达成最高分时棋盘高度
    double blockSpeed;    // 达成最高分时方块下落速度

    static QStringList names;    // 玩家信息更新
};

#endif    // PLAYER_H
