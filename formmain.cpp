#include "formmain.h"

FormMain::FormMain(QWidget *parent)
    : QMainWindow(parent)
{
    sdb = new Database();
    parseList();
}

FormMain::~FormMain()
{
    delete sdb;
}

void FormMain::parseList()
{

    /*
    QString listFileName = QFileDialog::getOpenFileName(this,
                                    QString(tr("Открыть список воспроизведения")),
                                    QDir::currentPath(),
                                    tr("Списки воспроизведения (*.m3u *.m3u8);;Все файла (*.*)"));


    // Пользователь отменил открытие файла
    if (listFileName.isEmpty())
        return;

    */

    QString listFileName = "E:\\Projects\\CPP\\m3u\\playlists\\5368.m3u";

    QString msg = tr("Читаем файл [%1]...");
    msg = msg.arg(listFileName);

    // Получаем список каналов из списка
    parser = new Parser(listFileName);
    Playlist playlist = parser->getPlaylist();
    QList<Channel> channels = parser->getChannels();
    msg = "Получено каналов: %1. Сохраняем в БД...";
    msg = msg.arg(channels.count());

    // Сохраняем каналы в БД
    for (int i=0; i<channels.count(); i++)
    {
        Channel channel = channels.at(i);

        // Получаем идент группы
        int groupId = sdb->getGroupId(channel.getGroupName());
        if (groupId == -1)
        {
            groupId = sdb->addGroup(channel.getGroupName());
        }


        // Ищем идент звуковой дорожки
        int trackId = sdb->getTrackId(channel.getAudioTrack());
        if (trackId == -1)
        {
            trackId = sdb->addTrack(channel.getAudioTrack());
        }


        channel.setGroupUid(groupId);
        channel.setAudioTrackUid(trackId);
        sdb->addChannel(channel);
    }
}

