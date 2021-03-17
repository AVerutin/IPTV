#ifndef CHANNEL_H
#define CHANNEL_H

#include <QString>


class Channel
{
public:
    Channel();
    void setBaseId(int);
    int getBaseId() const;
    void setName(const QString &);
    QString getName() const;
    void setPosition(int);
    int getPosition() const;
    void setDuration(int);
    int getDuration() const;
    void setTvgId(const QString &);
    QString getTvgId() const ;
    void setTvgName(const QString &);
    QString getTvgName() const;
    void setTvgLogo(const QString &);
    QString getTvgLogo() const;
    void setTvgEpg(const QString &);
    QString getTvgEpg() const;
    void setTvgShift(int);
    int getTvgShift() const;
    void setGroupName(const QString &);
    QString getGroupName() const;
    void setAudioTrack(const QString &);
    QString getAudioTrack() const;
    void setRadio(bool);
    bool isRadio() const;
    void setAspectRatio(const QString &);
    QString getAspectRatio() const;
    void setCrop(const QString &);
    QString getCrop() const;
    void setRecordable(bool);
    bool isRecordable() const;
    void setCensored(bool);
    bool isCensored() const;
    void setAgeRestricted(bool);
    bool isAgeRestricted() const;
    void setMono(bool);
    bool isMono() const;
    void setNameAsKey(bool);
    bool isNameAsKey() const;
    void setUrlM3u(const QString &);
    QString getUrlM3u() const;
    void setUrl(const QString &);
    QString getUrl() const;
    void setUserAgent(const QString &);
    QString getUserAgent() const;
    void setHttpReferrer(const QString &);
    QString getHttpReferrer() const;

private:
    int baseId;
    QString name;
    int position;
    int duration;
    QString tvgId;
    QString tvgName;
    QString tvgLogo;
    QString tvgEpg;
    int tvgShift;
    QString groupName;
    QString audioTrack;
    bool radio;
    QString aspect;
    QString crop;
    bool recordable;
    bool censored;
    bool ageRectricted;
    bool mono;
    bool nameAsKey;
    QString urlM3u;
    QString url;
    QString userAgent;
    QString httpReferrer;
};

#endif // CHANNEL_H
