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
#include "imagewidget.h"
#include "utils.h"
#ifdef WEBP_SUPPORT
#include "webpdecoder.h"
#endif

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent)
{
    //default bacground type is chessboard, i like it :P
    backType = ImageWidget::squares;

    //default color is something like grey
    backColor = QColor(104, 102, 101);

    //this size of the squares i think is the best
    cbsSize = 16;

    //i don't know why i did this
    originalSize = QSize(-1,-1);

    //scaleFactor;
    scaleFactor = 1;

    //me cago en la mier*, i mean, setting the bug size
    bugSize = QSize();

    //eType
    eType = ImageWidget::None;

    //restar animation when zooming
    restartWhenZooming = false;

    //stop when movie get its final frame
    stopWhenFinish = false;

    //movie angle and orientation
    this->defaulTransMovie();

    //movieSpeed
    movieSpeed = 100;//200 = 2x, 100 = 1, 50 = 0.5x

    //
    move=false;

    //use eggs
    invertedColors = false;

    //configure imageLabel
    imageLabel = new QLabel;
    imageLabel->setScaledContents(true);

    //configure movie object
    movie = new QMovie;
    connect(movie, SIGNAL(frameChanged(int)), this, SLOT(movieSlot()));

    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(imageLabel);
    this->setLayout(mainLayout);
}

void ImageWidget::egg()
{
    QImage image = pixmap.toImage();
    QSize d = image.size();
    image.invertPixels();

    QPainter p(&image);
    p.setPen(Qt::blue);
    int k = QTime::currentTime().hour();
    k = k%2;
    switch(k){
    case 0:
        p.drawEllipse(0,0,d.width(),d.height());
        p.drawLine(0,0,d.width(),d.height());
        p.drawLine(0,d.height(),d.width(),0);
        p.drawLine(0,d.height()/2,d.width(),d.height()/2);
        p.drawLine(d.width()/2,0,d.width()/2,d.height());
        p.drawLine(0,0,d.width(),0);
        p.drawLine(d.width()-1,0,d.width()-1,d.height());
        p.drawLine(0,0,0,d.height());
        p.drawLine(0,d.height()-1,d.width(),d.height()-1);
        break;
    case 1:
        QFont f;
        f.setPixelSize(40);
        p.setFont(f);
        p.drawText(0,0,d.width(),d.height(),Qt::AlignJustify|Qt::TextWordWrap,tr("I own your OS"));
    }


    p.end();

    pixmap = QPixmap().fromImage(image);
}

void ImageWidget::resizeEvent(QResizeEvent *e)
{
    qDebug()<<"tamaño de image widget: "<<e->size();
}

void ImageWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        move = true;
        point = e->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void ImageWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(!move){
        return;
    }

    //QPoint g = e->pos() - point;
    //point = e->pos();
    emit moveWidget(point, e->pos());
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *)
{
    if(!move){
        return;
    }

    setCursor(Qt::ArrowCursor);
}

QString ImageWidget::getBGType() const
{
    switch(backType){
    case squares:
        return "squares";
        break;
    case solidColor:
        return "solidColor";
        break;
    case none:
        return "none";
        break;
    }

    return "squares";
}

bool ImageWidget::canSave() const
{
    if(eType == ImageWidget::Picture){
        return true;
    }

    return false;
}

QSize ImageWidget::getPictureSize() const
{
    switch(eType){
    case ImageWidget::Picture:
        if(!pixmap.isNull()){
            return pixmap.size();
        }
        else{
            return QSize(-1,-1);
        }
        break;
    case ImageWidget::Movie:
        if(movie->isValid()){
            return movie->currentPixmap().size();
        }
        else{
            return QSize(-1,-1);
        }
        break;
    default:
        return QSize(-1,-1);
        break;
    }

    return QSize();
}

void ImageWidget::setBackgroundType(QString d)
{
    if(d == "squares"){
        backType = ImageWidget::squares;
    }
    
    else if(d == "solidColor"){
        backType = ImageWidget::solidColor;
    }
    
    else if(d == "none"){
        backType = ImageWidget::none;
    }
    
    else{
        //i like the squares background, that's the reason it's the default one
        backType = ImageWidget::squares;
    }
}

void ImageWidget::clear()
{
    originalSize = QSize(-1,-1);
    eType = ImageWidget::None;
    movieSpeed = 100;
    move = false;
    movie->stop();
    imageLabel->clear();
    imageLabel->resize(QSize(0,0));
    pixmap = QPixmap();
    defaulTransMovie();
    scaleFactor = 1;
    this->adjustSize();
}

void ImageWidget::setPicture(const QString name)
{
    clear();
    eType = ImageWidget::Picture;

#ifdef WEBP_SUPPORT
    if(name.indexOf(".webp") != -1){
        WebpDecoder wepI;
        wepI.setFile(name);
        pixmap = wepI.getPixmap();
    }
    else
        pixmap = QPixmap(name);
#else
    if(name.indexOf(".webp") != -1)
        emit couldOpen(false);

    else
        pixmap = QPixmap(name);
#endif

    if(invertedColors){
        egg();
    }

    drawPixmap();
    emit couldOpen(true);
    prevFile = name;
}

void ImageWidget::setMovie(const QString d)
{
    clear();
    eType = ImageWidget::Movie;
    movie->setFileName(d);
    movie->start();
    emit couldOpen(true);
    prevFile = d;
}

void ImageWidget::makeZoom(double f)
{
    if(eType == ImageWidget::Movie && restartWhenZooming){
        movie->stop();
    }

    scaleFactor = f;

    if(f==1){
        imageLabel->adjustSize();
        this->adjustSize();
    }

    else{
        imageLabel->resize(pixmap.size() * f);
        this->resize((pixmap.size() + bugSize) * f);
    }

    if(eType == ImageWidget::Movie && restartWhenZooming){
        movie->start();
    }
}

void ImageWidget::setMovieSpeed(int d)
{
    movieSpeed = d;
    movie->setSpeed(movieSpeed);
}

void ImageWidget::movieSlot()
{
    if(stopWhenFinish && (movie->currentFrameNumber() == movie->frameCount() - 1)){
        movie->stop();
    }

    pixmap = movie->currentPixmap();
    makeDynamicTransformation();

    if(invertedColors){
        egg();
    }

    drawPixmap();

    if(scaleFactor==1){
        scaleFactor = 1;
        imageLabel->adjustSize();
        this->adjustSize();
        return;
    }

    imageLabel->resize(pixmap.size() * scaleFactor);
    this->resize((pixmap.size() + bugSize) * scaleFactor);
}

void ImageWidget::makeDynamicTransformation()
{
    if(movieFlipX != 1 || movieFlipY != 1){
        pixmap = pixmap.transformed(QTransform().scale(movieFlipX, movieFlipY));
    }
    if(movieAngle != 0){
        pixmap = pixmap.transformed(QTransform().rotate(movieAngle));
    }
}

void ImageWidget::drawPixmap()
{
    if(pixmap.isNull()){
        emit couldOpen(false);
        return;
    }

    originalSize = pixmap.size();
    imageLabel->setPixmap(pixmap);
}

void ImageWidget::defaulTransMovie()
{
    movieAngle = 0;
    movieFlipX = 1;
    movieFlipY = 1;
}

void ImageWidget::calculateNewState(Transform d)
{
    qDebug()<<"I've founded a book where i've learned to manipulate "
              "animated images, so i can do it now ^_^";
    switch(d){
    case toLeft:
        movieAngle -= 90;
        break;
    case toRight:
        movieAngle += 90;
        break;
    case horizontally:
        movieFlipX = -movieFlipX;
        break;
    case vertically:
        movieFlipY = -movieFlipY;
        break;
    default:
        qDebug()<<"chan";//a little bit of craziness
        break;
    }

    if(double(movieAngle / 360) == 1){
        movieAngle = 0;
    }

    if( abs(movieAngle) == 90 || abs(movieAngle) == 270 ){
        switch(d){
        case toLeft:
            //nothing needed here, just added to see the message below
            qDebug()<<"ImageWidget/calculateNewState->toRight";
            break;
        case toRight:
            qDebug()<<"ImageWidget/calculateNewState->toRight";
            break;
        case horizontally:
            movieFlipX = -movieFlipX;
            movieFlipY = -movieFlipY;
            break;
        case vertically:
            movieFlipX = -movieFlipX;
            movieFlipY = -movieFlipY;
            break;
        }
    }

    //i can't save animated images, so i don't have to do this
    //emit pixmapChanged();
}

void ImageWidget::makeTransformation(Transform d)
{
    QTransform transformation;
    QSize f = pixmap.size();

    switch(d){
    case toLeft:
        transformation.rotate(-90);
        pixmap = pixmap.transformed(transformation);
        imageLabel->setPixmap(pixmap);
        f.transpose();
        imageLabel->resize(f);
        this->adjustSize();
        emit picSizeChanged();
        break;
    case toRight:
        transformation.rotate(90);
        pixmap = pixmap.transformed(transformation);
        imageLabel->setPixmap(pixmap);
        f.transpose();
        imageLabel->resize(f);
        this->adjustSize();
        emit picSizeChanged();
        break;
    case horizontally:
        transformation.scale(-1,1);
        pixmap = pixmap.transformed(transformation);
        imageLabel->setPixmap(pixmap);
        break;
    case vertically:
        transformation.scale(1,-1);
        pixmap = pixmap.transformed(transformation);
        imageLabel->setPixmap(pixmap);
        break;
    }

    emit pixmapChanged();

    if(scaleFactor != 1){
        imageLabel->resize(pixmap.size() * scaleFactor);
        this->resize((pixmap.size() + bugSize) * scaleFactor);
    }

}

void ImageWidget::transformImage(Transform d)
{
    if(eType != Movie)
    {
        makeTransformation(d);
    }
    else{
        calculateNewState(d);
    }
}

/* FIXME
  The reason to add  + bugSize to all the resizing
  funtions is becouse for some reason that i don't know
  there's a difference between the imageLabel size
  and the widget size, so i need to add this
  */

void ImageWidget::paintEvent(QPaintEvent *)
{
    if(pixmap.hasAlphaChannel()){
        QPainter p(this);
        switch(int(backType)){
        case ImageWidget::squares:
            p.drawTiledPixmap(rect(), Utils::chessBoard(cbsSize));
            break;
        case ImageWidget::solidColor:
            p.fillRect(rect(), backColor);
            break;
        case ImageWidget::none:
            break;
        }
    }
}

void ImageWidget::wheelEvent(QWheelEvent *event)
 {
     int numDegrees = event->delta() / 8;
     int numSteps = numDegrees / 15;
     if(numSteps > 0){
         emit wheelZoom(1);
     }
     if(numSteps < 0){
         emit wheelZoom(-1);
     }
 }
