#include <QSettings>
#include "eggsdialog.h"
#include "settings.h"

EggsDialog::EggsDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.okButton, SIGNAL(clicked()), SLOT(saveSettings()));
    ui.okButton->setIcon(QIcon::fromTheme("dialog-ok"));
    ui.cancelButton->setIcon(QIcon::fromTheme("dialog-cancel"));

    loadSettings();
    if(!ui.enableEgg->isChecked()){
      ui.imageJokes->setEnabled(false);
      ui.binaryButton->setEnabled(false);
      ui.endTime->setEnabled(false);
      ui.beginTime->setEnabled(false);
      ui.label_4->setEnabled(false);
      ui.label_5->setEnabled(false);
      ui.label->setEnabled(false);
      ui.label_2->setEnabled(false);
    }
}

void EggsDialog::saveSettings()
{
    QSettings settings("QIviewer", "qiviewer");
    settings.beginGroup("Eggs");
    settings.setValue("enableEggs", ui.enableEgg->isChecked());
    settings.setValue("imageJokes", ui.imageJokes->isChecked());
    settings.setValue("useBinary", ui.binaryButton->isChecked());
    settings.setValue("eggStart", setHour(ui.beginTime->time().hour(), ui.beginTime->time().minute()));
    settings.setValue("eggEnd", setHour(ui.endTime->time().hour(), ui.endTime->time().minute()));
    settings.endGroup();
    this->close();
}

void EggsDialog::loadSettings()
{
    QSettings settings("QIviewer", "qiviewer");
    settings.beginGroup("Eggs");
    ui.enableEgg->setChecked(settings.value("enableEggs", true).toBool());
    ui.imageJokes->setChecked(settings.value("imageJokes", false).toBool());
    ui.binaryButton->setChecked(settings.value("useBinary", true).toBool());

    int endH, endM, begH, begM;
    getHour(settings.value("eggStart", setHour(0,0)).toString(), begH, begM);
    getHour(settings.value("eggEnd", setHour(0,30)).toString(), endH, endM);
    ui.endTime->setTime(QTime(endH, endM));
    ui.beginTime->setTime(QTime(begH, begM));
    settings.endGroup();
}

void EggsDialog::getHour(QString time, int &hour, int &min)
{
    if(time.indexOf("@Time(") == -1){
        return;
    }

    time.remove("@Time(");
    time.remove(")");

    QStringList g = time.split(" ");
    hour = g.at(0).toInt();
    min = g.at(1).toInt();

    while(min > 59){
        hour++;
        min -= 60;
    }

    while(hour > 23){
        hour -= 24;
    }

}

QString EggsDialog::setHour(int hour, int min)
{
    return QString("@Time(%1 %2)").arg(hour).arg(min);
}
