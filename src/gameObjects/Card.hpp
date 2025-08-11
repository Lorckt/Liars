#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include "core/BaseColor.hpp" // Corrigido

enum class CardValue {
    ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, JOKER
};

enum class CardSuit {
    HEARTS, DIAMONDS, CLUBS, SPADES, NONE
};

class Card {
public:
    Card(CardValue value, CardSuit suit);

    CardValue getValue() const;
    CardSuit getSuit() const;
    COR::Cor getSuitColor() const;

    std::string valueToString() const;
    wchar_t suitToSymbol() const;

private:
    CardValue value;
    CardSuit suit;
};

#endif // CARD_HPP