#include "Table.hpp"
#include <iostream>
#include <algorithm>
#include <random>

// Construtor: Cria o baralho correto do Liar's Deck
Table::Table(int numPlayers) {
    // Cria os jogadores
    players.push_back(new Player("Voce", true));
    for (int i = 1; i < numPlayers; ++i) {
        players.push_back(new Player("IA " + std::to_string(i), false));
    }
    
    // Cria o baralho de 20 cartas
    for (int i = 0; i < 6; ++i) {
        deck.push_back(Card(ACE, SPADES));
        deck.push_back(Card(KING, SPADES));
        deck.push_back(Card(QUEEN, SPADES));
    }
    deck.push_back(Card(JOKER, NONE));
    deck.push_back(Card(JOKER, NONE));

    currentPlayerIndex = 0;
    announcedValue = ACE; // Valor inicial qualquer
}

Table::~Table() {
    for (auto p : players) {
        delete p;
    }
}

void Table::setupNewRound() {
    // Retorna as cartas da mesa para o baralho principal
    deck.insert(deck.end(), cardsOnTable.begin(), cardsOnTable.end());
    cardsOnTable.clear();

    // Embaralha e distribui
    auto rd = std::random_device{}; 
    auto g = std::mt19937{rd()};
    std::shuffle(deck.begin(), deck.end(), g);
    
    for(auto p : players) p->clearHand();
    
    // Distribui 3 cartas para cada jogador vivo
    int cardsToDeal = 3;
    for(int i = 0; i < cardsToDeal; ++i) {
        for(auto p : players) {
            if (p->isAlive() && !deck.empty()) {
                p->addCard(deck.back());
                deck.pop_back();
            }
        }
    }
    announcedValue = ACE; // Reseta a carta da mesa
}

Player* Table::getCurrentPlayer() { 
    if (currentPlayerIndex >= 0 && (size_t)currentPlayerIndex < players.size())
        return players[currentPlayerIndex];
    return nullptr;
}

Player* Table::getPlayer(int index) { 
    if (index >= 0 && (size_t)index < players.size())
        return players[index];
    return nullptr;
}

int Table::getPlayerCount() const { return players.size(); }

int Table::getCurrentPlayerIndex() const { return currentPlayerIndex; }
CardValue Table::getAnnouncedValue() const { return announcedValue; }
const std::vector<Card>& Table::getTableCards() const { return cardsOnTable; }


bool Table::checkLie(const std::vector<Card>& playedCards) {
    for (const auto& card : playedCards) {
        // Não é mentira se for a carta anunciada ou um Coringa
        if (card.value != announcedValue && card.value != JOKER) {
            return true; // Mentiu!
        }
    }
    return false; // Falou a verdade
}

bool Table::performRussianRoulette(int playerIndex) {
    if (Player* p = getPlayer(playerIndex)) {
        p->incrementRouletteCount();
        if (p->getRouletteCount() >= 2) {
            p->kill();
            return true; // Morreu
        }
    }
    return false; // Sobreviveu
}

void Table::nextTurn() { 
    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    } while (!players[currentPlayerIndex]->isAlive());
}

int Table::getLivingPlayerCount() const { 
    int count = 0;
    for(const auto p : players) {
        if (p->isAlive()) {
            count++;
        }
    }
    return count;
}