#include "formchannels.h"

FormChannels::FormChannels(QWidget *parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    createWidget();
    modified = false;
    sdb = new Database();
    logger = new Logger;
    unitName = typeid(this).name();

    showChannels();
}


/// Установить список каналов для отображения
void FormChannels::setChannelsList(const QList<Channel> &list)
{
    channels = list;
    showChannels();
}


/// Получить измененный список каналов
QList<Channel> FormChannels::channelsList()
{
    return channels;
}


/// Получить признак модификации списка каналов
bool FormChannels::isModified()
{
    return modified;
}


/// Создание главного виджета формы
void FormChannels::createWidget()
{
    // Создание первой линии виджетов
    twChannels = new QTableView;
    model = new QStandardItemModel(twChannels);
    pbChannelAdd = new QPushButton("Добавить");
    pbChannelEdit = new QPushButton("Правка");
    pbChannelRemove = new QPushButton("Удалить");
    connect(pbChannelAdd, &QPushButton::clicked, this, &FormChannels::slotAddChannel);
    connect(pbChannelEdit, &QPushButton::clicked, this, &FormChannels::slotEditChannel);
    connect(pbChannelRemove, &QPushButton::clicked, this, &FormChannels::slotRemoveChannel);

    lineButtons = new QVBoxLayout;
    lineButtons->addWidget(pbChannelAdd);
    lineButtons->addWidget(pbChannelEdit);
    lineButtons->addWidget(pbChannelRemove);
    lineButtons->addStretch();

    line1 = new QHBoxLayout;
    line1->addWidget(twChannels);
    line1->addLayout(lineButtons);

    // Создание второй линии виджетов
    pbClose = new QPushButton("Закрыть");
    connect(pbClose, &QPushButton::clicked, this, &FormChannels::accept);

    line2 = new QHBoxLayout;
    line2->addStretch(0);
    line2->addWidget(pbClose);
    line2->addStretch(0);

    // Создание главного виджета
    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(line1);
    mainLayout->addLayout(line2);
    this->setLayout(mainLayout);

    this->setWindowTitle("Список каналов");
    this->setWindowIcon(QIcon(":/icons/icon.ico"));
    this->setFixedSize(QSize(800, 400));
}


/// Отобразить список каналов в таблице
void FormChannels::showChannels()
{
    channels = sdb->getChannels();
    int chCount = channels.count();

    twChannels->setModel(model);
    model->clear();

    for(int i=0; i<chCount; i++)
      {
        QList<QStandardItem*> items;
        Channel ch = channels.at(i);
        int id = ch.getBaseId();
        items.append(new QStandardItem(QString::number(id)));
        items.append(new QStandardItem(ch.getName()));
        items.append(new QStandardItem(ch.getGroupName()));
        items.append(new QStandardItem(ch.getUrl()));

        model->appendRow(items);
      }

    model->setHorizontalHeaderLabels(QStringList() <<"№" << "Имя" << "Группа" << "Источник");
    twChannels->setColumnWidth(0, 0);
    twChannels->setColumnWidth(1, 150);
    twChannels->setColumnWidth(2, 140);
    twChannels->setColumnWidth(3, 350);

    twChannels->setEditTriggers(QAbstractItemView::NoEditTriggers);
    twChannels->setSelectionBehavior(QAbstractItemView::SelectRows);
    // twChannels->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}


/// Обработка нажатия на кнопку Добавить
void FormChannels::slotAddChannel()
{
    Channel ch;
    QList<Group> groups = sdb->getGroups();
    QList<Track> tracks = sdb->getTracks();
    FormChannelEdit *formEdit = new FormChannelEdit(ch, groups, tracks, false);

    if (formEdit->exec() == QDialog::Accepted)
    {
        ch = formEdit->getChannel();
        modified = formEdit->isChanged();
    }

    delete formEdit;
    formEdit = nullptr;

    if (modified)
    {
        // Сохраняем измененный канал в базу
        int grp = sdb->getGroupId(ch.getGroupName());
        int trk = sdb->getTrackId(ch.getAudioTrack());
        ch.setGroupUid(grp);
        ch.setAudioTrackUid(trk);

        // Добавляем канал в БД
        sdb->addChannel(ch);

        modified = false;
        this->showChannels();
    }
}


/// Обработка нажатия на кнопку Правка
void FormChannels::slotEditChannel()
{
    // Получим идентификатор канала в БД
    int chId = model->data(model->index(twChannels->currentIndex().row(), 0)).toInt();
    QString chName = model->data(model->index(twChannels->currentIndex().row(), 1)).toString();

    // Только для QSqlQueryModel
    // QSqlRecord record;
    // int id = model->record(twChannels->currentIndex().row()).value("id").toString();

    Channel ch = sdb->getChannel(chId);
    QList<Group> groups = sdb->getGroups();
    QList<Track> tracks = sdb->getTracks();
    FormChannelEdit *formEdit = new FormChannelEdit(ch, groups, tracks, true);

    if (formEdit->exec() == QDialog::Accepted)
    {
        ch = formEdit->getChannel();
        modified = formEdit->isChanged();
    }

    delete formEdit;
    formEdit = nullptr;

    if (modified)
    {
        // Сохраняем измененный канал в базу
        int grp = sdb->getGroupId(ch.getGroupName());
        int trk = sdb->getTrackId(ch.getAudioTrack());
        ch.setGroupUid(grp);
        ch.setAudioTrackUid(trk);

        // Редактирование канала
        bool result = sdb->editChannel(ch);
        if (!result)
        {
            QString msg = "Не удалось изменить канал [%1] %2";
            msg = msg.arg(QString::number(chId), chName);
            logger->error(msg, unitName);
        }

        modified = false;
        this->showChannels();
    }
}


/// Обработка нажатия на кнопку Удалить
void FormChannels::slotRemoveChannel()
{
    int count = twChannels->selectionModel()->selectedRows().count();
    for (int i=0; i<count; i++)
    {
        // Получим идентификатор канала в БД
        int chId = model->data(model->index(twChannels->selectionModel()->selectedRows().at(i).row(), 0)).toInt();
        QString chName = model->data(model->index(twChannels->selectionModel()->selectedRows().at(i).row(), 1)).toString();
        modified = sdb->removeChannel(chId);

        if (!modified)
        {
            QString msg = "Не удалось удалить канал [%1] %2";
            msg = msg.arg(QString::number(chId), chName);
            logger->error(msg, unitName);
        }
        else
        {
            QString msg = "Удален канал [%1] %2";
            msg = msg.arg(QString::number(chId), chName);
            logger->warn(msg, unitName);
        }
    }

    this->showChannels();
}

