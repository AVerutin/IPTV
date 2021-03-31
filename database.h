#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QList>
#include "channel.h"
#include "playlist.h"
#include "logger.h"
#include "group.h"
#include "track.h"

class Database
{
public:
    Database();
    void initDb();
    bool openDb();
    void closeDb();

    // Группы каналов
    int addGroup(const QString &);
    QString getGroupName(int);
    int getGroupId(const QString &);
    QList<Group> getGroups();
    bool removeGroup(int);
    bool removeGroup(const QString &);
    bool clearGroups();

    // Звуковые дорожки
    int addTrack(const QString &);
    QString getTrackName(int);
    int getTrackId(const QString &);
    QList<Track> getTracks();
    bool removeTrack(int);
    bool removeTrack(const QString &);
    bool clearTracks();

    // Каналы
    QList<Channel> addChannel(const Channel &);
    QList<Channel> getChannels();
    QList<Channel> getChannels(const QString &);
    QList<Channel> getChannels(int);
    Channel getChannel(int);
    bool editChannel(const Channel&);
    bool removeChannel(int);
    bool clearChannels();

    // Списки воспроизведения
    QList<Playlist> addPlaylist(const Playlist &);
    QList<Playlist> getPlaylists();
    QList<Playlist> getPlaylist(const QString &);
    Playlist getPlaylist(int);
    bool editPlaylist(const Playlist&);
    bool removePlaylist(int);
    bool clearPlaylists();

    // Отношения каналов и списков воспроизведения
    bool setRelation(int, int, int);
    bool clearRelation(int, int);

private:
    QSqlDatabase sdb;
    QString unitName;
    Logger *logger;    
};

#endif // DATABASE_H
