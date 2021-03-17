#include "track.h"

Track::Track()
{

}


/// Уникальный идентификатор в БД
void Track::setId(int id)
{
    this->uid = id;
}

int Track::getId()
{
    return this->uid;
}


/// Наименование дорожки
void Track::setName(const QString &n)
{
    this->name = n;
}

QString Track::getName()
{
    return this->name;
}
