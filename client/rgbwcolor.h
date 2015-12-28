#ifndef RGBWCOLOR_H
#define RGBWCOLOR_H

#include <QObject>
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

    Q_PROPERTY(QString preview READ getRGBPrev)
    Q_PROPERTY(double brightness READ getBrightness)
    Q_PROPERTY(RGBWColor* copy READ getCopy)

    double getR();
    double getG();
    double getB();
    double getW();
    double getBrightness();
    RGBWColor* getCopy();

public slots:
    void setR(double r);
    void setG(double g);
    void setB(double b);
    void setW(double w);

    void setRPassiv(double r);
    void setGPassiv(double g);
    void setBPassiv(double b);
    void setWPassiv(double w);
signals:
    void colorActiveChanged();
    void colorChanged();
private:
double r,g,b,w;



};

#endif // RGBWCOLOR_H
