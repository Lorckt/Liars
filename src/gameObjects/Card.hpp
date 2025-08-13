#ifndef CARD_HPP
#define CARD_HPP

#include "Cores.hpp"
#include <string>

enum CardValue { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, JOKER };
enum CardSuit { HEARTS, DIAMONDS, CLUBS, SPADES, NONE };

class Card {
private:
    CardValue value;
    CardSuit suit;

public:
    Card(CardValue v, CardSuit s);

    CardValue getValue() const;
    CardSuit getSuit() const;
    
    std::wstring suitToSymbol() const;
    std::string valueToString() const;
    COR::Cor getSuitColor() const;
};

#endif // CARD_HPP