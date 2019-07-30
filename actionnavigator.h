#ifndef ACTIONNAVIGATOR_H
#define ACTIONNAVIGATOR_H

#include <QObject>

#include "actionhistoric.h"

class ActionNavigator : public QObject
{
    Q_OBJECT

public:
    explicit ActionNavigator(ActionHistoric &actionHistoric, QObject *parent = nullptr);

signals:
    void redoBet(Bet*);
    void redoFold(Fold*);
    void redoNewRound(NewRound*);
    void redoWin(Win*);
    void undoBet(Bet*);
    void undoFold(Fold*);
    void undoNewRound(NewRound*);
    void undoWin(Win*);

public slots:
    void doAction();
    void undoAction();

private:
    ActionHistoric &actionHistoric;
};

#endif // ACTIONNAVIGATOR_H
