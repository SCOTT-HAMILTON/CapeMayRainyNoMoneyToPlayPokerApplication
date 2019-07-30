#ifndef PLAYERBOX_H
#define PLAYERBOX_H

#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>

class PlayerBox : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerBox(QWidget *parent = nullptr);
    bool lookBetTransfer(QString, qulonglong);
    void discountForNewGame(qulonglong discountAmount);
    bool isInGame();
    void receiveMoney(qulonglong amount);
    void discountMoney(qulonglong amount);
    QString name() const {return playerName->text();}
    void setName(const QString &name){playerName->setText(name);}

    QVBoxLayout *mainVLay;

    QCheckBox *inGameCheck;

    QLineEdit   *playerName;

    QVBoxLayout *remainderBalanceForm;
    QLabel      *remainderBalanceFormText;
    QLCDNumber  *remainderBalanceFormNum;

    QFormLayout *betForm;
    QPushButton *betFormButton;
    QSpinBox    *betFormSpinBox;

    QPushButton *foldButton;

    QPushButton *winButton;

signals:
    void betTransfer(qulonglong, QString);
    void won(QString);
    void fold(QString);

public slots:
    void emitBetTransfer();
    void emitWon();
    void emitFold();
    void changeInGame(bool inGame);

private:

    qlonglong money;
};

#endif // PLAYERBOX_H
