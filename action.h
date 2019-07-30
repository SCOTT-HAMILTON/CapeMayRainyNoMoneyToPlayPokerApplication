#ifndef ACTION_H
#define ACTION_H

#include <QObject>
#include <QString>

struct Action : QObject{
    Q_OBJECT

public:
    enum struct ActionName{Bet, Fold, NewRound, Win};
    Action(ActionName actionName);
    Action(Action const &) = delete; // Constructeur par copie
    Action& operator=(Action const &) = delete; // Opérateur d'affectation

    virtual ~Action(){}
   // Action ( const Action & ) = default;

    ActionName actionName;
};

struct Bet : Action{
    Q_OBJECT

public:
    Bet(qulonglong amount, QString playerName);
    virtual ~Bet(){}
    qulonglong amount;
    QString playerName;
};

struct NewRound : Action{
    Q_OBJECT

public:
    NewRound(qulonglong discount, QStringList players);
    virtual ~NewRound(){}
    qulonglong discountPerPlayers;
    QStringList players;
};

struct Fold : Action{
    Q_OBJECT

public:
    Fold(QString playerName);
    virtual ~Fold(){}
    QString playerName;
};

struct Win : Action{
    Q_OBJECT

public:
    Win(qulonglong amount, QString playerName);
    virtual ~Win(){}
    qulonglong winAmount;
    QString playerName;
};

#endif // ACTION_H
