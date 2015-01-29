#ifndef LOCATIONPAGE_H
#define LOCATIONPAGE_H

#include <QWidget>

class Settings;

namespace Ui {
    class LocationPage;
}

class LocationPage : public QWidget
{
    Q_OBJECT

public:
    explicit LocationPage(QWidget *parent = 0);
    ~LocationPage();

signals:
    void settingsChanged();

public slots:
    void saveSettings();
    bool canSave() const{return settingsChangedBool;}

private:
    Ui::LocationPage *ui;
    Settings *settings;
    int getLastDirOption();
    bool settingsChangedBool;

private slots:
    void settingsChangedSlot();
    void openDirSlot();

};

#endif // LOCATIONPAGE_H
