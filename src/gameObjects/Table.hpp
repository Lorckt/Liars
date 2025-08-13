#ifndef TABLE_HPP
#define TABLE_HPP

#include "Player.hpp"
#include "Card.hpp"
#include <vector>
#include <string>

class Table {
public:
    Table(int numPlayers = 4);
    ~Table();

    void setupNewRound();
    Player* getCurrentPlayer();
    Player* getPlayer(int index);
    int getPlayerCount() const;
    int getCurrentPlayerIndex() const;
    
    // Novas funções de lógica
    void playerPlays(int playerIndex, const std::vector<int>& cardIndices, CardValue announcedValue);
    bool checkLie(const std::vector<Card>& playedCards);
    bool performRussianRoulette(int playerIndex);
    
    // Getters para o estado do jogo
    CardValue getAnnouncedValue() const;
    int getLivingPlayerCount() const;
    const std::vector<Card>& getTableCards() const;

    void nextTurn();

private:
    std::vector<Player*> players;
    std::vector<Card> deck;
    std::vector<Card> cardsOnTable;
    CardValue announcedValue; // A "carta da mesa"
    int currentPlayerIndex;
};

#endif // TABLE_HPP