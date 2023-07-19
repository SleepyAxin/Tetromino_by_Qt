#include"BlockZ.h"

BlockZ::BlockZ()
{
    this->blockShapeIndex = 7;

    // 形态1：水平

    // O O
    //   O O

    QList<QPoint> list_1;

    list_1.append(QPoint(0, 1));
    list_1.append(QPoint(1, 0));
    list_1.append(QPoint(1, 1));
    list_1.append(QPoint(2, 0));

    this->totalBlocks.append(list_1);

    // 形态2：竖直

    //   O
    // O O
    // O

    QList<QPoint> list_2;

    list_2.append(QPoint(0, 0));
    list_2.append(QPoint(0, 1));
    list_2.append(QPoint(1, 1));
    list_2.append(QPoint(1, 2));

    this->totalBlocks.append(list_2);
}
