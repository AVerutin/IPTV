#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QFile>
#include <QCoreApplication>
#include <QList>

#include "playlist.h"
#include "logger.h"
#include "channel.h"
#include "datatypes.h"

class Parser
{
public:
  Parser();
  Parser(QString);

  Playlist getPlaylist();
  QList<Channel> getChannels();


private:
  Channel *channel;
  QString playlistName;

  ////////////////
  QFile *listFile;
  Logger *logger;
  QString unitName;
  QString groupName;
  QString userAgent;
  QString httpReferrer;
  Playlist playlist;
  QList<Channel> channels;

  void parse();
  ////////////////

  Playlist getListTitle(const QString &);
  QString getGroupName(const QString &);
  QString getListName(const QString &);
  VlcInfo getVlcOpt(const QString &);
  Channel getChannelInfo(const QString &);
};

#endif // PARSER_H
