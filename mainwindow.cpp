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

#include "mainwindow.h"
#include "imagewidget.h"
#include "zoomutils.h"
#include "aboutdialog.h"
#include "fileproperties.h"
#include "configdialog.h"
#include "settings.h"
#include "fileutils.h"
#include "gotodialog.h"
#include "edittoolbar.h"
#include "shortcuteditor.h"
#include "actionsmanager.h"

MainWindow::MainWindow()
{
    //actions manager
    actionsManager = new ActionsManager(this);
    
    //configure the allowed formats
    setNameFilters();
    
    //create and configure action
    createActions();
    
    //create menus
    createMenus();
    
    //create toolbar
    createToolBar();
    
    //no image setted yet
    imageSetted = false;
    
    //if no image was setted, i guess it can't be modified, can it?
    pixmapChanged = false;
    
    //file utils
    fileUtils = new FileUtils;
    fileUtils->setNameFilters(nameFilters);
    connect(fileUtils, SIGNAL(filePathChanged(QString)), this, SLOT(setLastPathUsed(QString)));
    
    //configure the scrollarea
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Mid);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setFrameShape(QFrame::NoFrame);
    setCentralWidget(scrollArea);
    
    //create the image widget and make the conections
    imageWidget = new ImageWidget;
    scrollArea->setWidget(imageWidget);
    connect(imageWidget, SIGNAL(couldOpen(bool)), this, SLOT(couldOpen(bool)));
    connect(imageWidget, SIGNAL(pixmapChanged()), this, SLOT(pixmapChangedSlot()));
    connect(imageWidget, SIGNAL(wheelZoom(int)), this, SLOT(wheelZoom(int)));
    connect(imageWidget, SIGNAL(picSizeChanged()), this, SLOT(imageRotated()));
    connect(imageWidget, SIGNAL(moveWidget(QPoint, QPoint)), this, SLOT(moveWidget(QPoint, QPoint)));
    
    //the zoom widget
    zoomWidget = new ZoomUtils;
    connect(zoomWidget, SIGNAL(zoom(double)), imageWidget, SLOT(makeZoom(double)));
    connect(zoomWidget, SIGNAL(zoom(double)), this, SLOT(updateWindowTitle()));
    
    //add actions to imageWidget; zoomin, zoomout, separator, normalSize, adjustedsize,
    //separator, file properties, etc
    imageWidget->addAction(actionsManager->action("_zoomIn"));
    imageWidget->addAction(actionsManager->action("_zoomOut"));
    QAction *action1 = new QAction(this);
    action1->setSeparator(true);
    imageWidget->addAction(action1);    
    imageWidget->addAction(actionsManager->action("_normalSize"));
    imageWidget->addAction(actionsManager->action("_adjustSize"));
    QAction *action2 = new QAction(this);
    action2->setSeparator(true);
    imageWidget->addAction(action2);
    imageWidget->addAction(actionsManager->action("_fileProperties"));
    imageWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    
    //configure and load settings
    settings = new Settings;
    this->loadSettings();
    
    //update window title
    this->updateWindowTitle();
    
    //show or hide menu bar
    this->showHideMenuBar();
    
    //configure toolbar and menu bar menus
    this->configureToolBarToolTip();
    
}

void MainWindow::pixmapChangedSlot()
{
    pixmapChanged = true;
}

void MainWindow::configureToolBarToolTip()
{
    tbMenu = new QMenu(tr("Toolbar preferences"));
    tbMenu->addAction(actionsManager->action("_showMenuBar"));
    tbMenu->addAction(actionsManager->action("_toolbarMovable"));
    tbMenu->addAction(actionsManager->action("_configureToolBar"));
    
    mainToolBar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mainToolBar, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(toolBarContextMenu()));
    menuBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->menuBar(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(toolBarContextMenu()));
    
}

void MainWindow::toolBarContextMenu()
{
    QPoint pos = this->cursor().pos();
    tbMenu->popup(pos);
}

void MainWindow::moveWidget(QPoint e, QPoint d)
{
    //don't ask, i just toke this method code from gwenview (lib/scrolltool) and i don't know how it works
    //of course, i had to make some tweaks
    int width = scrollArea->width();
    int height = scrollArea->height();
    
    QPoint mousePos = d;
    
    int deltaX = e.x() - mousePos.x();
    int deltaY = e.y() - mousePos.y();
    
    if (mousePos.y() <= 4 && scrollArea->verticalScrollBar()->value() < scrollArea->verticalScrollBar()->maximum() - 10) {
        // wrap mouse from top to bottom
        mousePos.setY(height - 5);
    } else if (mousePos.y() >= height - 4 && scrollArea->verticalScrollBar()->value() > 10) {
        // wrap mouse from bottom to top
        mousePos.setY(5);
    }
    
    if (mousePos.x() <= 4 && scrollArea->horizontalScrollBar()->value() < scrollArea->horizontalScrollBar()->maximum() - 10) {
        // wrap mouse from left to right
        mousePos.setX(width - 5);
    } else if (mousePos.x() >= width - 4 && scrollArea->horizontalScrollBar()->value() > 10) {
        // wrap mouse from right to left
        mousePos.setX(5);
    }
    
    scrollArea->horizontalScrollBar()->setValue(scrollArea->horizontalScrollBar()->value() + deltaX);
    scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->value() + deltaY);
}

void MainWindow::imageRotated()
{
    //if hte button adjust size ain't checked, then get out
    if(!actionsManager->action("_adjustSize")->isChecked()){
        return;
    }
    
    //clear sizes
    zoomWidget->clearSizes();
    
    //pass the new size
    zoomWidget->setPicSize(imageWidget->getPictureSize());
    
    //pass the maximimun size
    zoomWidget->setMaxSize(centralWidget()->size()/* - QSize(20,20*/);
    
    //and, if it's enabled, set the zoom widget value
    if(showZoomSlider){
        zoomSlider->setValue(zoomWidget->getZoomValue());
    }
    
    this->pixmapChangedSlot();
    updateActions();
}

void MainWindow::setLastPathUsed(QString newPath)
{
    lastDirUsed = newPath;
}

void MainWindow::openLastFile()
{
    //check if the file exists
    if(!fileUtils->openFile(recentFilesPath.at(0))){
        QMessageBox::information(this, tr("QIviewer"), tr("The file doesn't exist, may be it's been moved or deleted."),
                                 QMessageBox::Ok);
        recentFilesPath.removeOne(recentFilesPath.at(0));
        createRecentFilesMenu();
        return;
    }
    
    else{
        openImage();
    }
}

void MainWindow::openImageFromCommandLine(const QStringList d)
{
    //sets the file
    QFileInfo currentFile;
    currentFile.setFile(d.at(1));
    
    bool open;
    int i, j;
    i=0; j=nameFilters.size(); open = false;
    
    //check if the file format is included in the supported formats
    //in case it is, open=true, but open=false
    QString g;
    while(!open && i<j){
        g = nameFilters.at(i);
        g.remove('*');
        g.remove('"');
        if(currentFile.absoluteFilePath().indexOf(g) != -1){
            open = true;
            i = j;
        }
        i++;
    }
    
    //if open=true, the app explore the folder and open the image
    if(open && fileUtils->openFile(currentFile.absoluteFilePath())){
        openImage();
    }
    
    //if open=false, show a message
    else{
        QMessageBox notOpen;
        notOpen.setIcon(QMessageBox::Information);
        notOpen.setText(tr("QIviewer couldn't open the image, what do you want to do?"));
        notOpen.setStandardButtons(QMessageBox::Ok | QMessageBox::Open);
        notOpen.setDefaultButton(QMessageBox::Ok);
        int ret = notOpen.exec();
        switch (ret) {
            case QMessageBox::Ok:
                break;
            case QMessageBox::Open:
                this->open();
                break;
            default:
                // should never be reached
                break;
        }
    }
}

void MainWindow::fileProperties()
{
    FileProperties fp;
    fp.setFile(fileUtils->absoluteFilePath(), settings->useBinaryStyle(), settings->getFileSizePrecision());
    fp.setPreviewPixmap(imageWidget->getPixmap());
    fp.setPictureSize(imageWidget->getPictureSize());
    fp.exec();
}

void MainWindow::about()
{
    AboutDialog abd(this);
    abd.exec();
}

void MainWindow::moveToSlot()
{
    if(!this->canCloseImage()){
        return;
    }
    
    QString newFileName = QFileDialog::getExistingDirectory(this, tr("Move to"), getLastDir());
    if(fileUtils->moveFile(newFileName)){
        qDebug()<<"moved";
        recentFilesPath.removeAt(0);
        createRecentFilesMenu();
        openImage();
    }
}

void MainWindow::configureProgram()
{
    saveSettings();
    ConfigDialog con(this);
    con.setWindowIcon(QIcon::fromTheme("configure"));
    con.setWindowTitle(tr("Configuration"));
    connect(&con, SIGNAL(settingsSaved()), this, SLOT(loadSettings()));
    con.exec();
}

void MainWindow::print()
{
    //copy/paste from qt exaples :-P
    Q_ASSERT(imageWidget->getPixmap());
    #ifndef QT_NO_PRINTER
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageWidget->getPictureSize();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageWidget->getPixmap().rect());
        painter.drawPixmap(0, 0, imageWidget->getPixmap());
    }
    #endif
}

void MainWindow::goToSlot()
{
    GoToDialog gt(this);
    gt.setWindowIcon(QIcon::fromTheme("go-jump"));
    gt.setList(fileUtils->getFilesList());
    gt.setRange(1,fileUtils->getFilesAmount(), fileUtils->getCurrentPosition());
    connect(&gt, SIGNAL(goTo(QString,int)), this, SLOT(goToSlot(QString, int)));
    gt.exec();
}

void MainWindow::configureToolBarSlot()
{
    qDebug()<<"configure toolbars";
    EditToolBar etb;
    etb.setActionsList(actionsManager->getActionsMap(), settings->getActionsLoaded());
    etb.setDefaultActions(settings->defaultActions());
    connect(&etb, SIGNAL(actionsChanged(QStringList)), this, SLOT(configureToolBarSlot2(QStringList)));
    etb.exec();
}

void MainWindow::configureToolBarSlot2(QStringList l)
{
    settings->setActionsLoaded(l);
    setUpToolBar(l,Qt::ToolBarArea(settings->getTBArea()),
                 Qt::ToolButtonStyle(settings->getTBButtomStyle()),
                 settings->getTBVisible(),
                 showZoomSlider);
}

void MainWindow::configureShortcutsSlot()
{
    ShortcutEditor sce;
    sce.setActionsList(actionsManager->getActionsList());
    sce.setDefaultShortcuts(defaultShortcuts);
    connect(&sce, SIGNAL(newShortcuts(QMap<QString,QKeySequence>)), this, SLOT(configureNewShortcuts(QMap<QString,QKeySequence>)));
    sce.exec();
}

void MainWindow::configureNewShortcuts(QMap<QString, QKeySequence> d)
{
    settings->setCreateNewShortCuts(false);
    QMapIterator<QString, QKeySequence> i(d);
    while(i.hasNext()){
        i.next();
        actionsManager->setShortcut(i.key(), i.value());
    }
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    if(imageSetted){
        zoomWidget->maxSizeChanged(this->centralWidget()->size()/* - QSize(20,20)*/);
    }
    qDebug()<<"tamaño del widget central: "<<this->centralWidget()->size();
}

void MainWindow::setUpToolBar(QStringList sl, Qt::ToolBarArea a, Qt::ToolButtonStyle s, bool v, bool zl)
{
    //clear tool bar
    mainToolBar->clear();
    
    //make it visible, or not
    mainToolBar->setVisible(v);
    
    //create a list with all the ids
    QStringList actList = actionsManager->getIds();
    
    //add the actions
    for(int i=0; i < sl.size(); i++){
        //add a separator
        if(sl.at(i) == "_separator"){
            mainToolBar->addSeparator();
        }
        
        //add an action
        for(int j=0; j < actList.size(); j++){
            QString id = actList.at(j);
            if(id == sl.at(i)){
                //try{
                    mainToolBar->addAction(actionsManager->action(id));
                    /*}
                     *                catch(std::string e){
                     *                    qDebug()<<"While trying to add action: " + QString().fromStdString(e);
                     }*/
        }
    }
}

//add the toolbar to the mainWindow
addToolBar(a, mainToolBar);

//settning button style
mainToolBar->setToolButtonStyle(s);

//zoom slider stuff
if(zl){
    zoomSlider = new QSlider(Qt::Horizontal);
    zoomSlider->setMaximumSize(QSize(75, 25));
    zoomSlider->setRange(zoomWidget->getMinZoomValue(),
                         zoomWidget->getMaxZoomValue());
    connect(zoomWidget, SIGNAL(sliderSignal(int)), zoomSlider, SLOT(setValue(int)));
    connect(zoomSlider, SIGNAL(valueChanged(int)), zoomWidget, SLOT(sliderSlot(int)));
    mainToolBar->addSeparator();
    mainToolBar->addWidget(zoomSlider);
    zoomSlider->setEnabled(false);
}
}

void MainWindow::createMenus()
{
    recentFilesMenu = new QMenu(tr("&Recent images"), this);
    recentFilesMenu->setIcon(QIcon::fromTheme("document-open-recent"));
    
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(actionsManager->action("_open"));
    fileMenu->addAction(actionsManager->action("_openFolder"));
    fileMenu->addAction(actionsManager->action("_openLastFile"));
    fileMenu->addMenu(recentFilesMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(actionsManager->action("_save"));
    fileMenu->addAction(actionsManager->action("_saveAs"));
    fileMenu->addSeparator();
    fileMenu->addAction(actionsManager->action("_fileProperties"));
    fileMenu->addAction(actionsManager->action("_print"));
    fileMenu->addSeparator();
    fileMenu->addAction(actionsManager->action("_moveTo"));
    fileMenu->addAction(actionsManager->action("_deleteFile"));
    fileMenu->addAction(actionsManager->action("_closeFile"));
    fileMenu->addSeparator();
    fileMenu->addAction(actionsManager->action("_exit"));
    
    editMenu = new QMenu(tr("&Edit"), this);
    editMenu->addAction(actionsManager->action("_showMenuBar"));
    editMenu->addAction(actionsManager->action("_showToolBar"));
    editMenu->addAction(actionsManager->action("_toolbarMovable"));
    editMenu->addAction(actionsManager->action("_configuration"));
    editMenu->addAction(actionsManager->action("_configureToolBar"));
    editMenu->addAction(actionsManager->action("_configureShortcuts"));
    
    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(actionsManager->action("_zoomIn"));
    viewMenu->addAction(actionsManager->action("_zoomOut"));
    viewMenu->addSeparator();
    viewMenu->addAction(actionsManager->action("_normalSize"));
    viewMenu->addAction(actionsManager->action("_adjustSize"));
    
    imageMenu = new QMenu(tr("&Image"), this);
    imageMenu->addAction(actionsManager->action("_rotateLeft"));
    imageMenu->addAction(actionsManager->action("_rotateRight"));
    imageMenu->addSeparator();
    imageMenu->addAction(actionsManager->action("_flipVertically"));
    imageMenu->addAction(actionsManager->action("_flipHorizontally"));
    
    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(actionsManager->action("_about"));
    helpMenu->addAction(actionsManager->action("_aboutQt"));
    
    goMenu = new QMenu(tr("&Go"), this);
    goMenu->addAction(actionsManager->action("_next"));
    goMenu->addAction(actionsManager->action("_previous"));
    goMenu->addSeparator();
    goMenu->addAction(actionsManager->action("_goFirst"));
    goMenu->addAction(actionsManager->action("_goLast"));
    goMenu->addSeparator(),
    goMenu->addAction(actionsManager->action("_goTo"));
    
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(editMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(imageMenu);
    menuBar()->addMenu(goMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::createToolBar()
{
    mainToolBar = new QToolBar(tr("Main toolbar"));
    mainToolBar->setFloatable(false);
}

void MainWindow::updateActions()
{
    this->updateWindowTitle();
    actionsManager->setEnabled("_openLastFile", (recentFilesPath.count() > 0) && !imageSetted);
    actionsManager->setEnabled("_zoomIn" , imageSetted);
    actionsManager->setEnabled("_zoomOut" , imageSetted);
    actionsManager->setEnabled("_moveTo" , imageSetted);
    actionsManager->setEnabled("_print" , imageSetted);
    actionsManager->setEnabled("_deleteFile" , imageSetted);
    actionsManager->setEnabled("_save" , imageSetted && pixmapChanged && imageType != Dynamic);
    actionsManager->setEnabled("_saveAs" , imageSetted && imageType != Dynamic);
    actionsManager->setEnabled("_normalSize" , imageSetted);
    actionsManager->setEnabled("_adjustSize" , imageSetted);
    actionsManager->setEnabled("_rotateLeft" , imageSetted);
    actionsManager->setEnabled("_rotateRight" , imageSetted);
    actionsManager->setEnabled("_flipVertically" , imageSetted);
    actionsManager->setEnabled("_flipHorizontally" , imageSetted);
    actionsManager->setEnabled("_closeFile", imageSetted);
    if(showZoomSlider){zoomSlider->setEnabled(imageSetted);}
    actionsManager->setEnabled("_fileProperties" , imageSetted);
    
    bool g = (fileUtils->getFilesAmount() > 1) && imageSetted;
    actionsManager->setEnabled("_next" , g);
    actionsManager->setEnabled("_previous" , g);
    actionsManager->setEnabled("_goLast" , g);
    actionsManager->setEnabled("_goFirst" , g);
    actionsManager->setEnabled("_goTo" , g);
}

void MainWindow::createActions()
{
    /*! actions
     *     "_about" "_aboutQt" "_adjustSize" "_closeFile" "_configuration" "_configureShortcuts" "_configureToolBar"
     *     "_deleteFile" "_exit" "_fileProperties" "_flipHorizontally" "_flipVertically" "_goFirst" "_goLast"
     *     "_goTo" "_moveTo" "_next" "_normalSize" "_open" "_openFolder" "_openLastFile" "_previous" "_print"
     *     "_rotateLeft" "_rotateRight" "_save" "_showMenuBar" "_showToolBar" "_toolbarMovable" "_zoomIn" "_zoomOut"
     */
    
    actionsManager->addAction(new QAction(tr("&Open"), this), "_open", this, this, SLOT(open()), QKeySequence::Open);
    actionsManager->setToolTip("_open", tr("Open an image"));
    actionsManager->setActionIcon("_open", "document-open");
    
    actionsManager->addAction(new QAction(tr("&Save"), this), "_save", this, this, SLOT(save()), QKeySequence::Save);
    actionsManager->setEnabled("_save", false);
    actionsManager->setActionIcon("_save", "document-save");
    
    actionsManager->addAction(new QAction(tr("&Save as"), this), "_saveAs", this, this, SLOT(saveAs()));
    actionsManager->setEnabled("_saveAs", false);
    actionsManager->setActionIcon("_saveAs", "document-save-as");
    
    actionsManager->addAction(new QAction(tr("&Exit"), this), "_exit", this, this, SLOT(close()), QKeySequence::Quit);
    actionsManager->setActionIcon("_exit", "application-exit");
    
    actionsManager->addAction(new QAction(tr("Properties"), this), "_fileProperties", this, this, SLOT(fileProperties()), QKeySequence("Ctrl+Return"));
    actionsManager->setEnabled("_fileProperties", false);
    actionsManager->setActionIcon("_fileProperties", "document-properties");
    
    actionsManager->addAction(new QAction(tr("Zoom In"), this),  "_zoomIn", this, this, SLOT(zoomIn()), QKeySequence::ZoomIn);
    actionsManager->setEnabled("_zoomIn", false);
    actionsManager->setActionIcon("_zoomIn", "zoom-in");
    
    actionsManager->addAction(new QAction(tr("Zoom Out"), this),  "_zoomOut", this, this, SLOT(zoomOut()), QKeySequence::ZoomOut);
    actionsManager->setEnabled("_zoomOut", false);
    actionsManager->setActionIcon("_zoomOut", "zoom-out");
    
    actionsManager->addAction(new QAction(tr("Normal Size"), this),  "_normalSize", this, this, SLOT(normalSize()), QKeySequence("1"));
    actionsManager->setEnabled("_normalSize", false);
    actionsManager->setActionIcon("_normalSize", "zoom-original");
    
    actionsManager->addAction(new QAction(tr("Best Fit"), this), true, "_adjustSize", this, this, SLOT(adjustSizeSlot()), QKeySequence("0"));
    actionsManager->setEnabled("_adjustSize", false);
    actionsManager->setActionIcon("_adjustSize", "zoom-fit-best");
    
    actionsManager->addAction(new QAction(tr("Rotate to right"), this), "_rotateRight", this, this, SLOT(rotateRight()));
    actionsManager->setToolTip("_rotateRight", tr("Rotate image in the clockwise clock"));
    actionsManager->setEnabled("_rotateRight", false);
    actionsManager->setActionIcon("_rotateRight", "object-rotate-right");
    
    actionsManager->addAction(new QAction(tr("Rotate to Left"), this), "_rotateLeft", this, this, SLOT(rotateLeft()));
    actionsManager->setEnabled("_rotateLeft", false);
    actionsManager->setToolTip("_rotateLeft", tr("Rotate image counter-clockwise to clockwise"));
    actionsManager->setActionIcon("_rotateLeft", "object-rotate-left");
    
    actionsManager->addAction(new QAction(tr("Flip vertically"), this), "_flipVertically", this, this, SLOT(flipVertically()));
    actionsManager->setEnabled("_flipVertically", false);
    actionsManager->setToolTip("_flipVertically", tr("Turns vertically the image"));
    actionsManager->setActionIcon("_flipVertically", "object-flip-vertical");
    
    actionsManager->addAction(new QAction(tr("Flip horizontally"), this), "_flipHorizontally", this, this, SLOT(flipHorizontally()));
    actionsManager->setEnabled("_flipHorizontally", false);
    actionsManager->setToolTip("_flipHorizontally", tr("Reflects the image"));
    actionsManager->setActionIcon("_flipHorizontally", "object-flip-horizontal");
    
    actionsManager->addAction(new QAction(tr("A&bout"), this), "_about", this, this, SLOT(about()), QKeySequence::HelpContents);
    actionsManager->setActionIcon("_about", "help-about");
    
    actionsManager->addAction(new QAction(tr("About &Qt"), this), "_aboutQt", this, qApp, SLOT(aboutQt()));
    actionsManager->action("_aboutQt")->setIcon(QIcon(":/trolltech/qmessagebox/images/qtlogo-64.png"));
    
    actionsManager->addAction(new QAction(tr("Ne&xt"), this), "_next", this, this, SLOT(next()), Qt::Key_Right);
    actionsManager->setEnabled("_next", false);
    actionsManager->setActionIcon("_next", "go-next");
    
    actionsManager->addAction(new QAction(tr("Go to the first"), this), "_goFirst", this, this, SLOT(goFirst()), Qt::Key_Home);
    actionsManager->setEnabled("_goFirst", false);
    actionsManager->setActionIcon("_goFirst", "go-first");
    
    actionsManager->addAction(new QAction(tr("Pre&vious"), this), "_previous", this, this, SLOT(previous()), Qt::Key_Left);
    actionsManager->setEnabled("_previous", false);
    actionsManager->setActionIcon("_previous", "go-previous");
    
    actionsManager->addAction(new QAction(tr("Go to the last"), this), "_goLast", this, this, SLOT(goLast()), Qt::Key_End);
    actionsManager->setEnabled("_goLast", false);
    actionsManager->setActionIcon("_goLast", "go-last");
    
    actionsManager->addAction(new QAction(tr("Open &Folder"), this), "_openFolder", this, this, SLOT(openDir()), QKeySequence("Ctrl+Shift+O"));
    actionsManager->setToolTip("_openFolder", tr("Open a folder to explore images inside it"));
    actionsManager->setActionIcon("_openFolder", "folder-open");
    
    actionsManager->addAction(new QAction(tr("Show Menu Bar"), this), true, "_showMenuBar", this, this, SLOT(showHideMenuBar()), QKeySequence("Ctrl+M"));
    actionsManager->setActionIcon("_showMenuBar", "show-menu");
    
    actionsManager->addAction(new QAction(tr("Lock toolbar"), this), true, "_toolbarMovable", this, this, SLOT(setToolBarMovable()));
    actionsManager->setToolTip("_toolbarMovable", tr("Lock/unlock toolbar"));
    actionsManager->setActionIcon("_toolbarMovable", "configure-toolbars");
    
    actionsManager->addAction(new QAction(tr("Configuration"), this), "_configuration", this, this, SLOT(configureProgram()), QKeySequence::Preferences);
    actionsManager->setActionIcon("_configuration", "configure");
    
    deleteRecentFilesAct = new QAction(tr("Delete list"), this);
    deleteRecentFilesAct->setIcon(QIcon::fromTheme("edit-clear"));
    connect(deleteRecentFilesAct, SIGNAL(triggered()), this, SLOT(deleteRecentFiles()));
    
    actionsManager->addAction(new QAction(tr("Print"), this), "_print", this, this, SLOT(print()), QKeySequence::Print);
    actionsManager->setEnabled("_print", false);
    actionsManager->setActionIcon("_print", "document-print");
    
    actionsManager->addAction(new QAction(tr("Delete"), this), "_deleteFile", this, this, SLOT(deleteFileSlot()), QKeySequence::Delete);
    actionsManager->setEnabled("_deleteFile", false);
    actionsManager->setToolTip("_deleteFile", tr("This deletes completly the file from the disk, doesn't move it to the trash"));
    actionsManager->setActionIcon("_deleteFile", "edit-delete");
    
    actionsManager->addAction(new QAction(tr("Move to..."), this), "_moveTo", this, this, SLOT(moveToSlot()));
    actionsManager->setEnabled("_moveTo", false);
    actionsManager->setActionIcon("_moveTo", "none");
    
    actionsManager->addAction(new QAction(tr("Go to"), this), "_goTo", this, this, SLOT(goToSlot()), QKeySequence("Ctrl+J"));
    actionsManager->setEnabled("_goTo", false);
    actionsManager->setActionIcon("_goTo", "go-jump");
    
    actionsManager->addAction(new QAction(tr("Show toolbar"), this), true, "_showToolBar", this, this, SLOT(setToolBarVisible(bool)));
    actionsManager->setActionIcon("_showToolBar", "configure-toolbars");
    
    actionsManager->addAction(new QAction(tr("Configure toolbar"), this), "_configureToolBar", this, this, SLOT(configureToolBarSlot()));
    actionsManager->setActionIcon("_configureToolBar", "configure-toolbars");
    
    actionsManager->addAction(new QAction(tr("Configure shortcuts"), this), "_configureShortcuts", this, this, SLOT(configureShortcutsSlot()));
    actionsManager->setActionIcon("_configureShortcuts", "configure-shortcuts");
    
    actionsManager->addAction(new QAction(tr("Open last file"), this), "_openLastFile", this, this, SLOT(openLastFile()));
    actionsManager->setActionIcon("_openLastFile", "document-open-recent");
    
    actionsManager->addAction(new QAction(tr("&Close the file"), this), "_closeFile", this, this, SLOT(closeFileSlot()));
    actionsManager->setActionIcon("_closeFile", "document-close");
    
    //store default shortcuts
    defaultShortcuts = actionsManager->getShortcuts();
}

void MainWindow::closeFileSlot()
{
    qDebug()<<"close file slot";
    imageWidget->clear();
    fileUtils->clear();
    zoomWidget->clearSizes();
    imageSetted = false;
    pixmapChanged = false;
    this->updateActions();
}

void MainWindow::setToolBarVisible(bool d)
{
    mainToolBar->setVisible(d);
    
    //you can't move a tool bar if you don't see it, can you?
    actionsManager->setEnabled("_toolbarMovable", d);
}

void MainWindow::setToolBarMovable()
{
    mainToolBar->setMovable(!actionsManager->action("_toolbarMovable")->isChecked());
}

void MainWindow::showHideMenuBar()
{
    if(actionsManager->action("_showMenuBar")->isChecked()){
        menuBar()->show();
    }
    else{
        menuBar()->hide();
    }
}

void MainWindow::flipHorizontally()
{
    imageWidget->transformImage(ImageWidget::horizontally);
}

void MainWindow::flipVertically()
{
    imageWidget->transformImage(ImageWidget::vertically);
}

void MainWindow::rotateRight()
{
    imageWidget->transformImage(ImageWidget::toRight);
}

void MainWindow::rotateLeft()
{
    imageWidget->transformImage(ImageWidget::toLeft);
}

void MainWindow::deleteRecentFiles()
{
    //qDebug()<<"delete";
    //clean menu and actions
    recentFilesMenu->clear();
    for(int i=0; i<recentFilesAct.size(); i++){
        recentFilesAct.at(i)->~QAction();
    }
    recentFilesAct.clear();
    
    //add action to clear list
    recentFilesMenu->addAction(deleteRecentFilesAct);
    recentFilesMenu->addSeparator();
    
    //remove any previous coincidence
    recentFilesPath.clear();
    
    //update action
    this->updateActions();
    
    createRecentFilesMenu();
}

void MainWindow::deleteFileSlot()
{
    //ask if user really wants to delete the file
    int result = QMessageBox::question(this, tr("Delete"), tr("Do you want to delete %1?").arg(fileUtils->getFileName()),
                                       QMessageBox::Yes|QMessageBox::No);
    
    if (result == QMessageBox::No){
        return;
    }
    
    //notify the user the result
    if(fileUtils->deleteFile()){
        qDebug()<<tr("File deleted");
        QMessageBox::information(this, tr("QIviewer"), tr("File deleted"), QMessageBox::Ok);
    }
    
    else{
        QMessageBox::information(this, tr("QIviewer"), tr("The file couldn't be deleted"), QMessageBox::Ok);
    }
    
    //if the file list ain't empty, then load another image, usually the previous one
    //in the list before the image was deleted
    if(fileUtils->getFilesAmount() != 0){
        //removes the file from the list of recent files, and since it was the last one
        //opened, then remove the first in the list.
        recentFilesPath.removeAt(0);
        createRecentFilesMenu();
        openImage();
    }
    
    //clear the image view
    else{
        imageWidget->clear();
    }
}

void MainWindow::pushNewFile()
{
    //clean menu and actions
    recentFilesMenu->clear();
    for(int i=0; i<recentFilesAct.size(); i++){
        recentFilesAct.at(i)->~QAction();
    }
    recentFilesAct.clear();
    
    //remove any previous coincidence
    recentFilesPath.removeAll(fileUtils->absoluteFilePath());
    
    //add the file to the top of the list
    recentFilesPath.prepend(fileUtils->absoluteFilePath());
    
    //make sure the list doesn't have more files than the allowed
    if(recentFilesPath.size() > maxRecentFiles){
        recentFilesPath.removeLast();
    }
    
    createRecentFilesMenu();
}

void MainWindow::createRecentFilesMenu()
{
    while(recentFilesPath.size() > maxRecentFiles){
        recentFilesPath.removeLast();
    }
    
    //remove the files moved or deleted from the list
    QStringList temp;
    foreach(QString flag, recentFilesPath){
        if(QFileInfo(flag).exists()){
            temp<<flag;
        }
    }
    recentFilesPath = temp;
    
    //clean menu and actions
    recentFilesMenu->clear();
    for(int i=0; i<recentFilesAct.size(); i++){
        recentFilesAct.at(i)->~QAction();
    }
    
    //add action to clear list
    recentFilesMenu->addAction(deleteRecentFilesAct);
    recentFilesMenu->addSeparator();
    
    //enable/disable the menu
    recentFilesMenu->setEnabled(recentFilesPath.size() > 0);
    
    recentFilesAct.clear();
    if(recentFilesPath.size() > 0){
        for(int i=0; i < recentFilesPath.size(); i++){
            //creo y agrego un objeto QAction con un icono generico y el nombre del archivo
            recentFilesAct<<new QAction(QIcon::fromTheme("image-x-generic"), QFileInfo(recentFilesPath.at(i)).fileName(), this);
            
            //agrego la direccion completa a la data del QAction correspondiente
            recentFilesAct.at(i)->setData(recentFilesPath.at(i));
            
            //connecto la accion con el slot encargado de abrir las imagenes recientes
            connect(recentFilesAct.at(i), SIGNAL(triggered()), this, SLOT(openRecentFile()));
            
            //agrego la accion al menu
            recentFilesMenu->addAction(recentFilesAct.at(i));
        }
    }
}

QString MainWindow::getLastDir() const
{
    switch(pathToUse){
        case 0:
            return lastDirUsed;
            break;
        case 1:
            return defaultPath;
            break;
        default:
            return defaultPath;
            break;
    }
}

void MainWindow::setNameFilters()
{
    nameFilters << "*.png" << "*.pbm"
    << "*.jpg" << "*.jpeg"
    << "*.gif" << "*.tiff"
    << "*.xbm" << "*.JPG"
    << "*.bmp" << "*.ppm"
    << "*.pgm" << "*.mng"
    << "*.xpm"
    #ifdef WEBP_SUPPORT
    << "*.webp"
    #endif
    << "*.svg" << "*.svgz";
    nameFilters.removeDuplicates();
}

void MainWindow::saveImage(QString fileName)
{
    //if fileName is empty, then uses the current file name and replaces it
    if(fileName.isEmpty()){
        fileName = fileUtils->absoluteFilePath();
    }
    
    //check if imageWidget can save
    if(!imageWidget->canSave()){
        QMessageBox::information(this, tr("QIviewer"), tr("I couldn't save the image"), QMessageBox::Ok);
        return;
    }
    
    //try to save the image
    bool couldsave = imageWidget->getPixmap().save(fileName, 0, compressLevel);
    
    //if succed, then open it
    if(couldsave && fileUtils->openFile(fileName)){
        pixmapChanged = false;
        this->openImage();
        this->updateActions();
        qDebug()<<tr("Image saved");
    }
}

void MainWindow::save()
{
    this->saveImage();
}

void MainWindow::saveAs()
{   
    //create a list with the formas that can be used as output
    QStringList g = nameFilters;
    g.removeAll("*.gif");
    g.removeAll("*.mng");
    g.removeAll("*.pbm");
    g.removeAll("*.pgm");
    g.removeAll("*.svg");
    g.removeAll("*.svgz");
    #ifdef WEBP_SUPPORT
    g.removeAll("*.webp");
    #endif
    
    //call the save dialog
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    fileUtils->getFilePath(),
                                                    tr("Image Files (%1)").arg(g.join(" ")));
    
    //save the image
    this->saveImage(fileName);
}

void MainWindow::openDir()
{
    //check if wanna discard changes
    if(!this->canCloseImage()){
        return;
    }
    
    //get new path
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open folder"),
                                                        getLastDir(), QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    
    //spam to the terminal
    qDebug()<<"folder"<<dirName;
    
    //if the path name is empty then get out
    if(dirName.isEmpty()){
        qDebug()<<tr("No folder selected");
        return;
    }
    
    //if succes opening the folder, then open the first image inside it
    if(fileUtils->openFolder(dirName)){
        pixmapChanged = false;
        openImage();
    }
    
    //show a message
    else{
        QMessageBox::information(this, tr("QIviewer"), tr("No folder selected, i can't do anything"),
                                 QMessageBox::Ok);
    }
}

bool MainWindow::canCloseImage()
{
    if(!pixmapChanged){
        return true;
    }
    
    else{
        int g = QMessageBox::information(this, tr("Image changed"),
                                         tr("The image has changed. Do you want to "
                                         "save it before exit?"),
                                         QMessageBox::Save, QMessageBox::Discard, QMessageBox::Cancel);
                                         switch(g){
                                             case QMessageBox::Save:
                                                 this->saveAs();
                                                 break;
                                             case QMessageBox::Cancel:
                                                 return false;
                                         }
                                         
                                         pixmapChanged = false;
    }
    
    return true;
}

void MainWindow::goToSlot(QString s, int g)
{
    //check if wanna discard changes
    if(!this->canCloseImage()){
        return;
    }
    
    bool d = fileUtils->goToFile(s,g);
    if(!d){
        qDebug()<<"the file doesn't exists";
    }
    
    else{
        this->openImage();
    }
}

void MainWindow::openRecentFile()
{
    //check if wanna discard changes
    if(!this->canCloseImage()){
        return;
    }
    
    /*i don't have a clue of how the next stuff wokrs,
     *i just copy/pasted it from the qt examples :-P*/
    QString temp;
    QAction *action = qobject_cast<QAction *>(sender());
    if (action){
        temp = action->data().toString();
    }
    
    //check if the file exists
    if(!fileUtils->openFile(temp)){
        QMessageBox::information(this, tr("QIviewer"), tr("The file doesn't exist, may be it's been moved or deleted."),
                                 QMessageBox::Ok);
        recentFilesPath.removeOne(temp);
        createRecentFilesMenu();
        return;
    }
    
    else{
        openImage();
    }
}

void MainWindow::open()
{
    //check if wanna discard changes
    if(!this->canCloseImage()){
        return;
    }
    
    //get a filename from a open dialog
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open image", "dialog to open file"),
                                                    getLastDir(),
                                                    tr("Image Files (%1)").arg(nameFilters.join(" ")));

    //if the filename name isn't empty, open the file
    if(!(fileName.isEmpty())){
        if(fileUtils->openFile(fileName)){
            pixmapChanged = false;
            openImage();
        }

        else{
            QMessageBox::information(this, tr("QIviewer"), tr("QIviewer couldn't open the image."),
                                     QMessageBox::Ok);
        }
    }
}

void MainWindow::goFirst()
{
    //check if wanna discard changes
    if(!this->canCloseImage()){
        return;
    }
    
    if(fileUtils->goToFile(FileUtils::First)){
        openImage();
    }
}

void MainWindow::goLast()
{
    //check if wanna discard changes
    if(!this->canCloseImage()){
        return;
    }
    
    if(fileUtils->goToFile(FileUtils::Last)){
        openImage();
    }
}

void MainWindow::next()
{
    //check if wanna discard changes
    if(!this->canCloseImage()){
        return;
    }
    
    if(fileUtils->goToFile(FileUtils::Next)){
        openImage();
    }
}

void MainWindow::previous()
{
    //check if wanna discard changes
    if(!this->canCloseImage()){
        return;
    }
    
    if(fileUtils->goToFile(FileUtils::Previous)){
        openImage();
    }
}

void MainWindow::openStatic()
{
    imageWidget->setPicture(fileUtils->absoluteFilePath());
    zoomWidget->clearSizes();
    zoomWidget->setPicSize(imageWidget->getPictureSize());
    zoomWidget->setMaxSize(centralWidget()->size()/* - QSize(20,20)*/);
    if(showZoomSlider){
        zoomSlider->setValue(zoomWidget->getZoomValue());
    }
    updateActions();
}

void MainWindow::openDynamic()
{
    imageWidget->setMovie(fileUtils->absoluteFilePath());
    zoomWidget->clearSizes();
    zoomWidget->setPicSize(imageWidget->getPictureSize());
    zoomWidget->setMaxSize(centralWidget()->size()/* - QSize(20,20)*/);
    if(showZoomSlider){
        zoomSlider->setValue(zoomWidget->getZoomValue());
    }
    updateActions();
}

void MainWindow::openImage()
{
    determinateImageType();
    
    this->setCursor(Qt::BusyCursor);
    switch(imageType){
        case Static:
            openStatic();
            break;
        case Dynamic:
            openDynamic();
            break;
        case Webp:
            openStatic();
    }
    
    this->setCursor(Qt::ArrowCursor);
    this->pushNewFile();
    this->updateWindowTitle();
}

void MainWindow::determinateImageType()
{
    #ifdef WEBP_SUPPORT
    if(fileUtils->getFileName().indexOf(".webp") != -1){
        imageType = Webp;
        return;
    }
    #endif
    
    QMovie m(fileUtils->absoluteFilePath());
    if(m.isValid()){
        if(m.frameCount() > 1){
            imageType = Dynamic;
        }
        else{
            imageType = Static;
        }
    }
}

void MainWindow::updateWindowTitle()
{
    if(fileUtils->getFilesAmount() == 0){
        this->setWindowTitle(tr("QIviewer"));
    }
    else{
        QString windowTitleFormat = QString("appName :: pos/amount - zoomLevel - imageName");
        windowTitleFormat.replace("appName", "QIviewer");
        windowTitleFormat.replace("imageName", fileUtils->getFileName());
        windowTitleFormat.replace("pos", QString("%1").arg(fileUtils->getCurrentPosition()+1));
        windowTitleFormat.replace("amount", QString("%1").arg(fileUtils->getFilesAmount()));
        windowTitleFormat.replace("zoomLevel", QString("%1%").arg(zoomWidget->getZoomValue()));
        this->setWindowTitle(windowTitleFormat);
    }
    
}

void MainWindow::couldOpen(bool d)
{
    imageSetted = d;
}

void MainWindow::wheelZoom(int d)
{
    if(!imageSetted){
        return;
    }
    
    if(d > 0){
        zoomWidget->zoomIn();
    }
    
    if(d < 0){
        zoomWidget->zoomOut();
    }
    
    actionsManager->setChecked("_adjustSize", zoomWidget->bestZoomValue());
}

void MainWindow::zoomOut()
{
    zoomWidget->zoomOut();
    actionsManager->setChecked("_adjustSize", zoomWidget->bestZoomValue());
}

void MainWindow::adjustSizeSlot()
{
    zoomWidget->setAdjustedSize(actionsManager->action("_adjustSize")->isChecked());
}

void MainWindow::zoomIn()
{
    zoomWidget->zoomIn();
    actionsManager->setChecked("_adjustSize", zoomWidget->bestZoomValue());
}

void MainWindow::normalSize()
{
    zoomWidget->normalSize();
    actionsManager->setChecked("_adjustSize", zoomWidget->bestZoomValue());
    zoomWidget->setAdjustedSize(false);
}

void MainWindow::getColorFromSettings(QStringList d)
{
    imageWidget->setBackgroundColor(QColor(d.at(0).toInt(),
                                           d.at(1).toInt(),
                                           d.at(2).toInt()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void MainWindow::loadSettings()
{
    settings->loadSettings();
    
    /*main app settings*/
    resize(settings->getWindowSize());
    getColorFromSettings(settings->getBackgroundColor());
    actionsManager->setChecked("_showMenuBar", settings->getShowMenuBar());
    actionsManager->setChecked("_adjustSize", settings->getLoadFixedSize());
    zoomWidget->setZoomIncrement(settings->getZoomIncrement());
    zoomWidget->setAdjustedSize(settings->getLoadFixedSize());
    imageWidget->setBackgroundType(settings->getBGToShow());
    imageWidget->setCBSSize(settings->getSquaresSize());
    imageWidget->setRestartWhenZooming(settings->getRestartWhenZooming());
    imageWidget->setMovieSpeed(settings->getMovieSpeed());
    imageWidget->setStopWhenFinish(settings->getStopMovieWhenFinish());
    imageWidget->invertColors(settings->imageJokes());
    showZoomSlider  = settings->getShowZoomSlider();
    compressLevel   = settings->getCompressLevel();
    maxRecentFiles  = settings->getMaxRecentFiles();
    recentFilesPath = settings->getRecentFilesList();    
    fileUtils->setSorting(FileUtils::Order(settings->getSorting()));
    this->createRecentFilesMenu();
    this->adjustSizeSlot();
    this->showHideMenuBar();
    
    //tool bar settings
    mainToolBar->setMovable(settings->getTBMovable());
    actionsManager->setChecked("_toolbarMovable", !settings->getTBMovable());
    actionsManager->setChecked("_showToolBar", settings->getTBVisible());
    actionsManager->setEnabled("_toolbarMovable", settings->getTBVisible());
    setUpToolBar(settings->getActionsLoaded(),
                 Qt::ToolBarArea(settings->getTBArea()),
                 Qt::ToolButtonStyle(settings->getTBButtomStyle()),
                 settings->getTBVisible(),
                 showZoomSlider);
    
    //last dir used
    lastDirUsed = settings->getLastDirUsed();
    defaultPath = settings->getDefaultPath();
    pathToUse   = settings->getPathToUse();
    
    if(!settings->getCreateNewShortCuts()){
        actionsManager->setNewShortcuts(settings->getShortcuts());
    }
    
    this->updateActions();
}

void MainWindow::saveSettings()
{
    //much more clear than loadSettings(), don't you think?
    settings->setTBButtomStyle(mainToolBar->toolButtonStyle());
    settings->setTBArea(mainToolBar->allowedAreas());
    settings->setLoadFixedSize(actionsManager->action("_adjustSize")->isChecked());
    settings->setWindowSize(this->size());
    settings->setRecentFilesList(recentFilesPath);
    settings->setShowMenuBar(actionsManager->action("_showMenuBar")->isChecked());
    settings->setTBMovable(mainToolBar->isMovable());
    settings->setTBArea(int(this->toolBarArea(mainToolBar)));
    settings->setTBVisible(mainToolBar->isVisible());//necesary?
    settings->setLastDirUsed(lastDirUsed);
    if(!settings->getCreateNewShortCuts()){
        settings->setShortcuts(actionsManager->getShortcuts());
    }
    settings->saveSettings();
}
