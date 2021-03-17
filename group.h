#ifndef GROUP_H
#define GROUP_H

#include <QString>

class Group
{
public:
    Group();
    void setId(int);
    int getId();
    void setName(const QString &);
    QString getName();

private:
    int uid;
    QString name;
};

#endif // GROUP_H
