#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include "utils.h"

QPixmap Utils::chessBoard(int cbsSize)
{
    QPixmap m = QPixmap(cbsSize*2,cbsSize*2);
    QPainter p(&m);
    p.fillRect(m.rect(), QColor(128,128,128));
    QColor light = QColor(192,192,192);
    p.fillRect(0,0,cbsSize,cbsSize,light);
    p.fillRect(cbsSize,cbsSize,cbsSize,cbsSize, light);
    p.end();
    return m;
}

QString Utils::stringFromFile(const QString &name)
{
  QString ret_string;
  QFile file(name);
  if (file.open(QIODevice::ReadOnly))
  {
      QTextStream ts(&file);
      ts.setCodec("UTF-8");
      ret_string = ts.readAll();
      file.close();
  }
  return ret_string;
}
