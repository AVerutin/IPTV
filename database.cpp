#include "database.h"

Database::Database()
{
    logger = new Logger;
    unitName = typeid(this).name();
    dbInit();
}


/// Инициализация подключения к базе данных
void Database::dbInit()
{
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    //  sdb.setDatabaseName("m3u.dat");
    //  sdb.setHostName("localhost");
    //  sdb.setUserName("admin");
    //  sdb.setPassword("admin");

    QString dbPath = QCoreApplication::applicationDirPath() + "/data.db";
    sdb.setDatabaseName(dbPath);
    query = new QSqlQuery;


    // Подключение к базе данных
    if (!sdb.open())
    {
          logger->error(sdb.lastError().text(), unitName);
    }
    else
    {
        /////////// DDL query ///////////
        QString str = "CREATE TABLE IF NOT EXISTS `groups` ("
            "`id`	INTEGER NOT NULL,"
            "`group`	TEXT NOT NULL,"
            "PRIMARY KEY(`id` AUTOINCREMENT)"
        ");";

        bool b = query->exec(str);
        if (!b)
        {
            logger->error(sdb.lastError().text(), unitName);
        }

        str = "CREATE TABLE IF NOT EXISTS `tracks` ("
            "`id`	INTEGER NOT NULL,"
            "`track`	TEXT NOT NULL,"
            "PRIMARY KEY(`id` AUTOINCREMENT)"
        ");";

        b = query->exec(str);
        if (!b)
        {
            logger->error(sdb.lastError().text(), unitName);
        }

        str = "CREATE TABLE IF NOT EXISTS `channels` ("
            "`id`	INTEGER NOT NULL,"
            "`name`	TEXT NOT NULL,"
            "`position`	INTEGER,"
            "`duration`	INTEGER NOT NULL DEFAULT 0,"
            "`tvg_id`	TEXT,"
            "`tvg_name`	TEXT,"
            "`tvg_logo`	TEXT,"
            "`tvg_epg`	TEXT,"
            "`tvg_shift`	INTEGER,"
            "`group`	INTEGER NOT NULL,"
            "`track`	INTEGER NOT NULL,"
            "`radio`	INTEGER,"
            "`aspect`	TEXT,"
            "`crop`	TEXT,"
            "`recordable`	INTEGER,"
            "`censored`	INTEGER,"
            "`age_restrict`	INTEGER,"
            "`mono`	INTEGER,"
            "`name_as_key`	INTEGER,"
            "`url_m3u`	TEXT,"
            "`url`	TEXT,"
            "`user_agent`	TEXT,"
            "`http_referrer`	TEXT,"
            "PRIMARY KEY(`id` AUTOINCREMENT),"
            "FOREIGN KEY(`track`) REFERENCES `tracks`(`id`),"
            "FOREIGN KEY(`group`) REFERENCES `groups`(`id`)"
        ");";

        b = query->exec(str);
        if (!b)
        {
            logger->error(sdb.lastError().text(), unitName);
        }

        str = "CREATE TABLE IF NOT EXISTS `playlists` ("
            "`id`	INTEGER NOT NULL,"
            "`name`	TEXT NOT NULL,"
            "`url_tvg`	TEXT,"
            "`tvg_shift`	INTEGER,"
            "`cache`	INTEGER,"
            "`deinterlace`	INTEGER,"
            "`aspect`	TEXT,"
            "`crop`	TEXT,"
            "`refresh`	INTEGER,"
            "`autoload`	INTEGER,"
            "PRIMARY KEY(`id` AUTOINCREMENT)"
        ");";

        b = query->exec(str);
        if (!b)
        {
            logger->error(sdb.lastError().text(), unitName);
        }

        str = "CREATE TABLE IF NOT EXISTS `relations` ("
            "`id`	INTEGER NOT NULL,"
            "`playlist`	INTEGER NOT NULL,"
            "`channel`	INTEGER NOT NULL,"
            "`position`	INTEGER NOT NULL DEFAULT 1,"
            "FOREIGN KEY(`playlist`) REFERENCES `playlists`(`id`),"
            "FOREIGN KEY(`channel`) REFERENCES `channels`(`id`),"
            "PRIMARY KEY(`id` AUTOINCREMENT)"
        ");";

        b = query->exec(str);
        if (!b)
        {
            logger->error(sdb.lastError().text(), unitName);
        }

        str = "CREATE INDEX `channels_id` ON `channels` (`id`	ASC);";
        b = query->exec(str);
        if (!b)
        {
            logger->error(sdb.lastError().text(), unitName);
        }

        str = "CREATE INDEX `playlists_id` ON `playlists` (`id`);";
        b = query->exec(str);
        if (!b)
        {
            logger->error(sdb.lastError().text(), unitName);
        }

        /////////// Data query ///////////
        if (!query->exec("SELECT count(*) as rows_cnt FROM `groups`;"))
        {
            logger->error(sdb.lastError().text(), unitName);
        }

        QSqlRecord rec = query->record();
        int cnt = 0;

        while (query->next())
        {
            cnt = query->value(rec.indexOf("rows_cnt")).toInt();
            if (cnt==0)
            {
                addGroup("Общие");
                addGroup("Информационные");
                addGroup("Образовательные");
                addGroup("Познавательные");
                addGroup("Детские");
                addGroup("Фильмы");
                addGroup("Региональные");
            }
        }

        if (!query->exec("SELECT count(*) as rows_cnt FROM `tracks`;"))
        {
            logger->error(sdb.lastError().text(), unitName);
        }

        rec = query->record();
        while (query->next())
        {
            cnt = query->value(rec.indexOf("rows_cnt")).toInt();
            if (cnt==0)
            {
                addTrack("RU");
                addTrack("EN");
                addTrack("UA");
            }
        }
    }
}


/// Закрытие покдлючения к базе данных
void Database::dbClose()
{
    sdb.close();
}



// Управление группами каналов

/// Добавить новую группу
int Database::addGroup(const QString &grpName)
{
    if (!grpName.isEmpty())
        {
          QString queryString = "INSERT INTO `groups` (`group`) VALUES ('%1');";
          queryString = queryString.arg(grpName);

          int res = query->exec(queryString);
          if (!res)
            {
              logger->error(sdb.lastError().text(), unitName);
            }
        }

    int result = getLastId();

    return result;
}


/// Получить наименование группы по идентификатору
QString Database::getGroupName(int uid)
{
    QString result;
    if (uid>0)
    {
        QString queryStr = "SELECT `group` FROM `groups` WHERE `id`='%1';";
        queryStr = queryStr.arg(uid);

        if (!query->exec(queryStr))
        {
            logger->error(sdb.lastError().text(), unitName);
        }
        else
        {
            QSqlRecord rec = query->record();
            while (query->next())
            {
                result = query->value(rec.indexOf("name")).toString();
            }
        }
    }

    return result;
}


/// Получить идентификатор группы по наименованию
int Database::getGroupId(const QString &name)
{
    int result = -1;
    if (name.size()>0)
    {
        QString queryStr = "SELECT `id` FROM `groups` WHERE `group`='%1';";
        queryStr = queryStr.arg(name);

        if (!query->exec(queryStr))
        {
            logger->error(sdb.lastError().text(), unitName);
        }
        else
        {
            QSqlRecord rec = query->record();
            while (query->next())
            {
                result = query->value(rec.indexOf("id")).toInt();
            }
        }
    }

    return result;
}


/// Получить список всех групп
QList<Group> Database::getGroups()
{
    QList<Group> result;
    if (!query->exec("SELECT `id`, `group` FROM `groups` ORDER BY `group`;"))
    {
        logger->error(sdb.lastError().text(), unitName);
    }
    else
    {
        QSqlRecord rec = query->record();

        while (query->next())
        {
            Group grp;
            grp.setId(query->value(rec.indexOf("id")).toInt());
            grp.setName(query->value(rec.indexOf("group")).toString());
            result.append(grp);
        }
    }

    return result;
}


/// УДалить группу по идентификатору
bool Database::removeGroup(int uid)
{
    bool result = false;
    if (uid>0)
    {
        QString queryStr = "DELETE FROM `groups` WHERE `id`='%1';";
        queryStr = queryStr.arg(uid);
        result = query->exec(queryStr);

        if (!result)
        {
            logger->error(sdb.lastError().text(), unitName);
        }
    }

    return result;
}


/// Удалить группу по наименованию
bool Database::removeGroup(const QString &name)
{
    bool result = false;
    if (name.size()>0)
    {
        QString queryStr = "DELETE FROM `groups` WHERE `group`='%1';";
        queryStr = queryStr.arg(name);
        result = query->exec(queryStr);

        if (!result)
        {
            logger->error(sdb.lastError().text(), unitName);
        }
    }

    return result;
}


/// Очистить список групп
bool Database::clearGroups()
{
    bool result = false;
    QString queryStr = "DELETE FROM `groups`;";
    result = query->exec(queryStr);

    if (!result)
    {
        logger->error(sdb.lastError().text(), unitName);
    }

    return result;
}



// Управление звуковыми дорожками

/// Добавить новую звуковую дорожку в БД
int Database::addTrack(const QString &trkName)
{
    if (!trkName.isEmpty())
    {
        QString queryString = "INSERT INTO `tracks` (`track`) VALUES ('%1');";
        queryString = queryString.arg(trkName);

        int res = query->exec(queryString);
        if (!res)
        {
            logger->error(sdb.lastError().text(), unitName);
        }
    }

    int result = getLastId();

    return result;
}


/// Получмть наименование дорожки по идентификатору
QString Database::getTrackName(int uid)
{
    QString result;
    if (uid>0)
    {
        QString queryStr = "SELECT `track` FROM `tracks` WHERE `id`='%1';";
        queryStr = queryStr.arg(uid);

        if (!query->exec(queryStr))
        {
            logger->error(sdb.lastError().text(), unitName);
        }
        else
        {
            QSqlRecord rec = query->record();
            while (query->next())
            {
                result = query->value(rec.indexOf("name")).toString();
            }
        }
    }

    return result;
}


/// Получить идентификатор звуковой дорожки по наименованию
int Database::getTrackId(const QString &name)
{
    int result = -1;
    if (name.size()>0)
    {
        QString queryStr = "SELECT `id` FROM `tracks` WHERE `track`='%1';";
        queryStr = queryStr.arg(name);

        if (!query->exec(queryStr))
        {
            logger->error(sdb.lastError().text(), unitName);
        }
        else
        {
            QSqlRecord rec = query->record();
            while (query->next())
            {
                result = query->value(rec.indexOf("id")).toInt();
            }
        }
    }

    return result;
}


/// Получить список звуковых дорожек из БД
QList<Track> Database::getTracks()
{
    QList<Track> result;
    if (!query->exec("SELECT `id`, `track` FROM `tracks` ORDER BY `track`;"))
    {
        logger->error(sdb.lastError().text(), unitName);
    }
    else
    {
        QSqlRecord rec = query->record();

        while (query->next())
        {
            Track trk;
            trk.setId(query->value(rec.indexOf("id")).toInt());
            trk.setName(query->value(rec.indexOf("track")).toString());
            result.append(trk);
        }
    }

    return result;
}


/// Удалить звуковую дорожку по идентификатору
bool Database::removeTrack(int uid)
{
    bool result = false;
    if (uid>0)
    {
        QString queryStr = "DELETE FROM `tracks` WHERE `id`='%1';";
        queryStr = queryStr.arg(uid);
        result = query->exec(queryStr);

        if (!result)
        {
            logger->error(sdb.lastError().text(), unitName);
        }
    }

    return result;
}


/// Удалить звуковую дорожку по наименованию
bool Database::removeTrack(const QString &name)
{
    bool result = false;
    if (name.size()>0)
    {
        QString queryStr = "DELETE FROM `tracks` WHERE `track`='%1';";
        queryStr = queryStr.arg(name);
        result = query->exec(queryStr);

        if (!result)
        {
            logger->error(sdb.lastError().text(), unitName);
        }
    }

    return result;
}


/// Удалить все звуковые дорожки в БД
bool Database::clearTracks()
{
    QString queryStr = "DELETE FROM `tracks`;";
    bool result = query->exec(queryStr);

    if (!result)
    {
        logger->error(sdb.lastError().text(), unitName);
    }

    return result;
}



// Управление каналами

/// Добавить новый канал в БД
QList<Channel> Database::addChannel(const Channel &channel)
{
    QString queryStr = "INSERT INTO `channels` (`name`,`position`,`duration`,`tvg_id`,`tvg_name`,`tvg_logo`,`tvg_epg`,`tvg_shift`,"
        "`group`,`track`,`radio`,`aspect`,`crop`,`recordable`,`censored`,`age_restrict`,`mono`,"
        "`name_as_key`,`url_m3u`,`url`,`user_agent`,`http_referrer`) VALUES ('%1',%2,%3,'%4','%5','%6','%7',%8,%9,%10,"
        "%11,'%12','%13',%14,%15,%16,%17,%18,'%19','%20','%21','%22');";
    queryStr = queryStr.arg(channel.getName(), QString::number(channel.getPosition()), QString::number(channel.getDuration()));
    queryStr = queryStr.arg(channel.getTvgId(), channel.getTvgName(), channel.getTvgLogo());
    queryStr = queryStr.arg(channel.getTvgEpg(), QString::number(channel.getTvgShift()), QString::number(channel.getGroupUid()));
    queryStr = queryStr.arg(QString::number(channel.getAudioTrackUid()), channel.isRadio() ? "1" : "0", channel.getAspectRatio());
    queryStr = queryStr.arg(channel.getCrop(), channel.isRecordable() ? "1" : "0", channel.isCensored() ? "1" : "0");
    queryStr = queryStr.arg(channel.isAgeRestricted() ? "1" : "0", channel.isMono() ? "1" : "0", channel.isNameAsKey() ? "1" : "0");
    queryStr = queryStr.arg(channel.getUrlM3u(), channel.getUrl(), channel.getUserAgent(), channel.getHttpReferrer());

    bool res = query->exec(queryStr);
    if (!res)
    {
        logger->error(sdb.lastError().text(), unitName);
    }

    QList<Channel> result = getChannels();

    return result;
}


/// Получить список каналов из БД
QList<Channel> Database::getChannels()
{
    QList<Channel> result;
    QString queryStr =
    "SELECT `ch`.`id` `base_id`, `name`,`position`,`duration`,`tvg_id`,`tvg_name`,`tvg_logo`,`tvg_epg`,`tvg_shift`,"
        "`gr`.`group` `group`,`tr`.`track` `track`,`radio`,`aspect`,`crop`,`recordable`,`censored`,`age_restrict`,`mono`,"
        "`name_as_key`,`url_m3u`,`url`,`user_agent`,`http_referrer` "
    "FROM `channels` ch "
        "INNER JOIN `groups` gr on `ch`.`group` = `gr`.`id` "
        "INNER JOIN `tracks` tr on `ch`.`track` = `tr`.`id` "
    "ORDER BY `gr`.`group`, `ch`.`name`;";

    bool res = query->exec(queryStr);
    if (!res)
    {
        logger->error(sdb.lastError().text(), unitName);
    }
    else
    {
        QSqlRecord rec = query->record();
        while (query->next())
        {
            Channel channel;
            channel.setBaseId(query->value(rec.indexOf("base_id")).toInt());
            channel.setName(query->value(rec.indexOf("name")).toString());
            channel.setPosition(query->value(rec.indexOf("position")).toInt());
            channel.setDuration(query->value(rec.indexOf("duration")).toInt());
            channel.setTvgId(query->value(rec.indexOf("tvg_id")).toString());
            channel.setTvgName(query->value(rec.indexOf("tvg_name")).toString());
            channel.setTvgLogo(query->value(rec.indexOf("tvg_logo")).toString());
            channel.setTvgEpg(query->value(rec.indexOf("tvg_epg")).toString());
            channel.setTvgShift(query->value(rec.indexOf("tvg_shift")).toInt());
            channel.setGroupName(query->value(rec.indexOf("group")).toString());
            channel.setAudioTrack(query->value(rec.indexOf("track")).toString());
            channel.setRadio(query->value(rec.indexOf("radio")).toInt());
            channel.setAspectRatio(query->value(rec.indexOf("aspect")).toString());
            channel.setCrop(query->value(rec.indexOf("crop")).toString());
            channel.setRecordable(query->value(rec.indexOf("recordable")).toInt());
            channel.setCensored(query->value(rec.indexOf("censored")).toInt());
            channel.setAgeRestricted(query->value(rec.indexOf("age_restrict")).toInt());
            channel.setMono(query->value(rec.indexOf("mono")).toInt());
            channel.setNameAsKey(query->value(rec.indexOf("name_as_key")).toInt());
            channel.setUrlM3u(query->value(rec.indexOf("url_m3u")).toString());
            channel.setUrl(query->value(rec.indexOf("url")).toString());
            channel.setUserAgent(query->value(rec.indexOf("user_agent")).toString());
            channel.setHttpReferrer(query->value(rec.indexOf("http_referrer")).toString());

            result.append(channel);
        }
    }

    return result;
}


/// Получить список каналов по наименованию
QList<Channel> Database::getChannels(const QString &chName)
{
    QList<Channel> result;
    QString queryStr =
    "SELECT `ch`.`id` `base_id`,`name`,`position`,`duration`,`tvg_id`,`tvg_name`,`tvg_logo`,`tvg_epg`,`tvg_shift`,"
        "`gr`.`group` `group`,`tr`.`track` `track`,`radio`,`aspect`,`crop`,`recordable`,`censored`,`age_restrict`,`mono`,"
        "`name_as_key`,`url_m3u`,`url`,`user_agent`,`http_referrer` "
    "FROM `channels` ch "
        "INNER JOIN `groups` gr on `ch`.`group` = `gr`.`id` "
        "INNER JOIN `tracks` tr on `ch`.`track` = `tr`.`id` "
    "WHERE `ch`.`name` = '%1' "
    "ORDER BY `ch`.`name`;";
    queryStr = queryStr.arg(chName);

    bool res = query->exec(queryStr);
    if (!res)
    {
        logger->error(sdb.lastError().text(), unitName);
    }
    else
    {
        QSqlRecord rec = query->record();
        while (query->next())
        {
            Channel channel;
            channel.setBaseId(query->value(rec.indexOf("base_id")).toInt());
            channel.setName(query->value(rec.indexOf("name")).toString());
            channel.setPosition(query->value(rec.indexOf("position")).toInt());
            channel.setDuration(query->value(rec.indexOf("duration")).toInt());
            channel.setTvgId(query->value(rec.indexOf("tvg_id")).toString());
            channel.setTvgName(query->value(rec.indexOf("tvg_name")).toString());
            channel.setTvgLogo(query->value(rec.indexOf("tvg_logo")).toString());
            channel.setTvgEpg(query->value(rec.indexOf("tvg_epg")).toString());
            channel.setTvgShift(query->value(rec.indexOf("tvg_shift")).toInt());
            channel.setGroupName(query->value(rec.indexOf("group")).toString());
            channel.setAudioTrack(query->value(rec.indexOf("track")).toString());
            channel.setRadio(query->value(rec.indexOf("radio")).toInt());
            channel.setAspectRatio(query->value(rec.indexOf("aspect")).toString());
            channel.setCrop(query->value(rec.indexOf("crop")).toString());
            channel.setRecordable(query->value(rec.indexOf("recordable")).toInt());
            channel.setCensored(query->value(rec.indexOf("censored")).toInt());
            channel.setAgeRestricted(query->value(rec.indexOf("age_restrict")).toInt());
            channel.setMono(query->value(rec.indexOf("mono")).toInt());
            channel.setNameAsKey(query->value(rec.indexOf("name_as_key")).toInt());
            channel.setUrlM3u(query->value(rec.indexOf("url_m3u")).toString());
            channel.setUrl(query->value(rec.indexOf("url")).toString());
            channel.setUserAgent(query->value(rec.indexOf("user_agent")).toString());
            channel.setHttpReferrer(query->value(rec.indexOf("http_referrer")).toString());

            result.append(channel);
        }
    }

    return result;
}


/// Получить список каналов по идентификатору списка воспроизведения
QList<Channel> Database::getChannels(int plId)
{
    QList<Channel> result;
    QString queryStr =
            "SELECT `ch`.* "
            "FROM `relations` `rel` "
                "JOIN `channels` `ch` on `rel`.`channel`=`ch`.`id` "
                "JOIN `playlists` `pl` on `rel`.`playlist`=`pl`.`id` AND `pl`.`id`=%1;";
    queryStr = queryStr.arg(plId);

    if (!query->exec(queryStr))
    {
        logger->error(sdb.lastError().text(), unitName);
    }
    else
    {
        QSqlRecord rec = query->record();
        while (query->next())
        {
            Channel channel;
            channel.setBaseId(query->value(rec.indexOf("base_id")).toInt());
            channel.setName(query->value(rec.indexOf("name")).toString());
            channel.setPosition(query->value(rec.indexOf("position")).toInt());
            channel.setDuration(query->value(rec.indexOf("duration")).toInt());
            channel.setTvgId(query->value(rec.indexOf("tvg_id")).toString());
            channel.setTvgName(query->value(rec.indexOf("tvg_name")).toString());
            channel.setTvgLogo(query->value(rec.indexOf("tvg_logo")).toString());
            channel.setTvgEpg(query->value(rec.indexOf("tvg_epg")).toString());
            channel.setTvgShift(query->value(rec.indexOf("tvg_shift")).toInt());
            channel.setGroupName(query->value(rec.indexOf("group")).toString());
            channel.setAudioTrack(query->value(rec.indexOf("track")).toString());
            channel.setRadio(query->value(rec.indexOf("radio")).toInt());
            channel.setAspectRatio(query->value(rec.indexOf("aspect")).toString());
            channel.setCrop(query->value(rec.indexOf("crop")).toString());
            channel.setRecordable(query->value(rec.indexOf("recordable")).toInt());
            channel.setCensored(query->value(rec.indexOf("censored")).toInt());
            channel.setAgeRestricted(query->value(rec.indexOf("age_restrict")).toInt());
            channel.setMono(query->value(rec.indexOf("mono")).toInt());
            channel.setNameAsKey(query->value(rec.indexOf("name_as_key")).toInt());
            channel.setUrlM3u(query->value(rec.indexOf("url_m3u")).toString());
            channel.setUrl(query->value(rec.indexOf("url")).toString());
            channel.setUserAgent(query->value(rec.indexOf("user_agent")).toString());
            channel.setHttpReferrer(query->value(rec.indexOf("http_referrer")).toString());

            result.append(channel);
        }
    }

    return result;
}


/// Получить канал по идентификатору
Channel Database::getChannel(int chId)
{
    Channel channel;
    QString queryStr =
                "SELECT `ch`.`id` `base_id`,`name`,`position`,`duration`,`tvg_id`,`tvg_name`,`tvg_logo`,`tvg_epg`,`tvg_shift`,"
                    "`gr`.`group` `group`,`tr`.`track` `track`,`radio`,`aspect`,`crop`,`recordable`,`censored`,`age_restrict`,`mono`,"
                    "`name_as_key`,`url_m3u`,`url`,`user_agent`,`http_referrer` "
                "FROM `channels` ch "
                    "INNER JOIN `groups` gr on `ch`.`group` = `gr`.`id` "
                    "INNER JOIN `tracks` tr on `ch`.`track` = `tr`.`id` "
                "WHERE `ch`.`id` = '%1' "
                "ORDER BY `ch`.`name`;";
    queryStr = queryStr.arg(chId);

    bool res = query->exec(queryStr);
    if (!res)
    {
        logger->error(sdb.lastError().text(), unitName);
    }
    else
    {
        QSqlRecord rec = query->record();

        while (query->next())
        {
            channel.setBaseId(query->value(rec.indexOf("base_id")).toInt());
            channel.setName(query->value(rec.indexOf("name")).toString());
            channel.setPosition(query->value(rec.indexOf("position")).toInt());
            channel.setDuration(query->value(rec.indexOf("duration")).toInt());
            channel.setTvgId(query->value(rec.indexOf("tvg_id")).toString());
            channel.setTvgName(query->value(rec.indexOf("tvg_name")).toString());
            channel.setTvgLogo(query->value(rec.indexOf("tvg_logo")).toString());
            channel.setTvgEpg(query->value(rec.indexOf("tvg_epg")).toString());
            channel.setTvgShift(query->value(rec.indexOf("tvg_shift")).toInt());
            channel.setGroupName(query->value(rec.indexOf("group")).toString());
            channel.setAudioTrack(query->value(rec.indexOf("track")).toString());
            channel.setRadio(query->value(rec.indexOf("radio")).toInt());
            channel.setAspectRatio(query->value(rec.indexOf("aspect")).toString());
            channel.setCrop(query->value(rec.indexOf("crop")).toString());
            channel.setRecordable(query->value(rec.indexOf("recordable")).toInt());
            channel.setCensored(query->value(rec.indexOf("censored")).toInt());
            channel.setAgeRestricted(query->value(rec.indexOf("age_restrict")).toInt());
            channel.setMono(query->value(rec.indexOf("mono")).toInt());
            channel.setNameAsKey(query->value(rec.indexOf("name_as_key")).toInt());
            channel.setUrlM3u(query->value(rec.indexOf("url_m3u")).toString());
            channel.setUrl(query->value(rec.indexOf("url")).toString());
            channel.setUserAgent(query->value(rec.indexOf("user_agent")).toString());
            channel.setHttpReferrer(query->value(rec.indexOf("http_referrer")).toString());
        }
    }

    return channel;
}



/// Удалить канал из БД по идентификатору
bool Database::removeChannel(int uid)
{
    bool result = false;
    if (uid>0)
    {
        QString queryStr = "DELETE FROM `channels` WHERE `id`='%1';";
        queryStr = queryStr.arg(uid);
        result = query->exec(queryStr);

        if (!result)
        {
            logger->error(sdb.lastError().text(), unitName);
        }
    }

    return result;
}



/// Удалить все каналы из БД
bool Database::clearChannels()
{
    bool result = false;

    QString queryStr = "DELETE FROM `channels';";
    result = query->exec(queryStr);

    if (!result)
    {
        logger->error(sdb.lastError().text(), unitName);
    }

    return result;
}


// Управление списками

/// Добавить новый список воспроизведения
QList<Playlist> Database::addPlaylist(const Playlist &playlist)
{
    QString queryStr =
        "INSERT INTO `playlists` (`name`,`url_tvg`,`tvg_shift`,`cache`,`deinterlace`,`aspect`,`crop`,`refresh`,`autoload`) "
        "VALUES ('%1','%2',%3,%4,%5,'%6','%7',%8,%9); ";
    queryStr = queryStr.arg(playlist.getName(), playlist.getUrlTvg(), QString::number(playlist.getTvgShift()));
    queryStr = queryStr.arg(QString::number(playlist.getCache()), QString::number(playlist.getDeinterlace()), playlist.getAspectRatio());
    queryStr = queryStr.arg(playlist.getCrop(), QString::number(playlist.getRefreshPeriod()), playlist.isAutoload() ? "1" : "0");

    bool res = query->exec(queryStr);
    if (!res)
    {
        logger->error(sdb.lastError().text(), unitName);
    }

    QList<Playlist> result = getPlaylists();
    return result;
}


/// Получит список списков воспроизведения
QList<Playlist> Database::getPlaylists()
{
    QList<Playlist> result;
    QString queryStr = "SELECT * FROM `playlists` ORDER BY `name`;";
    bool res = query->exec(queryStr);
    if (!res)
    {
        logger->error(sdb.lastError().text(), unitName);
    }
    else
    {
        QSqlRecord rec = query->record();

        while (query->next())
        {
            Playlist playlist;
            playlist.setBaseId(query->value(rec.indexOf("id")).toInt());
            playlist.setName(query->value(rec.indexOf("name")).toString());
            playlist.setUrlTvg(query->value(rec.indexOf("url_tvg")).toString());
            playlist.setTvgShift(query->value(rec.indexOf("tvg_shift")).toInt());
            playlist.setCache(query->value(rec.indexOf("cache")).toInt());
            playlist.setDeinterlace(query->value(rec.indexOf("deinterlace")).toInt());
            playlist.setAspectRatio(query->value(rec.indexOf("aspect")).toString());
            playlist.setCrop(query->value(rec.indexOf("crop")).toString());
            playlist.setRefreshPeriod(query->value(rec.indexOf("refresh")).toInt());
            playlist.setAutoload(query->value(rec.indexOf("autoload")).toInt());

            result.append(playlist);
        }
    }

    return result;
}


/// Получить список списков востпроизведения по наименованию
QList<Playlist> Database::getPlaylist(const QString &plName)
{
    QList<Playlist> result;
    QString queryStr = "SELECT * FROM `playlists` ORDER BY `id` WHERE `name`='%1';";
    queryStr = queryStr.arg(plName);

    bool res = query->exec(queryStr);
    if (!res)
    {
        logger->error(sdb.lastError().text(), unitName);
    }
    else
    {
        QSqlRecord rec = query->record();

        while (query->next())
        {
            Playlist playlist;
            playlist.setBaseId(query->value(rec.indexOf("id")).toInt());
            playlist.setName(query->value(rec.indexOf("name")).toString());
            playlist.setUrlTvg(query->value(rec.indexOf("url_tvg")).toString());
            playlist.setTvgShift(query->value(rec.indexOf("tvg_shift")).toInt());
            playlist.setCache(query->value(rec.indexOf("cache")).toInt());
            playlist.setDeinterlace(query->value(rec.indexOf("deinterlace")).toInt());
            playlist.setAspectRatio(query->value(rec.indexOf("aspect")).toString());
            playlist.setCrop(query->value(rec.indexOf("crop")).toString());
            playlist.setRefreshPeriod(query->value(rec.indexOf("refresh")).toInt());
            playlist.setAutoload(query->value(rec.indexOf("autoload")).toInt());

            result.append(playlist);
        }
    }

    return result;
}


/// Получить список воспроизведения по идентификатору
Playlist Database::getPlaylist(int uid)
{
    Playlist playlist;
    QString queryStr = "SELECT * FROM `playlists` WHERE `id`='%1';";
    queryStr = queryStr.arg(uid);

    bool res = query->exec(queryStr);
    if (!res)
    {
        logger->error(sdb.lastError().text(), unitName);
    }
    else
    {
        QSqlRecord rec = query->record();

        while (query->next())
        {
            playlist.setBaseId(query->value(rec.indexOf("id")).toInt());
            playlist.setName(query->value(rec.indexOf("name")).toString());
            playlist.setUrlTvg(query->value(rec.indexOf("url_tvg")).toString());
            playlist.setTvgShift(query->value(rec.indexOf("tvg_shift")).toInt());
            playlist.setCache(query->value(rec.indexOf("cache")).toInt());
            playlist.setDeinterlace(query->value(rec.indexOf("deinterlace")).toInt());
            playlist.setAspectRatio(query->value(rec.indexOf("aspect")).toString());
            playlist.setCrop(query->value(rec.indexOf("crop")).toString());
            playlist.setRefreshPeriod(query->value(rec.indexOf("refresh")).toInt());
            playlist.setAutoload(query->value(rec.indexOf("autoload")).toInt());
        }
    }

    return playlist;
}


/// Удалить список воспроизведения по идентификтору
bool Database::removePlaylist(int uid)
{
    bool result = false;
    if (uid>0)
    {
        QString queryStr = "DELETE FROM `playlists` WHERE `id`='%1';";
        queryStr = queryStr.arg(uid);
        result = query->exec(queryStr);

        if (!result)
        {
            logger->error(sdb.lastError().text(), unitName);
        }
    }

    return result;
}


/// Удалить все списки вопроизведения из БД
bool Database::clearPlaylists()
{
    bool result = false;

    QString queryStr = "DELETE FROM `playlists`;";
    result = query->exec(queryStr);

    if (!result)
    {
        logger->error(sdb.lastError().text(), unitName);
    }

    return result;
}



// Управление соответствием списков воспроизведения и каналов

/// Установить соответствие списка воспроизведения и канала
bool Database::setRelation(int plId, int chId, int chPos)
{
    bool result = false;

    // Проверить, есть ли уже соответствие канала плейлисту
    QString queryStr = "SELECT count(*) FROM `relations` WHERE playlist=%1 AND channel=%1;";
    queryStr = queryStr.arg(plId, chId);

    if (!query->exec())
    {
        logger->error(sdb.lastError().text(), unitName);
    }
    else
    {
        query->first();
        bool related = query->value(0).toInt() > 0;
        if (!related)
        {
            // Если нет, установить
            queryStr = "INSERT INTO `relations` (`playlist`, `channel`, `position`) VALUES (%1, %2, %3);";
            queryStr = queryStr.arg(plId, chId, chPos);

            related = query->exec(queryStr);
            if (!related)
            {
                logger->error(sdb.lastError().text(), unitName);
            }
        }
    }

    return result;
}


/// Удалить соответствие списка воспроизведения и канала
bool Database::clearRelation(int plId, int chId)
{
    bool result = false;

    // Проверить, есть ли уже соответствие канала плейлисту
    QString queryStr = "SELECT count(*) FROM `relations` WHERE playlist=%1 AND channel=%1;";
    queryStr = queryStr.arg(plId, chId);

    if (!query->exec())
    {
        logger->error(sdb.lastError().text(), unitName);
    }
    else
    {
        query->first();
        bool related = query->value(0).toInt() > 0;
        if (related)
        {
            // Если нет, установить
            queryStr = "DELETE FROM `relations` WHERE `playlist`=%1 AND `channel`=%2;";
            queryStr = queryStr.arg(plId, chId);

            related = query->exec(queryStr);
            if (!related)
            {
                logger->error(sdb.lastError().text(), unitName);
            }
        }
    }

    return result;
}


/// Получить идентификатор последней вставленной записи в БД
int Database::getLastId()
{
    int result = -1;

    QString queryStr = "SELECT last_insert_rowid() `last_id`;";

    bool res = query->exec(queryStr);
    if (!res)
    {
        logger->error(sdb.lastError().text(), unitName);
    }
    else
    {
        QSqlRecord rec = query->record();

        while (query->next())
        {
            result = query->value(rec.indexOf("last_id")).toInt();
        }
    }

    return result;
}


