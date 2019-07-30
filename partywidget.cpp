#include "partywidget.h"
#include "size.h"

#include <QIcon>
#include <QDebug>

PartyWidget::PartyWidget(QWidget *parent, size_t nb_players, QString unit, qulonglong discountAmount) : QWidget(parent),
  discountAmount(discountAmount), unit(' '+unit), nbPlayers(nb_players), betAmount(0), gotWinner(false),
  actionHistoric(), actionNavigator(actionHistoric), currentPlayer(0)
{
    setFixedSize(static_cast<int>(Size::APP_SIZEW), static_cast<int>(Size::APP_SIZEH));
    mainHLay   = new QHBoxLayout;
    mainHLay->setMargin(0);
    mainHLay->setContentsMargins(-10, -10, -10, -10);
    mainHLay->setSpacing(0);

    playersLay = new QVBoxLayout;
    playersLay->setMargin(0);
    playersLay->setContentsMargins(QMargins(0,0,0,0));
    playersLay->setSpacing(0);

#ifdef THIRDPLAYER_COLUMN
    thirdPlayersLay = new QVBoxLayout;
    thirdPlayersLay->setMargin(0);
    thirdPlayersLay->setContentsMargins(QMargins(0,0,0,0));
    thirdPlayersLay->setSpacing(0);
#endif

    rightPanelLay  = new QVBoxLayout;
    betGroupBox    = new QGroupBox;
    betLay         = new QVBoxLayout;
    betCountText   = new QLabel(tr("The Bet is"));
    QFont font;
    font.setFamily(QString::fromUtf8("Nimbus Sans"));
    font.setPointSize(FONT_SIZE_1);
    font.setBold(false);
    font.setWeight(60);
    font.setKerning(true);
    betCountText->setFont(font);
    betCount       = new QLabel(tr("0")+unit);
    betCount->setFont(font);
    newRoundButton = new QPushButton(tr("New round"));
    newRoundButton->setFixedSize(static_cast<int>(300.0*Size::SIZE_FACTOR), static_cast<int>(60.0*Size::SIZE_FACTOR));

    font.setBold(true);
    font.setWeight(75);
    newRoundButton->setFont(font);

    betGroupBox->setMaximumHeight(static_cast<int>(400.0*Size::SIZE_FACTOR));

    betLay->addWidget(betCountText);
    betLay->addWidget(betCount);
    betLay->addWidget(newRoundButton);
    betGroupBox->setLayout(betLay);

    navButtonsLay  = new QHBoxLayout;
    undoButton     = new QCommandLinkButton(tr(""));
    redoButton     = new QCommandLinkButton(tr(""));
    undoButton->setMaximumHeight(static_cast<int>(100.0*Size::SIZE_FACTOR));
    undoButton->setIcon(QIcon("://icons/icons8-back-to-96.png"));
    font.setPointSize(FONT_SIZE_0);
    double s = 100;
    undoButton->setIconSize(QSize(static_cast<int>(s*Size::SIZE_FACTOR), static_cast<int>(s*Size::SIZE_FACTOR)));
    redoButton->setMaximumHeight(static_cast<int>(100.0*Size::SIZE_FACTOR));
    redoButton->setIcon(QIcon("://icons/icons8-next-page-96.png"));
    redoButton->setIconSize(QSize(static_cast<int>(s*Size::SIZE_FACTOR), static_cast<int>(s*Size::SIZE_FACTOR)));
    undoButton->setFont(font);
    redoButton->setFont(font);
    undoButton->setStyleSheet("background-color: rgba(255,255,255,0);");
    redoButton->setStyleSheet("background-color: rgba(255,255,255,0);");
    undoButton->setFixedSize(static_cast<int>(s*Size::SIZE_FACTOR)+20, static_cast<int>(s*Size::SIZE_FACTOR)+20);
    redoButton->setFixedSize(static_cast<int>(s*Size::SIZE_FACTOR+20), static_cast<int>(s*Size::SIZE_FACTOR)+20);


    navButtonsLay->addWidget(undoButton);
    navButtonsLay->addWidget(redoButton);

    for (size_t i = 0; i < nbPlayers; i++ ){
        PlayerBox *playerBox = nullptr;
        playerBox = new PlayerBox;
        playerBox->setName("Player "+QString::number(i+1));
        connect(playerBox, &PlayerBox::betTransfer, this, &PartyWidget::transferToBetCount);
        connect(playerBox, &PlayerBox::won, this, &PartyWidget::throwBetWin);
        connect(playerBox, &PlayerBox::fold, this, &PartyWidget::makePlayerFold);
        if (i <= 2){
            playersLay->addWidget(playerBox, 0, Qt::AlignTop);
        }
        if (i == 3 || i == 4){

            rightPanelLay->addWidget(playerBox);
            if (i == 3){
                rightPanelLay->addWidget(betGroupBox);
            }
            qDebug("added to right panel");
        }
        if (i > 4){
            thirdPlayersLay->addWidget(playerBox, 0, Qt::AlignTop);
        }
        playerBoxList.append(playerBox);
    }

    if (nbPlayers<=3){
        rightPanelLay->addWidget(betGroupBox);
    }

    rightPanelLay->addLayout(navButtonsLay);

    mainHLay->addLayout(playersLay);
    mainHLay->addLayout(rightPanelLay);
#ifdef THIRDPLAYER_COLUMN
    mainHLay->addLayout(thirdPlayersLay);
#endif

    setLayout(mainHLay);

    collectRoundDiscount();

    connect(newRoundButton, &QPushButton::clicked, this, &PartyWidget::makeNewRound);
    connect(undoButton, &QCommandLinkButton::clicked, &actionNavigator, &ActionNavigator::undoAction);
    connect(redoButton, &QCommandLinkButton::clicked, &actionNavigator, &ActionNavigator::doAction);
    connect(&actionNavigator, &ActionNavigator::undoBet, this, &PartyWidget::undoBet);
    connect(&actionNavigator, &ActionNavigator::redoBet, this, &PartyWidget::redoBet);
    connect(&actionNavigator, &ActionNavigator::undoFold, this, &PartyWidget::undoFold);
    connect(&actionNavigator, &ActionNavigator::redoFold, this, &PartyWidget::redoFold);
    connect(&actionNavigator, &ActionNavigator::undoNewRound, this, &PartyWidget::undoNewRound);
    connect(&actionNavigator, &ActionNavigator::redoNewRound, this, &PartyWidget::redoNewRound);
    connect(&actionNavigator, &ActionNavigator::undoWin, this, &PartyWidget::undoWin);
    connect(&actionNavigator, &ActionNavigator::redoWin, this, &PartyWidget::redoWin);

    static_cast<PlayerBox*>(playerBoxList[currentPlayer])->betFormSpinBox->setFocus();

    qDebug() << " geo command link buttons : " << undoButton->geometry();
}

void PartyWidget::injectAmountToEachClient(qulonglong amount)
{
    for (auto player : playerBoxList){
        static_cast<PlayerBox*>(player)->receiveMoney(amount);
    }
}

void PartyWidget::transferToBetCount(qulonglong amount, QString playerBettingName)
{
    actionHistoric.newBet(amount, playerBettingName);
    qDebug() << actionHistoric.toString();
    nextPlayer();
    static_cast<PlayerBox*>(playerBoxList[currentPlayer])->betFormSpinBox->setFocus();

    betAmount+=amount;
    updateBetCountText();
}

void PartyWidget::throwBetWin(QString winnerName)
{
    if (gotWinner){
        qDebug("Already have winner!!!");
        return;
    }
    for (auto player : playerBoxList){
        if ( static_cast<PlayerBox*>(player)->lookBetTransfer(winnerName, betAmount)){
            qDebug("Player catched.");
            gotWinner = true;
            break;
        }
    }
    if (gotWinner){
        actionHistoric.newWin(betAmount, winnerName);
        qDebug() << actionHistoric.toString();
        betAmount = 0;
        updateBetCountText();
    }else{
        qDebug("Weird case, no winner");
    }
}

void PartyWidget::makePlayerFold(QString playerName)
{
    for (int i = 0; i < playerBoxList.size(); i++){
        auto playerBox = static_cast<PlayerBox*>(playerBoxList[i]);
        if (playerBox->name() == playerName ){
            playerBox->setEnabled(false);
            nextPlayer();
            static_cast<PlayerBox*>(playerBoxList[currentPlayer])->betFormSpinBox->setFocus();
            qDebug() << "Still doing my thing";
            actionHistoric.newFold(playerName);
            qDebug() << "Still doing my 2nd thing";
            break;
        }
    }
}

void PartyWidget::makeNewRound()
{
    if (!gotWinner){
        qDebug("no winner, can't make new round");
        return;
    }

    gotWinner = false;

    QStringList playersInGame = collectRoundDiscount();

    actionHistoric.newRound(discountAmount, playersInGame);

    for (int i = 0; i < playerBoxList.size(); i++){
        auto playerBox = static_cast<PlayerBox*>(playerBoxList[i]);
        if (playerBox->isInGame()){
            playerBox->setEnabled(true);
        }
    }

    qDebug() << actionHistoric.toString();
}

void PartyWidget::undoBet(Bet *bet)
{
    for (int i = 0; i < playerBoxList.size(); i++){
        auto playerBox = static_cast<PlayerBox*>(playerBoxList[i]);
        if (playerBox->name() == bet->playerName ){
            currentPlayer = i;
            playerBox->receiveMoney(bet->amount);
            betAmount -= bet->amount;
            break;
        }
    }
    updateBetCountText();
}

void PartyWidget::redoBet(Bet *bet)
{
    for (int i = 0; i < playerBoxList.size(); i++){
        auto playerBox = static_cast<PlayerBox*>(playerBoxList[i]);
        if (playerBox->name() == bet->playerName ){
            currentPlayer = i;
            playerBox->discountMoney(bet->amount);
            betAmount += bet->amount;
            nextPlayer();
            break;
        }
    }
    updateBetCountText();
}

void PartyWidget::undoFold(Fold *fold)
{
    for (int i = 0; i < playerBoxList.size(); i++){
        auto playerBox = static_cast<PlayerBox*>(playerBoxList[i]);
        if (playerBox->name() == fold->playerName ){
            currentPlayer = i;
            playerBox->inGameCheck->setCheckState(Qt::CheckState::Checked);
            playerBox->setEnabled(true);
            static_cast<PlayerBox*>(playerBoxList[currentPlayer])->betFormSpinBox->setFocus();
            break;
        }
    }

}

void PartyWidget::redoFold(Fold *fold)
{
    for (int i = 0; i < playerBoxList.size(); i++){
        auto playerBox = static_cast<PlayerBox*>(playerBoxList[i]);
        if (playerBox->name() == fold->playerName ){
            currentPlayer = i;
            playerBox->inGameCheck->setCheckState(Qt::CheckState::Checked);
            playerBox->setEnabled(false);
            nextPlayer();
            static_cast<PlayerBox*>(playerBoxList[currentPlayer])->betFormSpinBox->setFocus();
            break;
        }
    }
}

void PartyWidget::undoNewRound(NewRound *newRound)
{
    betAmount = 0;
    updateBetCountText();
    for (int i = 0; i < playerBoxList.size(); i++){
        auto playerBox = static_cast<PlayerBox*>(playerBoxList[i]);
        if (playerBox->isInGame()){
           playerBox->receiveMoney(newRound->discountPerPlayers);
        }
    }
}

void PartyWidget::redoNewRound(NewRound *newRound)
{
    betAmount = 0;
    for (int i = 0; i < playerBoxList.size(); i++){
        auto playerBox = static_cast<PlayerBox*>(playerBoxList[i]);
        if (playerBox->isInGame()){
            betAmount += newRound->discountPerPlayers;
           playerBox->discountMoney(newRound->discountPerPlayers);
        }
    }
    updateBetCountText();
}

void PartyWidget::undoWin(Win *win)
{
    for (int i = 0; i < playerBoxList.size(); i++){
        auto playerBox = static_cast<PlayerBox*>(playerBoxList[i]);
        if (playerBox->name() == win->playerName ){
            playerBox->discountMoney(win->winAmount);
            betAmount += win->winAmount;
            gotWinner = false;
            break;
        }
    }
    updateBetCountText();
}

void PartyWidget::redoWin(Win *win)
{
    for (int i = 0; i < playerBoxList.size(); i++){
        auto playerBox = static_cast<PlayerBox*>(playerBoxList[i]);
        if (playerBox->name() == win->playerName ){
            playerBox->receiveMoney(win->winAmount);
            betAmount -= win->winAmount;
            gotWinner = true;
            break;
        }
    }
    updateBetCountText();
}

void PartyWidget::nextPlayer()
{
    currentPlayer++;
    if (currentPlayer>=playerBoxList.size())currentPlayer=0;
}

void PartyWidget::updateBetCountText()
{
    betCount->setText(QString::number(betAmount)+unit);
}

QStringList PartyWidget::collectRoundDiscount()
{
    QStringList playersInGame;
    for (auto player : playerBoxList){
        auto playerBox = static_cast<PlayerBox*>(player);
        if (playerBox->isInGame()){
            playersInGame.append(playerBox->name());
            playerBox->discountForNewGame(discountAmount);
            betAmount += discountAmount;
        }
    }
    updateBetCountText();

    return playersInGame;
}
