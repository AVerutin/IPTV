#ifndef FORMPLAYLISTEDIT_H
#define FORMPLAYLISTEDIT_H

#include <QDialog>
#include <QtWidgets>
#include "playlist.h"

class FormPlaylistEdit : public QDialog
{
    Q_OBJECT
public:
    explicit FormPlaylistEdit(QWidget *parent = nullptr);
    Playlist getPlaylist();
    void editPlaylist(const Playlist&);
private:
    QVBoxLayout *mainLayout;
    QHBoxLayout *line1;
    QLabel *lblName;
    QLabel *lblShift;
    QLineEdit *leName;
    QLineEdit *leShift;

    QHBoxLayout *line2;
    QLabel *lblUrlTvg;
    QCheckBox *cbAutoload;
    QLineEdit *leUrlTvg;

    QHBoxLayout *line3;
    QLabel *lblCache;
    QLabel *lblDeinterlace;
    QLabel *lblRefresh;
    QLineEdit *leCache;
    QLineEdit *leDeinterlace;
    QLineEdit *leRefresh;

    QHBoxLayout *line4;
    QLabel *lblAspect;
    QLabel *lblCrop;
    QLineEdit *leAspect;
    QLineEdit *leCrop;

    QHBoxLayout *line5;
    QPushButton *pbOk;
    QPushButton *pbCancel;

    Playlist playlist;
    QString title;

    void createWidget();

private slots:
    void slotAccept();
    void changeName();
};

#endif // FORMPLAYLISTEDIT_H
