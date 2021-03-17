#include "group.h"

Group::Group()
{

}

/// Идентификатор группы в БД
void Group::setId(int id)
{
    this->uid = id;
}

int Group::getId()
{
    return this->uid;
}


/// Наименование группы
void Group::setName(const QString &n)
{
    this->name = n;
}

QString Group::getName()
{
    return this->name;
}
