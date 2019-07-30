#ifndef PARTYWIDGET_H
#define PARTYWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QCommandLinkButton>
#include <QWidgetList>

#include "playerbox.h"
#include "actionhistoric.h"
#include "actionnavigator.h"

class PartyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PartyWidget(QWidget *parent = nullptr, size_t nb_players = 3, QString unit="£", qulonglong discountAmount = 1);
    void injectAmountToEachClient(qulonglong amount);
    qulonglong discountAmount;
    QString unit;

signals:

public slots:
    void transferToBetCount(qulonglong amount, QString playerBettingName);
    void throwBetWin(QString winnerName);
    void makePlayerFold(QString playerName);
    void makeNewRound();
    void undoBet(Bet *bet);
    void redoBet(Bet *bet);
    void undoFold(Fold *fold);
    void redoFold(Fold *fold);
    void undoNewRound(NewRound *newRound);
    void redoNewRound(NewRound *newRound);
    void undoWin(Win *win);
    void redoWin(Win *win);

    void nextPlayer();

private:
    QHBoxLayout *mainHLay;

    QVBoxLayout *playersLay;
    QWidgetList playerBoxList;
    size_t nbPlayers;

    QVBoxLayout *thirdPlayersLay;

    QVBoxLayout *rightPanelLay;
    QGroupBox *betGroupBox;
    QVBoxLayout *betLay;
    QLabel *betCountText;
    QLabel *betCount;
    QPushButton *newRoundButton;

    QHBoxLayout *navButtonsLay;
    QCommandLinkButton *undoButton;
    QCommandLinkButton *redoButton;

    qulonglong betAmount;


    bool gotWinner;

    ActionHistoric actionHistoric;
    ActionNavigator actionNavigator;

    void updateBetCountText();
    QStringList collectRoundDiscount();

    int currentPlayer;
};

#endif // PARTYWIDGET_H
