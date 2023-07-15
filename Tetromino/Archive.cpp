#include"Archive.h"

Archive::Archive() { }

// public 函数

void Archive::saveScreenshot(QMainWindow* window)
{
    QPixmap screenshot = window->grab();    // 截取当前屏幕的画面

    // 保存截屏为图片文件

    QString filePath = "Archives/Images/Screenshot ";

    filePath += QString::number(Archive::getNewestID() + 1);
    filePath += ".jpg";

    if (!filePath.isEmpty())
        screenshot.save(filePath);
}

void Archive::setArchiveID(const int id) { this->archiveID = id; }

int Archive::getArchiveID() const { return this->archiveID; }

void Archive::setPlayerInfo(const Player &player)
{
    this->player.setPlayerName(player.getPlayerName());
    this->player.setPlayerID(player.getPlayerID());
    this->player.setPlayerHighestScore(player.getPlayerHighestScore());

    this->player.setBoardWidth(player.getBoardWidth());
    this->player.setBoardHeight(player.getBoardHeight());
    this->player.setBlockSpeed(player.getBlockSpeed());
}

Player Archive::getPlayerInfo() const
{
    Player thePlayer;

    thePlayer.setPlayerName(this->player.getPlayerName());
    thePlayer.setPlayerID(this->player.getPlayerID());
    thePlayer.setPlayerHighestScore(this->player.getPlayerHighestScore());

    thePlayer.setBoardWidth(this->player.getBoardWidth());
    thePlayer.setBoardHeight(this->player.getBoardHeight());
    thePlayer.setBlockSpeed(this->player.getBlockSpeed());

    return thePlayer;
}

void Archive::setBoardInfo(const int width, const int height)
{
    this->boardWidth = width;
    this->boardHeight = height;
}

int Archive::getBoardWidth() const { return this->boardWidth; }

int Archive::getBoardHeight() const { return this->boardHeight; }

void Archive::setGameScore(const int score) { this->gameScore = score; }

int Archive::getGameScore() const { return this->gameScore; }

void Archive::setBlockInfo(const Block& currentBlock, const Block& nextBlock)
{
    this->currentBlockShape = currentBlock.getCurrentShape();
    this->currentBlockStatus = currentBlock.getCurrentStatus();

    this->nextBlockShape = nextBlock.getCurrentShape();
}

int Archive::getCurrentBlockShape() const { return this->currentBlockShape; }

int Archive::getCurrentBlockStatus() const { return this->currentBlockStatus; }

int Archive::getNextBlockShape() const { return this->nextBlockShape; }

void Archive::setFilledGridPoint(const QVector<QPoint>& filledGridPoints)
{
    this->filledGridPoints = filledGridPoints;
}

QVector<QPoint> Archive::getFilledGridPoints() const { return this->filledGridPoints; }

void Archive::setBlockPosition(const QPoint& p)
{
    this->blockPosition.setX(p.x());
    this->blockPosition.setY(p.y());
}

QPoint Archive::getBlockPosition() const { return this->blockPosition; }

bool Archive::isArchiveValid() const { return this->archiveValidity; }

void Archive::createArchive()
{
    QJsonObject archive;

    // 转化点向量为 Json 数组
    QJsonArray pointJsonArray = Archive::convertPointVectorToJsonArray(this->filledGridPoints);

    archive["id"] = Archive::getNewestID() + 1;

    archive["playerName"] = this->player.getPlayerName();

    archive["boardWidth"] = this->boardWidth;
    archive["boardHeight"] = this->boardHeight;

    archive["gameScore"] = this->gameScore;

    archive["currentBlockShape"] = this->currentBlockShape;
    archive["currentBlockStatus"] = this->currentBlockStatus;
    archive["nextBlockShape"] = this->nextBlockShape;

    archive["filledGridPoints"] = pointJsonArray;

    archive["blockPositionX"] = this->blockPosition.x();
    archive["blockPositionY"] = this->blockPosition.y();

    QVector<QJsonObject> archives = Archive::loadArchives();    // 加载全部存档

    archives.append(archive);    // 将当前存档加入全部存档当中

    Archive::saveArchives(archives);    // 保存当前存档
}

void Archive::readArchive()
{
    QVector<QJsonObject> archives = Archive::loadArchives();

    for (const QJsonObject& archive: archives)
    {
        if (this->archiveID == archive["id"].toInt())
        {
            // 转化 Json 数组为点向量
            QVector<QPoint> pointVector = Archive::convertJsonArrayToPointVector(archive["filledGridPoints"].toArray()); 

            this->boardWidth = archive["boardWidth"].toInt();
            this->boardHeight = archive["boardHeight"].toInt();

            this->gameScore = archive["gameScore"].toInt();

            this->player.setPlayerName(archive["playerName"].toString());

            this->currentBlockShape = archive["currentBlockShape"].toInt();
            this->currentBlockStatus = archive["currentBlockStatus"].toInt();
            this->nextBlockShape = archive["nextBlockShape"].toInt();

            this->filledGridPoints = pointVector;

            this->blockPosition.setX(archive["blockPositionX"].toInt());
            this->blockPosition.setY(archive["blockPositionY"].toInt());

            return;
        }
    }
}

void Archive::updateArchive()
{
    QStringList names = Player::getUpdatedPlayerInfo();

    if (names == QStringList())
        return;

    if (names.size() == 2)
    {
        QVector<QJsonObject> archives = Archive::loadArchives();

        for (QJsonObject& archive: archives)
        {
            if (archive["playerName"] == names[0])
            {
                archive["playerName"] = names[1];
                break;
            }
        }

        Archive::saveArchives(archives);
    }
}

// protected 函数

void Archive::saveArchives(const QVector<QJsonObject>& archives)
{   
    for (const QJsonObject& archive: archives)
    {
        QString filePath = "Archives/Archive ";

        filePath.append(QString::number(archive["id"].toInt()));
        filePath.append(".json");

        QFile file(filePath);

        if (file.open(QIODevice::WriteOnly))
        {
            QJsonDocument jsonDoc(archive);

            file.write(jsonDoc.toJson());
            file.close();
        }
    }
}

QVector<QJsonObject> Archive::loadArchives()
{
    QVector<QJsonObject> archives;

    QDir dir("Archives");

    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name | QDir::Reversed);

    QStringList files = dir.entryList();

    int count = 0;

    for (QString& file: files)
    {
        if (file.remove("Archive ").remove(".json").toInt() > count)
            count = file.remove("Archive ").remove(".json").toInt();
    }

    for (int i = 1; i <= count; i++)
    {
        QString filePath = "Archives/Archive ";

        filePath.append(QString::number(i));
        filePath.append(".json");

        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray data = file.readAll();

            QJsonDocument jsonDoc(QJsonDocument::fromJson(data));

            file.close();

            archives.append(jsonDoc.object());
        }
    }

    return archives;
}

int Archive::getNewestID()
{
    QVector<QJsonObject> archives = Archive::loadArchives();    // 读取所有存档

    int id = 0;

    // 遍历存档获得最新序号
    for (const QJsonObject& archive: archives)
    {
        if (archive["id"].toInt() >= id)
            id = archive["id"].toInt();
    }

    return id;
}

// private 函数

QJsonArray Archive::convertPointVectorToJsonArray(const QVector<QPoint>& points)
{
    QJsonArray pointJsonArray;

    // 将每个 QPoint 对象转换为包含 x 和 y 值的 QJsonObject
    for (const QPoint& point: points)
    {
        QJsonObject jsonPoint;

        jsonPoint["x"] = point.x();
        jsonPoint["y"] = point.y();

        pointJsonArray.append(jsonPoint);
    }

    return pointJsonArray;
}

QVector<QPoint> Archive::convertJsonArrayToPointVector(const QJsonArray& points)
{
    QVector<QPoint> pointVector;

    // 将每个 QJsonObject 转换为 QPoint 并添加到 QVector 中
    for (const QJsonValue& pointValue: points)
    {
        QJsonObject jsonPoint = pointValue.toObject();

        int x = jsonPoint["x"].toInt();
        int y = jsonPoint["y"].toInt();

        pointVector.append(QPoint(x, y));
    }

    return pointVector;
}























