#include "rgbwcolor.h"
#include <QColor>
#include <QDebug>

RGBWColor::RGBWColor() : r(0), g(0), b (0), w(0)
{

}

RGBWColor::RGBWColor(RGBWColor *copy) : r(copy->getR()),g(copy->getG()),b(copy->getB()), w(copy->getW())
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

double RGBWColor::getBrightness()
{
    return (r+g+b+w)/4;
}

RGBWColor *RGBWColor::getCopy()
{
    return new RGBWColor(this);
}

void RGBWColor::setR(double r)
{
    setRPassiv(r);
    emit colorActiveChanged();
    emit colorChanged();
}

void RGBWColor::setG(double g)
{
    setGPassiv(g);
    emit colorActiveChanged();
    emit colorChanged();
}

void RGBWColor::setB(double b)
{
    setBPassiv(b);
    emit colorActiveChanged();
    emit colorChanged();
}

void RGBWColor::setW(double w)
{
    setWPassiv(w);
    emit colorActiveChanged();
    emit colorChanged();
}

void RGBWColor::setRPassiv(double r)
{
    this->r = r;
    emit colorChanged();
}

void RGBWColor::setGPassiv(double g)
{
    this->g = g;
    emit colorChanged();
}

void RGBWColor::setBPassiv(double b)
{
    this->b = b;
    emit colorChanged();
}

void RGBWColor::setWPassiv(double w)
{
    this->w = w;
    emit colorChanged();
}

