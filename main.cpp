#include "widget.h"
#include "size.h"

#include <QApplication>
#include <QDebug>
#include <QTranslator>
#include <QLocale>
#include <QIcon>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QFile>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    qDebug() << QGuiApplication::screens().first()->availableSize();
    {
        int w = QGuiApplication::screens().first()->availableSize().width();
        int h = QGuiApplication::screens().first()->availableSize().height();
#ifdef ANDROID_SIZE
        //LANDSCAPE VIEW or less than 18:9 (like 16:9)
        if (w>=h/2){
            Size::APP_SIZEH = h;
            Size::APP_SIZEW = h/2;
            Size::APP_X = (w-Size::APP_SIZEW)/2;
        }
        //for even more little ratio than 18:9 like 20:9
        else {
            Size::APP_SIZEW = w;
            Size::APP_SIZEH = w*2;
            Size::APP_Y = (h-Size::APP_SIZEH)/2;
        }
        Size::SIZE_FACTOR = static_cast<double>(Size::APP_SIZEH)/1310.0;

#else
        h -= 80;
        Size::APP_SIZEH = h;
        Size::APP_SIZEW = h*960/1310;
        Size::SIZE_FACTOR = static_cast<double>(Size::APP_SIZEH)/1310.0;

#endif

    }

    qDebug() << "SIZE_FACTOR : " << Size::SIZE_FACTOR;
    qDebug() << "APP SIZE : " << Size::APP_SIZEW << ", " << Size::APP_SIZEH;

    QTranslator translator;
    // look up ://translations/CapeMayRainyDayNoMoneyToPlayPokerApplication-en_US.qm
    QLocale locale;//(QLocale::English, QLocale::UnitedStates);

    if (translator.load("://translations/CapeMayRainyDayNoMoneyToPlayPokerApplication-"+locale.name()+".qm"))
        app.installTranslator(&translator);

    Widget w;
    qDebug() << "Widet size: " << w.geometry();
     w.setWindowIcon(QIcon("://icons/icons8-cards-96.png"));
    w.show();

    return app.exec();
}
