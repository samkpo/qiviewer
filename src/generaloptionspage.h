#ifndef GENERALOPTIONSPAGE_H
#define GENERALOPTIONSPAGE_H

#include <QWidget>

class Settings;

namespace Ui {
    class GeneralOptionsPage;
}

class GeneralOptionsPage : public QWidget
{
    Q_OBJECT

public:
    GeneralOptionsPage(QWidget *parent = 0);
    ~GeneralOptionsPage();

public slots:
    void saveSettings();
    bool canSave() const{return settingsChangedBool;}

signals:
    void settingsChanged();

private:
    Ui::GeneralOptionsPage *ui;
    Settings *settings;

    bool settingsChangedBool;

    QStringList rflBack;
    QColor color;
    QPixmap iconForButton();
    QStringList setColorSettings();
    void getColorFromSettings(QStringList);
    int getBGSelection(int);//0->returns the settings selectins
                            //1->returns the combobox selecteditem

private slots:
    void paintSquaresSample(int);
    void bgComboBoxSlot(int);
    void selectColorSlot();
    void deleteRecentFilesList(QString);
    void settingsChangedSlot();

};

#endif // GENERALOPTIONSPAGE_H
