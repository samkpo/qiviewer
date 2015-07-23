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
#include "aboutdialog.h"
#include "defines.h"
#include "utils.h"

AboutDialog::AboutDialog(QWidget* parent, Qt::WFlags fl) : QDialog( parent, fl )
{
    ui.setupUi(this);
#ifdef WEBP_SUPPORT
    qDebug()<<"experimental webp support enabled";
#endif

    ui.versionLabel->setText(tr("Version %1").arg(QLatin1String(PROJECT_VERSION)));
    
    QString descString;
    descString.append("<p>");
    descString.append(tr("A simple and lightweight image viewer, written totally in Qt."));
    descString.append("</p><p>");
    descString.append("<a href=\"https://github.com/samkpo/qiviewer/\">");
    descString.append(tr("Home page"));
    descString.append("</a></p>");
    ui.descriptionLabel->setText(descString);
    ui.descriptionLabel->setTextFormat(Qt::RichText);

    QString authorString;
    authorString.append("<p>");
    authorString.append(tr("Use %1Github issues page%2 to report bugs, patches or ideas.").arg("<a href=\"https://github.com/samkpo/qiviewer/issues\">", "</a>"));
    authorString.append("</p><p><b>");
    authorString.append("Aguilera Dario");
    authorString.append("</b><br />");
    authorString.append(tr("Developer."));
    authorString.append("</p>");
    authorString.append("<p><a href=\"mailto:aguilera.dario.i@gmail.com?Subject=");
    authorString.append(tr("Hello"));
    authorString.append("\">");
    authorString.append(tr("Email"));
    authorString.append("</a> - ");
    authorString.append("<a href=\"http://sammyie.wordpress.com\">");
    authorString.append(tr("Personal blog"));
    authorString.append("</a></p>");
    ui.authorLabel->setText(authorString);

    QString thanksTo;
    thanksTo.append("<p><b>Salvador Parra Camacho</b><ul><li>");
    thanksTo.append(tr("Icons on systems without icon themes"));
    thanksTo.append("</li><li>");
    thanksTo.append(tr("OS/2 support"));
    thanksTo.append("</li></ul></p>");    
    thanksTo.append("<p><b>Acidrums4</b><ul><li>");
    thanksTo.append(tr("QIviewer icon"));
    thanksTo.append("</li></ul></p>");
    ui.thaksToLabel->setText(thanksTo);

    /*!
      The file translators.txt must be setted as:
      name of the translator
      language
      */
    QString translators;
    QStringList temp = Utils::stringFromFile(":translators.txt").split("\n");
    for(int i=0; i < temp.count(); i++){
        if(i%2 == 0){
            translators.append("<p><b>" + temp.at(i) + "</b><ul><li>");
        }
        else{
            translators.append(temp.at(i) + "</li><li></p>");
        }
    }
    ui.translatorLabel->setText(translators);

    ui.licenseText->setText(Utils::stringFromFile(":license.txt"));
    ui.okButton->setIcon(QIcon::fromTheme("dialog-ok"));

    this->resize(QSize(470, 270));
}
