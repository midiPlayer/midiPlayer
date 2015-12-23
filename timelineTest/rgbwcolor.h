#ifndef RGBWCOLOR_H
#define RGBWCOLOR_H

#include <QObject>
class RGBWColor : public QObject
{
Q_OBJECT
public:
    RGBWColor();
    QString getRGBPrev();
    Q_PROPERTY(double r READ getR  WRITE setR)
    Q_PROPERTY(double g READ getG  WRITE setG)
    Q_PROPERTY(double b READ getB  WRITE setB)
    Q_PROPERTY(double w READ getW  WRITE setW)
    Q_PROPERTY(QString preview READ getRGBPrev)

    double getR();
    double getG();
    double getB();
    double getW();

public slots:
    void setR(double r);
    void setG(double g);
    void setB(double b);
    void setW(double w);
private:
double r,g,b,w;



};

#endif // RGBWCOLOR_H
