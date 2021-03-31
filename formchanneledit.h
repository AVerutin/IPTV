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
    FormChannelEdit(const Channel&, bool);
    FormChannelEdit(const Channel&, const QList<Group>&, const QList<Track>&, bool);
    Channel getChannel();
    bool isChanged();
    void setGroups(const QList<Group>&);
    void setTracks(const QList<Track>&);
    void editChannel();
    void addChannel();

private:
    // Элементы формы
    QHBoxLayout *line1;
    QLabel *lblName;
    QLabel *lblDuration;
    QLabel *lblGroup;
    QLineEdit *leName;
    QLineEdit *leDuration;
    QComboBox *cbGroup;

    QHBoxLayout *line2;
    QLabel *lblTvgId;
    QLabel *lblTvgName;
    QLabel *lblTrack;
    QLineEdit *leTvgId;
    QLineEdit *leTvgName;
    QComboBox *cbTrack;

    QHBoxLayout *line3;
    QLabel *lblTvgEpg;
    QLabel *lblTvgShift;
    QLabel *lblTvgLogo;
    QLineEdit *leTvgEpg;
    QLineEdit *leTvgShift;
    QLineEdit *leTvgLogo;

    QHBoxLayout *line4;
    QLabel *lblAspect;
    QLabel *lblCrop;
    QLineEdit *leAspect;
    QLineEdit *leCrop;

    QHBoxLayout *line5;
    QLabel *lblUserAgent;
    QLabel *lblHttpReferrer;
    QLineEdit *leUserAgent;
    QLineEdit *leHttpReferrer;

    QHBoxLayout *line6;
    QLabel *lblUrlM3u;
    QLineEdit *leUrlM3u;

    QHBoxLayout *line7;
    QLabel *lblUrl;
    QLineEdit *leUrl;

    QHBoxLayout *line8;
    QCheckBox *cbRadio;
    QCheckBox *cbRecord;
    QCheckBox *cbMono;

    QHBoxLayout *line9;
    QCheckBox *cbCensored;
    QCheckBox *cbRestricted;
    QCheckBox *cbNameKey;

    QHBoxLayout *line10;
    QPushButton *pbOk;
    QPushButton *pbCancel;

    QVBoxLayout *mainLayout;

    Channel channel;
    QList<QString> groupsNames;
    QList<QString> tracksNames;
    bool editMode;
    bool changed;
    void createWidget();
    QString title;

private slots:
    void slotButtonOk();
    void slotChanged();
};

#endif // FORMCHANNELEDIT_H
