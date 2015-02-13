#include <QtGui/QFileDialog>
#include "mname.h"
#include "locationpage.h"
#include "settings.h"
#include "ui_locationpage.h"

LocationPage::LocationPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocationPage)
{
    //set up ui
    ui->setupUi(this);

    //settings
    settings = new Settings;
    settings->loadSettings();

    //connections, use just one button, becouse when its unselected the other one is selected, and the signal is emited
    connect(ui->selectFolderButton, SIGNAL(pressed()), this, SLOT(openDirSlot()));
    connect(ui->locationOption1, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));

    //sets last dir option tooltip
    ui->locationOption1->setToolTip(settings->getLastDirUsed());

    //sets option prefered
    switch (settings->getPathToUse())
    {
      case 0:
        ui->locationOption1->setChecked(true);
        break;
      case 1:
        ui->locationOption2->setChecked(true);
        break;
    }

    //sets the select folder button icon and tooltip, then make the connection
    ui->selectFolderButton->setIcon(QIcon::fromTheme("document-open-folder"));
    ui->selectFolderButton->setToolTip(tr("Change default location"));

    //sets the line edit text
    ui->defaultPathLineEdit->setText(settings->getDefaultPath());

    settingsChangedBool = false;
}

int LocationPage::getLastDirOption()
{
    if (ui->locationOption1->isChecked())
    {
      return 0;
    }
    if (ui->locationOption2->isChecked())
    {
      return 1;
    }
    return 0;
}

void LocationPage::openDirSlot()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select folder"),
                      settings->getDefaultPath(), QFileDialog::ShowDirsOnly
                      | QFileDialog::DontResolveSymlinks);

    if (dirName.size() != 0)
    {
      settings->setDefaultPath(dirName);
      ui->defaultPathLineEdit->setText(dirName);
      settingsChangedSlot();
    }
}

LocationPage::~LocationPage()
{
    delete ui;
}

void LocationPage::saveSettings()
{
    qDebug() << __METHOD_NAME__ ;
    if(settingsChangedBool){
        settings->setDefaultPath(ui->defaultPathLineEdit->text());
        settings->setPathToUse(this->getLastDirOption());
        settings->saveSettings();
        settingsChangedBool = false;
    }
}

void LocationPage::settingsChangedSlot()
{
    settingsChangedBool = true;
    emit settingsChanged();
}
