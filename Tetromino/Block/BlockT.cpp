#include"BlockT.h"

BlockT::BlockT()
{
    this->blockShapeIndex = 6;

    // 形态1

    // O O O
    //   O

    QList<QPoint> list_1;

    list_1.append(QPoint(0, 1));
    list_1.append(QPoint(1, 0));
    list_1.append(QPoint(1, 1));
    list_1.append(QPoint(2, 1));

    this->totalBlocks.append(list_1);

    // 形态2

    //   O
    // O O
    //   O

    QList<QPoint> list_2;

    list_2.append(QPoint(0, 1));
    list_2.append(QPoint(1, 0));
    list_2.append(QPoint(1, 1));
    list_2.append(QPoint(1, 2));

    this->totalBlocks.append(list_2);

    // 形态3

    //   O
    // O O O

    QList<QPoint> list_3;

    list_3.append(QPoint(0, 0));
    list_3.append(QPoint(1, 0));
    list_3.append(QPoint(1, 1));
    list_3.append(QPoint(2, 0));

    this->totalBlocks.append(list_3);

    // 形态4

    // O
    // O O
    // O

    QList<QPoint> list_4;

    list_4.append(QPoint(0, 0));
    list_4.append(QPoint(0, 1));
    list_4.append(QPoint(0, 2));
    list_4.append(QPoint(1, 1));

    this->totalBlocks.append(list_4);
}
