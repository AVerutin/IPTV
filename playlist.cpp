#include "playlist.h"

Playlist::Playlist()
{
    name = "";
    urlTvg = "";
    tvgShift = 0;
    cache = 0;
    deinterlace = 0;
    aspectRatio = "16:9";
    crop = "";
    refreshPeriod = 0;
    autoload = false;
}


/// Установить уникальный идентификатор
void Playlist::setBaseId(int uid)
{
    this->baseId = uid;
}


/// Получить уникальный идентификатор
int Playlist::getBaseId() const
{
    return this->baseId;
}


/// Наименование списка воспроизведения
void Playlist::setName(const QString &nm)
{
    this->name = nm;
}

QString Playlist::getName() const
{
    return this->name;
}


/// Ссылка на программу передач
void Playlist::setUrlTvg(const QString &url)
{
    this->urlTvg = url;
}

QString Playlist::getUrlTvg() const
{
    return this->urlTvg;
}


/// Сдвиг времени в программе
void Playlist::setTvgShift(int shift)
{
    this->tvgShift = shift;
}

int Playlist::getTvgShift() const
{
    return this->tvgShift;
}


/// Размер кеша
void Playlist::setCache(int size)
{
    this->cache = size;
}

int Playlist::getCache() const
{
    return this->cache;
}


/// Режим черезстрочности
void Playlist::setDeinterlace(int interlace)
{
    this->deinterlace = interlace;
}

int Playlist::getDeinterlace() const
{
    return this->deinterlace;
}


/// Соотношение сторон
void Playlist::setAspectRatio(const QString &aspect)
{
    this->aspectRatio = aspect;
}

QString Playlist::getAspectRatio() const
{
    return this->aspectRatio;
}


/// Обрезка
void Playlist::setCrop(const QString &crp)
{
    this->crop = crp;
}

QString Playlist::getCrop() const
{
    return this->crop;
}


/// Период обновления
void Playlist::setRefreshPeriod(int period)
{
    this->refreshPeriod = period;
}

int Playlist::getRefreshPeriod() const
{
    return this->refreshPeriod;
}


/// Автозагрузка списка
void Playlist::setAutoload(bool load)
{
    this->autoload = load;
}

bool Playlist::isAutoload() const
{
    return this->autoload;
}

