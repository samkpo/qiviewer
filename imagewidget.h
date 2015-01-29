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

#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

class QLabel;
class QString;
class QHBoxLayout;
class QPixmap;
class QMovie;
class QPaintEvent;
class QSize;
class QPoint;

class ImageWidget : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *);
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);

public:
    /**
     * \enum ImageWidget::Transform
     */
    enum Transform{
        toLeft,       ///<the image is rotated 90 degrees to the Left
        toRight,      ///<the image is rotated 90 degrees to the right
        horizontally, ///<the image is flipped horizontally
        vertically    ///<vertically the image is flipped vertically
    };
    
    /**
     * \enum ImageWidget::BackgroundType
     */
    enum BackgroundType{
        squares,    ///<uses a chessboard as background
        solidColor, ///<uses a color as background
        none        ///<the background isn't setted
    };

    /**
     * constructor
     */
    ImageWidget(QWidget *parent = 0);
    
    /**
     * Transfom the image to the given \a Transform
     * */
    void transformImage(Transform);

    /**
      *removes any image or movie loaded and sets everything to default
      */
    void clear();
    
    /**
     * sets the \a name to be the picture to show
     */
    void setPicture(const QString name);
    
    /**
     * sets the \a name as the movie to show
     */
    void setMovie(const QString name);
    
    /**
     * sets the background type.
     * it might be \a squares, \a solidColor or \a none
     */
    void setBackgroundType(const QString type);

    /**
     * sets \a size as each square side size in the chessboard background
     */
    inline void setCBSSize(int size){if(size<=32 && size>0){cbsSize = size;}}
    
    /**
     * sets \a color as the color in the solidColor background
     */
    inline void setBackgroundColor(QColor color){backColor = color;}
    
    /**
     * if \a restart is true, when making zoom the image reproduction will
     * be restarted
     * it's only for animated images
     */
    inline void setRestartWhenZooming(bool restart){restartWhenZooming = restart;}
    
    /**
     * if \a stop is true, the animated image reproduction will be stopped
     * after it reaches it's final frame
     */
    inline void setStopWhenFinish(bool stop){stopWhenFinish = stop;}
    
    /**
     * this is an egg
     * if \a invert is true, inverts all pixel values in the image
     */
    inline void invertColors(bool invert){invertedColors = invert;}
    
    /**
     * @return a copy of the image
     */
    inline QPixmap getPixmap() const{return pixmap;}
    
    /**
     * @return the name of the previous image that was setted
     */
    inline QString getPrevFile() const{return prevFile;}
    
    /**
     * @return the animated image speed
     */
    inline int getMovieSpeed() const{return movieSpeed;}
    
    /**
     * @return if the animated image reproduction will be stopped or not
     * after reach its final frame
     */
    inline bool getStopWhenFinish() const{return stopWhenFinish;}
    
    /**
     * @return the image's size
     */
    QSize getPictureSize() const;
    
    /**
     * @returns the background type
     */
    QString getBGType() const;

    /**
      * @returns true if the current image, tha one that is going to be saved, is a non-animated
      *image, otherwise return false
      */
    bool canSave() const;


public slots:
    /**
     * this slot is used to make zoom
     */
    void makeZoom(double); //when this is iqual to 1, the zoom is %100

    /**
     * sets \a speed as the movie speed
     * it's in percentage, being 100 the normal and default speed
     */
    void setMovieSpeed(int speed);

signals:
    /**
     * this signal is sent every time an image is setted. it's true if
     * the object could open the image and false if not
     */
    void couldOpen(bool);
    
    /**
     * signal sent every time the image size change, for example when making zoom
     */
    void picSizeChanged();
    
    /*!
      signal sent when the image change, for example when it makes a transformation
      */
    void pixmapChanged();
    
    /**
     * signal sent when making zoom with the mouse wheel
     * -1->zoom in, -1->zoom out
     */
    void wheelZoom(int);
    
    /**
     * this is to move the image with the mouse,
     * being the first argument the inicial position and the second the final
     * position
     */
    void moveWidget(QPoint, QPoint);

private:
   /**
     * \enum ImageWidget::Elementipe
     * posibles types for images.
     */
    enum ElementType{
        None,       ///<not specified
        Picture,    ///<A static image
        Movie,      ///<A dynamic image
        Vectorial   ///<A vectorial image
    };

    ElementType eType;
    BackgroundType backType;
    QColor backColor;
    int cbsSize;//chessboard square size (cbsSize)
    QLabel *imageLabel;
    QHBoxLayout *mainLayout;
    QPixmap pixmap;
    QMovie *movie;
    QSize bugSize;
    QSize originalSize;
    QString prevFile;
    double scaleFactor;
    bool restartWhenZooming;
    bool stopWhenFinish;
    int movieAngle;
    int movieFlipX;
    int movieFlipY;
    int movieSpeed;//it's in percentage;
    bool invertedColors;
    bool move;
    QPoint point;

    void drawPixmap();
    void defaulTransMovie();
    inline bool restartMovieWhenZooming(){return restartWhenZooming;}
    void calculateNewState(Transform);
    void makeTransformation(Transform);
    void makeDynamicTransformation();
    void calculateMovement(const QPoint);
    void imageModified();
    void egg();

private slots:
    void movieSlot();

};

#endif // IMAGEWIDGET_H
