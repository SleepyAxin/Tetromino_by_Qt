#include"PlayerWindow.h"
#include"ui_PlayerWindow.h"

PlayerWindow::PlayerWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::PlayerWindow)
{
    this->ui->setupUi(this);

    this->setFixedSize(900, 720);

    this->addNameToComboBox();    // 加载玩家名字

    // 默认角色为最新角色

    QString name = this->ui->selectplayerComboBox->currentText();

    this->setPlayerName(name);
    this->setPlayerID(this->getPlayerID());
    this->setPlayerHighestScore(this->getPlayerHighestScore());

    // 信号：选择 - 选择游戏玩家 -> 槽：选择游戏玩家
    connect(this->ui->selectplayerComboBox, &QComboBox::currentTextChanged, this, &PlayerWindow::selectPlayer);

    // 信号：输入 - 创建游戏玩家 -> 槽：创建游戏玩家
    connect(this->ui->createNewPlayerLineEdit, &QLineEdit::textChanged, this, &PlayerWindow::createNewPlayer);

    // 信号：点击按钮 - 修改名字 -> 槽：修改玩家名字
    connect(this->ui->changeNameButton, &QPushButton::clicked, this, &PlayerWindow::changePlayerName);

    // 信号：点击按钮 - 删除玩家 -> 槽：删除玩家
    connect(this->ui->deleteButton, &QPushButton::clicked, this, &PlayerWindow::deletePlayer);

    // 信号：点击按钮 - 删除全部玩家 -> 槽：删除全部玩家
    connect(this->ui->deleteAllButton, &QPushButton::clicked, this, &PlayerWindow::deleteAllPlayers);

    // 信号：点击按钮 - 进入游戏 -> 槽：确定玩家，槽：重置界面
    connect(this->ui->enterGameButton, &QPushButton::clicked, this, &PlayerWindow::ensurePlayer);
    connect(this->ui->enterGameButton, &QPushButton::clicked, this, &PlayerWindow::reset);

    // 信号：点击按钮 - 返回主菜单 -> 信号：返回主菜单
    connect(this->ui->returnToMenuButton, &QPushButton::clicked, this, &PlayerWindow::returnToMenuSignal);
}

PlayerWindow::~PlayerWindow()
{
    if (this->ui != nullptr)
    {
        delete this->ui;
        this->ui = nullptr;
    }
}

void PlayerWindow::adjustFontColor(const QPixmap& backgroundPic)
{
    QImage image = backgroundPic.toImage();
    QColor backgroundColor = image.pixelColor(0, 0);

    // 根据背景颜色选择字体颜色
    if (backgroundColor.lightness() < 128)
    {
        this->ui->selectplayerLabel->setStyleSheet("color: white;");
        this->ui->createNewPlayerLabel->setStyleSheet("color: white;");
    }
    else
    {
        this->ui->selectplayerLabel->setStyleSheet("color: black;");
        this->ui->createNewPlayerLabel->setStyleSheet("color: black;");
    }
}

// private 函数

void PlayerWindow::addNameToComboBox()
{
    QStringList nameList;

    QVector<QJsonObject> players = Player::loadPlayersInfo();

    for (QJsonObject player: players)
        nameList.push_front(player["name"].toString());

    this->ui->selectplayerComboBox->addItems(nameList);
}

// private 槽函数

void PlayerWindow::selectPlayer()
{
    QString name = this->ui->selectplayerComboBox->currentText();

    this->setPlayerName(name);
    this->setPlayerID(this->getPlayerID());
    this->setPlayerHighestScore(this->getPlayerHighestScore());
}

void PlayerWindow::createNewPlayer()
{
    QString name = this->ui->createNewPlayerLineEdit->text();

    if (name == "")
    {
        this->ui->selectplayerLabel->show();
        this->ui->selectplayerComboBox->show();

        this->ui->changeNameButton->show();

        this->ui->deleteButton->show();

        return;
    }

    this->ui->selectplayerLabel->hide();
    this->ui->selectplayerComboBox->hide();

    this->ui->changeNameButton->hide();

    this->ui->deleteButton->hide();

    for (QChar ch: name)
    {
        if (ch == ' ')
        {
            QMessageBox::information(this, "警告", "玩家名字不能带有空格");

            this->ui->createNewPlayerLineEdit->clear();

            return;
        }
    }

    if (name.size() > 7)
    {
        QMessageBox::information(this, "警告", "玩家名字不能大于七个字符");

        this->ui->createNewPlayerLineEdit->clear();

        return;
    }

    QVector<QJsonObject> players = Player::loadPlayersInfo();

    for (const QJsonObject player: players)
    {
        if (player["name"].toString() == name)
        {
            QMessageBox::information(this, "警告", "玩家名字已被占用");

            this->ui->createNewPlayerLineEdit->clear();

            return;
        }
    }

    this->setPlayerID(Player::getNewestID() + 1);
    this->setPlayerName(name);
    this->setPlayerHighestScore(0);
}

void PlayerWindow::changePlayerName()
{
    QVector<QJsonObject> players = Player::loadPlayersInfo();

    bool isOK;

    QString name = this->ui->selectplayerComboBox->currentText();
    QString newName = QInputDialog::getText(nullptr, "", "请输入修改后的名字", QLineEdit::Normal, "", &isOK);

    if (!isOK)
        return;

    if (isOK && newName.isEmpty())
    {
        QMessageBox::information(this, "修改失败", "玩家名字不能为空");
        return;
    }

    if (isOK && newName.size() > 7)
    {
        QMessageBox::information(this, "修改失败", "玩家名字不能大于七个字符");
        return;
    }

    for (QChar ch: newName)
    {
        if (ch == ' ')
        {
            QMessageBox::information(this, "警告", "玩家名字不能带有空格");

            this->ui->createNewPlayerLineEdit->clear();

            return;
        }
    }

    for (const QJsonObject player: players)
    {
        if (player["name"].toString() == newName)
        {
            QMessageBox::information(this, "修改失败", "玩家名字已被占用");
            return;
        }
    }

    QMessageBox::information(this, "", "修改成功");

    for (int i = 0; i < players.size(); i++)
    {
        if (players[i]["name"] == name)
        {
            players[i]["name"] = newName;
            break;
        }
    }

    Player::savePlayersInfo(players);    // 保存玩家信息

    this->reset();

    this->ui->selectplayerComboBox->setCurrentText(newName);

    // 设置玩家信息更新

    QStringList names;

    names.append(name);
    names.append(newName);

    Player::setUpdatedPlayerInfo(names);
}

void PlayerWindow::deletePlayer()
{
    QString name = this->ui->selectplayerComboBox->currentText();

    Player thePlayer;
    thePlayer.setPlayerName(name);

    QString jsonPath = "Players/Player ";

    jsonPath.append(QString::number(thePlayer.getPlayerID()));
    jsonPath.append(".json");

    QFile jsonFile(jsonPath);

    if (jsonFile.exists())
        jsonFile.remove();

    this->reset();
}

void PlayerWindow::deleteAllPlayers()
{
    QDir dir("Players");
    dir.removeRecursively();

    this->reset();
}

void PlayerWindow::ensurePlayer()
{
    if (this->ui->selectplayerComboBox->count() == 0 &&
        this->ui->createNewPlayerLineEdit->text() == "")
    {
        QMessageBox::information(this, "警告", "未选择玩家或创建玩家");

        return;
    }

    // 触发信号：进入游戏
    emit this->enterGameSignal();

    if (this->getPlayerID() > Player::getNewestID())
    {
        this->createPlayerInfo();
        this->ui->selectplayerComboBox->addItem(this->getPlayerName());
    }
}

void PlayerWindow::reset()
{
    // 若不存在玩家目录，则创建存档目录
    QDir dir;
    dir.mkdir("Players");

    this->ui->createNewPlayerLineEdit->clear();    // 清空新建玩家输入框
    this->ui->selectplayerComboBox->clear();       // 清空选择玩家选择框

    this->addNameToComboBox();    // 加载玩家名字到玩家选择框
}














