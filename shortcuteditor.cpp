#include <QtCore/QDebug>
#include <QtGui/QMessageBox>

#include "shortcuteditor.h"
#include "actiondata.h"
#include "qkeysequencewidget.h"
#include "ui_shortcuteditor.h"

ShortcutEditor::ShortcutEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShortcutEditor)
{
    //setup ui
    ui->setupUi(this);

    //icon size
    iconSize = this->style()->pixelMetric(QStyle::PM_SmallIconSize);

    //tree widget headers
    QStringList columnNames = QStringList()<<tr("Action")<<tr("Shortcut");
    ui->shortcutsWidget->setHeaderLabels(columnNames);

    //set icons to the buttons
    ui->applyButton->setIcon(QIcon::fromTheme("dialog-ok-apply"));
    ui->okButton->setIcon(QIcon::fromTheme("dialog-ok"));
    ui->cancelButton->setIcon(QIcon::fromTheme("dialog-cancel"));
    ui->clearButton->setIcon(QIcon::fromTheme("edit-clear-locationbar-rtl"));
    ui->setDefaultsButton->setIcon(QIcon::fromTheme("document-revert"));

    //configure the key seuqnce widget
    keySWidget = new QKeySequenceWidget;
    keySWidget->setEnabled(false);
    keySWidget->setClearButtonShow(QKeySequenceWidget::ShowRight);
    keySWidget->setClearButtonIcon(QIcon::fromTheme("edit-clear"));
    ui->csLayout->addWidget(keySWidget);

    //connections
    connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyButtonSlot()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okButtonSlot()));
    connect(ui->searchWidget, SIGNAL(textChanged(QString)), this, SLOT(searchSlot(QString)));
    connect(ui->shortcutsWidget, SIGNAL(itemSelectionChanged()), SLOT(changeShortcut()));
    connect(ui->setDefaultsButton, SIGNAL(clicked()), this, SLOT(setDefaultsShortcutsSlot()));
    connect(keySWidget, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(keySequenceChanged(QKeySequence)));

    apply = false;

    //the default shortcuts haven't been setted
    defShortcutsMade = false;

    //total elements count to be added
    totalElements = 0;
}

QIcon ShortcutEditor::emptyIcon()
{//copied from kedittoolbar class, in kdelibs
    QPixmap m_emptyIcon = QPixmap(iconSize, iconSize);
    m_emptyIcon.fill(Qt::transparent);
    return QIcon(m_emptyIcon);
}

void ShortcutEditor::keySequenceChanged(QKeySequence d)
{
    //if the new key is the same than the older, no need to continue
    if(oldShortcut == d.toString()){
        return;
    }

    //check if the shortcut isn't duplciated
    if(this->shortcutDuplicated(d.toString())){
        return;
    }

    //set the new key to the item in the list
    ui->shortcutsWidget->currentItem()->setText(1,d.toString());

    //set the item in the actions changed eith it new shortcut
    actionsChanged[ui->shortcutsWidget->currentItem()->data(0,32).toString()] = d;

    //enable buttons
    ui->okButton->setEnabled(true);
    ui->applyButton->setEnabled(true);

    apply = true;
}

bool ShortcutEditor::shortcutDuplicated(QString shortcut)
{
    foreach(ActionData *ad, actionsList){
        if(ad->getShortcut() == shortcut){
            QMessageBox::information(this, this->windowTitle(),
                                     tr("The shortcut %1 belong to %2, it won't be reasigned.").arg(shortcut, ad->getName()),
                                     QMessageBox::Ok);
            return true;
        }
    }

    return false;
}

void ShortcutEditor::changeShortcut()
{
    //enable key button
    keySWidget->setEnabled(true);

    //save the current shortcut
    oldShortcut = ui->shortcutsWidget->currentItem()->text(1);

    //set the shortcut to the button
    keySWidget->setKeySequence(QKeySequence(oldShortcut));
}

ShortcutEditor::~ShortcutEditor()
{
    delete ui;
}

void ShortcutEditor::addItem(ActionData *item)
{
    //create the new item
    testItem = new QTreeWidgetItem(ui->shortcutsWidget);

    //set the name of the item, AKA the action name
    testItem->setText(0, item->getName());

    //set action data
    testItem->setData(0, 32, item->getId());

    //set action shortcut
    if((!item->getShortcut().isEmpty()) && (item->getShortcut() != "none")){
        testItem->setText(1, item->getShortcut());
    }

    //set the action/item icon
    if((!item->getIconName().isEmpty()) && (item->getIconName() != "none")){
        testItem->setIcon(0, QIcon::fromTheme(item->getIconName()));
    }    
    else{
        testItem->setIcon(0, this->emptyIcon());
    }

    ui->shortcutsWidget->resizeColumnToContents(0);
    ui->shortcutsWidget->resizeColumnToContents(1);
}

void ShortcutEditor::setActionsList(QList<ActionData *> actions)
{
    actionsList = actions;

    //store amount of elements
    totalElements = actionsList.count();

    //add all items
    this->addAllItems();

    //if i don't do this, the buttons gets enabled
    ui->okButton->setEnabled(false);
    ui->applyButton->setEnabled(false);
}

void ShortcutEditor::setDefaultShortcuts(QMap<QString, QString> defshortcuts)
{
  defaultShortcuts = defshortcuts;
  defShortcutsMade = true;
//  QMapIterator<QString, QString> i(defshortcuts);
//  while(i.hasNext()){
//    i.next();
//    qDebug()<<i.key()<<" "<<i.value();
//  }
}

void ShortcutEditor::setDefaultsShortcutsSlot()
{
  //clear list of actionsChanged
  actionsChanged.clear();

  for(int i = 0; i< totalElements; i++){
    //if the action is in the map with the default shortcuts, set it
    if(defaultShortcuts.contains(actionsList.at(i)->getId())){
      actionsList.at(i)->setShortcut(defaultShortcuts[actionsList.at(i)->getId()]);
    }

    //make the action shortcut empty
    else{
      actionsList.at(i)->setShortcut(QString());
    }

    //set the item in the actions changed eith it new shortcut
    actionsChanged[actionsList.at(i)->getId()] = actionsList.at(i)->getShortcut();
  }

  //enable buttons
  ui->okButton->setEnabled(true);
  ui->applyButton->setEnabled(true);

  apply = true;

  //add the items to the widget
  this->addAllItems();
}

void ShortcutEditor::addAllItems()
{
  ui->shortcutsWidget->clear();

  for(int i=0; i < totalElements; i++){
      //add the new item
      this->addItem(actionsList.at(i));
  }
}

void ShortcutEditor::searchSlot(QString d)
{
    //clear the list
    ui->shortcutsWidget->clear();

    //if no string to search is passed, then add the all list
    if(d.isEmpty()){
        this->addAllItems();
        return;
    }

    QString temp;
    for(int i=0; i<totalElements; i++){
        temp = actionsList.at(i)->getName();
        if(temp.contains(d, Qt::CaseInsensitive)){
            //add the item
            this->addItem(actionsList.at(i));
        }
    }
}

void ShortcutEditor::okButtonSlot()
{
    qDebug()<<"ok button slot";
    if(apply){
        this->applyButtonSlot();
    }
    this->close();
}

void ShortcutEditor::applyButtonSlot()
{
    qDebug()<<"apply button slot";
//    QMapIterator<QString, QKeySequence> i(actionsChanged);
//    while (i.hasNext()) {
//         i.next();
//         qDebug() << i.key() << ": " << i.value().toString();
//     }
    apply = false;
    emit newShortcuts(actionsChanged);
}
