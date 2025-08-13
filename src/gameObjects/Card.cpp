#include "Card.hpp"

Card::Card(CardValue v, CardSuit s) : value(v), suit(s) {}

CardValue Card::getValue() const { return value; }
CardSuit Card::getSuit() const { return suit; }

std::wstring Card::suitToSymbol() const {
    switch (suit) {
        case HEARTS:   return L"♥";
        case DIAMONDS: return L"♦";
        case CLUBS:    return L"♣";
        case SPADES:   return L"♠";
        default:       return L"?";
    }
}

COR::Cor Card::getSuitColor() const {
    switch (suit) {
        case HEARTS:
        case DIAMONDS:
            return COR::VERMELHA;
        case CLUBS:
        case SPADES:
            return COR::BRANCA;
        default:
            return COR::MAGENTA;
    }
}

std::string Card::valueToString() const {
    switch (value) {
        case ACE:   return "A";
        case TWO:   return "2";
        case THREE: return "3";
        case FOUR:  return "4";
        case FIVE:  return "5";
        case SIX:   return "6";
        case SEVEN: return "7";
        case EIGHT: return "8";
        case NINE:  return "9";
        case TEN:   return "10";
        case JACK:  return "J";
        case QUEEN: return "Q";
        case KING:  return "K";
        case JOKER: return "JOKER";
        default:    return "?";
    }
}