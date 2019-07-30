#include "action.h"

Action::Action(ActionName actionName) :
    QObject(),actionName(actionName)
{}

Fold::Fold(QString playerName) :
    Action(ActionName::Fold), playerName(playerName)
{}

Bet::Bet(qulonglong amount, QString playerName) :
    Action(ActionName::Bet), amount(amount), playerName(playerName)
{}

NewRound::NewRound(qulonglong discount, QStringList players) :
    Action(ActionName::NewRound), discountPerPlayers(discount),
    players(players)
{}

Win::Win(qulonglong amount, QString playerName) :
    Action(ActionName::Win), winAmount(amount), playerName(playerName)
{}
