#ifndef INITIALMENU_H
#define INITIALMENU_H

#include <QWidget>

#include "partywidget.h"

namespace Ui {
class Widget;
}


class InitialMenu : public QWidget
{
    Q_OBJECT
public:
    explicit InitialMenu(QWidget *parent = nullptr);
    ~InitialMenu();
    Ui::Widget *ui;
    PartyWidget* startTheParty(QWidget *parent);

    enum class Theme{Dark, Light};

signals:
    void startParty();
    void themeChanged(Theme newtheme);

public slots:
    void emitStartParty();
    void updateThemeFromBox(int newindex);

private:
    Theme currentTheme;

};

#endif // INITIALMENU_H
