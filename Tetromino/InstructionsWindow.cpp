#include"InstructionsWindow.h"
#include"ui_InstructionsWindow.h"

InstructionsWindow::InstructionsWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::InstructionsWindow)
{
    this->ui->setupUi(this);

    this->setFixedSize(900, 720);

    this->setInstructionsText();    // 设置游戏说明文字

    this->ui->instructionsLabel->setWordWrap(true);    // 设置自动换行

    // 信号：点击按钮 - 返回主菜单 -> 信号：返回主菜单
    connect(this->ui->returnToMenuButton, &QPushButton::clicked, this, &InstructionsWindow::returnToMenuSignal);
}

InstructionsWindow::~InstructionsWindow()
{
    if (this->ui != nullptr)
    {
        delete this->ui;
        this->ui = nullptr;
    }
}

// public 函数

void InstructionsWindow::adjustFontColor(const QPixmap& backgroundPic)
{
    QImage image = backgroundPic.toImage();
    QColor backgroundColor = image.pixelColor(0, 0);

    // 根据背景颜色选择字体颜色
    if (backgroundColor.lightness() < 128)
    {
        this->ui->titleLabel->setStyleSheet("color: white;");
        this->ui->instructionsLabel->setStyleSheet("color: white;");
    }
    else
    {
        this->ui->titleLabel->setStyleSheet("color: black;");
        this->ui->instructionsLabel->setStyleSheet("color: black;");
    }
}

void InstructionsWindow::setInstructionsText()
{
    QJsonObject settings = SettingsWindow::loadSettings();

    QString text;

    text.append("*游戏介绍*\n");

    text.append("俄罗斯方块是一款经典的益智游戏，最早由俄罗斯程序员Alexey Pajitnov于1984年开发。"
                "这款游戏的目标是通过旋转、移动和放置不同形状的方块，使它们在游戏区域的底部形成完整的水平行，"
                "当一行被填满时，它将被消除并得到分数。\n");

    text.append("游戏区域是一个矩形网格，通常是10列宽和20行高。"
                "方块由四个小方块组成，可以是不同的形状，如长条形、正方形、L形、T形等。"
                "玩家可以使用键盘上的方向键左右移动方块，在方块下降时按下方向键向下加速下降，按下空格键可以直接将方块放置到底部。"
                "这款游戏的目标是通过旋转、移动和放置不同形状的方块，使它们在游戏区域的底部形成完整的水平行，"
                "当一行被填满时，它将被消除并得到分数。\n");

    text.append("游戏的挑战在于，随着时间的推移，方块下降的速度会逐渐增加，"
                "玩家需要快速思考和做出决策，以便正确地放置方块并清除行。"
                "如果方块堆积到游戏区域的顶部，游戏就会结束。\n");

    text.append("\n*操作说明*\n");

    text.append(settings["moveLeftKey"].toString());
    text.append(" : 控制方块向左移动\n");

    text.append(settings["moveRightKey"].toString());
    text.append(" : 控制方块向右移动\n");

    text.append(settings["rorateKey"].toString());
    text.append(" : 控制方块旋转\n");

    text.append(settings["quickenKey"].toString());
    text.append(" : 控制方块加速\n");

    text.append(settings["pauseKey"].toString());
    text.append(" : 暂停游戏");

    this->ui->instructionsLabel->setText(text);
}








