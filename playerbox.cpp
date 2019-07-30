#include "playerbox.h"
#include "size.h"
#include <typeinfo>

#include <QFont>

PlayerBox::PlayerBox(QWidget *parent) : QWidget(parent), money(0)
{
    setMaximumWidth(static_cast<int>(340.0*Size::SIZE_FACTOR));
    setMinimumHeight(static_cast<int>(445.0*Size::SIZE_FACTOR));

   // setPalette(QPalette(QColor(156, 39, 176, 100)));
    setAutoFillBackground(true);

    mainVLay = new QVBoxLayout;

    inGameCheck              = new QCheckBox;
    double sCheck = 28;
    inGameCheck->setFixedSize(static_cast<int>(sCheck*Size::SIZE_FACTOR), static_cast<int>(sCheck*Size::SIZE_FACTOR));
    inGameCheck->setCheckState(Qt::CheckState::Checked);

    playerName               = new QLineEdit(tr("Unknown"));
    QFont font;
    font.setFamily(QString::fromUtf8("Nimbus Sans"));
    font.setPointSize(FONT_SIZE_1);
    font.setBold(false);
    font.setWeight(60);
    font.setKerning(true);
    playerName->setFont(font);
    //playerName->setPalette(QPalette(QColor(137, 205, 225)));
    playerName->setAutoFillBackground(true);

    remainderBalanceForm     = new QVBoxLayout;
    remainderBalanceFormText = new QLabel(tr("Money : "));
    remainderBalanceFormNum  = new QLCDNumber;
    remainderBalanceFormNum->setMinimumHeight(static_cast<int>(60.0*Size::SIZE_FACTOR));
    remainderBalanceFormNum->setDigitCount(static_cast<int>(10.0*Size::SIZE_FACTOR));
    remainderBalanceFormNum->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);

    betForm                  = new QFormLayout;
    betFormButton            = new QPushButton(tr("Bet"));
    betFormButton->setFlat(true);
    betFormButton->setFixedSize(static_cast<int>(100.0*Size::SIZE_FACTOR), static_cast<int>(40.0*Size::SIZE_FACTOR));
    //betFormButton->setPalette(QColor(128, 75, 189));
    betFormButton->setAutoFillBackground(true);
    betFormSpinBox           = new QSpinBox;
    betFormSpinBox->setFixedWidth(static_cast<int>(140.0*Size::SIZE_FACTOR));
    betFormSpinBox->setMaximum(INT_MAX);

    foldButton               = new QPushButton(tr("Fold"));
    foldButton->setFlat(true);
    foldButton->setFixedHeight(static_cast<int>(40.0*Size::SIZE_FACTOR));
    //foldButton->setPalette(QPalette(QColor(170, 99, 167)));
    foldButton->setAutoFillBackground(true);

    winButton                = new QPushButton(tr("Win"));
    winButton->setFlat(true);
    winButton->setFixedHeight(static_cast<int>(40.0*Size::SIZE_FACTOR));
    //winButton->setPalette(QPalette(QColor(198, 168, 23)));
    winButton->setAutoFillBackground(true);


    remainderBalanceForm->addWidget(remainderBalanceFormText);
    remainderBalanceForm->addWidget(remainderBalanceFormNum);
    betForm->addRow(betFormButton, betFormSpinBox);

    mainVLay->addWidget(inGameCheck);
    mainVLay->addWidget(playerName);

    mainVLay->addLayout(remainderBalanceForm);
    mainVLay->addLayout(betForm);

    mainVLay->addWidget(foldButton);
    mainVLay->addStretch(static_cast<int>(10.0*Size::SIZE_FACTOR));

    mainVLay->addWidget(winButton);

    setLayout(mainVLay);

    connect(betFormButton, &QPushButton::clicked, this, &PlayerBox::emitBetTransfer);
    connect(winButton, &QPushButton::clicked, this, &PlayerBox::emitWon);
    connect(foldButton, &QPushButton::clicked, this, &PlayerBox::emitFold);
    connect(inGameCheck, &QCheckBox::stateChanged, this, &PlayerBox::changeInGame);
}

void PlayerBox::emitBetTransfer()
{
    if (inGameCheck->checkState() == Qt::CheckState::Unchecked){
        return;
    }
    qulonglong amount = static_cast<qulonglong>(betFormSpinBox->value());
    money -= amount;
    remainderBalanceFormNum->display(QString::number(money));
    betFormSpinBox->setValue(0);
    emit betTransfer(amount, name());
}

void PlayerBox::emitWon()
{
    if (inGameCheck->checkState() == Qt::CheckState::Unchecked){
        return;
    }
    emit won(playerName->text());
}

void PlayerBox::emitFold()
{
    if (inGameCheck->checkState() == Qt::CheckState::Unchecked){
        return;
    }
    emit fold(playerName->text());
}

void PlayerBox::changeInGame(bool inGame)
{
    playerName->setEnabled(inGame);
    betFormButton->setEnabled(inGame);
    betFormSpinBox->setEnabled(inGame);
    foldButton->setEnabled(inGame);
    winButton->setEnabled(inGame);
}

bool PlayerBox::lookBetTransfer(QString name, qulonglong betAmount)
{
    if (inGameCheck->checkState() == Qt::CheckState::Unchecked){
        return false;
    }
    if (name == playerName->text()){
        money += betAmount;
        remainderBalanceFormNum->display(QString::number(money));
        return true;
    }
    return false;
}

void PlayerBox::discountForNewGame(qulonglong discountAmount)
{
    money -= discountAmount;
    remainderBalanceFormNum->display(QString::number(money));
}

bool PlayerBox::isInGame()
{
    return inGameCheck->checkState() == Qt::CheckState::Checked;
}

void PlayerBox::receiveMoney(qulonglong amount)
{
    if (inGameCheck->checkState() == Qt::CheckState::Unchecked){
        return;
    }
    money += amount;
    remainderBalanceFormNum->display(QString::number(money));
}

void PlayerBox::discountMoney(qulonglong amount)
{
    if (inGameCheck->checkState() == Qt::CheckState::Unchecked){
        return;
    }
    money -= amount;
    remainderBalanceFormNum->display(QString::number(money));
}
