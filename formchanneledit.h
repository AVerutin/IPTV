#ifndef FORMCHANNELEDIT_H
#define FORMCHANNELEDIT_H

#include <QDialog>
#include <QString>
#include <QtWidgets>
#include <QList>
#include "channel.h"
#include "group.h"
#include "track.h"

class FormChannelEdit : public QDialog
{
public:
    FormChannelEdit();
    FormChannelEdit(const Channel&);
    Channel getChannel();
    bool isChanged();
    void setGroups(const QList<Group>&);
    void setTracks(const QList<Track>&);

private:
    // Элементы формы
    QLabel *label1;
    QVBoxLayout *mainLayout;

    Channel channel;
    QList<QString> groupsNames;
    QList<QString> tracksNames;
    bool editMode;
    bool changed;
    void createWidget();

private slots:
    void slotButtonOk();
};

#endif // FORMCHANNELEDIT_H
