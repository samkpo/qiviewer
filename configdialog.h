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

#ifndef CONFIGDIALOG2_H
#define CONFIGDIALOG2_H

#include <QDialog>
#include "ui_configdialog.h"

class ActionData;
class LocationPage;
class GeneralOptionsPage;
class ToolbarPage;

class ConfigDialog : public QDialog
{
  Q_OBJECT

public:
  ConfigDialog(QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~ConfigDialog();

signals:
  /**
   * signal sent when the dialog saves the configuration
   */
  void settingsSaved();

protected:
  void closeEvent(QCloseEvent *event);
    
private:
  Ui_ConfigDialog ui;
  LocationPage *lPage;
  GeneralOptionsPage *gPage;
  ToolbarPage *tbPage;
  int closeDialog();
  bool canSave();

private slots:
  void saveSettings();
  void settingsChangedSlot();

};

#endif
