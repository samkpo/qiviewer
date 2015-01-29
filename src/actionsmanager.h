#ifndef ACTIONSMANAGER_H
#define ACTIONSMANAGER_H

#include <QObject>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtGui/QKeySequence>

class QAction;
class QWidget;
class QShortcut;
class ActionData;

class ActionsManager : public QObject
{
    /*!
      \brief This class is used to manage all the action in the app.

      Start by creating a new object, then add an action(s), with it's respective
      name, id, icon name and shortcut, and the slot it has to be connected
      Then, if you want to add an action to some widget, just do it by:
      widget->addAction(actManager->getAction(id))
      */

    Q_OBJECT

public:
    /*!
      constructor
      */
    ActionsManager(QObject *parent);

    /*!
      \returns a map with the action's ActionData, being the QString \param the id
      */
    QMap<QString, ActionData *> getActionsMap();

    /*!
      \returns a list with the actions
      */
    QList<ActionData*> getActionsList();

    /*!
      Adds a new action to the manager
      \param id is the identification for the action, i.e. _open
      if the action already has a shortcut, then \a shortcut is ignored.
      \param shortcut is the action shortcut, i.e. Ctrl+O
      \returns a pointer to the added action
     */
    QAction* addAction(QAction *action, QString id, QWidget *parent, QKeySequence shortcut = QKeySequence());

    /*!
      Adds a new action to the manager
      \param id is the identification for the action, i.e. _open
      if the action already has a shortcut, then \a shortcut is ignored.
      \param shortcut is the action shortcut, i.e. Ctrl+O
      \returns a pointer to the added action
     */
    QAction* addAction(QAction *action,
                   QString id,
                   QWidget *parent,
                   QObject *receiver,
                   std::string slot,
                   QKeySequence shortcut = QKeySequence());

    /*!
      Adds a new action to the manager
      \param id is the identification for the action, i.e. _open
      if the action already has a shortcut, then \a shortcut is ignored.
      \param shortcut is the action shortcut, i.e. Ctrl+O
      \returns a pointer to the added action
     */
    QAction* addAction(QAction *action,
                   bool checkable,
                   QString id,
                   QWidget *parent,
                   QObject *receiver,
                   std::string slot,
                   QKeySequence shortcut);

    /*!
      Adds a new action to the manager
      \param id is the identification for the action, i.e. _open
      if the action already has a shortcut, then \a shortcut is ignored.
      \param shortcut is the action shortcut, i.e. Ctrl+O
      \returns a pointer to the added action
     */
    QAction* addAction(QAction *action,
                   bool checkable,
                   QString id,
                   QWidget *parent,
                   QObject *receiver,
                   std::string slot);

    /*!
      \a returns the action attached to \a id, otherwise \return a null action
      */
    QAction *action(QString id);

    /*!
      connects the action asigned to \a id, to the receiver \a receiver to the
      slot \a slot. Note: you get the slot doing SLOT(yourSlot())
      */
    void connectAction(QString id, QObject *receiver, std::string slot);

    /*!
      Enable disable the wanted action. If you added the action, then you have to use this,
      and it will enable/disable it's shortcut, but if you make:
      \code actionManaget->action(id)->setEnabled(enabled)
      the shortcut wont be enabled/disabled
      */
    void setEnabled(QString id, bool enabled);

    /*!
      \returns a list with all the id's stored
      */
    QStringList getIds();

    /*!
      set the \a iconName icon from the theme to the action \a id
      */
    void setActionIcon(QString id, QString iconName);

    /*!
      \returns the \a actionName name
      */
    QString removeSString(QString actionName);

    /*!
      set \a shortcut to \a id
      */
    void setShortcut(QString id, QKeySequence shortcut);

    /*!
      Makes the action \a id checkable
      */
    void setCheckable(QString id, bool checkable);

    /*!
      Makes the \a id action \a checked
      */
    void setChecked(QString id, bool checked);

    /*!
      Sets the given \a tooltip to the \a id action
      */
    void setToolTip(QString id, QString tooltip);

    /*!
      \returns a Map with the action id and it corresponding shortcut
      the map key is the id, and the value the shortcut
      this has just the actions that have shortcut
      \see setShortcuts(QMap<QString, QString> list)
      */
    QMap<QString, QString> getShortcuts();

    /*!
      Sets the shortcuts stored in list to the action correspondig to the id
      The format is: id=map key, shortcut=map value
      see getShortcuts()
      */
    void setNewShortcuts(QMap<QString, QString> list);

    /*!
      \returns the \a id action data
      */
    ActionData *getActionData(QString id);

private:
    /*!
      The name of every action created will have the next format:
      \a name + \t + \a shortcut, i.e. Open\tCtrl+O
      The string \t is used to add the shortcut string in the action
      name to make it look as the action shortcut, but it's not.
      */

    /*!
      \struct this struct contains an action, it's shortcut
      */
    struct InternalActData{
        QAction *action;    ///<action
        QShortcut *shortcut;///<actions's shortcut
        QString iconName;   ///<actions iconName
        QObject *receiver;
        std::string slot;   ///<slot the action is connected to
    };

    /*!
      This QMap object contains a list with the \a actions and it's respective
      \a id's.
      */
    QMap<QString, InternalActData> ActionsList;

    /*!
      updates the action name and tooltip, corresponding to the \a id
      i.e. when whe modifi the action shortcut
      */
    void setNameAndToolTip(QString id);

    /*!
      removes all shortcuts from the actions stored
      */
    void clearShortcuts();

    /*!
      Shows a message (in command line) taht adverts than the given
      \a id ain't valid
      */
    QString invalidId(QString id);
};

#endif // ACTIONSMANAGER_H
