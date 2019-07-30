#ifndef ACTIONHISTORIC_H
#define ACTIONHISTORIC_H

#include <QObject>
#include <memory>
#include <QList>
#include <QPointer>

#include "action.h"

class ActionHistoric : public QObject
{

    Q_OBJECT
public:
    explicit ActionHistoric(QObject *parent = nullptr);

    void newBet(qulonglong amount, QString playerName);
    void newFold(QString playerName);
    void newRound(qulonglong discountAmount, QStringList players);
    void newWin(qulonglong winAmount, QString playerName);
    Action* currentAction() const;
    void backToPreviousAction();
    bool forwardToNextAction();
    QString toString() const;
    void wipeFrontActions();

signals:

public slots:

private:
    QList<QPointer<Action>> actionsHistoric;
    int current_action;

};

#endif // ACTIONHISTORIC_H
