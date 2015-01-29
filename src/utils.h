#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QtCore/QStringList>

class QPixmap;
class QColor;

class Utils : public QObject
{
    Q_OBJECT
public:
    inline Utils();
    
    /*!
      \returns a chess board pixmap
      */
    static QPixmap chessBoard(int squareSize);
    
    /*!
      \returns a string containgn all the strings in the file given
      to the constructor
      */
    static QString stringFromFile(const QString &fileName);

private:
  
};

#endif // UTILS_H
