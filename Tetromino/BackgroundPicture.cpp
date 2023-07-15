#include"BackgroundPicture.h"

BackgroundPicture::BackgroundPicture()
{
    // 添加图片地址到 pictureList
    this->pictureList << ":/Resource/BackgroundPicture/Pure White.jpg"
                      << ":/Resource/BackgroundPicture/Impression - Sunrise.jpg"
                      << ":/Resource/BackgroundPicture/Impression - Sunset.jpg"
                      << ":/Resource/BackgroundPicture/Impression - Forest.jpg"
                      << ":/Resource/BackgroundPicture/Impression - Desert.jpg"
                      << ":/Resource/BackgroundPicture/Impression - Star Sky.jpg"
                      << ":/Resource/BackgroundPicture/Skyrim.jpg"
                      << ":/Resource/BackgroundPicture/PVZ.jpg"
                      << ":/Resource/BackgroundPicture/Creeper.jpg"
                      << ":/Resource/BackgroundPicture/Your Name.jpg"
                      << ":/Resource/BackgroundPicture/Boating.jpg";

    this->loadSettings();    // 加载设置
}

// public 函数

void BackgroundPicture::showPicture(QMainWindow* mainWindow)
{
    if(this->pictureChoice >= 0 && this->pictureChoice < this->pictureList.size())
    {
        QPixmap backgroundPicture(this->pictureList[this->pictureChoice]);    // 读取图片选项

        QPalette palette;

        palette.setBrush(QPalette::Background, backgroundPicture);    // 设置颜色和属性

        mainWindow->setPalette(palette);    // 填充背景图片
    }
}

void BackgroundPicture::setChoice(const int choice) { this->pictureChoice = choice; }

void BackgroundPicture::setChoice(const QString& choice)
{
    int picChoice = BackgroundPicture::getChoiceFromString(choice);

    this->pictureChoice = picChoice;
}

QPixmap BackgroundPicture::getPicture() const
{
    if (this->pictureChoice >=0 && this->pictureChoice < this->pictureList.size())
        return QPixmap(this->pictureList[this->pictureChoice]);
    else
        return QPixmap();
}

// private 函数

int BackgroundPicture::getChoiceFromString(const QString& choiceString)
{
    int choice;

    if (choiceString == "纯白")
        choice = 0;
    else if (choiceString == "印象 - 日出")
        choice = 1;
    else if (choiceString == "印象 - 日落")
        choice = 2;
    else if (choiceString == "印象 - 森林")
        choice = 3;
    else if (choiceString == "印象 - 沙漠")
        choice = 4;
    else if (choiceString == "印象 - 星空")
        choice = 5;
    else if (choiceString == "上古卷轴5：天际")
        choice = 6;
    else if (choiceString == "植物大战僵尸")
        choice = 7;
    else if (choiceString == "苦力怕")
        choice = 8;
    else if (choiceString == "你的名字。")
        choice = 9;
    else if (choiceString == "泛舟")
        choice = 10;
    else
        choice = 0;

    return choice;
}

void BackgroundPicture::loadSettings()
{
    QJsonObject settings = SettingsWindow::loadSettings();

    if (!settings.isEmpty())
        qDebug() << "背景图片设置读取成功!";
    else
        return;

    // 读取背景图片

    QString pictureChoiceString = settings["pictureChoice"].toString();

    int pictureChoice = BackgroundPicture::getChoiceFromString(pictureChoiceString);

    this->setChoice(pictureChoice);    // 应用图片选项到变量
}














