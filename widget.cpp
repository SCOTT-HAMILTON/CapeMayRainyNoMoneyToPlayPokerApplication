#include "widget.h"
#include "size.h"

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(static_cast<int>(Size::APP_SIZEW), static_cast<int>(Size::APP_SIZEH));
    //setPalette(QPalette(QColor(93, 188, 210)));
//    mainVLay = new QVBoxLayout;
//    mainVLay->setMargin(0);
//    mainVLay->setContentsMargins(QMargins(0,0,0,0));
//    mainVLay->setSpacing(0);
    partyWidget = nullptr;
    initialMenu = new InitialMenu(this);
    initialMenu->move(Size::APP_X, Size::APP_Y);

    setTheme(InitialMenu::Theme::Light);

//    mainVLay->addWidget(initialMenu);
//    setLayout(mainVLay);
    qDebug() << "initial menu size : " <<initialMenu->geometry();
    connect(initialMenu, &InitialMenu::startParty, this, &Widget::startTheParty);
    connect(initialMenu, &InitialMenu::themeChanged, this, &Widget::setTheme);
}

QByteArray Widget::readTextFile(const QString &file_path)
{
    QFile input_file(file_path);
    QByteArray input_data;

    if (input_file.open(QIODevice::Text | QIODevice::Unbuffered | QIODevice::ReadOnly)) {
        input_data = input_file.readAll();
        input_file.close();
        return input_data;
    }
    else {
        return QByteArray();
    }
}

void Widget::startTheParty()
{

    partyWidget = initialMenu->startTheParty(this);
    partyWidget->move(Size::APP_X, Size::APP_Y);
    qDebug() << partyWidget->geometry();
//    mainVLay->removeWidget(initialMenu);
//    mainVLay->addWidget(partyWidget, 0, Qt::AlignTop);
//    mainVLay->insertStretch(-1, 1);
    initialMenu->hide();
}

void Widget::setTheme(InitialMenu::Theme newtheme)
{
    if (newtheme == InitialMenu::Theme::Dark){
        QString style_sheet = readTextFile("://Material-blue.qss");
        setStyleSheet(style_sheet);
    }else{
        QString style_sheet = readTextFile("://Material-white.qss");
        setStyleSheet(style_sheet);
    }
}
