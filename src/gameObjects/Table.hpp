#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>
#include <string>
#include <memory>
#include "Player.hpp"
#include "Card.hpp"

class Table {
public:
    Table(const std::vector<std::string>& playerNames);

    void setupNewRound();
    
    Player* getPlayer(int index) const;
    Player* getCurrentPlayer() const;
    CardValue getTableCardValue() const;
    int getCurrentPlayerIndex() const;
    int getLivingPlayerCount() const;

    void nextTurn();
    bool checkLie(const std::vector<Card>& playedCards) const;
    bool performRussianRoulette(int playerIndex);

private:
    std::vector<std::unique_ptr<Player>> players; 
    std::vector<Card> deck;
    CardValue tableCardValue;
    int currentPlayerIndex;

    void createDeck();
    void shuffleDeck();
    void dealCards(int numCards);
};

#endif // TABLE_HPP
