#ifndef INSTRUCTIONSWINDOW_H
#define INSTRUCTIONSWINDOW_H

#include<QMainWindow>

#include"SettingsWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class InstructionsWindow; }
QT_END_NAMESPACE

// 游戏介绍界面类
class InstructionsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit InstructionsWindow(QWidget *parent = nullptr);    // 构造函数
    ~InstructionsWindow();                                     // 析构函数

    void adjustFontColor(const QPixmap& backgroundPic);    // 根据背景颜色调整字体颜色

    void setInstructionsText();    // 设置游戏介绍文本

private:
    Ui::InstructionsWindow* ui;

signals:
    void returnToMenuSignal();    // 信号：返回主菜单
};

#endif    // INSTRUCTIONSWINDOW_H
