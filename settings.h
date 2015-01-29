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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QtCore>
#include <QDebug>
#include <QSize>

class Settings : public QObject
{
    Q_OBJECT

public:
    /*!
      constructor
      */
    Settings();

    /*!
      return true if its setted to use easter eggs in the images, otherwise returns false
      */
    bool imageJokes();

    /*!
      return true if its setted to use binary sistem, otherwise returns false
      */
    bool useBinaryStyle();

    /*!
      returns true if its setted to use easter eggs , evaluating if it's timeto use them or not,
      */
    bool useEggs();

    /*!
      returns the easter eggs end \a hour and \a min
      */
    void getEggsEndHour(int& hour, int& min);

    /*!
      returns the easter eggs begining \a hour and \a min
      */
    void getEggsBeginHour(int& hour, int& min);

    /*!
      \returns a Map with the action id and it corresponding shortcut
      the map key is the id, and the value the shortcut
      this has just the actions that have shortcut
      \see setShortcuts(QMap<QString, QString> list)
      */
    inline QMap<QString, QString> getShortcuts(){return shortcuts;}

    /*!
      Sets the shortcuts stored in list to the action correspondig to the id
      The format is: id=map key, shortcut=map value
      see getShortcuts()
      */
    inline void setShortcuts(QMap<QString, QString> list){shortcuts = list;}

    inline QStringList getActionsLoaded() const{return actionsLoaded;}
    inline QStringList getBackgroundColor() const{return backgroundColor;}
    inline QStringList getRecentFilesList() const{return recentFilesList;}
    QStringList defaultActions() const;
    inline QString getBGToShow() const{return backgroundToShow;}
    inline QString getLastDirUsed() const{return lastDirUsed;}
    inline QString getDefaultPath() const{return defaultPath;}
    inline QSize getWindowSize() const{return windowSize;}
    inline int getSorting() const{return sorting;}
    inline int getPathToUse() const{return pathToUse;}
    inline int getSquaresSize() const{return squaresSize;}
    inline int getFileSizePrecision() const{return precision;}
    inline int getMovieSpeed() const{return movieSpeed;}
    inline int getZoomIncrement() const{return zoomIncrement;}
    inline int getMaxRecentFiles() const{return maxRecentFiles;}
    inline int getCompressLevel() const{return compressLevel;}
    inline int getTBButtomStyle() const{return tbButtomStyle;}
    inline int getTBArea() const{return tbArea;}
    inline bool getRestartWhenZooming() const{return restartWhenZooming;}
    inline bool getStopMovieWhenFinish() const{return stopMovieWhenFinish;}
    inline bool getShowZoomSlider() const{return showZoomSlider;}
    inline bool getShowMenuBar() const{return showMenuBar;}
    inline bool getTBMovable() const{return toolBarMovable;}
    inline bool getLoadFixedSize() const{return loadFixedSize;}
    inline bool getTBVisible() const{return toolBarVisible;}
    inline bool getCreateNewShortCuts() const{return createNewShortCuts;}

    inline void setActionsLoaded(const QStringList d){actionsLoaded = d;}
    inline void setBackgroundColor(const QStringList d){backgroundColor = d;}
    inline void setRecentFilesList(const QStringList d){recentFilesList = d;}
    inline void setBGTosShow(const QString d){backgroundToShow = d;}
    inline void setLastDirUsed(const QString d){lastDirUsed = d;}
    inline void setDefaultPath(const QString d){defaultPath = d;}
    inline void setWindowSize(const QSize d){windowSize = d;}
    inline void setSorting(const int sort){sorting = sort;}
    inline void setPathToUse(const int d){pathToUse = d;}
    inline void setSquaresSize(const int d){squaresSize = d;}
    inline void setMovieSpeed(const int d){movieSpeed = d;}
    inline void setFileSizePrecision(const int fSizePrecision){precision = fSizePrecision;}
    inline void setZoomIncrement(const int d){zoomIncrement = d;}
    inline void setMaxRecentFiles(const int d){maxRecentFiles = d;}
    inline void setCompressLevel(const int d){compressLevel = d;}
    inline void setTBButtomStyle(const int d){tbButtomStyle = d;}
    inline void setTBArea(const int d){tbArea = d;}
    inline void setRestartWhenZooming(const bool d){restartWhenZooming = d;}
    inline void setStopMovieWhenFinish(const bool d){stopMovieWhenFinish = d;}
    inline void setShowZoomSlider(const bool d){showZoomSlider = d;}
    inline void setShowMenuBar(const bool d){showMenuBar = d;}
    inline void setTBMovable(const bool d){toolBarMovable = d;}
    inline void setLoadFixedSize(const bool d){loadFixedSize = d;}
    inline void setTBVisible(const bool d){toolBarVisible = d;}
    inline void setCreateNewShortCuts(const bool d){createNewShortCuts = d;}

public slots:
    void saveSettings();
    void loadSettings();

private:
    //variables
    QMap<QString, QString> shortcuts;
    QStringList actionsLoaded;
    QStringList backgroundColor;
    QStringList recentFilesList;
    QString backgroundToShow;
    QString lastDirUsed;
    QString defaultPath;
    QString eggEnd;
    QString eggBegin;
    QSize windowSize;
    int sorting;
    int pathToUse;
    int squaresSize;
    int zoomIncrement;
    int maxRecentFiles;
    int precision;
    int movieSpeed;
    int compressLevel;
    int tbButtomStyle;
    int tbArea;
    bool binary;
    bool restartWhenZooming;
    bool stopMovieWhenFinish;
    bool showZoomSlider;
    bool showMenuBar;
    bool toolBarMovable;
    bool loadFixedSize;
    bool toolBarVisible;
    bool createNewShortCuts;
    bool previousSettings;
    bool enableEggs;
    bool imageJoke;
    bool useBinary;

    //methods
    void getPreviousConfig();
    bool isTime();
    
    /**
      * @return hour and min from the given string
      * @param time string that contains the time
      * @param hour returns the hour
      * @param min returns the min
      */
    void getHour(QString time, int& hour, int& min);
    
    /**
      * @return a string containing the hour and min
      * @param hour pass the hour
      * @param min pass the minutes
      */
    QString setHour(int hour, int min);
    QStringList defaultColor() const;

};

#endif // SETTINGS_H
