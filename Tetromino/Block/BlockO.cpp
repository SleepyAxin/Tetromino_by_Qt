#include"BlockO.h"

BlockO::BlockO()
{
    this->blockShapeIndex = 4;

    // O O
    // O O

    QList<QPoint> list;

    list.append(QPoint(0, 0));
    list.append(QPoint(0, 1));
    list.append(QPoint(1, 0));
    list.append(QPoint(1, 1));

    this->totalBlocks.append(list);
}
