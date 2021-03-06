#ifndef FORMMAIN_H
#define FORMMAIN_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtWidgets>

#include "parser.h"
#include "logger.h"
#include "database.h"
#include "formchannels.h"
#include "formplaylistedit.h"

class FormMain : public QMainWindow
{
    Q_OBJECT

public:
    FormMain(QWidget *parent = nullptr);
    ~FormMain();

private:
    void createWidget();
    void createMenu();

    // Элементы главного меню
    QMenuBar *mainMenu;
    QMenu *mnPlaylists;
    QMenu *mnChannels;
    QAction *aCreatePlaylist;
    QAction *aEditPlaylist;
    QAction *aRemovePlaylist;
    QAction *aExportPlaylist;
    QAction *aShowChannels;
    QAction *aImportChannels;
    QAction *aQuit;

    // Элементы главного виджета
    QTableWidget *twPlaylists;
    QPushButton *btnCreate;
    QPushButton *btnEdit;
    QPushButton *btnRemove;
    QPushButton *btnExport;
    QStatusBar *statBar;

    Parser *parser;
    Logger *logger;
    QString unitName;
    Database *sdb;
    FormChannels *formChannels;
    FormPlaylistEdit *editPlaylist;

private slots:
    void slotAppClose();
    void slotCreatePlaylist();
    void slotEditPlaylist();
    void slotExportPlaylist();
    void slotRemovePlaylist();
    void slotImportChannels();
    void slotShowChannels();
};
#endif // FORMMAIN_H
