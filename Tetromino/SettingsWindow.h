#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include<QDebug>
#include<QFile>
#include<QJsonDocument>
#include<QJsonObject>
#include<QKeyEvent>
#include<QMainWindow>
#include<QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsWindow; }
QT_END_NAMESPACE

// 设置界面类
class SettingsWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget* parent = nullptr);    // 构造函数
    ~SettingsWindow();                                     // 析构函数

    int getMusicVolume() const;          // 获取背景音乐音量
    QString getMusicChoice() const;      // 获取背景音乐曲目
    QString getPictureChoice() const;    // 获取背景图片选项

    void adjustFontColor(const QPixmap& backgroundPic);    // 根据背景颜色调整字体颜色

    static QJsonObject loadSettings();    // 加载存档

public slots:
    void applySettings();    // 应用设置到设置界面

private slots:
    void saveSettings();           // 保存设置
    void saveSpecialSettings();    // 保存特殊设置

    void changeSliderValue(const int value);    // 改变滑块数值

    void checkKey();    // 检测按键合法性

    static void resetSettings();    // 重置设置

private:
    Ui::SettingsWindow* ui;

signals:
    void returnToMenuSignal();    // 信号：返回主菜单

    void setMusicVolumeSignal();    // 信号：设置背景音乐音量
    void setMusicChoiceSignal();    // 信号：设置背景音乐曲目

    void setPictureChoiceSignal();    // 信号：设置背景图片选择
};

#endif    // SETTINGSWINDOW_H
