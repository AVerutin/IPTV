#ifndef TRACK_H
#define TRACK_H

#include <QString>

class Track
{
public:
    Track();
    void setId(int);
    int getId();
    void setName(const QString &);
    QString getName();

private:
    int uid;
    QString name;
};

#endif // TRACK_H
