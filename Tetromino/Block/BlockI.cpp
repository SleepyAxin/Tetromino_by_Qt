#include"BlockI.h"

BlockI::BlockI()
{
    this->blockShapeIndex = 1;

    // 形态1：水平

    // O O O O

    QList<QPoint> list_1;

    list_1.append(QPoint(0, 0));
    list_1.append(QPoint(1, 0));
    list_1.append(QPoint(2, 0));
    list_1.append(QPoint(3, 0));

    this->totalBlocks.append(list_1);

    // 形态2：竖直

    // O
    // O
    // O
    // O

    QList<QPoint> list_2;

    list_2.append(QPoint(0, 0));
    list_2.append(QPoint(0, 1));
    list_2.append(QPoint(0, 2));
    list_2.append(QPoint(0, 3));

    this->totalBlocks.append(list_2);
}
