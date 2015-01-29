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
#include <getopt.h>
#include <iostream>
#include "defines.h"
#include "mainwindow.h"
#include "eggsdialog.h"
#include "configdialog.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    //all the translation stuff was taken from minitube
    const QString locale = QLocale::system().name();

    // qt translations
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    // app translations
    QString dataDir = QLatin1String(PKGDATADIR);
    qDebug() << "Data dir: " << __FUNCTION__ << dataDir;
  
#ifdef USE_DEVELOPING
    qDebug() << "Developing";
    QString    localeDir = qApp->applicationDirPath() + QDir::separator() + "locale";
#else
    qDebug() << "not developing";
#if defined(Q_OS_OS2) //|| defined(Q_OS_WIN) ->this isn't checked
    QString    localeDir = qApp->applicationDirPath() + QDir::separator() + "locale";
#else
    QString    localeDir = dataDir + QDir::separator() + "locale";
#endif
#endif

    qDebug() << "Locale dir: " << localeDir;
    QTranslator translator;
    translator.load(locale, localeDir);
    app.installTranslator(&translator);

#ifndef Q_OS_LINUX
    QString BUILTIN_ICON_THEME = "oxygen";
    QIcon::setThemeName(BUILTIN_ICON_THEME);
#endif

    /**
      *command line stuff
      */
    int next_option;
    int re = 0;
    const char* const short_options = "hevc";
    const struct option long_options[] =
    {
        {"help",     0, NULL, 'h'},
        {"eggs",     0, NULL, 'e'},
        {"version",  0, NULL, 'v'},
        {"configure",0, NULL, 'c'},
        {NULL,       0, NULL, 0}
    };

    next_option = getopt_long(argc, argv, short_options, long_options, NULL);

    if (next_option == 'h')
    {
        std::cout << QString("If you have problems with the toolbar and the actions, try deleting the file .config/QIviewer/qiviewer.conf\n"
			     "How to use: qiviewer [OPTION/FILE]\n"
                             "Avaible options:\n"
                             "    -h --help shows this help and finish\n"
                             "    -v --version shows qiviewer version\n"
                             "    -e --eggs shows eggs dialog\n"
                             "    -c --configuration shows the configuration dialog\n").toStdString();
        re = 0;
    }

    else if (next_option == 'c')
    {
        ConfigDialog con;
        con.setWindowTitle("QIviewer configuration");
        con.exec();
        re = 0;
    }

    else if (next_option == '?')
    {
        std::cout << QString("Try 'qiviewer --help' for more information\n").toStdString();
        re = 0;
    }

    else if (next_option == 'v')
    {
        std::cout << QString("QIviewer %1\n"
                                 "Copyright (C) 2011 Aguilera Dario.\n"
                                 "License GPLv2+.\n"
                                 "<http://gnu.org/licenses/gpl.html>.\n"
                                 "This is free software: you are free to change it and redistribute.\n"
                                 "There is NO WARRANTY.\n"
                                 ).arg(QLatin1String(PROJECT_VERSION)).toStdString();
        re = 0;
    }

    else if (next_option == 'e')
    {
        EggsDialog ed;
        re = ed.exec();
    }

    else if (next_option == -1)
    {
        MainWindow imageViewer;
        if (QApplication::arguments().size() > 1)
            imageViewer.openImageFromCommandLine(QApplication::arguments());
        imageViewer.show();
        re = app.exec();
    }

    return re;
}
