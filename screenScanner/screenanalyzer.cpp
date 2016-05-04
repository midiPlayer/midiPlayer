#include "screenanalyzer.h"

#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QDebug>
#include <QTime>

class ScreenAnalyzerPrivate {
public:
    QTime timer;
};

ScreenAnalyzer::ScreenAnalyzer() : QThread(), d_ptr(new ScreenAnalyzerPrivate)
{

}

void ScreenAnalyzer::run()
{
    setPriority(QThread::HighestPriority);
    QScreen *screen = QApplication::primaryScreen();
    QImage image;
    if (screen) {
        d_ptr->timer.start();
        forever {
            if( QThread::currentThread()->isInterruptionRequested() ) return;

            d_ptr->timer.restart();

            image = screen->grabWindow(0).toImage();

            int width = screen->geometry().width();
            int height = screen->geometry().height();

            long red = 0, green = 0, blue = 0, sample_count = 0;

            // Sum all the colors in the given range
            for ( int x = 0; x < width; x += width/100) {
                for ( int y = 0; y < height; y += height/100) {

                    QColor color( image.pixel(x, y) );
                    red += color.red();
                    green += color.green();
                    blue += color.blue();
                    ++sample_count;
                }
            }

            // Get he avarage by dividen all the sums by the sample count
            red /= sample_count;
            green /= sample_count;
            blue /= sample_count;
            /*static int  i = 0;
            if(i++ > 10){
                qDebug() << "elapsed:" << d_ptr->timer.elapsed();
                i = 0;
            }*/
            QList<QColor> list;
            list.append(QColor(red, green, blue, 255) );
            emit color( list);

            msleep( qMax(100 - d_ptr->timer.elapsed(), 0) );
        }
    }
}

void ScreenAnalyzer::stop()
{
    requestInterruption();
}
