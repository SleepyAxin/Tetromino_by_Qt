#include"Player.h"

QStringList Player::names = QStringList();

// public 函数

void Player::setPlayerID(const int id) { this->playerID = id; }

int Player::getPlayerID() const
{
    QVector<QJsonObject> players = Player::loadPlayersInfo();

    int id = 9999;
    int index = -1;

    for (const QJsonObject player: players)
    {
        index++;

        if (this->playerName == player["name"].toString())
        {
            id = players[index]["id"].toInt();
            break;
        }
    }

    if (id == 9999)
        return this->playerID;
    else
    {
        qDebug() << "已读取当前玩家到对应ID！";
        return id;
    }
}

void Player::setPlayerName(const QString& name) { this->playerName = name; }

QString Player::getPlayerName() const { return this->playerName; }

void Player::setPlayerHighestScore(const int score) { this->playerHighestScore = score; }

int Player::getPlayerHighestScore() const
{
    QVector<QJsonObject> players = Player::loadPlayersInfo();

    int score = -1;
    int index = -1;

    for (const QJsonObject player: players)
    {
        index++;

        if (this->playerName == player["name"].toString())
        {
            score = players[index]["highestScore"].toInt();
            break;
        }
    }

    if (score == -1)
        return this->playerHighestScore;
    else
    {
        qDebug() << "已读取到当前玩家对应最高分数！";
        return score;
    }
}

void Player::setBoardWidth(const int w) { this->boardWidth = w; }

int Player::getBoardWidth() const { return this->boardWidth; }

void Player::setBoardHeight(const int h) { this->boardHeight = h; }

int Player::getBoardHeight() const { return this->boardHeight; }

void Player::setBlockSpeed(const int v) { this->blockSpeed = v; }

int Player::getBlockSpeed() const { return this->blockSpeed; }

void Player::updatePlayerInfo()
{   
    QVector<QJsonObject> players = Player::loadPlayersInfo();

    for (const QJsonObject player: players)
    {
        if (this->playerID == player["id"].toInt())
        {
            int index = this->playerID - 10000;

            // 更新玩家数据
            players[index]["highestScore"] = this->playerHighestScore;

            players[index]["boardWidth"] = this->boardWidth;
            players[index]["boardHeight"] = this->boardHeight;
            players[index]["blockSpeed"] = this->blockSpeed;

            Player::savePlayersInfo(players);    // 保存到文件

            break;
        }
    }
}

void Player::createPlayerInfo()
{
    QJsonObject player;

    player["id"] = this->playerID;
    player["name"] = this->playerName;
    player["highestScore"] = this->playerHighestScore;

    player["boardWidth"] = this->boardWidth;
    player["boardHeight"]  = this->boardHeight;
    player["blockSpeed"] = this->blockSpeed;

    QVector<QJsonObject> players = Player::loadPlayersInfo();    // 加载全部角色

    players.append(player);    // 将当前角色加入全部角色当中

    Player::savePlayersInfo(players);
}

int Player::getNewestID()
{
    QVector<QJsonObject> players = Player::loadPlayersInfo();

    int newestID = 10000;

    for (const QJsonObject jsonObj: players)
        newestID++;

    return newestID - 1;
}

void Player::setUpdatedPlayerInfo(const QStringList& names)
{
    Player::names.clear();    // 先将更新列表清空
    Player::names = names;
}

QStringList Player::getUpdatedPlayerInfo() { return Player::names; }

QVector<QJsonObject> Player::loadPlayersInfo()
{
    QVector<QJsonObject> players;

    // 读取最新存档序号

    QDir dir("Players");

    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name | QDir::Reversed);

    QStringList files = dir.entryList();

    QString lastFile = files.isEmpty() ? QString() : files.first();

    int count = 0;

    count = lastFile.remove("Player ").remove(".json").toInt();

    for (int i = 10000; i <= count; i++)
    {
        QString filePath = "Players/Player ";

        filePath.append(QString::number(i));
        filePath.append(".json");

        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray data = file.readAll();

            QJsonDocument jsonDoc(QJsonDocument::fromJson(data));

            file.close();

            players.append(jsonDoc.object());
        }
    }

    return players;
}

// protected 函数

void Player::savePlayersInfo(const QVector<QJsonObject>& players)
{
    for (const QJsonObject& player: players)
    {
        QString filePath = "Players/Player ";

        filePath.append(QString::number(player["id"].toInt()));
        filePath.append(".json");

        QFile file(filePath);

        if (file.open(QIODevice::WriteOnly))
        {
            QJsonDocument jsonDoc(player);

            file.write(jsonDoc.toJson());
            file.close();
        }
    }
}


















