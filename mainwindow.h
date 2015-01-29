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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QMap>
#ifndef QT_NO_PRINTER
#include <QtGui/QPrinter>
#endif

class QAction;
class QMenu;
class QScrollArea;
class QAbstractScrollArea;
class QToolBar;
class QSlider;
class QMovie;
class QFileInfo;
class QPaintEvent;
class QMouseEvent;
class QShortcut;
class ZoomUtils;
class ImageWidget;
class Settings;
class FileUtils;
class ActionsManager;

enum ImageType{
    Static, //this is for static images, such as jpeg's or not animated gif ones
    Dynamic, //animated images, such as animated gif or png
    Webp //needed becouse qt doesn't handle this yet
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  /**
   * constructor
   */
  MainWindow();
  
  /**
    * Load app command from command line
    * @param d is the list of the parameters passed
    */
  void openImageFromCommandLine(const QStringList);

public slots:
  /*!
    Loads the last file used
    */
  void openLastFile();

protected:
  void resizeEvent(QResizeEvent *);
  void closeEvent(QCloseEvent *event);
  
private:
  //variables
  FileUtils *fileUtils;
  Settings *settings;
  ActionsManager *actionsManager;
  ImageType imageType;
  ImageWidget *imageWidget;
  ZoomUtils *zoomWidget;
  QSlider *zoomSlider;
  bool showZoomSlider;
  bool imageSetted;
  bool pixmapChanged;
  int compressLevel;
  QStringList nameFilters;
  QScrollArea *scrollArea;
  QStringList actionsLoaded;
  QString lastDirUsed;//could i delete it and just use archivo->absoluteFilePath()? nop
  int pathToUse;//0=lastDir, 1=default
  QString defaultPath;
  int maxRecentFiles;
  QList<QAction *> recentFilesAct;
  QMap<QString, QString> defaultShortcuts;
  QStringList recentFilesPath;
  QAction *deleteRecentFilesAct;

  //toolbar and menus
  QToolBar *mainToolBar;
  QMenu *fileMenu;
  QMenu *viewMenu;
  QMenu *editMenu;
  QMenu *goMenu;
  QMenu *imageMenu;
  QMenu *helpMenu;
  QMenu *recentFilesMenu;
  QMenu *tbMenu;

#ifndef QT_NO_PRINTER
  QPrinter printer;
#endif

  //functions
  /*!
    \returns the last directory/path used
    */
  QString getLastDir() const;

  /*!
    create and configure the actions
    */
  void createActions();

  /*!
    create and configure the menus
    */
  void createMenus();

  /*!
    create the main toolbar
    */
  void createToolBar();

  /*!
    configures the allowed file formats
    */
  void setNameFilters();

  /*!
    Determinate the \see ImageType of the opened image
    */
  void determinateImageType();

  /*!
    Open the image
    */
  void openImage();

  /*!
    Update actions acording to the app status
    */
  void updateActions();

  /*!
    sets the color from the settings
    */
  void getColorFromSettings(QStringList);

  /*!
    if the image was modified shows a dialog asking if want to continue
    \returns true if the image wasn't modified or the user hits save
    \returns false if the user hits cancel/no
    */
  bool canCloseImage();

  /*!
    configures the toolbar and menubar right-click menu
    */
  void configureToolBarToolTip();

  /*!
    Creating actions for the recent files
    */
  void createRecentFilesMenu();
  
  /*!
    Add the loaded image into the recent files list/menu
    */
  void pushNewFile();
  
  /**
    * configure toolbar
    *
    * @param sl is the list of the actions to add to the toolbar
    * @param a is the toolbar area
    * @param s the toolbar buttom style
    * @param v toolbar visible or not
    * @param zl show zoom slider
    */
  void setUpToolBar(QStringList, Qt::ToolBarArea, Qt::ToolButtonStyle, bool, bool);
  
  /*!
   * Save the current image as \a fileName
   * If \a fileName is empty (fileName.isEmpty() == true), then this funcion uses current image
   * name and replace it.
   * \returns true if sucess, otherwise
   * \returns false if fail.
   */
  void saveImage(QString name="");
  
  
private slots:
  /*!
    Open an image
    */
  void open();

  /*!
    Save the current image
    */
  void saveAs();
  
  /*!
    Saves the changes
    */
  void save();

  /*!
    Print the current image
    */
  void print();

  /*!
    Go to the next image in the current image folder
    */
  void next();

  /*!
    Go to the previous image in the current image folder
    */
  void previous();

  /*!
    Go to the first image in the current image folder
    */
  void goFirst();

  /*!
    Go to the last image in the current image folder
    */
  void goLast();

  /*!
    Open a folder to view the images inside it
    */
  void openDir();

  /*!
    Open the file selected in the recent files menu
    */
  void openRecentFile();
  void openStatic();
  void openDynamic();
  void fileProperties();
  void zoomIn();
  void zoomOut();
  void wheelZoom(int);
  void normalSize();
  void adjustSizeSlot();
  void rotateLeft();
  void rotateRight();
  void flipHorizontally();
  void flipVertically();
  void about();
  void couldOpen(bool);
  void loadSettings();
  void saveSettings();
  void showHideMenuBar();
  void setToolBarMovable();
  void setToolBarVisible(bool);

  /*!
    Deletes the recent files menu in the File menu
    */
  void deleteRecentFiles();

  /*!
    This deletes the current image
    */
  void deleteFileSlot();
  void moveToSlot();
  void goToSlot();
  void goToSlot(QString name, int pos);

  /*!
    Makes the given \a path the last one used, so it is passed
    the open file/folder and save file dialog
    */
  void setLastPathUsed(QString path);
  void configureToolBarSlot();
  void configureToolBarSlot2(QStringList);
  void configureShortcutsSlot();
  void configureNewShortcuts(QMap<QString, QKeySequence>);
  void pixmapChangedSlot();
  void toolBarContextMenu();
  void closeFileSlot();
  void updateWindowTitle();

  /**
    *this methos is for moving the image with the mouse
    * @param e is the QPoint value from mousePressEvent
    * @param d is the QPoint value from mouseMoveEvent
    */
  void moveWidget(QPoint, QPoint);

  /**
    * configuration dialog
    */
  void configureProgram();

  /**
    *this is for when the image is rotated and the button fixed size
    *is checked
    */
  void imageRotated();

};

#endif
