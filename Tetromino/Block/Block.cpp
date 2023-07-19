#include"Block.h"

Block::Block()
{
    this->currentPosition.setX(GameBoard::getBoardWidth() / 2 - 1);
    this->currentPosition.setY(0);
}

// public 函数

int Block::getCurrentShape() const { return this->blockShapeIndex; }

int Block::getCurrentStatus() const { return this->blockStatusIndex; }

void Block::previousStatus()
{
    int totalStatus = this->totalBlocks.count();

    this->blockStatusIndex--;

    if (this->blockStatusIndex < 0)
        this->blockStatusIndex = totalStatus - 1;
}

bool Block::nextStatus()
{
    int totalStatus = this->totalBlocks.count();
    int num = this->getIndexPoints().count();

    this->blockStatusIndex++;

    if (this->blockStatusIndex >= totalStatus)
        this->blockStatusIndex = 0;

    if (num == this->totalBlocks[0].count())
        return true;
    else
        return false;
}

QList<QPoint> Block::getIndexPoints() const
{
    int index = this->blockStatusIndex;
    int totalNum = this->totalBlocks.count();

    if (index >= 0 && index < totalNum)
    {
        QList<QPoint> ret;

        for (QPoint p: totalBlocks.at(index))
        {
            int x = this->currentPosition.x() + p.x();
            int y = this->currentPosition.y() + p.y();

            if (x >= 0 && x <= GameBoard::getBoardWidth() &&
                y >= 0 && y <= GameBoard::getBoardHeight() )
                ret.append(QPoint(x, y));
        }

        return ret;
    }
    else
        return QList<QPoint>();
}

QList<QPoint> Block::getIndexPoints(int index) const
{
    int totalNum = this->totalBlocks.count();

    if (index >= 0 && index <= totalNum)
    {
        QList<QPoint> ret;

        for (QPoint p: totalBlocks.at(index))
        {
            int x = this->currentPosition.x() + p.x();
            int y = this->currentPosition.y() + p.y();

            if (x >= 0 && x <= GameBoard::getBoardWidth() &&
                y >= 0 && y <= GameBoard::getBoardHeight() )
                ret.append(QPoint(x, y));
        }

        return ret;
    }
    else
        return QList<QPoint>();
}

void Block::setPosition(const QPoint& p)
{
    this->currentPosition.setX(p.x());
    this->currentPosition.setY(p.y());
}

bool Block::movePosition(int x, int y)
{
    int newX = this->currentPosition.x() + x;
    int newY = this->currentPosition.y() + y;

    bool ret = false;

    // 判断方块是否到达边界

    for (QPoint p: this->getIndexPoints())
    {
        int testX = p.x() + x;
        int testY = p.y() + y;

        if (testX >= 0 && testX < GameBoard::getBoardWidth() &&
            testY >= 0 && testY < GameBoard::getBoardHeight() ) { }
        else
        {
            ret = true;

            break;
        }
    }

    if (ret == false)
    {
        this->currentPosition.setX(newX);
        this->currentPosition.setY(newY);

        return true;
    }
    else
        return false;
}
