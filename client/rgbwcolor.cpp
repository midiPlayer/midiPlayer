#include "rgbwcolor.h"
#include <QColor>
#include <QDebug>

RGBWColor::RGBWColor() : r(0), g(0), b (0), w(0), deviceWhiteColor(0,0,0),hasR(false),hasG(false),hasB(false),hasW(false)
{

}

RGBWColor::RGBWColor(RGBWColor *copy) : r(copy->getR()),g(copy->getG()),b(copy->getB()), w(copy->getW()),
    deviceWhiteColor(copy->deviceWhiteColor),hasR(copy->hasR),hasG(copy->hasG),hasB(copy->hasB),hasW(copy->hasW)
{

}

QString RGBWColor::getRGBPrev()
{
    //TODO: gewichtung anpassen
 double pr = r * 0.75 + (w *0.25 * deviceWhiteColor.redF());
 double pg = g * 0.75 + (w *0.25 * deviceWhiteColor.greenF());
 double pb = b * 0.75 + (w *0.25 * deviceWhiteColor.blueF());

 QColor c((int)(pr*255),(int)(pg*255),(int)(pb*255));
 return maximizeColor(c).name();
}

QColor RGBWColor::maximizeColor(QColor c)
{
    c.toHsv();
    int  h = c.hue();
    int s = c.saturation();
    c.setHsv(h,s,255);
    c.toRgb();
    return c;
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

bool RGBWColor::getHasR()
{
    return hasR;
}

bool RGBWColor::getHasG()
{
    return hasG;
}

bool RGBWColor::getHasB()
{
    return hasB;
}

bool RGBWColor::getHasW()
{
    return hasW;
}

double RGBWColor::getBrightness()
{
    if(!hasR && !hasG && !hasB && !hasW)
        return 0;
    return (r+g+b+w)/(hasR + hasG + hasB + hasW);
}

QString RGBWColor::getDeviceWhiteColor()
{
    return deviceWhiteColor.name();
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

void RGBWColor::setHasR(bool has)
{
    hasR = has;
}

void RGBWColor::setHasG(bool has)
{
    hasG = has;
}

void RGBWColor::setHasB(bool has)
{
    hasB = has;
}

void RGBWColor::setHasW(bool has)
{
    hasW = has;
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

void RGBWColor::setDeviceWhiteColor(QString name)
{
    deviceWhiteColor.setNamedColor(name);
    deviceWhiteColor = maximizeColor(deviceWhiteColor);
}

