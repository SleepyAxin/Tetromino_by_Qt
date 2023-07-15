#ifndef BACKGROUNDPICTURE_H
#define BACKGROUNDPICTURE_H

#include<QDebug>
#include<QFile>
#include<QJsonDocument>
#include<QJsonObject>
#include<QMainWindow>
#include"QPainter"
#include<QPalette>
#include<QPixmap>

#include"SettingsWindow.h"

// 背景图片类
class BackgroundPicture
{
public:
    BackgroundPicture();    // 构造函数

    void showPicture(QMainWindow* mainWindow);   // 显示背景图片

    void setChoice(const int choice);         // 设置背景图片选择：传入索引
    void setChoice(const QString& choice);    // 设置背景图片选择：传入名字

    QPixmap getPicture() const;          // 获取背景图片

private:
    static int getChoiceFromString(const QString& choiceString);    // 转化背景图片选择字符串为索引

    void loadSettings();    // 加载设置

    QStringList pictureList;    // 背景图片选项列表

    int pictureChoice = 0;    // 背景图片选项
};

#endif    // BACKGROUNDPICTURE_H
