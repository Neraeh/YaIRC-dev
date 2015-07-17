#ifndef CHAN_H
#define CHAN_H

#include <QObject>
#include "irc.h"
#include "user.h"

class Irc;
class Chan : public QObject
{
    Q_OBJECT

public:
    explicit Chan(Irc *_parent = 0, QString _name = "");
    ~Chan();
    QString getName();
    void addUser(User *_user);
    QList<User*> getUserlist();

signals:

public slots:

private:
    Irc *parent;
    QString name, content;
    QList<User*> userlist;
    bool joined;
};

#endif // CHAN_H
