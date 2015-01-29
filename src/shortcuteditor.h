#ifndef SHORTCUTEDITOR_H
#define SHORTCUTEDITOR_H

#include <QDialog>
#include <QtCore/QMap>

class ActionData;
class QTreeWidgetItem;
class QKeySequenceWidget;

namespace Ui {
    class ShortcutEditor;
}

class ShortcutEditor : public QDialog
{
    Q_OBJECT

public:
    /*!
      Contrucst a new shortcuts editor dialog
      */
    explicit ShortcutEditor(QWidget *parent = 0);

    /*!
      Load the given \a actions to modify their shortcuts
      */
    void setActionsList(QList<ActionData *> actions);

    /*!
      Desctructor
      */
    ~ShortcutEditor();

    /*!
      set \a defShortcuts as the default shortcuts to use
      if need to restore them
      @param defShortcuts is a map whe the key is the action id
      that has a shortcut and the value is the shortcuts it self,
      passed as a string
      */
    void setDefaultShortcuts(QMap<QString, QString> defshortcuts);

signals:
    /*!
      \returns a map with the (new) \a id and the \a shortcut
      */
    void newShortcuts(QMap<QString, QKeySequence>);

private:
    Ui::ShortcutEditor *ui;
    QString oldShortcut;
    QKeySequenceWidget *keySWidget;
    QTreeWidgetItem *testItem;
    QMap<QString, QKeySequence> actionsChanged;
    QMap<QString, QString> defaultShortcuts;
    bool defShortcutsMade;//change name
    QList<ActionData*> actionsList;
    QIcon emptyIcon();
    bool apply;
    int iconSize;
    int totalElements;

    void addItem(ActionData *item);
    void addAllItems();
    bool shortcutDuplicated(QString shortcut);

private slots:
    void okButtonSlot();
    void applyButtonSlot();
    void setDefaultsShortcutsSlot();
    void searchSlot(QString);
    void changeShortcut();
    void keySequenceChanged(QKeySequence);

};

#endif // SHORTCUTEDITOR_H
