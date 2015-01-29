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

#ifndef ZOOMUTILS_H
#define ZOOMUTILS_H

#include <QObject>
#include <QtCore/QSize>

class QSlider;
class QSize;
class QPushButton;

class ZoomUtils : public QObject
{
    Q_OBJECT

public:
    /**
     * constructor
     */
    ZoomUtils(QObject *parent = 0);
    
    /**
     * sets \a mSize as the maximun size
     */
    void setMaxSize(const QSize mSize);
    
    /**
     * sets \a pSize as the picture size
     * */
    void setPicSize(const QSize pSize);
    
    /**
     * @return true if the available zoom is iqual to the best zoom value
     * otherwise returns false
     */
    bool bestZoomValue() const;
    
    /**
     * @return the max zoom value
     */
    inline int getMaxZoomValue() const{return maxZoomValue;}
    
    /**
     * @returns the minimun zoom value
     */
    inline int getMinZoomValue() const{return minZoomValue;}
    
    /**
     * @return the zoom incremente value
     */
    inline int getZoomIncrement() const{return zoomIncrement;}
    
    /**
     * @returns the zoom value
     */
    inline int getZoomValue() const{return zoomValue;}
    
    /**
     * sets \a d as de zoom increment
     */
    inline void setZoomIncrement(int d){zoomIncrement = d;}
    
    /**
     * sets \a d as the maximun zoom value
     */
    inline void setMaxZoomValue(int d){maxZoomValue = d;}
    
    /**
     * sets \a d as the minimum zoom increment
     */
    inline void setMinZoomValue(int d){minZoomValue = d;}
    
    /**
     * clear everything
     */
    inline void clearSizes(){setDefaults();}
    
    /**
     * makes a zoom in
     */
    inline void zoomIn(){makeZoom(zoomValue + zoomIncrement);}
    
    /**
     * makes a zoom out
     */
    inline void zoomOut(){makeZoom(zoomValue - zoomIncrement);}
    
    
    inline void normalSize(){makeZoom(100);}
    inline void adjustPicSize(){makeZoom(zoomValueForBestFit);}

signals:
    /**
     zoom(double d) is emited to make zoom, d is the value of
     the zoom, the final one, not an increment. 1 = normal size
     1.5 is iqual to a %150 zoom
     0.5 is iqual to a %50 zoom
     */
    void zoom(double);

    /**
      same that zoom(double d), but in this case
      the value sended is in percentage
      */
    void sliderSignal(int);

public slots:
    void maxSizeChanged(QSize);
    void setAdjustedSize(bool);
    inline void sliderSlot(int d){makeZoom(d);}

private:
    QSize maxSize;
    QSize picSize;
    int zoomValue;
    int zoomRestoreValue;
    int maxZoomValue;
    int minZoomValue;
    int zoomValueForBestFit;
    int zoomIncrement;
    bool needToMakeZoom;       //if the image is big
    bool canMakeZoom;
    bool adjustedSize;
    bool mSizeChanged;
    //bool fTime;

    void configure();
    void calculateZoom();
    void makeZoom(int);
    void setDefaults();

signals:

private slots:

};

#endif // ZOOMWIDGET_H
