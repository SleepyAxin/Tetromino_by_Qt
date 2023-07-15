#include"RankingListWindow.h"
#include"ui_RankingListWindow.h"

RankingListWindow::RankingListWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::RankingListWindow)
{
    this->ui->setupUi(this);

    this->setFixedSize(900, 720);

    this->reset();    // 重置界面

    this->addPlayersInfo();    // 载入玩家信息

    // 信号：点击按钮 - 返回主菜单 -> 信号：返回主菜单
    connect(this->ui->returnToMenuButton, &QPushButton::clicked, this, &RankingListWindow::returnToMenuSignal);
}

RankingListWindow::~RankingListWindow()
{
    if (this->ui != nullptr)
    {
        delete this->ui;
        this->ui = nullptr;
    }
}

// public 函数

void RankingListWindow::adjustFontColor(const QPixmap& backgroundPic)
{
    QImage image = backgroundPic.toImage();
    QColor backgroundColor = image.pixelColor(0, 0);

    // 根据背景颜色选择字体颜色
    if (backgroundColor.lightness() < 128)
        this->ui->rankingListLabel->setStyleSheet("color: white;");
    else
        this->ui->rankingListLabel->setStyleSheet("color: black;");
}

void RankingListWindow::addPlayersInfo()
{
    QVector<QJsonObject> players = Player::loadPlayersInfo();

    this->sortPlayersByScore(players);

    for (QJsonObject player: players)
    {
        int row = this->ui->rankingListTableWidget->rowCount();
        this->ui->rankingListTableWidget->insertRow(row);

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(player["id"].toInt()));
        QTableWidgetItem* nameItem = new QTableWidgetItem(player["name"].toString());
        QTableWidgetItem* scoreItem = new QTableWidgetItem(QString::number(player["highestScore"].toInt()));

        QTableWidgetItem* sizeItem = new QTableWidgetItem(QString::number(player["boardWidth"].toInt())
                                                        + " * "
                                                        + QString::number(player["boardHeight"].toInt()));

        QTableWidgetItem* speedItem = new QTableWidgetItem(QString::number(player["blockSpeed"].toDouble()));

        this->ui->rankingListTableWidget->setItem(row, 0, idItem);
        this->ui->rankingListTableWidget->setItem(row, 1, nameItem);
        this->ui->rankingListTableWidget->setItem(row, 2, scoreItem);
        this->ui->rankingListTableWidget->setItem(row, 3, sizeItem);
        this->ui->rankingListTableWidget->setItem(row, 4, speedItem);
    }
}

void RankingListWindow::reset()
{
    this->ui->rankingListTableWidget->setRowCount(0);

    QStringList rankingListText;
    rankingListText << "序号" << "名字" << "最高得分" << "棋盘宽高" << "方块下落速度";

    this->ui->rankingListTableWidget->setColumnCount(5);
    this->ui->rankingListTableWidget->setHorizontalHeaderLabels(rankingListText);

    //设置选择行为，以行为单位
    this->ui->rankingListTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置选择模式，选择单行
    this->ui->rankingListTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // 禁止编辑单元格
    this->ui->rankingListTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->ui->rankingListTableWidget->setStyleSheet("background: white;");
}

// private 函数

void RankingListWindow::sortPlayersByScore(QVector<QJsonObject>& players)
{
    for (int i = 0; i < players.size() - 1; ++i)
    {
        for (int j = i + 1; j < players.size(); ++j)
        {
            if (players[i]["highestScore"].toInt() < players[j]["highestScore"].toInt())
            {
                QJsonObject temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }
}







