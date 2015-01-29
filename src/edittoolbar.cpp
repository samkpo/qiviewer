#include <QtCore/QDebug>
#include "edittoolbar.h"

/* TODO
  emit the signal actionsListChanged() when moving an item in the current actions
  list by drag & drop
  */

EditToolBar::EditToolBar(QWidget *parent) :
    QDialog(parent)
{
    //configure u
    ui.setupUi(this);

    //no search made yet
    searchMade = false;

    //icons size - copied from kedittoolbar class, in kdelibs
    iconSize = this->style()->pixelMetric(QStyle::PM_SmallIconSize);

    //separator item
    separatorItem = new QListWidgetItem(QString("--- %1 ---").arg(tr("Separator")));
    separatorItem->setData(32, "_separator");

    //setup icons
    ui.okButton->setIcon(QIcon::fromTheme("dialog-ok"));
    ui.applyButton->setIcon(QIcon::fromTheme("dialog-ok-apply"));
    ui.cancelButton->setIcon(QIcon::fromTheme("dialog-cancel"));
    ui.clearButton1->setIcon(QIcon::fromTheme("edit-clear-locationbar-rtl"));
    ui.clearButton2->setIcon(QIcon::fromTheme("edit-clear-locationbar-rtl"));
    ui.addButton->setIcon(QIcon::fromTheme("go-next"));
    ui.removeButton->setIcon(QIcon::fromTheme("go-previous"));
    ui.moveDownButton->setIcon(QIcon::fromTheme("go-down"));
    ui.moveUpButton->setIcon(QIcon::fromTheme("go-up"));
    ui.setDefaultsButton->setIcon(QIcon::fromTheme("document-revert"));

    //connections
    connect(this, SIGNAL(actionsListChanged()), this, SLOT(updateButtons()));
    connect(ui.currentActions, SIGNAL(itemSelectionChanged()), this, SLOT(updateButtons()));
    connect(ui.availableActions, SIGNAL(itemSelectionChanged()), this, SLOT(updateButtons()));
    connect(this, SIGNAL(actionsListChanged()), this, SLOT(actionsChangedSlot()));
    connect(ui.okButton, SIGNAL(clicked()), this, SLOT(okSlot()));
    connect(ui.applyButton, SIGNAL(clicked()), this, SLOT(applySlot()));
    connect(ui.addButton, SIGNAL(clicked()), this, SLOT(addAction()));
    connect(ui.removeButton, SIGNAL(clicked()), this, SLOT(removeAction()));
    connect(ui.moveDownButton, SIGNAL(clicked()), this, SLOT(moveDownAction()));
    connect(ui.moveUpButton, SIGNAL(clicked()), this, SLOT(moveUpAction()));
    connect(ui.currentFilter, SIGNAL(textChanged(QString)), this, SLOT(searchInCurrentActionsSlot(QString)));
    connect(ui.availableFilter, SIGNAL(textChanged(QString)), this, SLOT(searchInAvailableActionsSlot(QString)));
    connect(ui.setDefaultsButton, SIGNAL(clicked()), this, SLOT(setDefaults()));

    this->setWindowTitle(tr("Configure toolbar"));
}

QIcon EditToolBar::emptyIcon()
{//copied from kedittoolbar class, in kdelibs
    QPixmap m_emptyIcon = QPixmap(iconSize, iconSize);
    m_emptyIcon.fill(Qt::transparent);
    return QIcon(m_emptyIcon);
}

QStringList EditToolBar::getActionsList()
{
    //create a list
    QStringList actions;

    if(searchMade){
        return actionsUsed;
    }

    //fill the list
    for(int i=0; i<ui.currentActions->count(); i++){
        actions<<ui.currentActions->item(i)->data(32).toString();
    }

    return actions;
}

void EditToolBar::moveAction(int d)
{
    /*
      if d > 0, that means i have to move up the action, so i need to insert in
      the previous position. By the other way, if d < 0, i have to move down the
      action, so i need to insert it in the next position. So the new action
      position will be actualPosition - d
      */
    //get current position
    int g = ui.currentActions->currentRow() - d;

    //move the action
    ui.currentActions->insertItem(g, ui.currentActions->takeItem(g + d));

    //set the action moved as the selected
    ui.currentActions->setCurrentRow(g);

    emit actionsListChanged();
}

void EditToolBar::moveUpAction()
{
    this->moveAction(1);
}

void EditToolBar::moveDownAction()
{
    this->moveAction(-1);
}

void EditToolBar::updateButtons()
{
    bool enableAvailable = ui.availableActions->currentRow() != -1 &&
                           ui.availableActions->count() > 0;
    bool enableCurrent = ui.currentActions->currentRow() != -1 &&
                         ui.currentActions->count() > 0;

    //enable remove button if the currernt actions isn't empty
    ui.removeButton->setEnabled(enableCurrent);

    //enable add button if any action in available actions list is selected
    ui.addButton->setEnabled(enableAvailable);

    //enable move up button only if we are in row 2+
    ui.moveUpButton->setEnabled((ui.currentActions->currentRow() >= 1) && enableCurrent && !searchMade);

    //enable move down button only
    ui.moveDownButton->setEnabled((ui.currentActions->currentRow() < ui.currentActions->count()-1) && enableCurrent && !searchMade);

}

int EditToolBar::getPositionToUse()
{
    int i;

    //if the list has all the items in the toolbar (searchMade=false)
    if(!searchMade){
        //get the position of the selected action in the current actions list
        i = ui.currentActions->currentRow();

        //we are at the end of the list
        if(i == ui.currentActions->count() - 1){
            i = -1;
        }
    }

    //if something was searched, then return -1 (don't know where to put the item :-S
    else{
        i = -1;
    }

    //qDebug()<<"position to insert item:"<<i;
    return i;
}

void EditToolBar::searchInCurrentActionsSlot(QString str)
{
    //user search
    searchMade = true;

    //check if str isn't empty
    if(str.isEmpty()){
        this->fillCurrentItemsWidget();
        return;
    }

    //clear list
    ui.currentActions->clear();

    QMapIterator<QString, ActionData*> ite(actionsMap);//ite, i, iterator, depending on the context and my energy
    while(ite.hasNext()){
        ite.next();
        if(ite.value()->getName().contains(str, Qt::CaseInsensitive) &&
           (actionsUsed.indexOf(ite.key()) != -1)){
            this->addItem(ite.key(), ui.currentActions);
        }
    }

}

void EditToolBar::addAction()
{
    //get the id from the action selected in the available actions list
    QString availableActionSelectedId = ui.availableActions->currentItem()->data(32).toString();

    //check that the action we want to add isn't already in the list, exept if its a separator
    for(int i=0; i<ui.currentActions->count(); i++){
        if((ui.currentActions->item(i)->data(32).toString() == availableActionSelectedId) &&
                (ui.currentActions->item(i)->data(32).toString() != "_separator")){
           return;
        }
    }

    //add the action to the widget
    this->addItem(availableActionSelectedId, ui.currentActions, getPositionToUse());

    //add the action to the list
    if(searchMade){
        actionsUsed.append(availableActionSelectedId);
    }
    else{
        actionsUsed = this->getActionsList();
    }

    //select the added action in the list
    ui.currentActions->setCurrentRow(-1);

    //emit the signal the actions list has changed
    emit actionsListChanged();
}

void EditToolBar::removeAction()
{
    //if i did a search, then remove the item fomr the list, so, when it clears the search field
    //the current list widget will filled with the actions according to the list, and the one removed
    //won't be there
    if(searchMade){
        actionsUsed.removeAll(ui.currentActions->currentItem()->data(32).toString());
        // remove the item
        ui.currentActions->currentItem()->~QListWidgetItem();
    }
    else{
        // remove the item
        ui.currentActions->currentItem()->~QListWidgetItem();
        actionsUsed = this->getActionsList();
    }

    //emit the signal
    emit actionsListChanged();
}

void EditToolBar::searchInAvailableActionsSlot(QString str)
{
    //check if the str isn't empty
    if(str.isEmpty()){
        this->fillAvailableItemsWidget();
        return;
    }

    //clear the list
    ui.availableActions->clear();

    //QString flag;
    QMapIterator<QString, ActionData*> i(actionsMap);
    while(i.hasNext()){
        i.next();
        if(i.value()->getName().contains(str, Qt::CaseInsensitive)){
            this->addItem(i.key(), ui.availableActions);
        }
    }

}

void EditToolBar::addItem(ActionData *ad, QListWidget *list, int position)
{
   //create the new item
    QListWidgetItem *itemFlag = new QListWidgetItem;

    //set the name of the item
    itemFlag->setText(ad->getName().remove('&'));

    //set action data
    itemFlag->setData(32, ad->getId());

    //set icon
    if((!ad->getIconName().isEmpty()) && (ad->getIconName() != "none")){
        itemFlag->setIcon(QIcon::fromTheme(ad->getIconName()));
    }
    else{
        //itemFlag->setText(QString("---  %1").arg(itemFlag->text()));
        itemFlag->setIcon(this->emptyIcon());
    }

    //add the item to the list
    if(position != -1){
        list->insertItem(position, itemFlag);
    }
    else{
        list->addItem(itemFlag);
   }
}

void EditToolBar::addItem(QString id, QListWidget *list, int position)
{
    //check if its a separator
    if(id == "_separator"){
        //insert the separator in the given position
        if(position != -1){
            list->insertItem(position, separatorItem->clone());
        }

        //or at the end of the list
        else{
            list->addItem(separatorItem->clone());
        }

        return;
    }

    //check if id is in the actions list
    if(!actionsMap.contains(id)){
        return;
    }

    //add the item
    this->addItem(actionsMap[id], list, position);
}

void EditToolBar::fillAvailableItemsWidget()
{
    //make sure the list is empty
    ui.availableActions->clear();

    //add the separator item
    ui.availableActions->addItem(separatorItem->clone());

    //add items
    QMapIterator<QString, ActionData*> i(actionsMap);
    while(i.hasNext()){
        i.next();
        this->addItem(i.value(), ui.availableActions);
    }
}

void EditToolBar::fillCurrentItemsWidget()
{
    //make sure the list is empty
    ui.currentActions->clear();

    //no search made
    searchMade = false;

    //add items
    foreach(QString id, actionsUsed){
        this->addItem(id, ui.currentActions);
    }
}

void EditToolBar::setDefaults()
{
  if(defaultActions.isEmpty()){
    qDebug()<<"No default actions passed";
    return;
  }

  //clear list
  ui.currentActions->clear();

  //no search made
  searchMade = false;

  //add items
  foreach(QString id, defaultActions){
      this->addItem(id, ui.currentActions);
  }

  this->actionsChangedSlot();
}

void EditToolBar::setActionsList(QMap<QString, ActionData *> l, QStringList d2)
{
    actionsMap = l;
    actionsUsed = d2;

    this->configureActionList();
}

void EditToolBar::configureActionList()
{
    this->fillAvailableItemsWidget();
    this->fillCurrentItemsWidget();
}

void EditToolBar::actionsChangedSlot()
{
    ui.applyButton->setEnabled(true);
    ui.okButton->setEnabled(true);
}

void EditToolBar::applySlot()
{
    emit actionsChanged(this->getActionsList());
}

void EditToolBar::okSlot()
{
    emit actionsChanged(this->getActionsList());
    this->close();
}

