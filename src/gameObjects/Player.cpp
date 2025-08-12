#include "Player.hpp"
#include <algorithm>
#include <stdexcept>

Player::Player(const std::string& name, bool isHuman)
    : name(name), human(isHuman), alive(true), rouletteCount(0) {}

const std::string& Player::getName() const {
    return name;
}

const std::vector<Card>& Player::getHand() const {
    return hand;
}

bool Player::isHuman() const {
    return human;
}

bool Player::isAlive() const {
    return alive;
}

void Player::addCard(const Card& card) {
    hand.push_back(card);
}

void Player::clearHand() {
    hand.clear();
}

std::vector<Card> Player::playCards(const std::vector<int>& cardIndices) {
    std::vector<Card> playedCards;
    std::vector<Card> remainingHand;

    std::vector<bool> toPlayFlags(hand.size(), false);
    for (int index : cardIndices) {
        if (index >= 0 && static_cast<size_t>(index) < hand.size()) {
            toPlayFlags[index] = true;
        }
    }

    for (size_t i = 0; i < hand.size(); ++i) {
        if (toPlayFlags[i]) {
            playedCards.push_back(hand[i]);
        } else {
            remainingHand.push_back(hand[i]);
        }
    }
    hand = remainingHand;
    return playedCards;
}

void Player::takeCards(std::vector<Card>& cards) {
    hand.insert(hand.end(), cards.begin(), cards.end());
    cards.clear();
}

void Player::kill() {
    alive = false;
}

void Player::incrementRouletteCount() {
    rouletteCount++;
}

int Player::getRouletteCount() const {
    return rouletteCount;
}