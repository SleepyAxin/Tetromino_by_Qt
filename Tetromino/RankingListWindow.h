#ifndef RANKINGLISTWINDOW_H
#define RANKINGLISTWINDOW_H

#include<QMainWindow>

#include"Player.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RankingListWindow; }
QT_END_NAMESPACE

// 排行榜界面类
class RankingListWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit RankingListWindow(QWidget *parent = nullptr);    // 构造函数
    ~RankingListWindow();                                     // 析构函数

    void adjustFontColor(const QPixmap& backgroundPic);    // 根据背景颜色调整字体颜色

    void addPlayersInfo();    // 载入玩家信息

    void reset();    // 重置界面

private:
    void sortPlayersByScore(QVector<QJsonObject>& players);    // 依据得分将玩家排序

    Ui::RankingListWindow* ui;

signals:
    void returnToMenuSignal();    // 信号：返回主菜单
};

#endif    // RANKINGLISTWINDOW_H
