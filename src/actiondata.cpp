#include "actiondata.h"

ActionData::ActionData(QString name, QString id, QString shortcut, QString iconName, QObject *parent) :
    QObject(parent)
{
    this->name = name;
    this->iconName = iconName;
    this->shortcut = shortcut;
    this->id = id;
}
