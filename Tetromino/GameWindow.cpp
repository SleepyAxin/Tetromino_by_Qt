#include"GameWindow.h"
#include"ui_GameWindow.h"

GameWindow::GameWindow(QWidget* parent, const Archive* archive)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    this->ui->setupUi(this);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));    // 添加随机数种子

    // 设置主场景

    this->mainScene = new QGraphicsScene(this);

    this->ui->mainGraphicsView->setScene(this->mainScene);
    this->ui->mainGraphicsView->setStyleSheet("background: transparent;");    // 设置背景为透明

    // 设置小场景

    this->smallScene = new QGraphicsScene(this);

    this->ui->smallGraphicsView->setScene(this->smallScene);
    this->ui->smallGraphicsView->setStyleSheet("background: transparent;");    // 设置背景为透明

    this->loadSettings();    // 读取设置

    this->appendBlocks();    // 载入方块

    // 判断是否读取存档
    if (archive == nullptr)
        this->drawUI();    // 绘制界面
    else
        this->readGame(*archive);    // 读取游戏

    // 信号：点击按钮 - 开始游戏 -> 槽：开始游戏
    connect(this->ui->startGameButton, &QPushButton::clicked, this, &GameWindow::startGame);

    // 信号：点击按钮 - 暂停游戏 继续游戏 -> 槽：暂停游戏 继续游戏
    connect(this->ui->controlGameButton, &QPushButton::clicked, this, &GameWindow::continueAndPauseGame);

    // 信号：点击按钮 - 开始新游戏 -> 槽：开始游戏
    connect(this->ui->startNewGameButton, &QPushButton::clicked, this, &GameWindow::startGame);

    // 信号：点击按钮 - 保存进度 -> 槽：保存进度
    connect(this->ui->saveGameButton, &QPushButton::clicked, this, &GameWindow::saveGame);

    // 信号：点击按钮 - 返回主菜单 -> 信号：返回主菜单
    connect(this->ui->returnToMenuButton, &QPushButton::clicked, this, &GameWindow::returnToMenuSignal);

    // 信号：点击按钮 - 返回主菜单 -> 槽：结束游戏，槽：重置游戏
    connect(this->ui->returnToMenuButton, &QPushButton::clicked, this, &GameWindow::endGame);
    connect(this->ui->returnToMenuButton, &QPushButton::clicked, this, &GameWindow::resetGame);
}

GameWindow::~GameWindow()
{
    if (this->ui != nullptr)
    {
        delete this->ui;
        this->ui = nullptr;
    }

    for (Block* block: this->allBlocks)
    {
        if (block != nullptr)
        {
            delete block;
            block = nullptr;
        }
    }
}

// public 函数

void GameWindow::showPlayerInfo(const Player &player)
{
    // 设置玩家信息
    this->player.setPlayerID(player.getPlayerID());
    this->player.setPlayerName(player.getPlayerName());
    this->player.setPlayerHighestScore(player.getPlayerHighestScore());
    this->player.setBoardWidth(GameBoard::getBoardWidth());
    this->player.setBoardHeight(GameBoard::getBoardHeight());
    this->player.setBlockSpeed(this->blockSpeed);

    // 展示玩家信息
    this->ui->playerIDLabel->setNum(player.getPlayerID());
    this->ui->playerNameLabel->setText(player.getPlayerName());
    this->ui->playerHighestScoreLabel->setNum(player.getPlayerHighestScore());
}

void GameWindow::adjustFontColor(const QPixmap& backgroundPic)
{
    QImage image = backgroundPic.toImage();
    QColor backgroundColor = image.pixelColor(0, 0);

    // 根据背景颜色选择字体颜色
    if (backgroundColor.lightness() < 128)
    {
        this->ui->playerIDTextLabel->setStyleSheet("color: white;");
        this->ui->playerIDLabel->setStyleSheet("color: white;");

        this->ui->playerNameTextLabel->setStyleSheet("color: white;");
        this->ui->playerNameLabel->setStyleSheet("color: white;");

        this->ui->playerHighestScoreTextLabel->setStyleSheet("color: white;");
        this->ui->playerHighestScoreLabel->setStyleSheet("color: white;");

        this->ui->nextBlockLabel->setStyleSheet("color: white;");

        this->ui->scoreTextLabel->setStyleSheet("color: white;");
        this->ui->scoreLabel->setStyleSheet("color: white;");
    }
    else
    {
        this->ui->playerIDTextLabel->setStyleSheet("color: black;");
        this->ui->playerIDLabel->setStyleSheet("color: black;");

        this->ui->playerNameTextLabel->setStyleSheet("color: black;");
        this->ui->playerNameLabel->setStyleSheet("color: black;");

        this->ui->playerHighestScoreTextLabel->setStyleSheet("color: black;");
        this->ui->playerHighestScoreLabel->setStyleSheet("color: black;");

        this->ui->nextBlockLabel->setStyleSheet("color: black;");

        this->ui->scoreTextLabel->setStyleSheet("color: black;");
        this->ui->scoreLabel->setStyleSheet("color: black;");
    }
}

int GameWindow::getIntFromQKeyQuence(const QKeySequence& key)
{
    // 键位对应 ASCⅡ 码
    // Qt::Key_A: 65
    // Qt::Key_B: 66
    // Qt::Key_C: 67
    // ...
    // Qt::Key_Z: 90

    QString keyString = key.toString();

    int keyInt = keyString[0].toLatin1();

    return keyInt;
}

// protected 函数

void GameWindow::timerEvent(QTimerEvent* event)
{
    // 如果游戏暂停，则不进行游戏
    if (this->isPauseGame == true)
        return;

    static int theTime = 0;    // 计数定时器触发的次数

    // 判断该事件是否是与该游戏定时器相关的事件
    if (event->timerId() != this->timerID)
        return;

    bool isNew = false;    // 是否需要创建新的方块

    theTime++;

    if (theTime < this->timerCounts)
        return;    // 还未到达处理逻辑的时机
    else
        theTime = 0;    // 将计数器重置为 0，以便下次定时器事件的计数

    this->blockReach(isNew);        // 判断方块是否到达底部
    this->blocksFillLine(isNew);    // 如果到达底部，判断是否堆满料
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    // 判断游戏是否开始
    if (this->isStartGame == false)
        return;

    int pauseKeyInt = GameWindow::getIntFromQKeyQuence(this->pauseKey);   // 获取暂停键键位

    if (event->key() == pauseKeyInt)
        this->continueAndPauseGame();

    // 判断游戏是否暂停
    if (this->isPauseGame == true)
        return;

    int moveLeftKeyInt = GameWindow::getIntFromQKeyQuence(this->moveLeftKey);      // 获取左移键键位
    int moveRightKeyInt = GameWindow::getIntFromQKeyQuence(this->moveRightKey);    // 获取右移键键位
    int rorateKeyInt = GameWindow::getIntFromQKeyQuence(this->rorateKey);          // 获取旋转键键位
    int quickenKeyInt = GameWindow::getIntFromQKeyQuence(this->quickenKey);        // 获取加速键键位

    QList<QPoint> currentList = this->currentBlock.getIndexPoints();    // 当前方块坐标组
    QList<QPoint> nextList = QList<QPoint>();                           // 下一个方块坐标组

    // 判断是否能移动或者旋转

    if (event->key() == moveLeftKeyInt)
    {
        if (this->currentBlock.movePosition(-1, 0) == false)
            return;

        nextList = this->currentBlock.getIndexPoints();
    }

    if (event->key() == moveRightKeyInt)
    {
        if(this->currentBlock.movePosition(1, 0) == false)
            return;

        nextList = this->currentBlock.getIndexPoints();
    }

    if (event->key() == rorateKeyInt)
    {
        // 尝试旋转方块
        if (this->currentBlock.nextStatus() == true)
            nextList = this->currentBlock.getIndexPoints();
        else
        {
            this->currentBlock.previousStatus();
            return;
        }
    }

    if (event->key() == quickenKeyInt)
    {
        this->timerCounts = 1;
        return;
    }

    // 判断方块是否越界

    bool isTran = true;    // 是否可以移动

    for (QPoint p: nextList)
    {
        if (p.x() < 0 || p.x() >= GameBoard::getBoardWidth() ||
            p.y() < 0 || p.y() >= GameBoard::getBoardHeight())
        {
            isTran = false;

            break;
        }

        if (this->mainBoard[p.x()][p.y()]->getGridState() == 2)
        {
            isTran = false;

            break;
        }
    }

    // 判断是否可以移动
    if (isTran == true)
    {
        for (QPoint p: currentList)
            this->mainBoard[p.x()][p.y()]->setGridState(0);    // 当前组格子状态置为 0

        for (QPoint p: nextList)
            this->mainBoard[p.x()][p.y()]->setGridState(1);    // 下个组格子状态置为 1
    }
    else
    {
        // 回档

        if (event->key() == moveLeftKeyInt)
            this->currentBlock.movePosition(1, 0);

        if (event->key() == moveRightKeyInt)
            this->currentBlock.movePosition(-1, 0);

        if (event->key() == rorateKeyInt)
            this->currentBlock.previousStatus();
    }
}

void GameWindow::keyReleaseEvent(QKeyEvent* event)
{
    // 判断游戏是否开始及暂停
    if (this->isStartGame == false || this->isPauseGame == true)
        return;

    int quickenKeyInt = GameWindow::getIntFromQKeyQuence(this->quickenKey);

    if (event->key() == quickenKeyInt)
    {
        this->timerCounts = 5;
        return;
    }
}

// private 函数

void GameWindow::loadSettings()
{
    QJsonObject settings = SettingsWindow::loadSettings();

    if (!settings.isEmpty())
        qDebug() << "游戏设置读取成功!";
    else
        return;

    // 读取方块下落速度
    double blockSpeed = settings["blockSpeed"].toDouble();

    // 读取棋盘宽度和高度
    int boardWidth = settings["boardWidth"].toInt();
    int boardHeight = settings["boardHeight"].toInt();

    // 读取键位
    QString moveLeftKeyString = settings["moveLeftKey"].toString();
    QString moveRightKeyString = settings["moveRightKey"].toString();
    QString rorateKeyString = settings["rorateKey"].toString();
    QString quickenKeyString = settings["quickenKey"].toString();
    QString pauseKeyString = settings["pauseKey"].toString();

    // 应用设置到变量

    this->moveLeftKey = QKeySequence::fromString(moveLeftKeyString);
    this->moveRightKey = QKeySequence::fromString(moveRightKeyString);
    this->rorateKey = QKeySequence::fromString(rorateKeyString);
    this->quickenKey = QKeySequence::fromString(quickenKeyString);
    this->pauseKey = QKeySequence::fromString(pauseKeyString);

    GameBoard::setBoardWidth(boardWidth);
    GameBoard::setBoardHeight(boardHeight);

    this->blockSpeed = blockSpeed;
}

void GameWindow::drawUI()
{
    this->ui->controlGameButton->hide();     // 隐藏按钮：暂停游戏 继续游戏
    this->ui->startNewGameButton->hide();    // 隐藏按钮：开始新游戏

    this->drawMainBoard();     // 绘制主棋盘
    this->drawSmallBoard();    // 绘制小棋盘

    this->rangeBlock();       // 随机下一个方块
    this->fillNextBlock();    // 在小棋盘中填充下一个方块
}


void GameWindow::drawMainBoard()
{
    int width = GameBoard::getBoardWidth() * GameBoard::getGridSize() * 10 + 20;      // 主棋盘边界宽度
    int height = GameBoard::getBoardHeight() * GameBoard::getGridSize() * 10 + 20;    // 主棋盘边界高度

    this->ui->mainGraphicsView->setGeometry(0, 0, width + 100, height + 100);    // 主图形尺寸

    this->mainRect = QRect(-10, -10, width, height);    // 主矩形尺寸

    this->mainScene->setSceneRect(this->mainRect);

    QGraphicsRectItem* rectItem = new QGraphicsRectItem(this->mainRect);

    this->mainScene->addItem(rectItem);

    // 绘制主棋盘
    for (int col = 0; col < GameBoard::getBoardWidth(); col++)
    {
        QVector<GameBoard*> tab;

        for (int row = 0; row < GameBoard::getBoardHeight(); row++)
        {
            GameBoard* gameBoard = new GameBoard;

            gameBoard->setPos(col * GameBoard::getGridSize() * 10,
                              row * GameBoard::getGridSize() * 10);

            this->mainScene->addItem(gameBoard);

            tab.append(gameBoard);
        }

        this->mainBoard.append(tab);
    }
}

void GameWindow::drawSmallBoard()
{
    int smallWidth = GameBoard::getSmallBoardSize() * GameBoard::getGridSize() * 10 + 20;     // 小棋盘边界宽度
    int smallHeight = GameBoard::getSmallBoardSize() * GameBoard::getGridSize() * 10 + 20;    // 小棋盘边界高度

    this->ui->smallGraphicsView->setFixedSize(smallWidth + 40,smallHeight + 40);

    QRect smallRect = QRect(-20, -20, smallWidth + 20, smallHeight + 20);

    this->smallScene->setSceneRect(smallRect);

    QGraphicsRectItem* rectItem = new QGraphicsRectItem(smallRect);

    this->smallScene->addItem(rectItem);

    // 绘制小棋盘
    for (int col = 0; col < GameBoard::getSmallBoardSize(); col++)
    {
        QVector<GameBoard*> tab;

        for (int row = 0; row < GameBoard::getSmallBoardSize(); row++)
        {
            GameBoard* gameBoard = new GameBoard;

            gameBoard->setPos(col * GameBoard::getGridSize() * 10,
                              row * GameBoard::getGridSize() * 10);

            this->smallScene->addItem(gameBoard);

            tab.append(gameBoard);
        }

        this->smallBoard.append(tab);
    }
}

void GameWindow::appendBlocks()
{
    this->allBlocks.append(new BlockI);
    this->allBlocks.append(new BlockJ);
    this->allBlocks.append(new BlockL);
    this->allBlocks.append(new BlockO);
    this->allBlocks.append(new BlockS);
    this->allBlocks.append(new BlockT);
    this->allBlocks.append(new BlockZ);
}

void GameWindow::rangeBlock()
{
    int index = qrand() % (this->allBlocks.count());    // 产生随机数

    this->nextBlock = *(this->allBlocks)[index];
}

void GameWindow::fillNextBlock()
{
    // 初始化小窗口
    for (int col = 0; col < this->smallBoard.size(); col++)
    {
        for (int row = 0; row < this->smallBoard[col].size(); row++)
           this->smallBoard[col][row]->setGridState(0);
    }

    Block block = this->nextBlock;

    block.movePosition(2 - GameBoard::getBoardWidth() / 2, GameBoard::getSmallBoardSize() / 2);

    QList<QPoint> points = block.getIndexPoints();

    for (const QPoint& p: points)
        this->smallBoard[p.x()][p.y()]->setGridState(1);
}

void GameWindow::blockReach(bool& isNew)
{
    QList<QPoint> points = this->currentBlock.getIndexPoints();

    for (const QPoint& p: points)
        mainBoard[p.x()][p.y()]->setGridState(0);

    // 判断是否可以向下移动
    if (this->currentBlock.movePosition(0, 1) == false)
    {
        QList<QPoint> newPoints = this->currentBlock.getIndexPoints();

        for (const QPoint& p: newPoints)
            this->mainBoard[p.x()][p.y()]->setGridState(2);

        isNew = true;
    }
    else
    {
        points = this->currentBlock.getIndexPoints();

        // 如果遇到重叠了

        for (const QPoint& p: points)
        {
            if(this->mainBoard[p.x()][p.y()]->getGridState() == 2)
                isNew = true;
        }

        // 刷新显示

        if (isNew == false)
        {
            for (const QPoint& p: points)
                this->mainBoard[p.x()][p.y()]->setGridState(1);
        }
        else
        {
            this->currentBlock.movePosition(0, -1);

            QList<QPoint> points = this->currentBlock.getIndexPoints();

            for (const QPoint& p: points)
                this->mainBoard[p.x()][p.y()]->setGridState(2);
        }
    }
}

void GameWindow::blocksFillLine(const bool isNew)
{
    if (isNew == true)
    {
        //检查是否发生了堆满料的情况

        QList<QPoint> points = this->currentBlock.getIndexPoints();
        QList<int> eliminatedLine;

        for (const QPoint& p: points)
        {
            int row = p.y();
            int col = 0;

            if (eliminatedLine.indexOf(row) >= 0)
                continue;

            for ( ; col < GameBoard::getBoardWidth(); col++)
            {
                if (this->mainBoard[col][row]->getGridState() != 2)
                    break;
            }

            if(col == GameBoard::getBoardWidth())
                eliminatedLine.append(row);
        }

        for (const int& row: eliminatedLine)
        {
            for (int col = 0; col < GameBoard::getBoardWidth(); col++)
                this->mainBoard[col][row]->setGridState(0);

            for (int mRow = row; mRow > 0; mRow--)
            {
                for (int col = 0; col < GameBoard::getBoardWidth(); col++)
                    this->mainBoard[col][mRow]->setGridState(this->mainBoard[col][mRow-1]->getGridState());
            }

            for (int col = 0; col < GameBoard::getBoardWidth(); col++)
                this->mainBoard[col][0]->setGridState(0);

            this->gameScore += 10;     // 统计分数
        }

        this->ui->scoreLabel->setNum(this->gameScore);

        this->currentBlock = this->nextBlock;

        this->rangeBlock();    // 随机下一个图形

        this->fillNextBlock();    // 在小棋盘中填充下一个方块

        points = this->currentBlock.getIndexPoints();

        for (const QPoint& p: points)
        {
            //  判断方块是否堆到顶部
            if (this->mainBoard[p.x()][p.y()]->getGridState() == 2)
            {
                this->endGame();

                QMessageBox::information(this, "提示", "您输了");

                if (this->gameScore > this->player.getPlayerHighestScore())
                {
                    this->player.setPlayerHighestScore(this->gameScore);
                    this->player.setBoardWidth(GameBoard::getBoardWidth());
                    this->player.setBoardHeight(GameBoard::getBoardHeight());

                    this->player.updatePlayerInfo();

                    this->ui->playerHighestScoreLabel->setNum(this->player.getPlayerHighestScore());
                }

                this->resetGame();
            }
            else
                this->mainBoard[p.x()][p.y()]->setGridState(1);
        }
    }
}

// private 槽函数

void GameWindow::startGame()
{
    this->ui->startGameButton->hide();
    this->ui->startNewGameButton->show();
    this->ui->controlGameButton->show();

    this->ui->controlGameButton->setText("暂停游戏");

    this->isStartGame = true;
    this->isPauseGame = false;

    this->currentBlock = this->nextBlock;    // 将下一个方块置为当前方块

    this->rangeBlock();       // 随机下一个图形
    this->fillNextBlock();    // 在小棋盘中填充下一个方块

    QList<QPoint> points = this->currentBlock.getIndexPoints();

    // 重置主棋盘
    for (int col = 0; col < GameBoard::getBoardWidth(); col++)
    {
        for (int row = 0; row < GameBoard::getBoardHeight(); row++)
            this->mainBoard[col][row]->setGridState(0);
    }

    for (const QPoint& p: points)
        this->mainBoard[p.x()][p.y()]->setGridState(1);

    this->ui->scoreLabel->setNum(this->gameScore);

    this->timerID = startTimer(100.0f / this->blockSpeed);
    this->timerCounts = 5;
}

void GameWindow::endGame()
{
    if (this->isStartGame == true)
    {
        this->isStartGame = false;

        killTimer(this->timerID);    // 结束计时器
    }
}

void GameWindow::continueAndPauseGame()
{
    if (this->isPauseGame == true)    // 如果暂停游戏
    {
        this->isPauseGame = false;    // 则继续游戏

        this->ui->controlGameButton->setText("暂停游戏");
    }
    else    // 如果正在游戏
    {
        this->isPauseGame = true;    // 则暂停游戏

        this->ui->controlGameButton->setText("继续游戏");
    }
}

void GameWindow::resetGame()
{
    this->ui->startGameButton->show();

    this->ui->startNewGameButton->hide();
    this->ui->controlGameButton->hide();

    this->isStartGame = false;
    this->isPauseGame = true ;

    this->currentBlock = this->nextBlock;

    this->rangeBlock();    // 随机下一个图形

    this->fillNextBlock();    // 在小棋盘中填充下一个方块

    for (int col = 0; col < GameBoard::getBoardWidth(); col++)
    {
        for (int row = 0; row < GameBoard::getBoardHeight(); row++)
            this->mainBoard[col][row]->setGridState(0);
    }

    this->gameScore = 0;
    this->ui->scoreLabel->setNum(this->gameScore);
}

void GameWindow::saveGame()
{
    if (this->gameScore > this->player.getPlayerHighestScore())
    {
        this->player.setPlayerHighestScore(this->gameScore);
        this->player.setBoardWidth(GameBoard::getBoardWidth());
        this->player.setBoardHeight(GameBoard::getBoardHeight());

        this->player.updatePlayerInfo();
    }

    Archive::saveScreenshot(this);

    QVector<QPoint> points;

    for (int col = 0; col < GameBoard::getBoardWidth(); col++)
    {
        for (int row = 0; row < GameBoard::getBoardHeight(); row++)
        {
            if (this->mainBoard[col][row]->getGridState() == 2)
                points.append(QPoint(col, row));
        }
    }

    QList<QPoint> pointList = this->currentBlock.getIndexPoints();

    for (int i = 1; i <= GameBoard::getBoardWidth(); i++)
    {
        for (int j = 1; j <= GameBoard::getBoardHeight(); j++)
        {
            for (int k = 0; k < this->currentBlock.getIndexPoints().size(); k++)
            {
                if (this->currentBlock.getIndexPoints()[k].x() - i == 0 &&
                    this->currentBlock.getIndexPoints()[k].y() - j == 0)
                {
                    this->newArchive.setBlockPosition(this->currentBlock.getIndexPoints()[k]);
                    break;
                }
            }
        }
    }

    this->newArchive.setPlayerInfo(this->player);
    this->newArchive.setBoardInfo(GameBoard::getBoardWidth(), GameBoard::getBoardHeight());
    this->newArchive.setGameScore(this->gameScore);
    this->newArchive.setBlockInfo(this->currentBlock, this->nextBlock);
    this->newArchive.setFilledGridPoint(points);

    this->newArchive.createArchive();

    QMessageBox::information(this, "提示", "保存成功");
}

void GameWindow::readGame(const Archive& archive)
{
    // 读取棋盘宽高
    GameBoard::setBoardWidth(archive.getBoardWidth());
    GameBoard::setBoardHeight(archive.getBoardHeight());

    this->drawUI();    // 绘制界面

    // 读取游戏的分
    this->gameScore = archive.getGameScore();
    this->ui->scoreLabel->setNum(this->gameScore);

    // 读取玩家信息

    Player player;

    player.setPlayerName(archive.getPlayerInfo().getPlayerName());
    player.setPlayerID(archive.getPlayerInfo().getPlayerID());
    player.setPlayerHighestScore(archive.getPlayerInfo().getPlayerHighestScore());

    this->showPlayerInfo(player);

    // 读取当前方块

    switch (archive.getCurrentBlockShape())
    {
    case 1:
        this->currentBlock = BlockI();
        break;
    case 2:
        this->currentBlock = BlockJ();
        break;
    case 3:
        this->currentBlock = BlockL();
        break;
    case 4:
        this->currentBlock = BlockO();
        break;
    case 5:
        this->currentBlock = BlockS();
        break;
    case 6:
        this->currentBlock = BlockT();
        break;
    case 7:
        this->currentBlock = BlockZ();
        break;
    }

    for (int i = 0; i < archive.getCurrentBlockStatus(); i++)
        this->currentBlock.nextStatus();

    if (archive.getBlockPosition().x() - GameBoard::getBoardWidth() / 2 <= GameBoard::getBoardWidth() &&
        archive.getBlockPosition().y() <= GameBoard::getBoardHeight())
    {
        QList<QPoint> pointList = this->currentBlock.getIndexPoints();

        for (const QPoint& p: pointList)
        {
            this->mainBoard[p.x() + archive.getBlockPosition().x() - GameBoard::getBoardWidth() / 2]
                           [p.y() + archive.getBlockPosition().y()]->setGridState(1);
        }

        this->currentBlock.movePosition(archive.getBlockPosition().x() - GameBoard::getBoardWidth() / 2,
                                        archive.getBlockPosition().y());
    }

    // 读取下一个方块
    switch (archive.getNextBlockShape())
    {
    case 1:
        this->nextBlock = BlockI();
        break;
    case 2:
        this->nextBlock = BlockJ();
        break;
    case 3:
        this->nextBlock = BlockL();
        break;
    case 4:
        this->nextBlock = BlockO();
        break;
    case 5:
        this->nextBlock = BlockS();
        break;
    case 6:
        this->nextBlock = BlockT();
        break;
    case 7:
        this->nextBlock = BlockZ();
        break;
    }

    // 读取已被填充的格子

    QVector<QPoint> points = archive.getFilledGridPoints();

    for (const QPoint p: points)
        this->mainBoard[p.x()][p.y()]->setGridState(2);

    this->ui->startGameButton->hide();

    this->ui->startNewGameButton->show();
    this->ui->controlGameButton->show();

    this->isPauseGame = false;

    this->continueAndPauseGame();

    this->fillNextBlock();    // 在小棋盘中填充下一个方块

    this->isStartGame = true;

    this->timerID = startTimer(100.0f / this->blockSpeed);
    this->timerCounts = 5;
}












