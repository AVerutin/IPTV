#include "formmain.h"

FormMain::FormMain(QWidget *parent)
    : QMainWindow(parent)
{
    sdb = new Database();
    createWidget();

    // parseList();
}

FormMain::~FormMain()
{
    delete sdb;
}


/// Компоновка главного виджета формы
void FormMain::createWidget()
{
    // Создание главного меню приложения
    createMenu();

    // Свойства главной формы
    this->setFixedSize(QSize(650, 350));
    this->setWindowTitle("Списки воспроизведения");
    this->setWindowIcon(QIcon(":/icons/appIcon.ico"));

    // Строка состояния
    statBar = this->statusBar();
    statBar->setSizeGripEnabled(false);
    statBar->showMessage("Готово.");
}


/// Создание главного меню приложения
void FormMain::createMenu()
{
    // Пункт меню Плейлисты - Создать
    aCreatePlaylist = new QAction("Создать");
    aCreatePlaylist->setShortcut(QKeySequence("CTRL+N"));
    aCreatePlaylist->setToolTip("Создать новый список воспроизведения");
    aCreatePlaylist->setStatusTip("Создание нового пустого списка воспроизведения");
    connect(aCreatePlaylist, &QAction::triggered, this, &FormMain::slotCreatePlaylist);

    // Пункт меню Плейлисты - Правка
    aEditPlaylist = new QAction("Правка");
    aEditPlaylist->setShortcut(QKeySequence("CTRL+E"));
    aEditPlaylist->setToolTip("Редактировать выбранный список воспроизведения");
    aEditPlaylist->setStatusTip("Изменение параметров выбранного списка воспроизведения");
    connect(aEditPlaylist, &QAction::triggered, this, &FormMain::slotEditPlaylist);

    // Пункт меню Плейлисты - Удалить
    aRemovePlaylist = new QAction("Удалить");
    aRemovePlaylist->setShortcut(QKeySequence("CTRL+D"));
    aRemovePlaylist->setToolTip("Удалить выбранный список воспроизведения");
    aRemovePlaylist->setStatusTip("Удаление выбранного списка воспроизведения");
    connect(aRemovePlaylist, &QAction::triggered, this, &FormMain::slotRemovePlaylist);

    // Пункт меню Плейлисты - Экспорт
    aExportPlaylist = new QAction("Экспорт");
    aExportPlaylist->setShortcut(QKeySequence("CTRL+S"));
    aExportPlaylist->setToolTip("Экспортировать выбранный список воспроизведения");
    aExportPlaylist->setStatusTip("Экспорт выбранного списка воспроизведения в файл");
    connect(aExportPlaylist, &QAction::triggered, this, &FormMain::slotExportPlaylist);

    // Меню Плейлисты
    mnPlaylists = new QMenu(this);
    mnPlaylists->setTitle("Плейлисты");
    mnPlaylists->setToolTip("Списки воспроизведения");
    mnPlaylists->setStatusTip("Организация и управление списками воспроизведения");
    mnPlaylists->addAction(aCreatePlaylist);
    mnPlaylists->addAction(aEditPlaylist);
    mnPlaylists->addAction(aRemovePlaylist);
    mnPlaylists->addSeparator();
    mnPlaylists->addAction(aExportPlaylist);

    // Пункт меню Каналы - Список
    aShowChannels = new QAction("Список");
    aShowChannels->setShortcut(QKeySequence("CTRL+H"));
    aShowChannels->setToolTip("Вывести список каналов");
    aShowChannels->setStatusTip("Вывести список всех имеющихся каналов");
    connect(aShowChannels, &QAction::triggered, this, &FormMain::slotShowChannels);

    // Пункт меню Каналы - Импорт
    aImportChannels = new QAction("Импортировать");
    aImportChannels->setShortcut(QKeySequence("CTRL+I"));
    aImportChannels->setToolTip("Импорт каналов");
    aImportChannels->setStatusTip("Импортировать каналы из файла плейлиста");
    connect(aImportChannels, &QAction::triggered, this, &FormMain::slotImportChannels);

    mnChannels = new QMenu(this);
    mnChannels->setTitle("Каналы");
    mnChannels->setToolTip("Список каналов");
    mnChannels->setStatusTip("Просмотр и организация списка каналов");
    mnChannels->addAction(aShowChannels);
    mnChannels->addAction(aImportChannels);

    // Пункт меню Выход
    aQuit = new QAction("Выход");
    aQuit->setShortcut(QKeySequence("CTRL+Q"));
    aQuit->setToolTip("Закрыть приложение");
    aQuit->setStatusTip("Выход из приложение");
    connect(aQuit, &QAction::triggered, this, &FormMain::slotAppClose);

    mainMenu = this->menuBar();
    mainMenu->addMenu(mnPlaylists);
    mainMenu->addMenu(mnChannels);
    mainMenu->addAction(aQuit);
}


void FormMain::slotImportChannels()
{
    QString listFileName = QFileDialog::getOpenFileName(this,
                                    QString(tr("Открыть список воспроизведения")),
                                    QDir::currentPath(),
                                    tr("Списки воспроизведения (*.m3u *.m3u8);;Все файла (*.*)"));


    // Пользователь отменил открытие файла
    if (listFileName.isEmpty())
        return;


    // QString listFileName = "E:\\Projects\\CPP\\m3u\\playlists\\5368.m3u";

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


/// Слот для вывода списка имеющихся каналов в таблицу
void FormMain::slotShowChannels()
{
    formChannels = new FormChannels(this);
    // QList<Channel> channels = sdb->getChannels();
    // formChannels->setChannelsList(channels);

    formChannels->exec();
    delete formChannels;
}


/// Слот для выхода из приложения
void FormMain::slotAppClose()
{
    this->close();
}


/// Слот для создания списка воспроизведения
void FormMain::slotCreatePlaylist()
{

}


/// Слот для редактирования списка воспроизведения
void FormMain::slotEditPlaylist()
{

}


/// Слот для экспорта списка в файл m3u
void FormMain::slotExportPlaylist()
{

}


/// Слот для удаления списка воспроизведения
void FormMain::slotRemovePlaylist()
{

}

