#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QString>
#include <QList>
#include "channel.h"


class Playlist
{
public:
    Playlist();
    void setBaseId(int);
    int getBaseId() const;
    void setName(const QString &);
    QString getName() const;
    void setUrlTvg(const QString &);
    QString getUrlTvg() const;
    void setTvgShift(int);
    int getTvgShift() const;
    void setCache(int);
    int getCache() const;
    void setDeinterlace(int);
    int getDeinterlace() const;
    void setAspectRatio(const QString &);
    QString getAspectRatio() const;
    void setCrop(const QString &);
    QString getCrop() const;
    void setRefreshPeriod(int);
    int getRefreshPeriod() const;
    void setAutoload(bool);
    bool isAutoload() const;

private:
    int baseId;
    QString name;
    QString urlTvg;
    int tvgShift;
    int cache;
    int deinterlace;
    QString aspectRatio;
    QString crop;
    int refreshPeriod;
    bool autoload;
};

#endif // PLAYLIST_H
