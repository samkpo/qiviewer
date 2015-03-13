#ifdef WEBP_SUPPORT
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include "webpdecoder.h"
#include "3rdparty/webp/webp/decode.h"

WebpDecoder::WebpDecoder(QObject *parent) : QObject(parent)
{
    qDebug()<<"webp image";
}

bool WebpDecoder::setFile(const QString d)
{
    return decodeWebpImage(d);
}

bool WebpDecoder::readImage(unsigned char* rgb, int width, int height, QPixmap& pixmap)
{
    //code copied from Qt class qppmhandler
    int mcc = 255;

    if (width <= 0 || width > 32767 || height <= 0 || height > 32767 || mcc <= 0)
        return false;                                        // weird P.M image

    int y;
    //int pbm_bpl;
    QImage::Format format = QImage::Format_RGB32;
    //QImage::Format format = QImage::Format_ARGB32;
    QImage outImage(width,height,format);

    //pbm_bpl = (nbits*width+7)/8; // bytes per scanline in PBM

    //pbm_bpl = mcc < 256 ? 3*width : 6*width;
    QRgb  *p;
    QRgb  *end;
    for (y=0; y<height; y++) {
        p = (QRgb *)outImage.scanLine(y);
        end = p + width;
        while (p < end) {
              if (mcc < 256) {
                 *p++ = qRgb/*a*/(rgb[0],rgb[1],rgb[2]/*,rgb[3]*/);
                 //rgb += 4;
                  rgb += 3;
              } else {
                 *p++ = qRgb/*a*/(((int(rgb[0]) * 256 + int(rgb[1]) + 1) * 256) / (mcc + 1) - 1,
                            ((int(rgb[2]) * 256 + int(rgb[3]) + 1) * 256) / (mcc + 1) - 1,
                            ((int(rgb[4]) * 256 + int(rgb[5]) + 1) * 256) / (mcc + 1) - 1/*,
                            ((int(rgb[6]) * 256 + int(rgb[7]) + 1) * 256) / (mcc + 1) - 1*/);
                  //rgb += 7;
                  rgb += 5;
              }
        }
    }

    pixmap = QPixmap().fromImage(outImage);
    return true;
}

bool WebpDecoder::decodeWebpImage(const QString d)
{
    const char *in_file = NULL;
    std::string name = d.toStdString();
    in_file = name.data();
    qDebug()<<tr("[WEBP]in file: %1").arg(in_file);

    int width, height;
    uint8_t* out = NULL;

    if (in_file == NULL) {
      qDebug()<<tr("[WEBP]missing input file!!\n");
      return false;
    }

    else{
        uint32_t data_size = 0;
        void* data = NULL;
        int ok;

        FILE* const in = fopen(in_file, "rb");

        if (!in) {
            qDebug()<<tr("[WEBP]cannot open input file '%1'").arg(in_file);
            return false;
        }

        fseek(in, 0, SEEK_END);
        data_size = ftell(in);
        fseek(in, 0, SEEK_SET);
        data = malloc(data_size);
        ok = (fread(data, data_size, 1, in) == 1);
        fclose(in);
        if (!ok) {
          free(data);
          return false;
        }

        out = WebPDecodeRGB((const uint8_t*)data, data_size, &width, &height);

        free(data);
    }

    if (!out) {
        qDebug()<<tr("[WEBP]decoding of %1 failed.").arg(in_file);
        return false;
    }

    qDebug()<<tr("[WEBP]%1 decoded. Dimensions: %2 x %3. Now opening...").arg(in_file).arg(width).arg(height);

    int ok = 1;
    ok &= readImage(out,width,height,pixmap);
    if (ok)
        qDebug()<<tr("[WEBP]file %1 opened").arg(in_file);
    else
        qDebug()<<tr("[WEBP]error opening file %s !!");

    free(out);
    return true;
}
#endif
