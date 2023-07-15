#include"BackgroundMusic.h"

BackgroundMusic::BackgroundMusic(QObject* parent) : QObject(parent)
{
    this->musicPlayer = new QMediaPlayer(this);

    this->musicPlayerlist = new QMediaPlaylist(this);
    this->musicPlayerlist->setPlaybackMode(QMediaPlaylist::Loop);    // 设置循环播放音乐

    // 添加音乐到 musicList
    this->musicList << "qrc:/Resource/BackgroundMusic/Call Of Magic.mp3"
                    << "qrc:/Resource/BackgroundMusic/Faster.mp3";

    this->loadSettings();    // 加载设置
}

// public 函数

void BackgroundMusic::playMusic()
{
    this->musicPlayerlist->clear();    // 清空播放列表

    if (this->musicChoice >= 0 && this->musicChoice < this->musicList.size())
        this->musicPlayerlist->addMedia(QUrl(this->musicList[this->musicChoice]));
    else
        this->musicPlayerlist->addMedia(QUrl(this->musicList[0]));

    this->musicPlayer->setPlaylist(this->musicPlayerlist);    // 设置音乐播放列表

    this->musicPlayer->play();    // 播放音乐
}

void BackgroundMusic::stopMusic()
{
    // 如果音乐播放器状态和当前状态相同，则停止音乐
    if (this->musicPlayer->state() == QMediaPlayer::PlayingState)
        this->musicPlayer->stop();
}


void BackgroundMusic::setVolume(const int volume)
{
    if (volume < 0)
        this->musicPlayer->setVolume(0);

    if (volume > 100)
        this->musicPlayer->setVolume(100);

    this->musicPlayer->setVolume(volume);
}

void BackgroundMusic::setChoice(const int choice)
{
    this->stopMusic();    // 停止音乐

    this->musicChoice = choice;

    this->playMusic();    // 播放音乐
}

void BackgroundMusic::setChoice(const QString& choice)
{
    int musicChoice = BackgroundMusic::getChoiceFromString(choice);

    this->stopMusic();    // 停止音乐

    this->musicChoice = musicChoice;

    this->playMusic();    // 播放音乐
}

// private 函数

int BackgroundMusic::getChoiceFromString(const QString& choiceString)
{
    int choice = 0;

    if (choiceString == "上古卷轴")
        choice = 0;

    if (choiceString == "植物大战僵尸")
        choice = 1;

    return choice;
}

void BackgroundMusic::loadSettings()
{
    QJsonObject settings = SettingsWindow::loadSettings();

    if (!settings.isEmpty())
        qDebug() << "背景音乐设置读取成功!";
    else
        return;

    // 读取背景音乐曲目、音量

    QString musicChoiceString = settings["musicChoice"].toString();

    int musicVolume = settings["musicVolume"].toInt();
    int musicChoice = BackgroundMusic::getChoiceFromString(musicChoiceString);

    this->setVolume(musicVolume);    // 应用音量设置到变量
    this->setChoice(musicChoice);    // 应用曲目设置到变量
}










