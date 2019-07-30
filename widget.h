#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>

#include "partywidget.h"
#include "initialmenu.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    QByteArray readTextFile(const QString &file_path);

public slots:
    void startTheParty();
    void setTheme(InitialMenu::Theme newtheme);

private:
    QVBoxLayout *mainVLay;

    InitialMenu *initialMenu;
    PartyWidget *partyWidget;
};

#endif // WIDGET_H
