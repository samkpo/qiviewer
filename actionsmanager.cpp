/*
         addAction(QString name, QString id, QString shortcut, QString iconName, QString slot)
           |
           |     +---------+
           +---->|Action   |_______
           +-----|Manager  |       |
           |     +---------+       +--->QList<ActionStuff *> getActions()
           |
          +-->QAction * getAction(QString id);
*/

#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtGui/QShortcut>
#include <QtGui/QAction>
#include <QtGui/QWidget>
#include "actionsmanager.h"
#include "actiondata.h"

ActionsManager::ActionsManager(QObject *parent) :
    QObject(parent)
{
    //qDebug()<<"new action manager object";
}

QString ActionsManager::invalidId(QString id)
{
    return tr("The id %1 isn't valid").arg(id);
}

void ActionsManager::setActionIcon(QString id, QString iconName)
{
    if(ActionsList.contains(id)){
        ActionsList[id].action->setIcon(QIcon::fromTheme(iconName));
        ActionsList[id].iconName = iconName;
    }
    else{
        qDebug()<<this->invalidId(id);
    }
}

QAction* ActionsManager::addAction(QAction *action, bool checkable, QString id, QWidget *parent, QObject *receiver, std::string slot)
{
    //make it checkable
    action->setCheckable(checkable);

    //add the action
    QAction *b = addAction(action, id, parent, QKeySequence());

    //connect it to the slot
    this->connectAction(id, receiver, slot);

    //return the result
    return b;
}

QAction* ActionsManager::addAction(QAction *action, bool checkable, QString id, QWidget *parent, QObject *receiver, std::string slot, QKeySequence shortcut)
{
    //make it checkable
    action->setCheckable(checkable);

    //add the action
    QAction *b = addAction(action, id, parent, shortcut);

    //connect it to the slot
    this->connectAction(id, receiver, slot);

    //return the result
    return b;
}

QAction* ActionsManager::addAction(QAction *action, QString id, QWidget *parent, QObject *receiver, std::string slot, QKeySequence shortcut)
{
    //add the action
    QAction *b = addAction(action, id, parent, shortcut);

    //connect it to the slot
    this->connectAction(id, receiver, slot);

    //return the result
    return b;
}

QAction* ActionsManager::addAction(QAction *action, QString id, QWidget *parent, QKeySequence key)
{
    //check if already have the action added
    if(ActionsList.contains(id)){
        return 0;
    }

    //create a new keysequence to use as shortcut. first, if it's posible take the
    //one the action owns, or use the one provided by key
    QKeySequence k;
    if(!action->shortcut().isEmpty()){
        //set the action's shortcut as the key
        k = action->shortcut();

        //and then remove that shortcut from the action
        action->setShortcut(QKeySequence());
    }
    else{
        //sets key as k
        k = key;
    }

    //create a new InternalActData struct to add to ActionsList
    InternalActData IAD;
    IAD.action = action;
    IAD.shortcut = new QShortcut(k, parent);

    //check if the action is enabled or not
    IAD.shortcut->setEnabled(IAD.action->isEnabled());

    //add the InternalActionData struct to ActionsList
    ActionsList[id] = IAD;

    //configure action's tooltip and name
    this->setNameAndToolTip(id);

    return this->action(id);
}

void ActionsManager::setNameAndToolTip(QString id)
{
    //check if the action exists
    if(!ActionsList.contains(id)){
        qDebug()<<this->invalidId(id);
        return;
    }

    //check if the actions's shortcut is empty
    QString shortcut = ActionsList[id].shortcut->key().toString();

    //create the action name
    QString name = ActionsList[id].action->text();
    name = this->removeSString(name);

    //create the tool tip
    QString tooltip = ActionsList[id].action->toolTip().remove('&');
    if(tooltip.indexOf(name) != -1){
        tooltip = name;
    }

    //create a string with the shortcut
    if(!shortcut.isEmpty()){
        name.append("\t" + shortcut);
    }

    ActionsList[id].action->setText(name);
    ActionsList[id].action->setToolTip(tooltip);
}

void ActionsManager::setToolTip(QString id, QString tooltip)
{
    if(!ActionsList.contains(id)){
        qDebug()<<this->invalidId(id);
        return;
    }

    ActionsList[id].action->setToolTip(tooltip);
}

void ActionsManager::setEnabled(QString id, bool enabled)
{
    if(ActionsList.contains(id)){
        ActionsList[id].action->setEnabled(enabled);
        ActionsList[id].shortcut->setEnabled(enabled);
    }
    else{
        qDebug()<<this->invalidId(id);
    }
}

QStringList ActionsManager::getIds()
{
    //create the list
    QStringList list;

    //create an iterator and iterate
    QMapIterator<QString, InternalActData> iterator(ActionsList);
    while (iterator.hasNext()) {
        iterator.next();
        list<<QString(iterator.key());
    }

    //return the list
    return list;
}

QList<ActionData *> ActionsManager::getActionsList()
{
    //create a list to return
    QList<ActionData *> list;

    //create an iterator and iterate over the ActionsList
    QMapIterator<QString, InternalActData> iterator(ActionsList);
    while (iterator.hasNext()) {
        iterator.next();

        //add the object to the list
        list<<this->getActionData(iterator.key());
    }

    //return the list
    return list;
}

QMap<QString, ActionData *> ActionsManager::getActionsMap()
{
    //create a list to return
    QMap<QString, ActionData *> list;

    //create an iterator and iterate over the ActionsList
    QMapIterator<QString, InternalActData> iterator(ActionsList);
    while (iterator.hasNext()) {
        iterator.next();

        //add the object to the list
        list[iterator.key()] = this->getActionData(iterator.key());
    }

    //return the map
    return list;
}

QMap<QString, QString> ActionsManager::getShortcuts()
{
    //create the map to be returned
    QMap<QString, QString> map;

    //create an iterator and iterate
    QMapIterator<QString, InternalActData> i(ActionsList);
    while(i.hasNext()){
        i.next();

        //check if the actio has a shortcut
        if(!i.value().shortcut->key().toString().isEmpty()){
            map[i.key()] = i.value().shortcut->key().toString();
        }
    }

    //return the map
    return map;
}

ActionData *ActionsManager::getActionData(QString id)
{
    if(!ActionsList.contains(id)){
        qDebug()<<this->invalidId(id);
        return 0;
    }

    //id
    QString i = id;

    //name
    QString name = this->removeSString(ActionsList[id].action->text());

    //icon name
    QString iconName = ActionsList[id].iconName;
    if(iconName.isEmpty()){
        iconName = "none";
    }

    //shorcut
    QString shortcut = ActionsList[id].shortcut->key().toString();
    if(shortcut.isEmpty()){
        shortcut = "none";
    }

    //create and return the ActionData object
    return new ActionData(name, i, shortcut, iconName);
}

void ActionsManager::setShortcut(QString id, QKeySequence shortcut)
{
    if(ActionsList.contains(id)){
        //set the new shortcut
        ActionsList[id].shortcut->setKey(shortcut);

        //reconf the action name and tooltip
        this->setNameAndToolTip(id);
    }

    else{
        qDebug()<<this->invalidId(id);
    }
}

void ActionsManager::clearShortcuts()
{
    QMapIterator<QString, InternalActData> i(ActionsList);
    while(i.hasNext()){
        i.next();
        this->setShortcut(i.key(), QKeySequence());
    }
}

void ActionsManager::setNewShortcuts(QMap<QString, QString> list)
{
    this->clearShortcuts();
    QMapIterator<QString, QString> i(list);
    while(i.hasNext()){
        i.next();
        if(ActionsList.contains(i.key())){
            this->setShortcut(i.key(), QKeySequence(i.value()));
        }
    }

}

QString ActionsManager::removeSString(QString str)
{
    int k = str.indexOf("\t");
    if(k != -1){
        return str.left(k).remove('&');
    }
    else{
        return str;
    }
}

QAction* ActionsManager::action(QString id)
{
    if(ActionsList.contains(id)){
        return ActionsList[id].action;
    }
    else{
        qDebug()<<invalidId(id);
        //throw QString("No action with id " + id).toStdString();
        return 0;
    }
}

void ActionsManager::setChecked(QString id, bool checked)
{
    //check if the action is contained
    if(!ActionsList.contains(id)){
        qDebug()<<this->invalidId(id);
        return;
    }

    if(ActionsList[id].action->isCheckable()){
        ActionsList[id].action->setChecked(checked);
    }

    else{
        qDebug()<<"The action " + id + " ain't checkable";
    }
}

void ActionsManager::setCheckable(QString id, bool checkable)
{
    //check if the action is contained
    if(!ActionsList.contains(id)){
        qDebug()<<this->invalidId(id);
        return;
    }

    //no need to continue if action was already checkable or not
    if(ActionsList[id].action->isCheckable() == checkable){
        return;
    }

    //set action checkable or not
    ActionsList[id].action->setCheckable(checkable);
    if(checkable){//make it unckeckable
        //it means the actions shortcut is connected to the action and the action
        //to the slot
        disconnect(ActionsList[id].shortcut, SIGNAL(activated()), ActionsList[id].action, SLOT(trigger()));
        disconnect(ActionsList[id].action, SIGNAL(triggered()), ActionsList[id].receiver, ActionsList[id].slot.data());
        connect(ActionsList[id].action, SIGNAL(toggled(bool)), ActionsList[id].receiver, ActionsList[id].slot.data());
        connect(ActionsList[id].shortcut, SIGNAL(activated()), ActionsList[id].action, SLOT(toggle()));
    }
    else{
        disconnect(ActionsList[id].action, SIGNAL(toggled(bool)), ActionsList[id].receiver, ActionsList[id].slot.data());
        disconnect(ActionsList[id].shortcut, SIGNAL(activated()), ActionsList[id].action, SLOT(toggle()));
        connect(ActionsList[id].shortcut, SIGNAL(activated()), ActionsList[id].action, SLOT(trigger()));
        connect(ActionsList[id].action, SIGNAL(triggered()), ActionsList[id].receiver, ActionsList[id].slot.data());
    }

}

void ActionsManager::connectAction(QString id, QObject *receiver, std::string slot)
{
    if(!ActionsList.contains(id)){
        qDebug()<<this->invalidId(id);
        return;
    }

    //save the object and the slot
    ActionsList[id].receiver = receiver;
    ActionsList[id].slot = slot;

    if(!ActionsList[id].action->isCheckable()){
        connect(ActionsList[id].shortcut, SIGNAL(activated()), ActionsList[id].action, SLOT(trigger()));
        connect(ActionsList[id].action, SIGNAL(triggered()), receiver, slot.data());
    }
    else{
        connect(ActionsList[id].action, SIGNAL(toggled(bool)), receiver, slot.data());
        connect(ActionsList[id].shortcut, SIGNAL(activated()), ActionsList[id].action, SLOT(toggle()));
    }
}
