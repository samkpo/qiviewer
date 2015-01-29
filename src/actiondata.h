#ifndef ACTIONDATA_H
#define ACTIONDATA_H

#include <QObject>

class QString;

class ActionData : public QObject
{
    Q_OBJECT
public:
    explicit ActionData(QString name, QString id, QString shortcut,
                        QString iconName, QObject *parent = 0);
    inline QString getName() const{return name;}
    inline QString getId() const{return id;}
    inline QString getShortcut() const{return shortcut;}
    inline QString getIconName() const{return iconName;}

    inline void setName(QString name){this->name = name;}
    inline void setIconName(QString iconName){this->iconName = iconName;}
    inline void setId(QString id){this->id = id;}
    inline void setShortcut(QString shortcut){this->shortcut = shortcut;}

private:
    QString name;
    QString id;
    QString shortcut;
    QString iconName;

};

#endif // ACTIONDATA_H
