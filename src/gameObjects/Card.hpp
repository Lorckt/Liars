#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include "core/BaseColor.hpp"

// Enums para os valores e naipes, com JOKER adicionado
enum CardValue { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, JOKER };
enum CardSuit { HEARTS, DIAMONDS, CLUBS, SPADES, NONE };

class Card {
public:
    CardValue value;
    CardSuit suit;

    Card(CardValue v, CardSuit s);

    std::string valueToString() const;
    std::wstring suitToSymbol() const;
    COR::Cor getSuitColor() const;
    std::string getSpritePath() const;
};

#endif // CARD_HPP