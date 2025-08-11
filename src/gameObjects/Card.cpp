#include "Card.hpp"

Card::Card(CardValue v, CardSuit s) : value(v), suit(s) {}

CardValue Card::getValue() const { return value; }
CardSuit Card::getSuit() const { return suit; }

wchar_t Card::suitToSymbol() const {
    switch (suit) {
        case CardSuit::HEARTS:   return L'♥';
        case CardSuit::DIAMONDS: return L'♦';
        case CardSuit::CLUBS:    return L'♣';
        case CardSuit::SPADES:   return L'♠';
        default:                 return L'J'; // J para Joker
    }
}

COR::Cor Card::getSuitColor() const { // Corrigido: Usa COR::Cor
    switch (suit) {
        case CardSuit::HEARTS:
        case CardSuit::DIAMONDS:
            return COR::VERMELHA; // Corrigido: Usa o namespace COR
        case CardSuit::CLUBS:
        case CardSuit::SPADES:
            return COR::BRANCA; // Corrigido: Usa o namespace COR
        default:
            return COR::MAGENTA; // Corrigido: Usa o namespace COR
    }
}

std::string Card::valueToString() const {
    switch (value) {
        case CardValue::ACE: return "A";
        case CardValue::TWO: return "2";
        case CardValue::THREE: return "3";
        case CardValue::FOUR: return "4";
        case CardValue::FIVE: return "5";
        case CardValue::SIX: return "6";
        case CardValue::SEVEN: return "7";
        case CardValue::EIGHT: return "8";
        case CardValue::NINE: return "9";
        case CardValue::TEN: return "10";
        case CardValue::JACK: return "J";
        case CardValue::QUEEN: return "Q";
        case CardValue::KING: return "K";
        case CardValue::JOKER: return "JOKER";
        default: return "?";
    }
}