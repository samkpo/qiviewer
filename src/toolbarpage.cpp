#include "toolbarpage.h"
#include "settings.h"
#include "ui_toolbarpage.h"

ToolbarPage::ToolbarPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolbarPage)
{
    //set up ui
    ui->setupUi(this);

    //settings
    settings = new Settings;
    settings->loadSettings();

    //toolbar area
    switch (settings->getTBArea())
    {
      case 1:
        ui->leftRadioButtom->setChecked(true);
        break;
      case 2:
        ui->rightRadioButtom->setChecked(true);
        break;
      case 4:
        ui->topRadioButtom->setChecked(true);
        break;
      case 8:
        ui->bottomRadioButtom->setChecked(true);
        break;
    }

    //toolbar button style
    switch (settings->getTBButtomStyle())
    {
      case 0:
        ui->onlyIconsRadioButtom->setChecked(true);
        break;
      case 1:
        ui->onlyTextRadioButtom->setChecked(true);
        break;
      case 2:
        ui->tbiRadioButtom->setChecked(true);
        break;
      case 3:
        ui->tuiRadioButtom->setChecked(true);
        break;
      case 4:
        ui->fsRadioButtom->setChecked(true);
        break;
    }

    //toolbar visible
    ui->toolbarVisible->setChecked(settings->getTBVisible());

    //toolbar movable
    ui->lockToolbar->setChecked(!settings->getTBMovable());
    ui->lockToolbar->setEnabled(settings->getTBVisible());

    //conections
    connect(ui->leftRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->rightRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->topRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->bottomRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->lockToolbar, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->toolbarVisible, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->onlyIconsRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->onlyTextRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->tbiRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->tuiRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->fsRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));

    settingsChangedBool = false;
}

int ToolbarPage::gettbpos()
{
  if (ui->leftRadioButtom->isChecked())
  {
    return 1;
  }
  if (ui->rightRadioButtom->isChecked())
  {
    return 2;
  }
  if (ui->topRadioButtom->isChecked())
  {
    return 4;
  }
  if (ui->bottomRadioButtom->isChecked())
  {
    return 8;
  }
  return 4;
}

int ToolbarPage::getButtonStyle()
{
  if (ui->onlyIconsRadioButtom->isChecked())
  {
    return 0;
  }
  if (ui->onlyTextRadioButtom->isChecked())
  {
    return 1;
  }
  if (ui->tbiRadioButtom->isChecked())
  {
    return 2;
  }
  if (ui->tuiRadioButtom->isChecked())
  {
    return 3;
  }
  if (ui->fsRadioButtom->isChecked())
  {
    return 4;
  }
  return 0;
}

void ToolbarPage::settingsChangedSlot()
{
    qDebug()<<"Settings changed slot tbp";
    settingsChangedBool = true;
    emit settingsChanged();
}

void ToolbarPage::saveSettings()
{
    if(settingsChangedBool){
        settings->setTBMovable(!ui->lockToolbar->isChecked());
        settings->setTBVisible(ui->toolbarVisible->isChecked());
        settings->setTBArea(this->gettbpos());
        settings->setTBButtomStyle(this->getButtonStyle());
        settings->saveSettings();
        settingsChangedBool = false;
    }
}

ToolbarPage::~ToolbarPage()
{
    delete ui;
}
