#ifndef FORMMAIN_H
#define FORMMAIN_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtWidgets>

#include "parser.h"
#include "database.h"

class FormMain : public QMainWindow
{
    Q_OBJECT

public:
    FormMain(QWidget *parent = nullptr);
    ~FormMain();
private:
    void parseList();

    Parser *parser;
    Database *sdb;
};
#endif // FORMMAIN_H
