#include"MainWindow.h"

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
    this->setFixedSize(900, 720);    // 设置固定窗口大小

    this->menuWindow = new MenuWindow(this);
    this->settingsWindow = new SettingsWindow(this);
    this->playerWindow = new PlayerWindow(this);
    this->gameWindow = new GameWindow(this, nullptr);
    this->archiveWindow = new ArchiveWindow(this);
    this->rankingListWindow = new RankingListWindow(this);
    this->instructionsWindow = new InstructionsWindow(this);

    // 使用 QStackedWidget 部件将多个页面存放在同一个界面中并且只显示其中一个页面

    this->stackedWidget = new QStackedWidget(this);

    this->stackedWidget->addWidget(this->menuWindow);
    this->stackedWidget->addWidget(this->playerWindow);
    this->stackedWidget->addWidget(this->gameWindow);
    this->stackedWidget->addWidget(this->archiveWindow);
    this->stackedWidget->addWidget(this->rankingListWindow);
    this->stackedWidget->addWidget(this->settingsWindow);
    this->stackedWidget->addWidget(this->instructionsWindow);

    this->setCentralWidget(this->stackedWidget);

    this->hide();

    // 创建并播放背景音乐
    this->backgroundMusic = new BackgroundMusic(this);
    this->backgroundMusic->playMusic();

    // 创建并显示背景图片
    this->backgroundPicture = new BackgroundPicture;
    this->backgroundPicture->showPicture(this);

    this->adjustFontColor();    // 调整字体颜色

    // 信号：新游戏 -> 槽：选择玩家
    connect(this->menuWindow, &MenuWindow::newGameSignal, this, &MainWindow::selectPlayer);

    // 信号：进入游戏 -> 槽：进入游戏
    connect(this->playerWindow, &PlayerWindow::enterGameSignal, this, &MainWindow::enterGame);

    // 信号：载入存档 -> 槽：载入存档
    connect(this->menuWindow, &MenuWindow::loadArchiveSignal, this, &MainWindow::loadArchive);

    // 信号：读取游戏 -> 槽：读取游戏
    connect(this->archiveWindow, &ArchiveWindow::readGameSignal, this, &MainWindow::readGame);

    // 信号：排行榜 -> 槽：排行榜
    connect(this->menuWindow, &MenuWindow::rankingListSignal, this, &MainWindow::rankingList);

    // 信号：设置 -> 槽：设置，槽：应用设置到设置界面
    connect(this->menuWindow, &MenuWindow::settingsSignal, this, &MainWindow::settings);
    connect(this->menuWindow, &MenuWindow::settingsSignal, this->settingsWindow, &SettingsWindow::applySettings);

    // 信号：游戏说明 -> 槽：游戏说明
    connect(this->menuWindow, &MenuWindow::instructionsSignal, this, &MainWindow::instructions);

    // 信号：设置背景音乐音量 -> 槽：设置背景音乐音量
    connect(this->settingsWindow, &SettingsWindow::setMusicVolumeSignal, this, &MainWindow::setMusicVolume);

    // 信号：设置背景音乐曲目 -> 槽：设置背景音乐曲目
    connect(this->settingsWindow, &SettingsWindow::setMusicChoiceSignal, this, &MainWindow::setMusicChoice);

    // 信号：设置背景图片 -> 槽：设置背景图片
    connect(this->settingsWindow, &SettingsWindow::setPictureChoiceSignal, this, &MainWindow::setPictureChoice);

    // 信号：退出游戏 -> 槽：关闭窗口
    connect(this->menuWindow, &MenuWindow::exitSignal, this, &QMainWindow::close);

    // 信号：返回主菜单 -> 槽：返回主菜单
    connect(this->playerWindow, &PlayerWindow::returnToMenuSignal, this, &MainWindow::playerReturnToMenu);
    connect(this->gameWindow, &GameWindow::returnToMenuSignal, this, &MainWindow::gameReturnToMenu);
    connect(this->archiveWindow, &ArchiveWindow::returnToMenuSignal, this, &MainWindow::archiveReturnToMenu);
    connect(this->rankingListWindow, &RankingListWindow::returnToMenuSignal, this, &MainWindow::rankingListReturnToMenu);
    connect(this->settingsWindow, &SettingsWindow::returnToMenuSignal, this, &MainWindow::settingsReturnToMenu);
    connect(this->instructionsWindow, &InstructionsWindow::returnToMenuSignal, this, &MainWindow::instructionsReturnToMenu);
}

MainWindow::~MainWindow() { }

// private 槽函数

void MainWindow::selectPlayer()
{
    this->menuWindow->hide();      // 隐藏主菜单窗口
    this->playerWindow->show();    // 显示选择玩家窗口
}

void MainWindow::playerReturnToMenu()
{
    this->playerWindow->hide();    // 隐藏选择玩家窗口
    this->menuWindow->show();      // 显示主菜单窗口
}

void MainWindow::enterGame()
{
    // 重置游戏界面

    this->stackedWidget->removeWidget(this->gameWindow);

    if (this->gameWindow != nullptr)
    {
        delete this->gameWindow;
        this->gameWindow = nullptr;
    }

    this->gameWindow = new GameWindow(this, nullptr);

    this->stackedWidget->addWidget(this->gameWindow);

    // 信号：游戏界面返回主菜单 -> 槽：游戏界面返回主菜单
    connect(this->gameWindow, &GameWindow::returnToMenuSignal, this, &MainWindow::gameReturnToMenu);

    this->gameWindow->adjustFontColor(this->backgroundPicture->getPicture());

    // 展示玩家信息
    this->gameWindow->showPlayerInfo(*this->playerWindow);

    this->playerWindow->hide();    // 隐藏选择玩家窗口
    this->gameWindow->show();      // 显示游戏窗口
}

void MainWindow::gameReturnToMenu()
{
    this->gameWindow->hide();    // 隐藏游戏窗口
    this->menuWindow->show();    // 显示主菜单窗口
}

void MainWindow::loadArchive()
{
    this->archiveWindow->reset();
    this->archiveWindow->addArchives();
    this->archiveWindow->showImage();

    this->menuWindow->hide();       // 隐藏主菜单窗口
    this->archiveWindow->show();    // 显示存档窗口
}

void MainWindow::archiveReturnToMenu()
{
    this->archiveWindow->hide();    // 隐藏存档窗口
    this->menuWindow->show();       // 显示主菜单窗口
}

void MainWindow::readGame()
{
    if (!this->archiveWindow->isArchiveValid())
    {
        QMessageBox::information(this, "警告", "玩家不存在，存档无法载入");
        return;
    }

    // 重置游戏界面

    this->stackedWidget->removeWidget(this->gameWindow);

    if (this->gameWindow != nullptr)
    {
        delete this->gameWindow;
        this->gameWindow = nullptr;
    }

    this->gameWindow = new GameWindow(this, this->archiveWindow);

    this->stackedWidget->addWidget(this->gameWindow);

    // 信号：游戏界面返回主菜单 -> 槽：游戏界面返回主菜单
    connect(this->gameWindow, &GameWindow::returnToMenuSignal, this, &MainWindow::gameReturnToMenu);

    this->gameWindow->adjustFontColor(this->backgroundPicture->getPicture());

    this->archiveWindow->hide();    // 隐藏存档窗口
    this->gameWindow->show();       // 显示游戏窗口
}

void MainWindow::rankingList()
{
    this->rankingListWindow->reset();
    this->rankingListWindow->addPlayersInfo();

    this->menuWindow->hide();           // 隐藏主菜单窗口
    this->rankingListWindow->show();    // 显示排行榜窗口
}

void MainWindow::rankingListReturnToMenu()
{
    this->rankingListWindow->hide();    // 隐藏排行榜窗口
    this->menuWindow->show();           // 显示主菜单窗口
}

void MainWindow::settings()
{
    this->menuWindow->hide();        // 隐藏主菜单窗口
    this->settingsWindow->show();    // 显示设置窗口
}

void MainWindow::settingsReturnToMenu()
{
    this->settingsWindow->hide();    // 隐藏设置窗口
    this->menuWindow->show();        // 显示主菜单窗口
}

void MainWindow::instructions()
{
    this->menuWindow->hide();            // 隐藏主菜单窗口
    this->instructionsWindow->show();    // 显示游戏说明窗口

    this->instructionsWindow->setInstructionsText();
}

void MainWindow::instructionsReturnToMenu()
{
    this->instructionsWindow->hide();    // 隐藏游戏说明窗口
    this->menuWindow->show();            // 显示主菜单窗口
}

void MainWindow::setMusicVolume()
{
    int volume = this->settingsWindow->getMusicVolume();

    this->backgroundMusic->setVolume(volume);    // 设置音乐音量
}

void MainWindow::setMusicChoice()
{
    QString choice = this->settingsWindow->getMusicChoice();

    this->backgroundMusic->stopMusic();          // 停止播放音乐
    this->backgroundMusic->setChoice(choice);    // 切换音乐
    this->backgroundMusic->playMusic();          // 播放音乐
}

void MainWindow::setPictureChoice()
{
    QString choice = this->settingsWindow->getPictureChoice();

    this->backgroundPicture->setChoice(choice);    // 切换背景图片
    this->backgroundPicture->showPicture(this);    // 显示背景图片

    this->adjustFontColor();    // 调整字体颜色
}

void MainWindow::adjustFontColor()
{
    QPixmap backgroundPic = this->backgroundPicture->getPicture();    // 获取当前背景图片

    this->menuWindow->adjustFontColor(backgroundPic);
    this->playerWindow->adjustFontColor(backgroundPic);
    this->gameWindow->adjustFontColor(backgroundPic);
    this->rankingListWindow->adjustFontColor(backgroundPic);
    this->settingsWindow->adjustFontColor(backgroundPic);
    this->instructionsWindow->adjustFontColor(backgroundPic);
}




















