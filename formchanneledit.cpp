#include "formchanneledit.h"

FormChannelEdit::FormChannelEdit()
{
    editMode = false;
    changed = false;
}

FormChannelEdit::FormChannelEdit(const Channel& ch)
{
    editMode = true;
    changed = false;
    this->channel = ch;
    createWidget();
}


/// Создать виджет формы
void FormChannelEdit::createWidget()
{
    if (editMode)
    {
        // Устанавлиываем начальные значения для полей формы
    }
    else
    {
        // Очищаем значения для полей формы
    }

    label1 = new QLabel(this);
    label1->setText(channel.getName());

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(label1);

    this->setLayout(mainLayout);
}


/// Обработчик нажатия на кнопку ОК
void FormChannelEdit::slotButtonOk()
{

}


/// Получить созданный, или отредактированный канал
Channel FormChannelEdit::getChannel()
{
    return this->channel;
}


/// Получить признак модификации параметров канала
bool FormChannelEdit::isChanged()
{
    return this->changed;
}


/// Установить список групп
void FormChannelEdit::setGroups(const QList<Group> &groups)
{
    groupsNames.clear();

    for (int i=0; i<groups.count(); i++)
    {
        Group group = groups.at(i);
        this->groupsNames.append(group.getName());
    }
}


/// Установить список звуковых дорожек
void FormChannelEdit::setTracks(const QList<Track> &tracks)
{
    tracksNames.clear();

    for (int i=0; i<tracks.count(); i++)
    {
        Track track = tracks.at(i);
        this->tracksNames.append(track.getName());
    }
}
