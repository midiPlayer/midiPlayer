#ifndef MYQSLIDER_H
#define MYQSLIDER_H

#include <QSlider>
/**
  This class helps to surpress signals from QSlider when th value is set using setValue
 * @brief The MyQSlider class
 */
class MyQSlider : public QSlider
{
    Q_OBJECT
public:
    explicit MyQSlider(QWidget *parent = 0);

signals:
    void userChangedValue(int val);
public slots:
    void setValue (int val);
    void onValueChanged(int val);
private:
    bool valueBeeingSet;
};

#endif // MYQSLIDER_H
