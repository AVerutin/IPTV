#include "parser.h"

Parser::Parser()
{
    logger = new Logger;
    unitName = typeid(this).name();

    // Запуск разбора файла списка
    parse();
}

Parser::Parser(QString fileName)
{
    if(!fileName.isEmpty())
        playlistName = fileName;

    logger = new Logger;
    unitName = typeid(this).name();

    // Запуск разбора файла списка
    parse();
}


/// Получить данные о списке воспроизведения
Playlist Parser::getPlaylist()
{
    return this->playlist;
}


/// Получить параметры списка воспроизведения
//void Parser::parse()
//{
//    listFile = new QFile(playlistName);
//    if ((listFile->exists()) && (listFile->open(QIODevice::ReadOnly)))
//    {
//        QString line = "";
//        while(!listFile->atEnd())
//        {
//            line = listFile->readLine().trimmed();
//            if (!line.isEmpty())
//            {
//                if (line.startsWith("#EXTM3U"))
//                {
//                    // Заголовок списка
//                    playlist = getListTitle(line);
//                }
//                else if (line.startsWith("#EXTINF"))
//                {
//                    // Заголовок канала
//                    Channel chan = getChannelInfo(line);
//                    channels.append(chan);
//                }
//                else if (line.startsWith("#PLAYLIST"))
//                {
//                    // Наименование списка
//                    QString listName = getListName(line);
//                    playlist.setName(listName);
//                }
//                else if (line.startsWith("#EXTGRP"))
//                {
//                    // Наименование группы
//                    QString group = getGroupName(line);
//                    groupName = group;
//                }
//                else if (line.startsWith("#EXTVLCOPT"))
//                {
//                    // Дополнительные параматры VLC
//                }
//                else if (line.contains("://"))
//                {
//                    // Ссылка на источник канала
//                }
//            }
//        }
//    }
//}


/// Получить список каналов из списка воспроизведения
QList<Channel> Parser::getChannels()
{
    return this->channels;
}



////////////////////////////////

/// Анализ файла списка воспроизведения
void Parser::parse()
{
  Channel *chan = nullptr;

  listFile = new QFile(playlistName);
  if ((listFile->exists()) && (listFile->open(QIODevice::ReadOnly)))
  {
      QString line = "";
      while(!listFile->atEnd())
      {
          line = listFile->readLine().trimmed();
          if(line.isEmpty())
            continue;

          /// Заголовок списка
          if(line.startsWith("#EXTM3U"))
            {
              // Поиск параметров списка при помощи регулярных выражений
              Playlist plInfo = getListTitle(line);

              playlist.setUrlTvg(plInfo.getUrlTvg());
              playlist.setTvgShift(plInfo.getTvgShift());
              playlist.setCache(plInfo.getCache());
              playlist.setDeinterlace(plInfo.getDeinterlace());
              playlist.setAspectRatio(plInfo.getAspectRatio());
              playlist.setCrop(plInfo.getCrop());
              playlist.setRefreshPeriod(plInfo.getRefreshPeriod());
              playlist.setAutoload(plInfo.isAutoload());
            }

          /// Заголовок канала
          else if(line.startsWith("#EXTINF"))
            {
              // Описание свойств канала
              Channel chInfo = getChannelInfo(line);
              if(chan==nullptr)
                chan = new Channel;

              chan->setDuration(chInfo.getDuration());
              chan->setTvgId(chInfo.getTvgId());
              chan->setTvgName(chInfo.getTvgName());
              chan->setName(chInfo.getName());
              chan->setTvgLogo(chInfo.getTvgLogo());
              chan->setTvgShift(chInfo.getTvgShift());
              chan->setGroupName(chInfo.getGroupName());
              chan->setRadio(chInfo.isRadio());
              chan->setAudioTrack(chInfo.getAudioTrack());
              chan->setAspectRatio(chInfo.getAspectRatio());
              chan->setRecordable(chInfo.isRecordable());
              chan->setCensored(chInfo.isCensored());
              chan->setAgeRestricted(chInfo.isAgeRestricted());
              chan->setUrlM3u(chInfo.getUrlM3u());
              chan->setNameAsKey(chInfo.isNameAsKey());
              chan->setCrop(chInfo.getCrop());
              chan->setMono(chInfo.isMono());
            }

          /// Наименование списка
          else if(line.startsWith("#PLAYLIST"))
            {
              QString listName = getListName(line);
              playlist.setName(listName);
            }

          /// Наименование группы
          else if(line.startsWith("#EXTGRP"))
            {
              if(chan==nullptr)
                chan = new Channel;

              QString groupName = getGroupName(line);
              chan->setGroupName(groupName);
            }

          /// Дополнительные параметры для VLC
          else if(line.startsWith("#EXTVLCOPT"))
            {
              if(chan==nullptr)
                chan = new Channel;

              VlcInfo vlc = getVlcOpt(line);
              chan->setUserAgent(vlc.userAgent);
              chan->setHttpReferrer(vlc.httpReferrer);
            }

          /// Ссылка на источник канал
          else if(line.contains("://"))
            {
              // Ссылка на источник канала
              if(chan==nullptr)
                chan = new Channel;

              chan->setUrl(line);
              channels.append(*chan);

              delete chan;
              chan = nullptr;
            }
        }

      listFile->close();
    }
}



/// Разбор параметров заголовка списка воспроизведения
Playlist Parser::getListTitle(const QString &listTitle)
{
  Playlist result;
  if(!listTitle.isEmpty())
    {
      // Анализируем строку на наличие параметров списка
      qDebug() << listTitle;

      // url-tvg="..." или url-xml="..." url-tvg=\"(.+?)\"
      QRegExp re("url-tvg=\"(.*)\"");
      re.setMinimal(true);
      int lastPos = 0;
      while((lastPos = re.indexIn(listTitle, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString url = re.cap(1);
          result.setUrlTvg(url);
        }

      re = QRegExp("url-xml=\"(.*)\"");
      re.setMinimal(true);
      lastPos = 0;
      while((lastPos = re.indexIn(listTitle, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString url = re.cap(1);
          result.setUrlTvg(url);
        }

      // cache="..."
      re = QRegExp("cache=\"*(\\d*)\"*");
      lastPos = 0;
      while((lastPos = re.indexIn(listTitle, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString cache = re.cap(1);
          result.setCache(cache.toInt());
        }

      // deinterlace="..."
      re = QRegExp("deinterlace=\"*(\\d*)?\"*");
      lastPos = 0;
      while((lastPos = re.indexIn(listTitle, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString deinterlace = re.cap(1);
          result.setDeinterlace(deinterlace.toInt());
        }

      // aspect-ratio="..."
      re = QRegExp("aspect-ratio=\"*(\\d+:\\d+)?\"*");
      lastPos = 0;
      while((lastPos = re.indexIn(listTitle, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString ratio = re.cap(1);
          result.setAspectRatio(ratio);
        }

      // crop="..."
      re = QRegExp("crop=\"(.*)\"\\s");
      lastPos = 0;
      while((lastPos = re.indexIn(listTitle, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString crop = re.cap(1);
          result.setCrop(crop);
        }

      // refresh="..."
      re = QRegExp("refresh=\"*(\\d*)?\"*");
      lastPos = 0;
      while((lastPos = re.indexIn(listTitle, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString refresh = re.cap(1);
          result.setRefreshPeriod(refresh.toInt());
        }

      // m3uautoload=1
      re = QRegExp("m3uautoload=\"*(\\d?)?\"*");
      lastPos = 0;
      while((lastPos = re.indexIn(listTitle, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString autoload = re.cap(1);
          result.setAutoload(autoload.toInt());
        }

      // tvg-shift=(...-2, -1, 0, +1, +2, ...)
      re = QRegExp("tvg-shift=\"*([+|-]?\\d?)?\"*");
      lastPos = 0;
      while((lastPos = re.indexIn(listTitle, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString shift = re.cap(1);
          result.setTvgShift(shift.toInt());
        }
    }

  return result;
}



/// Разбор строки с наименованием группы
QString Parser::getGroupName(const QString &grpName)
{
  QString res;
  QRegExp re("#EXTGRP:(.*)");
  int lastPos = 0;

  while((lastPos = re.indexIn(grpName, lastPos)) != -1)
    {
      lastPos += re.matchedLength();
      res = re.cap(1);
    }

  return res;
}



/// Разбор строки с наименованием списка воспроизведения
QString Parser::getListName(const QString &name)
{
  QString res;
  QRegExp re("#PLAYLIST:(.*)");
  int lastPos = 0;

  while((lastPos = re.indexIn(name, lastPos)) != -1)
    {
      lastPos += re.matchedLength();
      res = re.cap(1);
    }

  return res;
}



/// Разбор строки с параметрами VLC
VlcInfo Parser::getVlcOpt(const QString &vlc)
{
  VlcInfo res;
  QRegExp re("#EXTVLCOPT:http-user-agent=(.*)");
  int lastPos = 0;

  while((lastPos = re.indexIn(vlc, lastPos)) != -1)
    {
      lastPos += re.matchedLength();
      res.userAgent = re.cap(1);
    }

  re = QRegExp("#EXTVLCOPT:http-referrer=(.*)");
  lastPos = 0;

  while((lastPos = re.indexIn(vlc, lastPos)) != -1)
    {
      lastPos += re.matchedLength();
      res.httpReferrer = re.cap(1);
    }

  return res;
}



/// Разбор строки с параметрами канала
Channel Parser::getChannelInfo(const QString &chan)
{
  Channel res;
  if(!chan.isEmpty())
    {
      // Анализируем строку на наличие параметров списка

      // #EXTINF:...
      QRegExp re("#EXTINF:([+,-]?[0-9]+)");
      int lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(1);
          res.setDuration(val.toInt());
        }

      // tvg-id="..."
      re = QRegExp("tvg-id=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setTvgId(val);
        }

      // tvg-name="..."
      re = QRegExp("tvg-name=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setTvgName(val);
        }

      // tvg-logo="..."
      re = QRegExp("tvg-logo=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setTvgLogo(val);
        }

      // tvg-epg="..."
      re = QRegExp("tvg-epg=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setTvgEpg(val);
        }

      // tvg-shift="..."
      re = QRegExp("tvg-shift=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setTvgShift(val.toInt());
        }

      // group-title="..."
      re = QRegExp("group-title=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setGroupName(val);
        }

      // radio="..."
      re = QRegExp("radio=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setRadio(val.toInt());
        }

      // aspect-ratio="..."
      re = QRegExp("aspect-ratio=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setAspectRatio(val);
        }

      // audio-track="..."
      re = QRegExp("audio-track=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setAudioTrack(val);
        }

      // recordable="..."
      re = QRegExp("recordable=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setRecordable(val=="true");
        }

      // censored="..."
      re = QRegExp("censored=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setCensored(val.toInt());
        }

      // agerestriction="..."
      re = QRegExp("agerestriction=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setAgeRestricted(val.toInt());
        }

      // url-m3u="..."
      re = QRegExp("url-m3u=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setUrlM3u(val);
        }

      // nameaskey="..."
      re = QRegExp("nameaskey=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setNameAsKey(val.toInt());
        }

      // crop="..."
      re = QRegExp("crop=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setCrop(val);
        }

      // mono="..."
      re = QRegExp("mono=(\"|')([^\"']+)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(2);
          res.setMono(val.toInt());
        }

      // Название канала после запятой
      re = QRegExp(",([^\n]*)");
      lastPos = 0;
      while((lastPos = re.indexIn(chan, lastPos)) != -1)
        {
          lastPos += re.matchedLength();
          QString val = re.cap(1);
          res.setName(val);
        }
    }

  return res;
}
