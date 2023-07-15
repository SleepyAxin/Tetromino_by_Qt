#include"MenuWindow.h"
#include"ui_MenuWindow.h"

MenuWindow::MenuWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MenuWindow)
{
    this->ui->setupUi(this);

    this->setFixedSize(900, 720);

    // 信号：点击按钮 - 新游戏 -> 信号：新游戏
    connect(this->ui->newGameButton, &QPushButton::clicked, this, &MenuWindow::newGameSignal);

    // 信号：点击按钮 - 载入存档 -> 信号：载入存档
    connect(this->ui->loadArchiveButton, &QPushButton::clicked, this, &MenuWindow::loadArchiveSignal);

    // 信号：点击按钮 - 排行榜 -> 槽：排行榜
    connect(this->ui->rankingListButton, &QPushButton::clicked, this, &MenuWindow::rankingListSignal);

    // 信号：点击按钮 - 设置 -> 信号：设置
    connect(this->ui->settingsButton, &QPushButton::clicked, this, &MenuWindow::settingsSignal);

    // 信号：点击按钮 - 游戏说明 -> 信号：游戏说明
    connect(this->ui->instructionsButton, &QPushButton::clicked, this, &MenuWindow::instructionsSignal);

    // 信号：点击按钮 - 退出游戏 -> 信号：退出游戏
    connect(this->ui->exitButton, &QPushButton::clicked, this, &MenuWindow::exitSignal);
}

MenuWindow::~MenuWindow()
{
    if (this->ui != nullptr)
    {
        delete this->ui;
        this->ui = nullptr;
    }
}

// public 函数

void MenuWindow::adjustFontColor(const QPixmap& backgroundPic)
{
    QImage image = backgroundPic.toImage();
    QColor backgroundColor = image.pixelColor(0, 0);

    // 根据背景颜色选择字体颜色
    if (backgroundColor.lightness() < 128)
        this->ui->headlineLabel->setStyleSheet("color: white;");
    else
        this->ui->headlineLabel->setStyleSheet("color: black;");
}









