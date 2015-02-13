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
#include "mname.h"
#include "settings.h"

Settings::Settings()
{
    actionsLoaded = this->defaultActions();
    backgroundColor = this->defaultColor();
    backgroundToShow = QString("squares");
    defaultPath = QDir().homePath();
    windowSize = QSize(500,400);
    sorting = 0;
    pathToUse = 0;
    squaresSize = 8;
    zoomIncrement = 25;
    maxRecentFiles = 5;
    movieSpeed = 100;
    compressLevel = -1;
    tbButtomStyle = 0;
    tbArea = 4;
    precision = 2;
    restartWhenZooming = true;
    stopMovieWhenFinish = false;
    showZoomSlider = false;
    showMenuBar = true;
    toolBarMovable = false;
    loadFixedSize = true;
    toolBarVisible = true;
    createNewShortCuts = true;
    previousSettings = true;

    //eggs
    eggBegin = setHour(0,0);
    eggEnd = setHour(0,30);
    enableEggs = true;
    imageJoke = false;
    useBinary = true;
}

void Settings::getEggsEndHour(int& hour, int& min)
{
    getHour(eggEnd, hour, min);
}

void Settings::getEggsBeginHour(int& hour, int& min)
{
    getHour(eggBegin, hour, min);
}

void Settings::getHour(QString time, int &hour, int &min)
{
    //qDebug()<<time;
    if(time.indexOf("@Time(") == -1)
        return;

    time.remove("@Time(");
    time.remove(")");

    QStringList g = time.split(" ");
    hour = g.at(0).toInt();
    min = g.at(1).toInt();
    //qDebug()<<g;
    //qDebug()<<QString("%1:%2").arg(h).arg(m);

    //check min and hour are both valids
    while(min > 59){
        hour++;
        min -= 60;
    }
    
    while(hour > 23){
        hour -= 24;
    }

}

QString Settings::setHour(int hour, int min)
{
    return QString("@Time(%1 %2)").arg(hour).arg(min);
}

bool Settings::isTime()
{
    int bh=0, bm=0, eh=0, em=0;
    getHour(eggBegin,bh,bm);
    getHour(eggEnd,eh,em);
    //qDebug()<<QString("Begin: %1:%2").arg(bh).arg(bm);
    //qDebug()<<QString("End: %1:%2").arg(eh).arg(em);

    if(QTime(bh,bm) <= QTime::currentTime() && QTime::currentTime() <= QTime(eh,em))
        return true;

    else
        return false;
}

bool Settings::useEggs()
{
    if(!enableEggs || !isTime())
         return false;

    return true;
}

bool Settings::imageJokes()
{
    if(!useEggs() || !imageJoke)
        return false;

    else
        return true;
}

bool Settings::useBinaryStyle()
{
    if(!useEggs() || !useBinary)
        return false;

    else
        return true;
}

QStringList Settings::defaultActions() const
{
    QStringList defaultActions;
    defaultActions<<"_previous"<<"_next"<<"_separator"<<"_zoomIn"<<"_zoomOut"<<"_normalSize"<<"_adjustSize"
                  <<"_separator"<<"_rotateLeft"<<"_rotateRight";
    return defaultActions;
}

QStringList Settings::defaultColor() const
{
    QStringList defColor;
    defColor<<"104"<<"102"<<"101";
    return defColor;
}

void Settings::getPreviousConfig()
{
    QSettings settings("QIviewer", "qiviewer");
    if(settings.value("MainWindow/bgToShow").toString() == "scuares"){
        settings.setValue("MainWindow/bgToShow", "squares");
    }
    if(settings.value("ToolBar/tbArea") == "top"){
        settings.setValue("ToolBar/tbArea", 4);
    }
    if(settings.value("ToolBar/tbArea") == "bottom"){
        settings.setValue("ToolBar/tbArea", 8);
    }
    if(settings.value("ToolBar/tbArea") == "left"){
        settings.setValue("ToolBar/tbArea", 1);
    }
    if(settings.value("ToolBar/tbArea") == "right"){
        settings.setValue("ToolBar/tbArea", 2);
    }
    settings.setValue("lastDir/pathToUse", settings.value("lastDir/showlastdir", 0).toInt());
    settings.remove("lastDir/showlastdir");
    settings.remove("Shortcuts");
    settings.remove("ToolBar/actionsLoadedFromSettigns");
    settings.setValue("getPreviousSettings", false);
}

void Settings::loadSettings()
{
    //TODO no esta guardando nio leyendo
    //Los carga llamando por separado al dialogo de configuracion, sino no
    qDebug() << __METHOD_NAME__;
    QSettings settings("QIviewer", "qiviewer");

    //check if need to load previous version settings
    if(settings.value("getPreviousSettings", true).toBool()){
        this->getPreviousConfig();
    }

    /*main app settings*/
    settings.beginGroup("MainWindow");
    restartWhenZooming  = settings.value("restartWhenZooming",   true).toBool();
    showZoomSlider      = settings.value("showZoomSlider",       false).toBool();
    showMenuBar         = settings.value("showMenuBar",          true).toBool();
    stopMovieWhenFinish = settings.value("stopMovieWhenFinish",  false).toBool();
    loadFixedSize       = settings.value("loadFixedSize",        true).toBool();
    sorting             = settings.value("filesSorting",         0).toInt();
    compressLevel       = settings.value("compressLevel",        -1).toInt();
    maxRecentFiles      = settings.value("maxRecentFilesNumber", 5).toInt();
    zoomIncrement       = settings.value("zoomIncrement",        25).toInt();
    squaresSize         = settings.value("squaresSize",          8).toInt();
    movieSpeed          = settings.value("movieSpeed",           100).toInt();
    precision           = settings.value("fileSizePrecision",    2).toInt();
    windowSize          = settings.value("mw_size",              QSize(500, 400)).toSize();
    backgroundToShow    = settings.value("bgToShow",             "squares").toString();
#ifdef __linux__
    prefIconTheme       = settings.value("preferedIconTheme",    "oxygen").toString();
#endif
    backgroundColor     = settings.value("backgroundColor",      defaultColor()).toStringList();
    recentFilesList     = settings.value("recentFiles").toStringList();
    settings.endGroup();    
    //check max files
    if(maxRecentFiles < 0 || maxRecentFiles >15){maxRecentFiles = 5;}

    settings.beginGroup("Eggs");
    enableEggs		= settings.value("enableEggs",           true).toBool();
    imageJoke           = settings.value("imageJokes",           false).toBool();
    useBinary           = settings.value("useBinary",            true).toBool();
    eggBegin            = settings.value("eggStart",             setHour(0,0)).toString();
    eggEnd              = settings.value("eggEnd",               setHour(0,30)).toString();
    settings.endGroup();

    //toolbar settings
    settings.beginGroup("Toolbar");
    toolBarMovable = settings.value("toolbar_movable",           false).toBool();
    toolBarVisible = settings.value("toolBarVisible",            true).toBool();
    actionsLoaded  = settings.value("actionsLoadedFromSettigns", defaultActions()).toStringList();
    tbArea         = settings.value("tbArea",                    4).toInt();
    tbButtomStyle  = settings.value("tbbuttomstyle",             0).toInt();
    settings.endGroup();

    //last dir used
    settings.beginGroup("lastDir");
    pathToUse   = settings.value("pathToUse",    int(0)).toInt();
    lastDirUsed = settings.value("last_dir",     QDir().homePath()).toString();
    defaultPath = settings.value("user_default", QDir().homePath()).toString();
    settings.endGroup();

    //shortcuts
    settings.beginGroup("Shortcuts");
    shortcuts.clear();
    createNewShortCuts = settings.value("creatNewShorcuts", true).toBool();
    QStringList keysList = settings.childKeys();
    if(!createNewShortCuts){
        for(int i=0; i < keysList.size(); i++){
            shortcuts[keysList.at(i)] = settings.value(keysList.at(i)).toString();
            //shortCuts<<settings.value(shortCutsOwner.at(i)).toString();
        }
    }
    shortcuts.remove("creatNewShorcuts");
    settings.endGroup();

    qDebug() << __METHOD_NAME__ << ": Settings loaded";
}

void Settings::saveSettings()
{
    qDebug() << __METHOD_NAME__;
    QSettings settings("QIviewer", "qiviewer");
    settings.beginGroup("Eggs");
    settings.setValue("enableEggs", enableEggs);
    settings.setValue("imageJokes", imageJoke);
    settings.setValue("useBinary", useBinary);
    settings.setValue("eggStart", eggBegin);
    settings.setValue("eggEnd", eggEnd);
    settings.endGroup();

    settings.beginGroup("MainWindow");
    settings.setValue("movieSpeed", movieSpeed);
    settings.setValue("restartWhenZooming", restartWhenZooming);
    settings.setValue("showZoomSlider", showZoomSlider);
    settings.setValue("showMenuBar", showMenuBar);
    settings.setValue("fileSizePrecision", precision);
    settings.setValue("stopMovieWhenFinish", stopMovieWhenFinish);
    settings.setValue("loadFixedSize", loadFixedSize);
    settings.setValue("compressLevel", compressLevel);
    settings.setValue("maxRecentFilesNumber", maxRecentFiles);
    settings.setValue("zoomIncrement", zoomIncrement);
    settings.setValue("squaresSize", squaresSize);
    settings.setValue("mw_size", windowSize);
    settings.setValue("bgToShow", backgroundToShow);
    settings.setValue("backgroundColor", backgroundColor);
    settings.setValue("recentFiles", recentFilesList);
    settings.setValue("filesSorting", sorting);
#ifdef __linux__
    settings.setValue("preferedIconTheme", prefIconTheme);
#endif
    settings.endGroup();

    settings.beginGroup("Toolbar");
    settings.setValue("toolbar_movable", toolBarMovable);
    settings.setValue("toolBarVisible", toolBarVisible);
    settings.setValue("actionsLoadedFromSettigns", actionsLoaded);
    settings.setValue("tbArea", tbArea);
    settings.setValue("tbbuttomstyle", tbButtomStyle);
    settings.endGroup();

    settings.beginGroup("lastDir");
    settings.setValue("user_default", defaultPath);
    settings.setValue("last_dir", lastDirUsed);
    settings.setValue("pathToUse", pathToUse);
    settings.endGroup();

    settings.remove("Shortcuts");
    settings.beginGroup("Shortcuts");
    settings.setValue("creatNewShorcuts", createNewShortCuts);
    if(shortcuts.count() > 0){
        QMapIterator<QString, QString> i(shortcuts);
        while(i.hasNext()){
            i.next();
            settings.setValue(i.key(), i.value());
        }
    }
    settings.endGroup();
    qDebug() << __METHOD_NAME__ << "Settings saved";
}
