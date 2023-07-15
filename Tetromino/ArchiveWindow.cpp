#include"ArchiveWindow.h"
#include"ui_ArchiveWindow.h"

ArchiveWindow::ArchiveWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ArchiveWindow)
{
    this->ui->setupUi(this);

    this->setFixedSize(900, 720);

    this->reset();    // 重置界面

    this->addArchives();    // 添加存档

    this->showImage();    // 显示存档图片

    // 信号：选择 - 存档 -> 槽：显示图片
    connect(this->ui->selectArchiveComboBox, &QComboBox::currentTextChanged, this, &ArchiveWindow::showImage);

    // 信号：点击按钮 - 复制存档 -> 槽：复制存档
    connect(this->ui->copyButton, &QPushButton::clicked, this, &ArchiveWindow::copyArchive);

    // 信号：点击按钮 - 删除存档 -> 槽：删除存档
    connect(this->ui->deleteButton, &QPushButton::clicked, this, &ArchiveWindow::deleteArchive);

    // 信号：点击按钮 - 删除所有存档 -> 槽：删除所有存档
    connect(this->ui->deleteAllButton, &QPushButton::clicked, this, &ArchiveWindow::deleteAllArchives);

    // 信号：点击按钮 - 载入游戏 -> 信号：载入游戏，槽：确认存档
    connect(this->ui->readGameButton, &QPushButton::clicked, this, &ArchiveWindow::ensureArchive);
    connect(this->ui->readGameButton, &QPushButton::clicked, this, &ArchiveWindow::readGameSignal);

    // 信号：点击按钮 - 返回主菜单 -> 信号：返回主菜单
    connect(this->ui->returnToMenuButton, &QPushButton::clicked, this, &ArchiveWindow::returnToMenuSignal);
}

ArchiveWindow::~ArchiveWindow()
{
    if (this->ui != nullptr)
    {
        delete this->ui;
        this->ui = nullptr;
    }
}

// public 函数

void ArchiveWindow::reset()
{
    // 若不存在存档目录，则创建存档目录

    QDir dir;
    dir.mkdir("Archives");

    dir.setPath("Archives");
    dir.mkdir("Images");

    this->ui->selectArchiveComboBox->clear();    // 清空选择条
    this->ui->imageLabel->clear();               // 清空图片框
}

void ArchiveWindow::addArchives()
{
    Archive::updateArchive();    // 首先更新存档信息（玩家名字被修改）

    QVector<QJsonObject> archives = Archive::loadArchives();    // 读取全部存档

    QStringList nameList;

    for (const QJsonObject& archive: archives)
    {
        QString name = "存档 ";
        name += QString::number(archive["id"].toInt());

        nameList.push_front(name);    // 最新的存档排在最上面
    }

    this->ui->selectArchiveComboBox->addItems(nameList);    // 添加存档到选择条

    // 默认存档为最新存档

    QString name = this->ui->selectArchiveComboBox->currentText();

    this->setArchiveID(name.remove("存档 ").toInt());
}

// public 槽函数

void ArchiveWindow::showImage()
{
    // 获取当前存档序号

    QString name = this->ui->selectArchiveComboBox->currentText();

    name.remove(0, 3);

    int id = name.toInt();

    // 从文件中读取存档截图

    QString filePath = "Archives/Images/Screenshot ";

    filePath += QString::number(id);
    filePath += ".jpg";

    QPixmap originalImage(filePath);    // 原始图片
    QPixmap scaledImage(600, 480);      // 创建新的缩放后的 QPixmap 对象

    scaledImage.fill(Qt::transparent);     // 填充透明背景

    QPainter painter(&scaledImage);

    painter.setRenderHint(QPainter::SmoothPixmapTransform);    // 设置平滑绘制
    painter.drawPixmap(0, 0, 600, 480, originalImage);         // 绘制缩放后的图片
    painter.end();                                             // 结束绘制

    this->ui->imageLabel->setPixmap(scaledImage);    // 设置图片
    this->ui->imageLabel->resize(600, 480);          // 重新绘制尺寸
}

// private 槽函数

void ArchiveWindow::copyArchive()
{
    // 获取当前存档序号

    QString name = this->ui->selectArchiveComboBox->currentText();

    int id = name.remove("存档 ").toInt();

    QJsonObject newArchive;    // 创建新存档

    QVector<QJsonObject> archives = Archive::loadArchives();    // 读取所有存档

    for (const QJsonObject& archive: archives)
    {
        if (id == archive["id"].toInt())
        {
            newArchive = archive;    // 复制存档所有信息

            newArchive["id"] = Archive::getNewestID() + 1;

            break;
        }
    }

    archives.append(newArchive);

    Archive::saveArchives(archives);    // 保存存档

    // 复制存档截图

    QString filePath = "Archives/Images/Screenshot ";
    filePath += QString::number(id);
    filePath += ".jpg";

    QString newFilePath = "Archives/Images/Screenshot ";
    newFilePath += QString::number(Archive::getNewestID());
    newFilePath += ".jpg";

    QFile file(filePath);
    file.copy(newFilePath);

    // 将复制后存档添加到存档界面

    this->reset();

    this->addArchives();

    this->showImage();
}

void ArchiveWindow::deleteArchive()
{
    // 删除存档文件

    QString name = this->ui->selectArchiveComboBox->currentText();

    int id = name.remove("存档 ").toInt();    // 获取删除存档序号

    QString jsonPath = "Archives/Archive ";

    jsonPath.append(QString::number(id));
    jsonPath.append(".json");

    QFile jsonFile(jsonPath);

    if (jsonFile.exists())
        jsonFile.remove();

    // 删除存档图片

    QString imagePath = "Archives/Images/Screenshot ";

    imagePath += QString::number(id);
    imagePath += ".jpg";

    QFile imageFile(imagePath);

    if (imageFile.exists())
        imageFile.remove();

    // 将被删除的存档移出存档界面

    this->reset();

    this->addArchives();

    this->showImage();
}

void ArchiveWindow::deleteAllArchives()
{
    // 删除整个存档目录

    QDir dir("Archives");

    dir.removeRecursively();

    // 将所有存档移出存档界面

    this->reset();

    this->addArchives();

    this->showImage();
}

void ArchiveWindow::ensureArchive()
{
    QString name = this->ui->selectArchiveComboBox->currentText();

    this->setArchiveID(name.remove("存档 ").toInt());    // 设置存档序号

    this->readArchive();    // 依据存档序号读取存档信息

    // 检测存档中的玩家信息是否存在

    QString playerName = this->getPlayerInfo().getPlayerName();

    QVector<QJsonObject> players = Player::loadPlayersInfo();

    for (const QJsonObject& player: players)
    {
        if (player["name"] == playerName)
            this->archiveValidity = true;    // 如果存在，玩家有效性为 true
    }
}













