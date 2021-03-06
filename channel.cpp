#include "channel.h"

Channel::Channel()
{
    baseId = 0;
    name = "";
    position = 1;
    duration = 0;
    tvgId = "";
    tvgName = "";
    tvgLogo = "";
    tvgEpg = "";
    tvgShift = 0;
    groupName = "";
    groupUid = 0;
    audioTrack = "RU";
    audioTrackUid = 0;
    radio = false;
    aspect = "16:9";
    crop = "";
    recordable = false;
    censored = false;
    ageRectricted = false;
    mono = false;
    nameAsKey = false;
    urlM3u = "";
    url = "";
    userAgent = "";
    httpReferrer = "";
}


/// Установить уникальный идентификатор канала
void Channel::setBaseId(int uid)
{
    this->baseId = uid;
}


/// Получить уникальный идентификатор канала
int Channel::getBaseId() const
{
    return this->baseId;
}


/// Наименование канала
void Channel::setName(const QString &n)
{
    if (n.size()>0)
        this->name = n;
}

QString Channel::getName() const
{
    return this->name;
}


/// Номер позиции
void Channel::setPosition(int pos)
{
    if (pos>0)
        this->position = pos;
}

int Channel::getPosition() const
{
    return this->position;
}


/// Длительность трека
void Channel::setDuration(int dur)
{
    this->duration = dur;
}

int Channel::getDuration() const
{
    return this->duration;
}


/// tvg_id
void Channel::setTvgId(const QString &id)
{
    this->tvgId = id;
}

QString Channel::getTvgId() const
{
    return this->tvgId;
}

/// tvg_name
void Channel::setTvgName(const QString &n)
{
    this->tvgName = n;
}

QString Channel::getTvgName() const
{
    return this->tvgName;
}


/// tvg_logo
void Channel::setTvgLogo(const QString &logo)
{
    this->tvgLogo = logo;
}

QString Channel::getTvgLogo() const
{
    return this->tvgLogo;
}


/// tvg_epg
void Channel::setTvgEpg(const QString &epg)
{
    this->tvgEpg = epg;
}

QString Channel::getTvgEpg() const
{
    return this->tvgEpg;
}


/// tvg_shift
void Channel::setTvgShift(int shift)
{
    this->tvgShift = shift;
}

int Channel::getTvgShift() const
{
    return this->tvgShift;
}


/// group_name
void Channel::setGroupName(const QString &group)
{
    this->groupName = group;
}

QString Channel::getGroupName() const
{
    return this->groupName;
}


/// Внешний ключ на таблицу групп
void Channel::setGroupUid(int id)
{
    this->groupUid = id;
}

int Channel::getGroupUid() const
{
    return this->groupUid;
}


/// audiotrack
void Channel::setAudioTrack(const QString &track)
{
    this->audioTrack = track;
}

QString Channel::getAudioTrack() const
{
    return this->audioTrack;
}


/// Внешний ключ на таблицу звуковых дорожек
void Channel::setAudioTrackUid(int id)
{
    this->audioTrackUid = id;
}

int Channel::getAudioTrackUid() const
{
    return this->audioTrackUid;
}


/// radio
void Channel::setRadio(bool rad)
{
    this->radio = rad;
}

bool Channel::isRadio() const
{
    return this->radio;
}


/// aspect_ratio
void Channel::setAspectRatio(const QString &ratio)
{
    this->aspect = ratio;
}

QString Channel::getAspectRatio() const
{
    return this->aspect;
}


/// crop
void Channel::setCrop(const QString &cr)
{
    this->crop = cr;
}

QString Channel::getCrop() const
{
    return this->crop;
}


/// recordable
void Channel::setRecordable(bool rec)
{
    this->recordable = rec;
}

bool Channel::isRecordable() const
{
    return this->recordable;
}


/// censored
void Channel::setCensored(bool cense)
{
    this->censored = cense;
}

bool Channel::isCensored() const
{
    return this->censored;
}


/// age_restricted
void Channel::setAgeRestricted(bool restrict)
{
    this->ageRectricted = restrict;
}

bool Channel::isAgeRestricted() const
{
    return this->ageRectricted;
}


/// mono
void Channel::setMono(bool m)
{
    this->mono = m;
}

bool Channel::isMono() const
{
    return this->mono;
}


/// name_as_key
void Channel::setNameAsKey(bool key)
{
    this->nameAsKey = key;
}

bool Channel::isNameAsKey() const
{
    return this->nameAsKey;
}


/// url_m3u
void Channel::setUrlM3u(const QString &u)
{
    this->urlM3u = u;
}

QString Channel::getUrlM3u() const
{
    return this->urlM3u;
}


/// channel source
void Channel::setUrl(const QString &source)
{
    this->url = source;
}

QString Channel::getUrl() const
{
    return this->url;
}


/// vlc_opt user_agent
void Channel::setUserAgent(const QString &agent)
{
    this->userAgent = agent;
}

QString Channel::getUserAgent() const
{
    return this->userAgent;
}


/// vlc_opt http_referrer
void Channel::setHttpReferrer(const QString &referrer)
{
    this->httpReferrer = referrer;
}

QString Channel::getHttpReferrer() const
{
    return this->httpReferrer;
}


/// Перегрузка оператора присваиванию
const Channel &Channel::operator=(const Channel &channel)
{
    this->name = channel.name;
    this->position = channel.position;
    this->duration = channel.duration;
    this->tvgId = channel.tvgId;
    this->tvgName = channel.tvgName;
    this->tvgLogo = channel.tvgLogo;
    this->tvgEpg = channel.tvgEpg;
    this->tvgShift = channel.tvgShift;
    this->groupUid = channel.groupUid;
    this->groupName = channel.groupName;
    this->audioTrackUid = channel.audioTrackUid;
    this->audioTrack = channel.audioTrack;
    this->aspect = channel.aspect;
    this->crop = channel.crop;
    this->recordable = channel.recordable;
    this->censored = channel.censored;
    this->ageRectricted = channel.ageRectricted;
    this->mono = channel.mono;
    this->nameAsKey = channel.nameAsKey;
    this->urlM3u = channel.urlM3u;
    this->url = channel.url;
    this->userAgent = channel.userAgent;
    this->httpReferrer = channel.httpReferrer;

    return *this;
}


// /// Перегрузка оператора сравнения
//bool operator==(const Channel &left, const Channel &right)
//{
//    bool result = false;
//    result = left.getName() == right.getName();

//    return result;
//}
