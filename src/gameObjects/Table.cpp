#include "Table.hpp"
#include <random>
#include <algorithm>
#include <chrono>

Table::Table(const std::vector<std::string>& playerNames) : currentPlayerIndex(0) {
    for(size_t i = 0; i < playerNames.size(); ++i) {
        players.push_back(std::make_unique<Player>(playerNames[i], i == 0));
    }
}

void Table::createDeck() {
    deck.clear();
    const std::vector<CardSuit> suits = {CardSuit::HEARTS, CardSuit::DIAMONDS, CardSuit::CLUBS, CardSuit::SPADES};
    
    for (const auto& suit : suits) {
        for (int i = 0; i < 13; ++i) {
            deck.push_back(Card(static_cast<CardValue>(i), suit));
        }
    }
    deck.push_back(Card(CardValue::JOKER, CardSuit::NONE));
    deck.push_back(Card(CardValue::JOKER, CardSuit::NONE));
}

void Table::shuffleDeck() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
}

void Table::dealCards(int numCards) {
    for (auto& player : players) {
        player->clearHand();
        if(player->isAlive()) {
            for(int i = 0; i < numCards; ++i) {
                if (!deck.empty()) {
                    player->addCard(deck.back());
                    deck.pop_back();
                }
            }
        }
    }
}

void Table::setupNewRound() {
    createDeck();
    shuffleDeck();
    
    tableCardValue = deck.back().getValue();
    deck.pop_back();

    dealCards(5);
    
    currentPlayerIndex = 0;
    if(!players[currentPlayerIndex]->isAlive()) {
        nextTurn();
    }
}

Player* Table::getPlayer(int index) const {
    if (index >= 0 && (size_t)index < players.size()) {
        return players[index].get();
    }
    return nullptr;
}

Player* Table::getCurrentPlayer() const {
    return players[currentPlayerIndex].get();
}

CardValue Table::getTableCardValue() const {
    return tableCardValue;
}

int Table::getCurrentPlayerIndex() const {
    return currentPlayerIndex;
}

int Table::getLivingPlayerCount() const {
    int count = 0;
    for(const auto& p : players) {
        if (p->isAlive()) {
            count++;
        }
    }
    return count;
}

void Table::nextTurn() {
    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    } while (!players[currentPlayerIndex]->isAlive());
}

bool Table::checkLie(const std::vector<Card>& playedCards) const {
    for (const auto& card : playedCards) {
        if (card.getValue() != tableCardValue && card.getValue() != CardValue::JOKER) {
            return true;
        }
    }
    return false;
}

bool Table::performRussianRoulette(int playerIndex) {
    if (playerIndex < 0 || (size_t)playerIndex >= players.size()) return false;
    int roll = rand() % 6;
    if (roll == 0) {
        players[playerIndex]->kill();
        return true;
    }
    return false;
}
