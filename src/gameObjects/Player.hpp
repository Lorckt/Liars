#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include "Card.hpp"

class Player {
public:
    Player(const std::string& name, bool isHuman = true);

    void addCard(Card card);
    std::vector<Card> playCards(const std::vector<int>& indices);
    void clearHand();
    
    const std::string& getName() const;
    const std::vector<Card>& getHand() const;
    bool isAlive() const;
    bool isHuman() const;

    void kill();

private:
    std::string name;
    std::vector<Card> hand;
    bool alive;
    bool human;
};

#endif // PLAYER_HPP
