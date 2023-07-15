#include "MainWindow.h"

#include <QApplication>    // 包含一个应用程序类的头文件

// main 程序入口
// argc 命令行变量的数据
// argv 命令行变量的数组
int main(int argc, char* argv[])
{
    // a 应用程序对象，在 Qt 中，应用程序对象有且只有一个
    QApplication a(argc, argv);

    // 窗口对象 myWidget 父类 -> QWidget
    MainWindow w;

    // 窗口对象默认不会显示，需要调用 show 方法显示窗口
    w.show();

    // 让应用程序对象进入消息循环机制
    // 让代码阻塞到这行
    return a.exec();
}
