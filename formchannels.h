#ifndef FORMCHANNELS_H
#define FORMCHANNELS_H

#include <QDialog>
#include <QtWidgets>
#include <QList>
#include <QString>

#include "channel.h"
#include "formchanneledit.h"
#include "database.h"

class FormChannels : public QDialog
{
    Q_OBJECT
public:
    explicit FormChannels(QWidget *parent = nullptr);
    void setChannelsList(const QList<Channel> &);
    QList<Channel> channelsList();
    bool isModified();

private:
    QTableView *twChannels;
    QStandardItemModel *model;
    QPushButton *pbChannelAdd;
    QPushButton *pbChannelEdit;
    QPushButton *pbChannelRemove;
    QPushButton *pbClose;

    QVBoxLayout *mainLayout;
    QHBoxLayout *line1;
    QVBoxLayout *lineButtons;
    QHBoxLayout *line2;

    QList<Channel> channels;
    bool modified;
    Database *sdb;

    void createWidget();
    void showChannels();

private slots:
    void slotAddChannel();
    void slotEditChannel();
    void slotRemoveChannel();

};

#endif // FORMCHANNELS_H
