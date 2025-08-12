#ifndef TABLE_HPP
#define TABLE_HPP

#include "Player.hpp"
#include "Card.hpp"
#include <vector>
#include <memory>

class Table {
public:
    Table(const std::vector<std::string>& playerNames);
    void setupNewRound();
    Player* getPlayer(int index) const;
    int getPlayerCount() const;
    Player* getCurrentPlayer() const;
    int getCurrentPlayerIndex() const;
    void nextTurn();
    bool checkLie(const std::vector<Card>& lastPlayed) const;
    bool performRussianRoulette(int playerIndex);
    CardValue getTableCardValue() const;
    int getLivingPlayerCount() const;

private:
    void createDeck();
    void shuffleDeck();
    void dealCards(int numCards);

    std::vector<std::unique_ptr<Player>> players;
    std::vector<Card> deck;
    CardValue tableCardValue;
    int currentPlayerIndex;
};

#endif // TABLE_HPP