#include "rgbwcolor.h"
#include <QColor>

RGBWColor::RGBWColor()
{

}

QString RGBWColor::getRGBPrev()
{
    //TODO: gewichtung anpassen
 double pr = r * 0.75 + w *0.25;
 double pg = g * 0.75 + w *0.25;
 double pb = b * 0.75 + w *0.25;

 QColor c((int)(pr*255),(int)(pg*255),(int)(pb*255));
 c.toHsv();
 int  h = c.hue();
 int s = c.saturation();
 c.setHsv(h,s,255);
 c.toRgb();
 return c.name();
}

double RGBWColor::getR()
{
    return r;
}

double RGBWColor::getG()
{
    return g;
}

double RGBWColor::getB()
{
    return b;
}

double RGBWColor::getW()
{
    return w;
}

void RGBWColor::setR(double r)
{
    this->r = r;
}

void RGBWColor::setG(double g)
{
    this->g = g;
}

void RGBWColor::setB(double b)
{
    this->b = b;
}

void RGBWColor::setW(double w)
{
    this->w = w;
}

