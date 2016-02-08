#ifndef RGBWCOLOR_H
#define RGBWCOLOR_H

#include <QObject>
#include <QColor>
class RGBWColor : public QObject
{
Q_OBJECT

public:
    RGBWColor();
    RGBWColor(RGBWColor *copy);
    QString getRGBPrev();
    Q_PROPERTY(double r READ getR  WRITE setR)
    Q_PROPERTY(double g READ getG  WRITE setG)
    Q_PROPERTY(double b READ getB  WRITE setB)
    Q_PROPERTY(double w READ getW  WRITE setW)

    Q_PROPERTY(double passivR READ getR  WRITE setRPassiv)
    Q_PROPERTY(double passivG READ getG  WRITE setGPassiv)
    Q_PROPERTY(double passivB READ getB  WRITE setBPassiv)
    Q_PROPERTY(double passivW READ getW  WRITE setWPassiv)

    Q_PROPERTY(bool hasR READ getHasR  WRITE setHasR)
    Q_PROPERTY(bool hasG READ getHasG  WRITE setHasG)
    Q_PROPERTY(bool hasB READ getHasB  WRITE setHasB)
    Q_PROPERTY(bool hasW READ getHasW  WRITE setHasW)

    Q_PROPERTY(QString preview READ getRGBPrev)
    Q_PROPERTY(QString deviceWhiteColor READ getDeviceWhiteColor WRITE setDeviceWhiteColor)
    Q_PROPERTY(double brightness READ getBrightness)
    Q_PROPERTY(RGBWColor* copy READ getCopy)

    double getR();
    double getG();
    double getB();
    double getW();

    bool getHasR();
    bool getHasG();
    bool getHasB();
    bool getHasW();

    double getBrightness();
    QString getDeviceWhiteColor();
    RGBWColor* getCopy();

public slots:
    void setR(double r);
    void setG(double g);
    void setB(double b);
    void setW(double w);

    void setHasR(bool has);
    void setHasG(bool has);
    void setHasB(bool has);
    void setHasW(bool has);

    void setRPassiv(double r);
    void setGPassiv(double g);
    void setBPassiv(double b);
    void setWPassiv(double w);

    void setDeviceWhiteColor(QString name);
signals:
    void colorActiveChanged();
    void colorChanged();
private:
double r,g,b,w;
bool hasR, hasG, hasB, hasW;
QColor maximizeColor(QColor c);

QColor deviceWhiteColor;

};

#endif // RGBWCOLOR_H
