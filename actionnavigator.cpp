#include "actionnavigator.h"
#include "action.h"

#include <iostream>

ActionNavigator::ActionNavigator(ActionHistoric &actionHistoric, QObject *parent) :
    QObject(parent), actionHistoric(actionHistoric)
{

}

void ActionNavigator::doAction()
{
    std::cerr << "redo historic before : \n" << actionHistoric.toString().toStdString() << '\n';
    if (!actionHistoric.forwardToNextAction()){
        std::cerr << "no more actions to redo\n";
        return;
    }
    auto action = actionHistoric.currentAction();
    if (action == nullptr){
        std::cerr << "current action is nullptr!!!\n";
        return;
    }
    switch (action->actionName) {
        case Action::ActionName::Bet: {
            auto bet = static_cast<Bet*>(action);
            emit redoBet(bet);
            break;
        }
        case Action::ActionName::Fold: {
            auto fold = static_cast<Fold*>(action);
            emit redoFold(fold);
            break;
        }
        case Action::ActionName::NewRound: {
            auto newRound = static_cast<NewRound*>(action);
            emit redoNewRound(newRound);
            break;
        }
        case Action::ActionName::Win: {
            auto win = static_cast<Win*>(action);
            emit redoWin(win);
            break;
        }
    }
    std::cerr << "redo historic after : \n" << actionHistoric.toString().toStdString();
}

void ActionNavigator::undoAction()
{
    std::cerr << "undo historic before : \n" << actionHistoric.toString().toStdString() << '\n';
    auto action = actionHistoric.currentAction();
    if (action == nullptr){
        std::cerr << "current action is nullptr!!!\n";
        return;
    }

    actionHistoric.backToPreviousAction();

    switch (action->actionName) {
        case Action::ActionName::Bet: {
            auto bet = static_cast<Bet*>(action);
            emit undoBet(bet);
            break;
        }
        case Action::ActionName::Fold: {
            auto fold = static_cast<Fold*>(action);
            emit undoFold(fold);
            break;
        }
        case Action::ActionName::NewRound: {
            auto newRound = static_cast<NewRound*>(action);
            emit undoNewRound(newRound);
            break;
        }
        case Action::ActionName::Win: {
            auto win = static_cast<Win*>(action);
            emit undoWin(win);
            break;
        }
    }
    std::cerr << "undo historic after : \n" << actionHistoric.toString().toStdString();
}
