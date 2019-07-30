#include "actionhistoric.h"

#include <iostream>

ActionHistoric::ActionHistoric(QObject *parent) : QObject(parent),
    current_action(-1)
{

}

void ActionHistoric::newBet(qulonglong amount, QString playerName)
{
    wipeFrontActions();
    actionsHistoric.append( new Bet(amount, playerName) );
    current_action++;
}

void ActionHistoric::newFold(QString playerName)
{
    wipeFrontActions();
    actionsHistoric.append( new Fold(playerName) );
    current_action++;
}

void ActionHistoric::newRound(qulonglong discountAmount, QStringList players)
{
    wipeFrontActions();
    actionsHistoric.append( new NewRound(discountAmount, players) );
    current_action++;
}

void ActionHistoric::newWin(qulonglong winAmount, QString playerName)
{
    wipeFrontActions();
    actionsHistoric.append( new Win(winAmount, playerName) );
    current_action++;
}

Action *ActionHistoric::currentAction() const
{
//    std::cerr << "current action asked : \n" << toString().toStdString() << '\n';
    if (current_action>=0 and current_action<actionsHistoric.size()){
        return actionsHistoric[current_action];
    }
    return nullptr;
}

void ActionHistoric::backToPreviousAction()
{
    if (current_action>=0){
        current_action--;
    }
}

bool ActionHistoric::forwardToNextAction()
{
    if (current_action+1<actionsHistoric.size()){
        current_action++;
        return true;
    }
    return false;
}

QString ActionHistoric::toString() const
{
    QString out("ACTIONS HISTORIC, current action is "+QString::number(current_action)+" = \n");
    for (int i = 0; i < actionsHistoric.size(); i++){
        auto act = actionsHistoric[i].data();
        if (i == current_action) out += " > ";
        else out += " - ";
        switch (act->actionName){
            case Action::ActionName::Bet: {
                auto bet = static_cast<const Bet*>(act);
                out += bet->playerName + " BET " + QString::number(bet->amount) + '\n';
                break;
            }
            case Action::ActionName::Fold: {
                auto fold = static_cast<const Fold*>(act);
                out += fold->playerName + " FOLD\n";
                break;
            }
            case Action::ActionName::NewRound: {
                auto newRound = static_cast<const NewRound*>(act);
                out += "NEWROUND WITH  : \n";
                for (auto &player : newRound->players){
                    out += '\t' + player + '\n';
                }
                out += "\t(AND A DISCOUNT OF " + QString::number(newRound->discountPerPlayers) + " PER PLAYERS)\n";
                break;
            }
            case Action::ActionName::Win: {
                auto win = static_cast<const Win*>(act);
                out += win->playerName + " WON " + QString::number(win->winAmount) + '\n';
                break;
            }
        }
    }
    return out;
}

void ActionHistoric::wipeFrontActions()
{
    if (current_action >= 0 && current_action+1<actionsHistoric.size()){
        qDeleteAll(actionsHistoric.begin()+current_action+1, actionsHistoric.end());
        actionsHistoric.erase(actionsHistoric.begin()+current_action+1, actionsHistoric.end());
    }else if (current_action == -1) {
        qDeleteAll(actionsHistoric.begin(), actionsHistoric.end());
        actionsHistoric.clear();
    }
}
