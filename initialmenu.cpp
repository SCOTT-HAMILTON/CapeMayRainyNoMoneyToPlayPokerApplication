#include "initialmenu.h"
#include "ui_widget.h"
#include "size.h"

#include <QDebug>
#include <QComboBox>

InitialMenu::InitialMenu(QWidget *parent) : QWidget(parent),
    ui(new Ui::Widget), currentTheme(Theme::Light)
{
    ui->setupUi(this);
    {
        ui->verticalLayout->setSpacing(static_cast<int>(6.0*Size::SIZE_FACTOR));
        ui->verticalLayout->setContentsMargins(static_cast<int>(11.0*Size::SIZE_FACTOR),
                                               static_cast<int>(11.0*Size::SIZE_FACTOR),
                                               static_cast<int>(11.0*Size::SIZE_FACTOR),
                                               static_cast<int>(11.0*Size::SIZE_FACTOR));

        ui->verticalSpacer->changeSize(static_cast<int>(20.0*Size::SIZE_FACTOR), static_cast<int>(147.0*Size::SIZE_FACTOR), QSizePolicy::Minimum, QSizePolicy::Expanding);

        ui->formLayout->setSpacing(static_cast<int>(6.0*Size::SIZE_FACTOR));
        ui->formLayout->setContentsMargins(-1, -1, static_cast<int>(8.0*Size::SIZE_FACTOR), -1);
        QFont font;
        font.setFamily(QString::fromUtf8("Nimbus Sans"));
        font.setPointSize(FONT_SIZE_1);
        font.setBold(true);
        font.setWeight(75);
        font.setKerning(true);
        ui->label->setFont(font);

        ui->playersNumber->setFixedSize(static_cast<int>(200.0*Size::SIZE_FACTOR), static_cast<int>(60.0*Size::SIZE_FACTOR));
    #ifdef THIRDPLAYER_COLUMN
        ui->playersNumber->setMaximum(8);
    #endif

        QFont titlefont;
        titlefont.setFamily(QString::fromUtf8("Source Code Variable"));
        titlefont.setPointSize(TITLE_FONT_SIZE);
        titlefont.setBold(true);
        titlefont.setWeight(75);
        ui->label_5->setFont(titlefont);
        //theme clair :
        if (currentTheme == Theme::Light)
            ui->label_5->setStyleSheet("color: #000000");
        //theme foncé
        else{
            ui->label_5->setStyleSheet("color: #FFFFFF");
        }

        ui->settingsBox->setFixedSize(static_cast<int>(200.0*Size::SIZE_FACTOR), static_cast<int>(60.0*Size::SIZE_FACTOR));
        ui->settingsBox->addItem(tr("Dark theme"));
        ui->settingsBox->addItem(tr("Light theme"));
        ui->settingsBox->setCurrentIndex(1);
        ui->settingsBox->setCurrentText("");

        QFont font1;
        font1.setFamily(QString::fromUtf8("Nimbus Sans"));
        font1.setPointSize(FONT_SIZE_2);
        ui->playersNumber->setFont(font1);

        ui->label_2->setFont(font);

        ui->discountPerRoundSpin->setFixedSize(static_cast<int>(200.0*Size::SIZE_FACTOR), static_cast<int>(60.0*Size::SIZE_FACTOR));
        ui->discountPerRoundSpin->setFont(font);


        ui->starterRemainder->setFixedSize(static_cast<int>(200.0*Size::SIZE_FACTOR), static_cast<int>(60.0*Size::SIZE_FACTOR));
        ui->starterRemainder->setFont(font);

        ui->label_4->setFont(font);

        ui->moneyUnit->setFixedSize(static_cast<int>(200.0*Size::SIZE_FACTOR), static_cast<int>(60.0*Size::SIZE_FACTOR));
        ui->moneyUnit->setFont(font);

        ui->label_3->setFont(font);

        ui->verticalSpacer_2->changeSize(static_cast<int>(20.0*Size::SIZE_FACTOR), static_cast<int>(146.0*Size::SIZE_FACTOR), QSizePolicy::Minimum, QSizePolicy::Expanding);

        ui->horizontalLayout->setSpacing(static_cast<int>(6.0*Size::SIZE_FACTOR));
        ui->horizontalSpacer->changeSize(static_cast<int>(40.0*Size::SIZE_FACTOR), static_cast<int>(20.0*Size::SIZE_FACTOR), QSizePolicy::Expanding, QSizePolicy::Minimum);

        ui->startGameButton->setFixedSize(static_cast<int>(460.0*Size::SIZE_FACTOR), static_cast<int>(120.0*Size::SIZE_FACTOR));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Noto Sans"));
        font3.setPointSize(FONT_SIZE_3);
        font3.setBold(true);
        font3.setWeight(75);
        ui->startGameButton->setFont(font3);

        qDebug() << "start button size : " << ui->startGameButton->geometry();

        ui->horizontalSpacer_2->changeSize(static_cast<int>(40.0*Size::SIZE_FACTOR), static_cast<int>(20.0*Size::SIZE_FACTOR), QSizePolicy::Expanding, QSizePolicy::Minimum);

        ui->verticalSpacer_3->changeSize(static_cast<int>(20.0*Size::SIZE_FACTOR), static_cast<int>(147.0*Size::SIZE_FACTOR), QSizePolicy::Minimum, QSizePolicy::Expanding);
    }
    setFixedSize(static_cast<int>(Size::APP_SIZEW), static_cast<int>(Size::APP_SIZEH));
    double labelw = 400;
    ui->label->setFixedSize(static_cast<int>(labelw*Size::SIZE_FACTOR), static_cast<int>(60.0*Size::SIZE_FACTOR));
    ui->label_2->setFixedSize(static_cast<int>(labelw*Size::SIZE_FACTOR), static_cast<int>(60.0*Size::SIZE_FACTOR));
    ui->label_3->setFixedSize(static_cast<int>(labelw*Size::SIZE_FACTOR), static_cast<int>(60.0*Size::SIZE_FACTOR));
    ui->label_4->setFixedSize(static_cast<int>(labelw*Size::SIZE_FACTOR), static_cast<int>(60.0*Size::SIZE_FACTOR));
    qDebug() << "start button geo : " << ui->startGameButton->geometry();
    connect(ui->startGameButton, &QPushButton::clicked, this, &InitialMenu::emitStartParty);
    connect(ui->settingsBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &InitialMenu::updateThemeFromBox);
}

InitialMenu::~InitialMenu()
{
    delete ui;
}

PartyWidget* InitialMenu::startTheParty(QWidget *parent)
{
    PartyWidget* partyWidget = new PartyWidget(parent, static_cast<qulonglong>(ui->playersNumber->value()), ui->moneyUnit->text(), static_cast<qulonglong>(ui->discountPerRoundSpin->value()));
    partyWidget->injectAmountToEachClient(static_cast<qulonglong>(ui->starterRemainder->value()));
    partyWidget->setVisible(true);

    return partyWidget;

}

void InitialMenu::emitStartParty()
{
    emit startParty();
}

void InitialMenu::updateThemeFromBox(int newindex)
{
    //dark theme
    switch (newindex){
        case 0 :{
            if (currentTheme != Theme::Dark){
                ui->label_5->setStyleSheet("background-color: rgba(255,255,255, 0);color: #FFFFFF");
                ui->label->setStyleSheet("background-color: rgba(255,255,255, 0);color: #C7D0C7");
                ui->label_2->setStyleSheet("background-color: rgba(255,255,255, 0);color: #C7D0C7");
                ui->label_3->setStyleSheet("background-color: rgba(255,255,255, 0);color: #C7D0C7");
                ui->label_4->setStyleSheet("background-color: rgba(255,255,255, 0);color: #C7D0C7");

                currentTheme = Theme::Dark;
                emit themeChanged(currentTheme);
            }
            break;
        }
        case 1:{
            if (currentTheme != Theme::Light){
                ui->label_5->setStyleSheet("background-color: rgba(255,255,255, 0);color: #000000");
                ui->label->setStyleSheet("background-color: rgba(255,255,255, 0);color: #382F2F");
                ui->label_2->setStyleSheet("background-color: rgba(255,255,255, 0);color: #382F2F");
                ui->label_3->setStyleSheet("background-color: rgba(255,255,255, 0);color: #382F2F");
                ui->label_4->setStyleSheet("background-color: rgba(255,255,255, 0);color: #382F2F");

                currentTheme =  Theme::Light;
                emit themeChanged(currentTheme);
            }
            break;
        }
        default:;
    }

}


