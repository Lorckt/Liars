#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Card.hpp"
#include <vector>
#include <string>

class Player {
public:
    Player(const std::string& name, bool isHuman = false);

    const std::string& getName() const;
    const std::vector<Card>& getHand() const;
    bool isHuman() const;
    bool isAlive() const;

    void addCard(const Card& card);
    std::vector<Card> playCards(const std::vector<int>& cardIndices);
    void takeCards(std::vector<Card>& cards);
    void kill();
    void incrementRouletteCount();
    void clearHand();
    int getRouletteCount() const;

private:
    std::string name;
    std::vector<Card> hand;
    bool human;
    bool alive;
    int rouletteCount;
};

#endif // PLAYER_HPP