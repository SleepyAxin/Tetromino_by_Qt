#ifndef GAME_H
#define GAME_H

#include<QTimer>

// 游戏类
class Game
{
protected:
    bool isStartGame = false;    // 游戏开始状态
    bool isPauseGame = true;     // 游戏暂停状态

    int timerID;        // 计时器序号
    int timerCounts;    // 计时器计数

    int gameScore = 0;    // 游戏得分
};

#endif     // GAME_H
