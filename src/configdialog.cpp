/*********************************************************************
* Copyright (C) 2010 by Dario Ignacio Aguilera                       *
* dario_21_06@hotmail.com                                            *
*                                                                    *
* This program is free software; you can redistribute it and/or      *
* modify it under the terms of the GNU General Public License        *
* as published by the Free Software Foundation; either version 2     *
* of the License, or (at your option) any later version.             *
*                                                                    *
* This program is distributed in the hope that it will be useful,    *
* but WITHOUT ANY WARRANTY; without even the implied warranty of     *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      *
* GNU General Public License for more details.                       *
*                                                                    *
* You should have received a copy of the GNU General Public License  *
* along with this program; if not, write to the                      *
* Free Software Foundation, Inc.                                     *
* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.      *
**********************************************************************/

#include <QtGui>

#include "configdialog.h"
#include "locationpage.h"
#include "generaloptionspage.h"
#include "toolbarpage.h"

ConfigDialog::ConfigDialog(QWidget* parent, Qt::WFlags fl) : QDialog(parent, fl)
{
    //configure the ui
    ui.setupUi(this);

    //add icons to the list items
    ui.listWidget->item(0)->setIcon(QIcon::fromTheme("image-x-generic"));
    ui.listWidget->item(1)->setIcon(QIcon::fromTheme("edit-rename"));
    ui.listWidget->item(2)->setIcon(QIcon::fromTheme("configure-toolbars"));

    //set names to the labels
    ui.generalLabel->setText(QString("<h4>%1</h>").arg(tr("General")));
    ui.locationLabel->setText(QString("<h4>%1</h>").arg(tr("Location")));
    ui.toolbarLabel->setText(QString("<h4>%1</h>").arg(tr("Toolbar")));

    //set icons to the apply, ok and cancel button
    ui.applyButton->setIcon(QIcon::fromTheme("dialog-ok-apply"));
    ui.acceptButton->setIcon(QIcon::fromTheme("dialog-ok"));
    ui.cancelButton->setIcon(QIcon::fromTheme("dialog-cancel"));

    //make buttons connection
    connect(ui.applyButton, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect(ui.acceptButton, SIGNAL(clicked()), this, SLOT(saveSettings()));

    //configure widgets
    lPage = new LocationPage;
    ui.locationLayout->addWidget(lPage);
    gPage = new GeneralOptionsPage;
    ui.generalOptionsLayout->addWidget(gPage);
    tbPage = new ToolbarPage;
    ui.toolbarLayout->addWidget(tbPage);

    connect(lPage, SIGNAL(settingsChanged()), this, SLOT(settingsChangedSlot()));
    connect(gPage, SIGNAL(settingsChanged()), this, SLOT(settingsChangedSlot()));
    connect(tbPage, SIGNAL(settingsChanged()), this, SLOT(settingsChangedSlot()));

    ui.applyButton->setEnabled(false);
    ui.acceptButton->setEnabled(false);
}

ConfigDialog::~ConfigDialog()
{
    gPage->~GeneralOptionsPage();
    lPage->~LocationPage();
    tbPage->~ToolbarPage();
}

bool ConfigDialog::canSave()
{
    return tbPage->canSave() &&
           gPage->canSave() &&
           lPage->canSave();
}

int ConfigDialog::closeDialog()
{
    if (this->canSave()){
      return 1;
    }
    else{
      return 0;
    }
}

void ConfigDialog::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);

    switch (closeDialog())
    {
      case 0:
        break;
      case 1:
        qDebug() << "the changes have not been saved";
        break;
    }
}

void ConfigDialog::saveSettings()
{
    if(gPage->canSave()){
        gPage->saveSettings();
    }
    if(lPage->canSave()){
        lPage->saveSettings();
    }
    if(tbPage->canSave()){
        tbPage->saveSettings();
    }

    ui.applyButton->setEnabled(false);
    emit settingsSaved();
}

void ConfigDialog::settingsChangedSlot()
{
    ui.applyButton->setEnabled(true);
    ui.acceptButton->setEnabled(true);
}
