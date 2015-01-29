#ifndef EDITTOOLBAR_H
#define EDITTOOLBAR_H

#include <QtGui/QDialog>
#include <QtCore/QList>
#include "actiondata.h"
#include "ui_edittoolbar.h"

class QListWidgetItem;
class QListWidget;

class EditToolBar : public QDialog
{
    Q_OBJECT

public:
    /*!
      constructor
      */
    EditToolBar(QWidget *parent = 0);

    /*!
      \returns the new list with the actions to load
      */
    QStringList getActionsList();

    /*!
      loads the \a actions in the tool bar, and \a allActions all the actions, including
      the given in \a actions. by setting it id (the Map key) and the ActionData (the Map value)
      */
    void setActionsList(QMap<QString, ActionData *> allActions, QStringList actions);

    /*!
      Loads the default actions
      @param defActions is a string list with the default action id's
      */
    inline void setDefaultActions(QStringList defActions){defaultActions = defActions;}

signals:
    /*!
      Signal emitted  when the user hits apply. It sends the new actionslist oredered.
      */
    void actionsChanged(QStringList);

public slots:

private:
    Ui_Dialog ui;

    /*!
      a map with all the action data that belong to the given id
      */
    QMap<QString, ActionData *> actionsMap;

    /*!
      a listwith all the actions that were loaded into the toolbar
      */
    QStringList actionsUsed;

    /*!
      Stores the app default actions
      */
    QStringList defaultActions;

    /*!
      the separator item
      */
    QListWidgetItem *separatorItem;

    /*!
      size of the icons
      */
    int iconSize;

    /*!
      it's true if the user made any search in current actions list, otherwise
      it's iqual to false
      */
    bool searchMade;

    /*!
      if the action has no icon asigned, then i use this to make
      the action gets aligned with the others, "it's a gosht icon"
      */
    QIcon emptyIcon();

    /*!
      don't know why i declared this
      */
    int getPositionToUse();

    /*!
      this is called after set the actions list to fill them
      */
    void configureActionList();

    /*!
      Add the item \a id to the given \a list in the position \a position.
      If \a position=-1, then it appends the item
      */
    void addItem(QString id, QListWidget *list, int position = -1);

    /*!
      Same than \see addItem(QString id, QListWidget *list, int position = -1), but here
      i use the actionData \a ad to add the item
      */
    void addItem(ActionData *ad, QListWidget *list, int position = -1);

    /*!
      Clears and fills the available actions list
      */
    void fillAvailableItemsWidget();

    /*!
      Clears and fills the current actions list
      */
    void fillCurrentItemsWidget();

signals:
    void actionsListChanged();

private slots:
    void applySlot();
    void okSlot();
    void actionsChangedSlot();
    void setDefaults();

    void addAction();
    void removeAction();
    void moveUpAction();
    void moveDownAction();
    void moveAction(int d);
    void updateButtons();
    void searchInAvailableActionsSlot(QString);
    void searchInCurrentActionsSlot(QString);

};

#endif // EDITTOOLBAR_H
