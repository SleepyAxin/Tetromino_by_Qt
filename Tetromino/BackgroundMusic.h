#ifndef BACKGROUNDMUSIC_H
#define BACKGROUNDMUSIC_H

#include<QFile>
#include<QJsonDocument>
#include<QJsonObject>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include<QObject>

#include"SettingsWindow.h"

// 背景音乐类
class BackgroundMusic: public QObject
{
    Q_OBJECT

public:
    explicit BackgroundMusic(QObject* parent = nullptr);    // 构造函数

    void playMusic();    // 播放音乐
    void stopMusic();    // 停止音乐

    void setVolume(const int volume);         // 设置音乐音量
    void setChoice(const int choice);         // 设置音乐曲目：传入曲目索引
    void setChoice(const QString& choice);    // 设置音乐曲目：传入曲目名字

private:
    static int getChoiceFromString(const QString& choiceString);    // 转化背景音乐选择字符串为索引

    void loadSettings();     // 加载设置

    int musicChoice = 0;    // 音乐曲目

    QMediaPlayer* musicPlayer;    // 音乐播放器

    QMediaPlaylist* musicPlayerlist;    // 音乐播放列表

    QStringList musicList;    // 音乐曲目列表
};

#endif    // BACKGROUNDMUSIC_H
