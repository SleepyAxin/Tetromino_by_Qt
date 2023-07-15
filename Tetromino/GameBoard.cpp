#include"GameBoard.h"

int GameBoard::boardWidth = 15;
int GameBoard::boardHeight = 20;
int GameBoard::gridSize = 2;
int GameBoard::smallBoardSize = 5;

GameBoard::GameBoard()
{
    // 设置格子尺寸
    this->backGrid = QPixmap(10, 10);
    this->movingGrid = QPixmap(10, 10);
    this->fixedGrid = QPixmap(10, 10);

    // 设置格子黑边尺寸

    QPainter backGridPainter(&this->backGrid);

    QRectF rect(0.50f, 0.50f, 9.00f, 9.00f);

    backGridPainter.fillRect(this->backGrid.rect(), Qt::black);

    backGridPainter.fillRect(rect, Qt::white);

    backGridPainter.end();

    this->loadSettings();    // 载入设置

    this->setGridState(0);    // 重置格子状态

    this->setScale(GameBoard::gridSize);    // 设置缩放
}

void GameBoard::loadSettings()
{
    QJsonObject settings = SettingsWindow::loadSettings();

    if (settings.isEmpty())
        return;

    // 读取移动方块颜色、固定方块颜色

    QString movingBlockColorString = settings["movingBlockColor"].toString();
    QString fixedBlockColorString = settings["fixedBlockColor"].toString();

    QColor movingBlockColor = this->getColorFromString(movingBlockColorString);
    QColor fixedBlockColor = this->getColorFromString(fixedBlockColorString);

    // 应用设置到变量

    QPainter movingGridPainter(&this->movingGrid);
    QPainter fixedGridPainter(&this->fixedGrid);

    QRectF rect(0.50f, 0.50f, 9.00f, 9.00f);

    movingGridPainter.fillRect(this->movingGrid.rect(), Qt::black);
    fixedGridPainter.fillRect(this->fixedGrid.rect(), Qt::black);

    movingGridPainter.fillRect(rect, movingBlockColor);
    fixedGridPainter.fillRect(rect, fixedBlockColor);

    movingGridPainter.end();
    fixedGridPainter.end();
}

QColor GameBoard::getColorFromString(const QString& s)
{
    QColor color = Qt::red;

    if (s == "红色")
        color = Qt::red;
    if (s == "橙色")
        color.setNamedColor("#FFA500");
    if (s == "黄色")
        color = Qt::yellow;
    if (s == "绿色")
        color = Qt::green;
    if (s == "蓝色")
        color = Qt::blue;
    if (s == "粉色")
        color.setNamedColor("#FFC0CB");
    if (s == "紫色")
        color.setNamedColor("#800080");

    return color;
}

void GameBoard::setBoardWidth(const int w) { GameBoard::boardWidth = w; }

int GameBoard::getBoardWidth() { return GameBoard::boardWidth; }

void GameBoard::setBoardHeight(const int h) { GameBoard::boardHeight = h; }

int GameBoard::getBoardHeight() { return GameBoard::boardHeight; }

void GameBoard::setGridSize(const int s) { GameBoard::gridSize = s; }

int GameBoard::getGridSize() { return GameBoard::gridSize; }

void GameBoard::setSmallBoardSize(const int s) { GameBoard::smallBoardSize = s; }

int GameBoard::getSmallBoardSize() { return GameBoard::smallBoardSize; }

void GameBoard::setGridState(const int state)
{
    switch (state)
    {
    case 0:
        this->setPixmap(this->backGrid);    // 无方块
        break;
    case 1:
        this->setPixmap(this->movingGrid);    // 有移动方块
        break;
    case 2:
        this->setPixmap(this->fixedGrid);    // 有固定方块
        break;
    }

    this->gridState = state;
}

int GameBoard::getGridState() const { return this->gridState; }

