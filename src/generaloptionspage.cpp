#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <QtGui/QColorDialog>

#include "generaloptionspage.h"
#include "settings.h"
#include "fileutils.h"
#include "ui_generaloptionspage.h"
#include "utils.h"

GeneralOptionsPage::GeneralOptionsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeneralOptionsPage)
{
    //load ui
    ui->setupUi(this);

    //load settings
    settings = new Settings;
    settings->loadSettings();

    //eggs dialog
    QString eggsDialog;
    eggsDialog.append("<p>" + tr("To configure the easter eggs, you have to execute:") + " ");
    eggsDialog.append("<code>qiviewer -e</code></p>");
    ui->configureEggsLabel->setText(eggsDialog);
    ui->configureEggsLabel->setVisible(settings->useEggs());

    //files sorting
    ui->orderByComboBox->addItem(tr("Name"), FileUtils::Name);
    ui->orderByComboBox->addItem(tr("Type"), FileUtils::Type);
    ui->orderByComboBox->addItem(tr("Size"), FileUtils::Size);
    ui->orderByComboBox->addItem(tr("Time"), FileUtils::Time);
    ui->orderByComboBox->setCurrentIndex(settings->getSorting());

    //zoom increment
    ui->zoomIncrementSPB->setValue(settings->getZoomIncrement());

    //configure image saving compress level spinbox
    ui->compressLevelSPB->setValue(settings->getCompressLevel());

    //precision numbre
    ui->precisionNumberSpinBox->setValue(settings->getFileSizePrecision());

    //animated images speed spinbox
    ui->movieSpeedSPB->setValue(settings->getMovieSpeed());

    //show menu bar
    ui->showMenuBarButton->setChecked(settings->getShowMenuBar());

    //stop animated image when reachs the last frame
    ui->stopWhenFinishButton->setChecked(settings->getStopMovieWhenFinish());

    //restart animated image reproduccion when zooming
    ui->restartWhenZoomingButton->setChecked(settings->getRestartWhenZooming());

    //set max recent files list number
    ui->recentFilesNumberSPB->setValue(settings->getMaxRecentFiles());

    //squares size spin box number
    ui->squaresSizeSPB->setValue(settings->getSquaresSize());

    //show zoom slider
    ui->showZoomSliderButton->setChecked(settings->getShowZoomSlider());

    //color background
    getColorFromSettings(settings->getBackgroundColor());

    //making the tabs a little bit more nicer
    ui->tabs->setTabIcon(0, QIcon::fromTheme("preferences-other"));
    ui->tabs->setTabIcon(1, QIcon::fromTheme("image-x-generic"));
    ui->tabs->setTabIcon(2, QIcon::fromTheme("video-x-generic"));

    //sets the combobox
    ui->BGTypeComboBox->setCurrentIndex(getBGSelection(0));
    ui->BGTypeComboBox->setToolTip(tr("With this you select the kind of background you want \n"
                                     "to use for images with alpha channel."));

    //configuring the color button
    ui->colorButton->setIcon(QIcon(iconForButton()));

    //the squares size button
    ui->squaresSampleLabel->setScaledContents(true);
    connect(ui->squaresSizeSPB, SIGNAL(valueChanged(int)), this, SLOT(paintSquaresSample(int)));
    paintSquaresSample(ui->squaresSizeSPB->value());

    //enabling the background buttons
    if(settings->getBGToShow() == "squares"){
        bgComboBoxSlot(0);
    }
    if(settings->getBGToShow() == "solidColor"){
        bgComboBoxSlot(1);
    }
    if(settings->getBGToShow() == "none"){
        bgComboBoxSlot(2);
    }

    //delete recent files list
    if (settings->getRecentFilesList().count() == 0){
      ui->deletRecentFiles->setVisible(false);
    }

    else
    {
      QString drfl;
      drfl.append("<p><a href=\"delete\">");
      drfl.append(tr("Delete list"));
      drfl.append("</a></p>");
      ui->deletRecentFiles->setText(drfl);
    }

    //making connections
    connect(ui->compressLevelSPB, SIGNAL(valueChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui->showMenuBarButton, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->showZoomSliderButton, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->stopWhenFinishButton, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->restartWhenZoomingButton, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->BGTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui->orderByComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui->precisionNumberSpinBox, SIGNAL(valueChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui->colorButton, SIGNAL(clicked(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui->BGTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(bgComboBoxSlot(int)));
    connect(ui->colorButton, SIGNAL(clicked(bool)), this, SLOT(selectColorSlot()));
    connect(ui->recentFilesNumberSPB, SIGNAL(valueChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui->movieSpeedSPB, SIGNAL(valueChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui->squaresSizeSPB, SIGNAL(valueChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui->zoomIncrementSPB, SIGNAL(valueChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui->deletRecentFiles, SIGNAL(linkActivated(QString)), this, SLOT(deleteRecentFilesList(QString)));
    connect(ui->deletRecentFiles, SIGNAL(linkActivated(QString)), this, SLOT(settingsChangedSlot()));
    settingsChangedBool = false;

}

GeneralOptionsPage::~GeneralOptionsPage()
{
    delete ui;
}

void GeneralOptionsPage::saveSettings()
{
    qDebug()<<"save settings";
    if(settingsChangedBool){
        settings->setBackgroundColor(setColorSettings());
        settings->setCompressLevel(ui->compressLevelSPB->value());
        settings->setSorting(ui->orderByComboBox->currentIndex());
        settings->setMaxRecentFiles(ui->recentFilesNumberSPB->value());
        settings->setMovieSpeed(ui->movieSpeedSPB->value());
        settings->setRestartWhenZooming(ui->restartWhenZoomingButton->isChecked());
        settings->setShowMenuBar(ui->showMenuBarButton->isChecked());
        settings->setShowZoomSlider(ui->showZoomSliderButton->isChecked());
        settings->setSquaresSize(ui->squaresSizeSPB->value());
        settings->setStopMovieWhenFinish(ui->stopWhenFinishButton->isChecked());
        settings->setZoomIncrement(ui->zoomIncrementSPB->value());
        settings->setFileSizePrecision(ui->precisionNumberSpinBox->value());
        getBGSelection(1);
        settings->saveSettings();
        settingsChangedBool = false;
    }
}

void GeneralOptionsPage::paintSquaresSample(int d)
{
    qDebug()<<"paint squares smaple:"<<d;

    QPixmap n = QPixmap(34, 34);
    QPainter q(&n);
    q.drawTiledPixmap(0, 0, 34, 34, Utils::chessBoard(d));
    q.end();

    ui->squaresSampleLabel->setPixmap(n);
}

void GeneralOptionsPage::bgComboBoxSlot(int d)
{
    qDebug()<<"bg combo bos slot:"<<d;
    switch (d)
    {
      case 0:
        //qDebug()<<"Chesboard.";
        ui->colorButton->setEnabled(false);
        ui->colorLabel->setEnabled(false);
        ui->squaresSizeSPB->setEnabled(true);
        ui->squaresSizeLabel->setEnabled(true);
        ui->squaresSampleLabel->setEnabled(true);
        break;
      case 1:
        //qDebug()<<"Color.";
        ui->colorButton->setEnabled(true);
        ui->colorLabel->setEnabled(true);
        ui->squaresSizeSPB->setEnabled(false);
        ui->squaresSizeLabel->setEnabled(false);
        ui->squaresSampleLabel->setEnabled(false);
        break;
      case 2:
        //qDebug()<<"none.";
        ui->colorButton->setEnabled(false);
        ui->colorLabel->setEnabled(false);
        ui->squaresSizeSPB->setEnabled(false);
        ui->squaresSizeLabel->setEnabled(false);
        ui->squaresSampleLabel->setEnabled(false);
        break;
    }
}

QStringList GeneralOptionsPage::setColorSettings()
{
  QStringList g;
  g << QString("%1").arg(color.red());
  g << QString("%1").arg(color.green());
  g << QString("%1").arg(color.blue());
  return g;
}

void GeneralOptionsPage::getColorFromSettings(QStringList d)
{
  color = QColor(d.at(0).toInt(),
                 d.at(1).toInt(),
                 d.at(2).toInt());
}

QPixmap GeneralOptionsPage::iconForButton()
{
  QPixmap pi(ui->colorButton->sizeHint());
  pi.fill(color);
  return pi;
}

/*
 * if d == 0-> returns the type of selected background
 * if d == 1-> sets bgToShow to the selected item in the comboBox widget
 */
int GeneralOptionsPage::getBGSelection(int d)
{
  switch (d)
  {
    case 0:
      if (settings->getBGToShow() == "squares")
      {
        return 0;
      }
      if (settings->getBGToShow() == "solidColor")
      {
        return 1;
      }
      if (settings->getBGToShow() == "none")
      {
        return 2;
      }
      break;

    case 1:
      switch (ui->BGTypeComboBox->currentIndex())
      {
        case 0:
          settings->setBGTosShow(QString("squares"));
          break;
        case 1:
          settings->setBGTosShow(QString("solidColor"));
          break;
        case 2:
          settings->setBGTosShow(QString("none"));
          break;
      }
  }
  return 0;
}

void GeneralOptionsPage::settingsChangedSlot()
{
    qDebug()<<"Settings changed slot gop";
    settingsChangedBool = true;
    emit settingsChanged();
}

void GeneralOptionsPage::selectColorSlot()
{
    qDebug()<<"select color slot";
    color = QColorDialog::getColor(color);
    ui->colorButton->setIcon(QIcon(iconForButton()));
}

void GeneralOptionsPage::deleteRecentFilesList(QString d)
{
    qDebug()<<"delete recent files list:"<<d;

    QString drfl;
    if (d == "delete")
    {
      drfl.append("<p><a href=\"restore\">");
      drfl.append(tr("Restore list"));
      rflBack = settings->getRecentFilesList();
      settings->setRecentFilesList(QStringList());
    }
    if (d == "restore")
    {
      drfl.append("<p><a href=\"delete\">");
      drfl.append(tr("Delete list"));
      settings->setRecentFilesList(rflBack);
    }
    drfl.append("</a></p>");
    ui->deletRecentFiles->setText(drfl);
}
