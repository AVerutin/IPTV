#include "formplaylistedit.h"

FormPlaylistEdit::FormPlaylistEdit(QWidget *parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    title = "Новый список воспроизведения";
    this->createWidget();
}


/// Изменить параметры списка воспроизведения
Playlist FormPlaylistEdit::getPlaylist()
{
    return this->playlist;
}


/// Добавить новый список воспроизведения
void FormPlaylistEdit::editPlaylist(const Playlist& list)
{
    this->playlist = list;
    title = list.getName();

    // Установка начальных значений для виджетов окна
    leName->setText(this->playlist.getName());
    leShift->setText(QString::number(this->playlist.getTvgShift()));
    leUrlTvg->setText(this->playlist.getUrlTvg());
    cbAutoload->setChecked(this->playlist.isAutoload());
    leCache->setText(QString::number(this->playlist.getCache()));
    leDeinterlace->setText(QString::number(this->playlist.getDeinterlace()));
    leRefresh->setText(QString::number(this->playlist.getRefreshPeriod()));
    leAspect->setText(this->playlist.getAspectRatio());
    leCrop->setText(this->playlist.getCrop());
}


/// Создать главный виджет формы
void FormPlaylistEdit::createWidget()
{
    // Линия 1
    lblName = new QLabel("Имя");
    leName = new QLineEdit;
    lblName->setBuddy(leName);
    lblShift = new QLabel("Сдвиг");
    leShift = new QLineEdit;
    lblShift->setBuddy(leShift);
    leShift->setMaximumWidth(50);
    connect(leName, &QLineEdit::textChanged, this, &FormPlaylistEdit::changeName);

    line1 = new QHBoxLayout;
    line1->addWidget(lblName);
    line1->addWidget(leName);
    line1->addWidget(lblShift);
    line1->addWidget(leShift);

    // Линия 2
    lblUrlTvg = new QLabel("url-tvg");
    leUrlTvg = new QLineEdit;
    cbAutoload = new QCheckBox("Загружать автоматически");
    cbAutoload->setChecked(false);

    line2 = new QHBoxLayout;
    line2->addWidget(lblUrlTvg);
    line2->addWidget(leUrlTvg);
    line2->addWidget(cbAutoload);

    // Линия 3
    lblCache = new QLabel("Кеш");
    leCache = new QLineEdit;
    lblCache->setBuddy(leCache);
    lblDeinterlace = new QLabel("Деинтерлейс");
    leDeinterlace = new QLineEdit;
    lblDeinterlace->setBuddy(leDeinterlace);
    lblRefresh = new QLabel("Пер.обновл.");
    leRefresh = new QLineEdit;
    lblRefresh->setBuddy(leRefresh);

    line3 = new QHBoxLayout;
    line3->addWidget(lblCache);
    line3->addWidget(leCache);
    line3->addWidget(lblDeinterlace);
    line3->addWidget(leDeinterlace);
    line3->addWidget(lblRefresh);
    line3->addWidget(leRefresh);

    // Линия 4
    lblAspect = new QLabel("Соотн.сторон");
    leAspect = new QLineEdit;
    lblAspect->setBuddy(leAspect);
    lblCrop = new QLabel("Обрезка");
    leCrop = new QLineEdit;
    lblCrop->setBuddy(leCrop);

    line4 = new QHBoxLayout;
    line4->addWidget(lblAspect);
    line4->addWidget(leAspect);
    line4->addWidget(lblCrop);
    line4->addWidget(leCrop);

    // Линия 5
    pbOk = new QPushButton("OK");
    pbCancel = new QPushButton("Отмена");
    connect(pbOk, &QPushButton::clicked, this, &FormPlaylistEdit::slotAccept);
    connect(pbCancel, &QPushButton::clicked, this, &QDialog::reject);

    line5 = new QHBoxLayout;
    line5->addStretch();
    line5->addWidget(pbOk);
    line5->addWidget(pbCancel);
    line5->addStretch();

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(line1);
    mainLayout->addLayout(line2);
    mainLayout->addLayout(line3);
    mainLayout->addLayout(line4);
    mainLayout->addStretch();
    mainLayout->addLayout(line5);

    // Установка начальных значений для виджетов окна
    leName->setText("");
    leShift->setText("0");
    leUrlTvg->setText("");
    cbAutoload->setChecked(false);
    leCache->setText("0");
    leDeinterlace->setText("0");
    leRefresh->setText("0");
    leAspect->setText("16*9");
    leCrop->setText("");

    this->setLayout(mainLayout);
    this->setFixedSize(400, 170);
    this->setWindowTitle(title);
}


/// Реализация метода нажатия на кнопку OK
void FormPlaylistEdit::slotAccept()
{
    playlist.setName(leName->text().trimmed());
    playlist.setUrlTvg(leUrlTvg->text().trimmed());
    playlist.setTvgShift(leShift->text().toInt());
    playlist.setCache(leCache->text().toInt());
    playlist.setDeinterlace(leDeinterlace->text().toInt());
    playlist.setAspectRatio(leAspect->text().trimmed());
    playlist.setCrop(leCrop->text().trimmed());
    playlist.setRefreshPeriod(leRefresh->text().toInt());
    playlist.setAutoload(cbAutoload->isChecked());

    this->accept();
}


/// Реализация метода изменения наименования списка воспроизведения
void FormPlaylistEdit::changeName()
{
   QString tit = leName->text().trimmed();
   this->setWindowTitle(tit.isEmpty() ? title : tit);
}


