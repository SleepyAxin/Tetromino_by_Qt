#include"SettingsWindow.h"
#include"ui_SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::SettingsWindow)
{
    this->ui->setupUi(this);

    // 初始化滑块显示的数值范围

    this->ui->blockSpeedSlider->setMinimum(5);
    this->ui->blockSpeedSlider->setMaximum(30);
    this->ui->blockSpeedSlider->setToolTip(QString::number(this->ui->blockSpeedSlider->value() / 10.0f));

    this->ui->musicVolumeSlider->setMinimum(0);
    this->ui->musicVolumeSlider->setMaximum(100);
    this->ui->musicVolumeSlider->setToolTip(QString::number(this->ui->musicVolumeSlider->value()));

    this->applySettings();    // 应用设置

    // 信号：滑动滑块  -> 槽：改变滑块显示文本
    connect(this->ui->musicVolumeSlider, &QSlider::valueChanged, this, &SettingsWindow::changeSliderValue);
    connect(this->ui->blockSpeedSlider, &QSlider::valueChanged, this, &SettingsWindow::changeSliderValue);

    // 信号：滑动滑块 - 背景音乐音量 -> 信号：设置背景音乐音量，槽：保存特殊设置
    connect(this->ui->musicVolumeSlider, &QSlider::valueChanged, this, &SettingsWindow::setMusicVolumeSignal);
    connect(this->ui->musicVolumeSlider, &QSlider::valueChanged, this, &SettingsWindow::saveSpecialSettings);

    // 信号：选择 - 背景音乐曲目 -> 信号：设置背景音乐曲目，槽：保存特殊设置
    connect(this->ui->musicChoiceComboBox, &QComboBox::currentTextChanged, this, &SettingsWindow::setMusicChoiceSignal);
    connect(this->ui->musicChoiceComboBox, &QComboBox::currentTextChanged, this, &SettingsWindow::saveSpecialSettings);

    // 信号：输入 - 键位 -> 槽：检测键位合法性
    connect(this->ui->moveLeftKeyEdit, &QKeySequenceEdit::keySequenceChanged, this, &SettingsWindow::checkKey);
    connect(this->ui->moveRightKeyEdit, &QKeySequenceEdit::keySequenceChanged, this, &SettingsWindow::checkKey);
    connect(this->ui->rorateKeyEdit, &QKeySequenceEdit::keySequenceChanged, this, &SettingsWindow::checkKey);
    connect(this->ui->quickenKeyEdit, &QKeySequenceEdit::keySequenceChanged, this, &SettingsWindow::checkKey);
    connect(this->ui->pauseKeyEdit, &QKeySequenceEdit::keySequenceChanged, this, &SettingsWindow::checkKey);

    // 信号：选择 - 背景图片 -> 信号：设置背景图片，槽：保存特殊设置
    connect(this->ui->pictureChoiceComboBox, &QComboBox::currentTextChanged, this, &SettingsWindow::setPictureChoiceSignal);
    connect(this->ui->pictureChoiceComboBox, &QComboBox::currentTextChanged, this, &SettingsWindow::saveSpecialSettings);

    // 信号：点击按钮 - 保存设置 -> 槽：保存设置，槽：应用设置
    connect(this->ui->saveButton, &QPushButton::clicked, this, &SettingsWindow::saveSettings);
    connect(this->ui->saveButton, &QPushButton::clicked, this, &SettingsWindow::applySettings);

    // 信号：点击按钮 - 重置设置 -> 槽：重置设置，槽：应用设置
    connect(this->ui->resetButton, &QPushButton::clicked, this, &SettingsWindow::resetSettings);
    connect(this->ui->resetButton, &QPushButton::clicked, this, &SettingsWindow::applySettings);

    // 信号：点击按钮 - 返回主菜单 -> 信号：返回主菜单
    connect(this->ui->returnToMenuButton, &QPushButton::clicked, this, &SettingsWindow::returnToMenuSignal);
}

SettingsWindow::~SettingsWindow()
{
    if (this->ui != nullptr)
    {
        delete this->ui;
        this->ui = nullptr;
    }
}

// public 函数

int SettingsWindow::getMusicVolume() const { return this->ui->musicVolumeSlider->value(); }

QString SettingsWindow::getMusicChoice() const { return this->ui->musicChoiceComboBox->currentText();}

QString SettingsWindow::getPictureChoice() const { return this->ui->pictureChoiceComboBox->currentText(); }

void SettingsWindow::adjustFontColor(const QPixmap& backgroundPic)
{
    QImage image = backgroundPic.toImage();
    QColor backgroundColor = image.pixelColor(0, 0);

    // 根据背景颜色选择字体颜色
    if (backgroundColor.lightness() < 128)
    {
        this->ui->movingBlockColorLabel->setStyleSheet("color: white;");
        this->ui->fixedBlockColorLabel->setStyleSheet("color: white;");

        this->ui->blockSpeedLabel->setStyleSheet("color: white;");

        this->ui->widthLabel->setStyleSheet("color: white;");
        this->ui->heightLabel->setStyleSheet("color: white;");

        this->ui->musicChoiceLabel->setStyleSheet("color: white;");
        this->ui->musicVolumeLabel->setStyleSheet("color: white;");

        this->ui->moveLeftLabel->setStyleSheet("color: white;");
        this->ui->moveRightLabel->setStyleSheet("color: white;");
        this->ui->rorateLabel->setStyleSheet("color: white;");
        this->ui->quickenLabel->setStyleSheet("color: white;");
        this->ui->pauseLabel->setStyleSheet("color: white;");

        this->ui->pictureChoiceLabel->setStyleSheet("color: white;");
    }
    else
    {
        this->ui->movingBlockColorLabel->setStyleSheet("color: black;");
        this->ui->fixedBlockColorLabel->setStyleSheet("color: black;");

        this->ui->blockSpeedLabel->setStyleSheet("color: black;");

        this->ui->widthLabel->setStyleSheet("color: black;");
        this->ui->heightLabel->setStyleSheet("color: black;");

        this->ui->musicChoiceLabel->setStyleSheet("color: black;");
        this->ui->musicVolumeLabel->setStyleSheet("color: black;");

        this->ui->moveLeftLabel->setStyleSheet("color: black;");
        this->ui->moveRightLabel->setStyleSheet("color: black;");
        this->ui->rorateLabel->setStyleSheet("color: black;");
        this->ui->quickenLabel->setStyleSheet("color: black;");
        this->ui->pauseLabel->setStyleSheet("color: black;");

        this->ui->pictureChoiceLabel->setStyleSheet("color: black;");
    }
}

QJsonObject SettingsWindow::loadSettings()
{
    QJsonObject settings;

    QFile file("Settings.json");

    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();

        QJsonDocument doc(QJsonDocument::fromJson(data));

        file.close();

        settings = doc.object();
    }
    else
        qDebug() << "设置读取失败！";

    return settings;
}

// private 槽函数

void SettingsWindow::saveSettings()
{
    // 记录移动方块颜色、固定方块颜色
    QString movingBlockColor = this->ui->movingBlockColorComboBox->currentText();
    QString fixedBlockColor = this->ui->fixedBlockColorComboBox->currentText();

    // 记录方块下落速度
    double blockSpeed = this->ui->blockSpeedSlider->value() / 10.0f;

    // 记录棋盘宽度和高度
    int boardWidth = this->ui->widthSpinBox->value();
    int boardHeight = this->ui->heightSpinBox->value();

    // 记录背景音乐曲目、音量
    QString musicChoice = this->ui->musicChoiceComboBox->currentText();
    int musicVolume = this->ui->musicVolumeSlider->value();

    // 记录按键绑定的键序列
    QKeySequence moveLeftKey = this->ui->moveLeftKeyEdit->keySequence();
    QKeySequence moveRightKey = this->ui->moveRightKeyEdit->keySequence();
    QKeySequence rorateKey = this->ui->rorateKeyEdit->keySequence();
    QKeySequence quickenKey = this->ui->quickenKeyEdit->keySequence();
    QKeySequence pauseKey = this->ui->pauseKeyEdit->keySequence();

    QString moveLeftKeyString = moveLeftKey.toString();
    QString moveRightKeyString = moveRightKey.toString();
    QString rorateKeyString = rorateKey.toString();
    QString quickenKeyString = quickenKey.toString();
    QString pauseKeyString = pauseKey.toString();

    // 记录背景图片
    QString pictureChoice = this->ui->pictureChoiceComboBox->currentText();

    // 保存设置到 JSON 文件

    QJsonObject settings;

    settings["movingBlockColor"] = movingBlockColor;
    settings["fixedBlockColor"] = fixedBlockColor;

    settings["blockSpeed"] = blockSpeed;

    settings["boardWidth"] = boardWidth;
    settings["boardHeight"] = boardHeight;

    settings["musicChoice"] = musicChoice;
    settings["musicVolume"] = musicVolume;

    settings["moveLeftKey"] = moveLeftKeyString;
    settings["moveRightKey"] = moveRightKeyString;
    settings["rorateKey"] = rorateKeyString;
    settings["quickenKey"] = quickenKeyString;
    settings["pauseKey"] = pauseKeyString;

    settings["pictureChoice"] = pictureChoice;

    QJsonDocument doc(settings);    // 创建 JSON 文档

    QFile file("Settings.json");    // 将 JSON 文档写入文件

    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();

        qDebug() << "设置已保存到“Settings.json”文件！";
    }
    else
        qDebug() << "设置未保存到“Settings.json”文件！";
}

void SettingsWindow::changeSliderValue(const int value)
{
    if (this->ui->musicVolumeSlider)
        this->ui->musicVolumeSlider->setToolTip(QString::number(value));

    if (this->ui->blockSpeedSlider)
        this->ui->blockSpeedSlider->setToolTip(QString::number(value / 10.0f));
}

void SettingsWindow::applySettings()
{
    QFile file("Settings.json");

    // 若不存在设置文件，则创建设置文件
    if (!file.exists())
    {
        qDebug() << "设置文件不存在，将重置设置并创建设置文件";
        this->resetSettings();
    }

    this->ui->blockSpeedSlider->setToolTip(QString::number(this->ui->blockSpeedSlider->value() / 10.0f));
    this->ui->musicVolumeSlider->setToolTip(QString::number(this->ui->musicVolumeSlider->value()));

    QJsonObject settings = SettingsWindow::loadSettings();

    // 读取移动方块颜色、固定方块颜色
    QString movingBlockColor = settings["movingBlockColor"].toString();
    QString fixedBlockColor = settings["fixedBlockColor"].toString();

    // 读取方块下落速度
    double blockSpeed = settings["blockSpeed"].toDouble();

    // 读取棋盘宽度和高度
    int boardWidth = settings["boardWidth"].toInt();
    int boardHeight = settings["boardHeight"].toInt();

    // 读取背景音乐曲目、音量
    QString musicChoice = settings["musicChoice"].toString();
    int musicVolume = settings["musicVolume"].toInt();

    // 读取按键绑定的键序列
    QString moveLeftKeyString = settings["moveLeftKey"].toString();
    QString moveRightKeyString = settings["moveRightKey"].toString();
    QString rorateKeyString = settings["rorateKey"].toString();
    QString quickenKeyString = settings["quickenKey"].toString();
    QString pauseKeyString = settings["pauseKey"].toString();

    QKeySequence moveLeftKey = QKeySequence::fromString(moveLeftKeyString);
    QKeySequence moveRightKey = QKeySequence::fromString(moveRightKeyString);
    QKeySequence rorateKey = QKeySequence::fromString(rorateKeyString);
    QKeySequence quickenKey = QKeySequence::fromString(quickenKeyString);
    QKeySequence pauseKey = QKeySequence::fromString(pauseKeyString);

    // 读取背景图片
    QString pictureChoice = settings["pictureChoice"].toString();

    // 应用设置到设置界面部件

    this->ui->movingBlockColorComboBox->setCurrentText(movingBlockColor);
    this->ui->fixedBlockColorComboBox->setCurrentText(fixedBlockColor);

    this->ui->widthSpinBox->setValue(boardWidth);
    this->ui->heightSpinBox->setValue(boardHeight);

    this->ui->blockSpeedSlider->setValue(blockSpeed * 10);

    this->ui->musicChoiceComboBox->setCurrentText(musicChoice);
    this->ui->musicVolumeSlider->setValue(musicVolume);

    this->ui->moveLeftKeyEdit->setKeySequence(moveLeftKey);
    this->ui->moveRightKeyEdit->setKeySequence(moveRightKey);
    this->ui->rorateKeyEdit->setKeySequence(rorateKey);
    this->ui->quickenKeyEdit->setKeySequence(quickenKey);
    this->ui->pauseKeyEdit->setKeySequence(pauseKey);

    this->ui->pictureChoiceComboBox->setCurrentText(pictureChoice);
}

void SettingsWindow::resetSettings()
{
    // 记录移动方块颜色、固定方块颜色
    QString movingBlockColor = "红色";
    QString fixedBlockColor = "蓝色";

    // 记录方块下落速度
    double blockSpeed = 1.0;

    // 记录棋盘宽度和高度
    int boardWidth = 15;
    int boardHeight = 20;

    // 记录背景音乐曲目、音量
    QString musicChoice = "植物大战僵尸";
    int musicVolume = 50;

    // 记录按键绑定的键序列
    QKeySequence moveLeftKey = Qt::Key_A;
    QKeySequence moveRightKey = Qt::Key_D;
    QKeySequence rorateKey = Qt::Key_W;
    QKeySequence quickenKey = Qt::Key_S;
    QKeySequence pauseKey = Qt::Key_P;

    QString moveLeftKeyString = moveLeftKey.toString();
    QString moveRightKeyString = moveRightKey.toString();
    QString rorateKeyString = rorateKey.toString();
    QString quickenKeyString = quickenKey.toString();
    QString pauseKeyString = pauseKey.toString();

    // 记录背景图片
    QString pictureChoice = "纯白";

    // 保存设置到 JSON 文件

    QJsonObject settings;

    settings["movingBlockColor"] = movingBlockColor;
    settings["fixedBlockColor"] = fixedBlockColor;

    settings["blockSpeed"] = blockSpeed;

    settings["boardWidth"] = boardWidth;
    settings["boardHeight"] = boardHeight;

    settings["musicChoice"] = musicChoice;
    settings["musicVolume"] = musicVolume;

    settings["moveLeftKey"] = moveLeftKeyString;
    settings["moveRightKey"] = moveRightKeyString;
    settings["rorateKey"] = rorateKeyString;
    settings["quickenKey"] = quickenKeyString;
    settings["pauseKey"] = pauseKeyString;

    settings["pictureChoice"] = pictureChoice;

    QJsonDocument doc(settings);    // 创建 JSON 文档

    QFile file("Settings.json");    // 将 JSON 文档写入文件

    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();

        qDebug() << "设置已重置！";
    }
    else
        qDebug() << "设置未重置！";
}

void SettingsWindow::saveSpecialSettings()
{
    QFile file("Settings.json");

    QByteArray jsonData = QByteArray();

    // 从文件中读取 JSON 数据
    if (file.open(QIODevice::ReadOnly))
    {
        jsonData = file.readAll();

        file.close();
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);    // 解析 JSON 数据

    if (!jsonDoc.isObject())
        return;

    QJsonObject settings = jsonDoc.object();

    // 检索背景音乐曲目
    if (settings.contains("musicChoice"))
    {
        // 修改值
        settings["musicChoice"] = this->ui->musicChoiceComboBox->currentText();

        // 将修改后的 JSON 数据保存到文件中
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            QJsonDocument modifiedJsonDoc(settings);

            file.write(modifiedJsonDoc.toJson());
            file.close();
        }
    }

    // 检索背景音乐音量
    if (settings.contains("musicVolume"))
    {
        // 修改值
        settings["musicVolume"] = this->ui->musicVolumeSlider->value();

        // 将修改后的 JSON 数据保存到文件中
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            QJsonDocument modifiedJsonDoc(settings);

            file.write(modifiedJsonDoc.toJson());
            file.close();
        }
    }

    // 检索背景图片
    if (settings.contains("pictureChoice"))
    {
        // 修改值
        settings["pictureChoice"] = this->ui->pictureChoiceComboBox->currentText();

        // 将修改后的 JSON 数据保存到文件中
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            QJsonDocument modifiedJsonDoc(settings);

            file.write(modifiedJsonDoc.toJson());
            file.close();
        }
    }
}

void SettingsWindow::checkKey()
{
    // 读取按键绑定的键序列

    QJsonObject settings = SettingsWindow::loadSettings();

    QList<QKeySequence> keys;

    keys.append(QKeySequence::fromString(settings["moveLeftKey"].toString()));
    keys.append(QKeySequence::fromString(settings["moveRightKey"].toString()));
    keys.append(QKeySequence::fromString(settings["rorateKey"].toString()));
    keys.append(QKeySequence::fromString(settings["quickenKey"].toString()));
    keys.append(QKeySequence::fromString(settings["pauseKey"].toString()));

    // 读取 QKeySequenceEdit 部件的当前键序列

    QList<QKeySequenceEdit*> keyEdits;

    keyEdits.append(this->ui->moveLeftKeyEdit);
    keyEdits.append(this->ui->moveRightKeyEdit);
    keyEdits.append(this->ui->rorateKeyEdit);
    keyEdits.append(this->ui->quickenKeyEdit);
    keyEdits.append(this->ui->pauseKeyEdit);

    // 判断输入的键位是否为单个按键

    for (int i = 0; i < keyEdits.size(); i++)
    {
        if (keyEdits[i]->keySequence().toString().size() > 1)
        {
            QMessageBox::information(this, "警告", "只能输入单个非特殊按键");
            keyEdits[i]->setKeySequence(keys[i]);
        }
    }

    // 判断输入的键位是否重复

    for (int i = 0; i < keyEdits.size() - 1; i++)
    {
        for (int j = i + 1; j < keyEdits.size(); j++)
        {
            if (keyEdits[i]->keySequence().toString() == keyEdits[j]->keySequence().toString())
            {
                QMessageBox::information(this, "警告", "存在重复按键");

                keyEdits[i]->setKeySequence(keys[i]);
                keyEdits[j]->setKeySequence(keys[j]);
            }
        }
    }
}









