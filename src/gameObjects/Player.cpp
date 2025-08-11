#include "Player.hpp"
#include <algorithm>
#include <iostream>

Player::Player(const std::string& name, bool isHuman)
    : name(name), alive(true), human(isHuman) {}

void Player::addCard(Card card) {
    hand.push_back(card);
}

void Player::clearHand() {
    hand.clear();
}

const std::string& Player::getName() const {
    return name;
}

const std::vector<Card>& Player::getHand() const {
    return hand;
}

bool Player::isAlive() const {
    return alive;
}

bool Player::isHuman() const {
    return human;
}

void Player::kill() {
    alive = false;
}

std::vector<Card> Player::playCards(const std::vector<int>& indices) {
    std::vector<Card> playedCards;
    std::vector<Card> remainingHand;
    std::vector<bool> toRemove(hand.size(), false);

    for (int index : indices) {
        if (index >= 0 && (size_t)index < hand.size()) {
            playedCards.push_back(hand[index]);
            toRemove[index] = true;
        }
    }

    for (size_t i = 0; i < hand.size(); ++i) {
        if (!toRemove[i]) {
            remainingHand.push_back(hand[i]);
        }
    }

    hand = remainingHand;
    return playedCards;
}
