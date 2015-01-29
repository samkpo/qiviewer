#ifndef TOOLBARPAGE_H
#define TOOLBARPAGE_H

#include <QWidget>
#include <QtCore/QList>

class Settings;
class ActionData;

namespace Ui {
    class ToolbarPage;
}

class ToolbarPage : public QWidget
{
    Q_OBJECT

public:
    explicit ToolbarPage(QWidget *parent = 0);
    ~ToolbarPage();

public slots:
    void saveSettings();
    bool canSave() const{return settingsChangedBool;}

signals:
    void settingsChanged();

private:
    Ui::ToolbarPage *ui;
    Settings *settings;
    int getButtonStyle();
    int gettbpos();
    bool settingsChangedBool;

private slots:
    void settingsChangedSlot();

};

#endif // TOOLBARPAGE_H
