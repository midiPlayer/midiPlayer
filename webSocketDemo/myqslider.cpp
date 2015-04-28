#include "myqslider.h"

MyQSlider::MyQSlider(QWidget *parent) :
    QSlider(parent),valueBeeingSet(false)
{
    connect(this,SIGNAL(valueChanged(int)),this,SLOT(onValueChanged(int)));
}

void MyQSlider::setValue(int val)
{
    valueBeeingSet = true;
    QSlider::setValue(val);
    valueBeeingSet = false;
}

void MyQSlider::onValueChanged(int val)
{
    if(!valueBeeingSet)
        emit(userChangedValue(val));
}
