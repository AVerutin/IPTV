#include "formchanneledit.h"

FormChannelEdit::FormChannelEdit()
{
    editMode = false;
    changed = false;

    createWidget();
}

FormChannelEdit::FormChannelEdit(const Channel& ch, bool edit)
{
    editMode = edit;
    changed = false;
    this->channel = ch;

    createWidget();
}

FormChannelEdit::FormChannelEdit(const Channel& ch, const QList<Group> &groups, const QList<Track> &tracks, bool edit)
{
    editMode = edit;
    changed = false;
    this->channel = ch;

    createWidget();

    // Установка групп
    groupsNames.clear();
    cbGroup->clear();
    int groupItem = 0;

    for (int i=0; i<groups.count(); i++)
    {
        Group group = groups[i];
        this->groupsNames.append(group.getName());
        cbGroup->addItem(group.getName());

        // Установим текущую группу для канала
        if (channel.getGroupName() == group.getName())
            groupItem = i;
    }

    cbGroup->setCurrentIndex(groupItem);

    // Установка дорожек
    tracksNames.clear();
    cbTrack->clear();
    int trackItem = -1;
    int defTrack = 0;

    for (int i=0; i<tracks.count(); i++)
    {
        Track track = tracks[i];
        this->tracksNames.append(track.getName());
        cbTrack->addItem(track.getName());

        // Установим текущую дорожку
        if (channel.getAudioTrack() == track.getName())
            trackItem = i;

        if (track.getName() == "RU")
            defTrack = i;
    }

    cbTrack->setCurrentIndex(trackItem == -1 ? defTrack : trackItem);

    connect (cbGroup, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &FormChannelEdit::slotChanged);
    connect (cbTrack, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &FormChannelEdit::slotChanged);
}


/// Создать виджет формы
void FormChannelEdit::createWidget()
{
    // Линия 1
    lblName = new QLabel("Имя");
    leName = new QLineEdit;
    lblName->setBuddy(leName);
    connect (leName, &QLineEdit::textChanged, this, &FormChannelEdit::slotChanged);

    lblDuration = new QLabel(":");
    leDuration = new QLineEdit;
    leDuration->setMaximumWidth(50);
    lblDuration->setBuddy(leDuration);
    connect (leDuration, &QLineEdit::textChanged, this, &FormChannelEdit::slotChanged);

    lblGroup = new QLabel("Группа");
    cbGroup = new QComboBox;
    lblGroup->setBuddy(cbGroup);

    line1 = new QHBoxLayout;
    line1->addWidget(lblName);
    line1->addWidget(leName, 0, Qt::AlignLeft);
    line1->addWidget(lblDuration);
    line1->addWidget(leDuration, 1, Qt::AlignLeft);
    line1->addWidget(lblGroup);
    line1->addWidget(cbGroup, 0, Qt::AlignRight);

    // Линия 2
    lblTvgId = new QLabel("tvg-id");
    leTvgId = new QLineEdit;
    lblTvgId->setBuddy(leTvgId);
    connect (leTvgId, &QLineEdit::textChanged, this, &FormChannelEdit::slotChanged);

    lblTvgName = new QLabel("tvg-name");
    leTvgName = new QLineEdit;
    lblTvgName->setBuddy(leTvgName);
    connect (leTvgName, &QLineEdit::textChanged, this, &FormChannelEdit::slotChanged);

    lblTrack = new QLabel("Дорожка");
    cbTrack = new QComboBox;
    lblTrack->setBuddy(cbTrack);

    line2 = new QHBoxLayout;
    line2->addWidget(lblTvgId);
    line2->addWidget(leTvgId, 0, Qt::AlignLeft);
    line2->addWidget(lblTvgName);
    line2->addWidget(leTvgName, 0, Qt::AlignLeft);
    line2->addWidget(lblTrack);
    line2->addWidget(cbTrack, 1, Qt::AlignRight);

    // Линия 3
    lblTvgEpg = new QLabel("tvg-epg");
    leTvgEpg = new QLineEdit;
    lblTvgEpg->setBuddy(leTvgEpg);
    connect(leTvgEpg, &QLineEdit::textChanged, this, &FormChannelEdit::slotChanged);

    lblTvgShift = new QLabel("tvg-shift");
    leTvgShift = new QLineEdit;
    leTvgShift->setMaximumWidth(50);
    lblTvgShift->setBuddy(leTvgShift);
    connect(leTvgShift, &QLineEdit::textChanged, this, &FormChannelEdit::slotChanged);

    lblTvgLogo = new QLabel("tvg-logo");
    leTvgLogo = new QLineEdit;
    lblTvgLogo->setBuddy(leTvgLogo);
    connect(leTvgLogo, &QLineEdit::textChanged, this, &FormChannelEdit::slotChanged);

    line3 = new QHBoxLayout;
    line3->addWidget(lblTvgEpg);
    line3->addWidget(leTvgEpg, 0, Qt::AlignLeft);
    line3->addWidget(lblTvgShift);
    line3->addWidget(leTvgShift, 1, Qt::AlignLeft);
    line3->addWidget(lblTvgLogo);
    line3->addWidget(leTvgLogo, 0, Qt::AlignRight);

    // Линия 4
    lblAspect = new QLabel("Соотношение");
    leAspect = new QLineEdit;
    lblAspect->setBuddy(leAspect);
    connect(leAspect, &QLineEdit::textChanged, this, &FormChannelEdit::slotChanged);

    lblCrop = new QLabel("Обрезка");
    leCrop = new QLineEdit;
    lblCrop->setBuddy(leCrop);
    connect(leCrop, &QLineEdit::textChanged, this, &FormChannelEdit::slotChanged);

    line4 = new QHBoxLayout;
    line4->addWidget(lblAspect);
    line4->addWidget(leAspect, 0, Qt::AlignLeft);
    line4->addStretch();
    line4->addWidget(lblCrop);
    line4->addWidget(leCrop, 0, Qt::AlignRight);

    // Линия 5
    lblUserAgent = new QLabel("User agent");
    leUserAgent = new QLineEdit;
    leUserAgent->setMinimumWidth(150);
    lblUserAgent->setBuddy(leUserAgent);
    connect(leUserAgent, &QLineEdit::textChanged, this, &FormChannelEdit::slotChanged);

    lblHttpReferrer = new QLabel("HTTP referrer");
    leHttpReferrer = new QLineEdit;
    leHttpReferrer->setMinimumWidth(150);
    lblHttpReferrer->setBuddy(leHttpReferrer);
    connect(leHttpReferrer, &QLineEdit::textChanged, this, &FormChannelEdit::slotChanged);

    line5 = new QHBoxLayout;
    line5->addWidget(lblUserAgent);
    line5->addWidget(leUserAgent, 0, Qt::AlignLeft);
    line5->addStretch();
    line5->addWidget(lblHttpReferrer);
    line5->addWidget(leHttpReferrer, 0, Qt::AlignRight);

    // Линия 6
    lblUrlM3u = new QLabel("Ссылка M3U");
    leUrlM3u = new QLineEdit;
    // leUrlM3u->setMinimumWidth(150);
    lblUrlM3u->setBuddy(leUrlM3u);
    connect(leUrlM3u, &QLineEdit::textChanged, this, &FormChannelEdit::slotChanged);

    line6 = new QHBoxLayout;
    line6->addWidget(lblUrlM3u);
    line6->addWidget(leUrlM3u);

    // Линия 7
    lblUrl = new QLabel("Источник");
    leUrl = new QLineEdit;
    // leUrl->setMinimumWidth(150);
    lblUrl->setBuddy(leUrl);
    connect(leUrl, &QLineEdit::textChanged, this, &FormChannelEdit::slotChanged);

    line7 = new QHBoxLayout;
    line7->addWidget(lblUrl);
    line7->addWidget(leUrl);

    // Линия 8
    cbRadio = new QCheckBox("Радио");
    cbRecord = new QCheckBox("Архив передач");
    cbMono = new QCheckBox("Моно");
    connect(cbRadio, &QCheckBox::stateChanged, this, &FormChannelEdit::slotChanged);
    connect(cbRecord, &QCheckBox::stateChanged, this, &FormChannelEdit::slotChanged);
    connect(cbMono, &QCheckBox::stateChanged, this, &FormChannelEdit::slotChanged);

    line8 = new QHBoxLayout;
    line8->addWidget(cbRadio);
    line8->addWidget(cbRecord);
    line8->addWidget(cbMono);

    // Линия 9
    cbCensored = new QCheckBox("Для взрослых");
    cbRestricted = new QCheckBox("Возрастное ограничение");
    cbNameKey = new QCheckBox("ТВ-программа по имени");

    line9 = new QHBoxLayout;
    line9->addWidget(cbCensored);
    line9->addWidget(cbRestricted);
    line9->addWidget(cbNameKey);

    // Линия 10
    pbOk = new QPushButton("OK");
    pbCancel = new QPushButton("Отмена");
    connect(pbOk, &QPushButton::clicked, this, &FormChannelEdit::slotButtonOk);
    connect(pbOk, &QPushButton::clicked, this, &FormChannelEdit::accept);
    connect(pbCancel, &QPushButton::clicked, this, &FormChannelEdit::reject);

    line10 = new QHBoxLayout;
    line10->addStretch();
    line10->addWidget(pbOk);
    line10->addWidget(pbCancel);
    line10->addStretch();


    if (editMode)
    {
        // Устанавлиываем начальные значения для полей формы
        leName->setText(channel.getName());
        leDuration->setText(QString::number(channel.getDuration()));
        leTvgId->setText(channel.getTvgId());
        leTvgName->setText(channel.getTvgName());
        leTvgEpg->setText(channel.getTvgEpg());
        leTvgShift->setText(QString::number(channel.getTvgShift()));
        leTvgLogo->setText(channel.getTvgLogo());
        leAspect->setText(channel.getAspectRatio());
        leCrop->setText(channel.getCrop());
        leUserAgent->setText(channel.getUserAgent());
        leHttpReferrer->setText(channel.getHttpReferrer());
        leUrlM3u->setText(channel.getUrlM3u());
        leUrl->setText(channel.getUrl());
        cbRadio->setChecked(channel.isRadio());
        cbRecord->setChecked(channel.isRecordable());
        cbMono->setChecked(channel.isMono());
        cbCensored->setChecked(channel.isCensored());
        cbRestricted->setChecked(channel.isAgeRestricted());
        cbNameKey->setChecked(channel.isNameAsKey());
        title = channel.getName();
    }
    else
    {
        // Очищаем значения для полей формы
        leName->setText("");
        leDuration->setText("0");
        leTvgId->setText("");
        leTvgName->setText("");
        leTvgEpg->setText("");
        leTvgShift->setText("0");
        leTvgLogo->setText("");
        leAspect->setText("0x0");
        leCrop->setText("0+0+0*0");
        leUserAgent->setText("");
        leHttpReferrer->setText("");
        leUrlM3u->setText("");
        leUrl->setText("");
        cbRadio->setChecked(false);
        cbRecord->setChecked(false);
        cbMono->setChecked(false);
        cbCensored->setChecked(false);
        cbRestricted->setChecked(false);
        cbNameKey->setChecked(false);
        title = "Новый канал";
    }

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(line1);
    mainLayout->addLayout(line7);
    mainLayout->addLayout(line2);
    mainLayout->addLayout(line3);
    mainLayout->addLayout(line4);
    mainLayout->addLayout(line5);
    mainLayout->addLayout(line6);
    mainLayout->addStretch();
    mainLayout->addLayout(line8);
    mainLayout->addLayout(line9);
    mainLayout->addStretch();
    mainLayout->addLayout(line10);

    this->setLayout(mainLayout);
    this->setFixedSize(470, 300);
    this->setWindowTitle(title);
}


/// Обработчик нажатия на кнопку ОК
void FormChannelEdit::slotButtonOk()
{
    if (changed)
    {
        channel.setName(leName->text().trimmed());
        channel.setDuration(leDuration->text().toInt());
        channel.setTvgId(leTvgId->text().trimmed());
        channel.setTvgName(leTvgName->text().trimmed());
        channel.setTvgLogo(leTvgLogo->text().trimmed());
        channel.setTvgEpg(leTvgEpg->text().trimmed());
        channel.setTvgShift(leTvgShift->text().toInt());
        channel.setGroupName(cbGroup->currentText().trimmed());
        channel.setAudioTrack(cbTrack->currentText().trimmed());
        channel.setAspectRatio(leAspect->text().trimmed());
        channel.setCrop(leCrop->text().trimmed());
        channel.setRadio(cbRadio->isChecked());
        channel.setRecordable(cbRecord->isChecked());
        channel.setMono(cbMono->isChecked());
        channel.setCensored(cbCensored->isChecked());
        channel.setAgeRestricted(cbRestricted->isChecked());
        channel.setNameAsKey(cbNameKey->isChecked());
        channel.setUrlM3u(leUrlM3u->text().trimmed());
        channel.setUrl(leUrl->text().trimmed());
        channel.setUserAgent(leUserAgent->text().trimmed());
        channel.setHttpReferrer(leHttpReferrer->text().trimmed());

        changed = true;
    }

    this->close();
}

/// Обработчик изменения параметров канала
void FormChannelEdit::slotChanged()
{
    this->changed = true;
    this->setWindowTitle("*" + title);
}


/// Получить созданный, или отредактированный канал
Channel FormChannelEdit::getChannel()
{
    return this->channel;
}


/// Получить признак модификации параметров канала
bool FormChannelEdit::isChanged()
{
    return this->changed;
}


/// Установить список групп
void FormChannelEdit::setGroups(const QList<Group> &groups)
{
    groupsNames.clear();
    cbGroup->clear();
    int groupItem = 0;

    for (int i=0; i<groups.count(); i++)
    {
        Group group = groups.at(i);
        this->groupsNames.append(group.getName());
        cbGroup->addItem(group.getName());

        // Установим текущую группу для канала
        if (channel.getGroupName() == group.getName())
            groupItem = i;
    }

    cbGroup->setCurrentIndex(groupItem);
    connect (cbGroup, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &FormChannelEdit::slotChanged);
}


/// Установить список звуковых дорожек
void FormChannelEdit::setTracks(const QList<Track> &tracks)
{
    tracksNames.clear();
    cbTrack->clear();
    int trackItem = -1;
    int defTrack = 0;

    for (int i=0; i<tracks.count(); i++)
    {
        Track track = tracks.at(i);
        this->tracksNames.append(track.getName());
        cbTrack->addItem(track.getName());

        // Установим текущую дорожку
        if (channel.getAudioTrack() == track.getName())
            trackItem = i;

        if (track.getName() == "RU")
            defTrack = i;
    }

    cbTrack->setCurrentIndex(trackItem == -1 ? defTrack : trackItem);
    connect (cbTrack, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &FormChannelEdit::slotChanged);
}


