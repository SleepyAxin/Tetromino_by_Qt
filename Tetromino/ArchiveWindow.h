#ifndef ARCHIVEWINDOW_H
#define ARCHIVEWINDOW_H

#include<QDesktopWidget>
#include<QFileDialog>
#include<QMainWindow>

#include"Archive.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ArchiveWindow; }
QT_END_NAMESPACE

// 存档窗口类
class ArchiveWindow: public QMainWindow, public Archive
{
    Q_OBJECT

public:
    explicit ArchiveWindow(QWidget* parent = nullptr);    // 构造函数
    ~ArchiveWindow();                                     // 析构函数

    void reset();    // 重置界面

    void addArchives();    // 加载所有存档

public slots:
    void showImage();    // 显示存档图片

private:
    Ui::ArchiveWindow* ui;

private slots:
    void copyArchive();          // 复制存档
    void deleteArchive();        // 删除存档
    void deleteAllArchives();    // 删除所有存档
    void ensureArchive();        // 确认存档

signals:
    void readGameSignal();        // 信号：读取存档
    void returnToMenuSignal();    // 信号：返回主菜单
};

#endif   // ARCHIVEWINDOW_H
