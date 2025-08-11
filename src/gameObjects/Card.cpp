#include "Card.hpp"

// O construtor apenas armazena o valor e o naipe
Card::Card(CardValue v, CardSuit s) : value(v), suit(s) {}

CardValue Card::getValue() const { return value; }
CardSuit Card::getSuit() const { return suit; }

// Retorna o símbolo Unicode (wchar_t) para o naipe
wchar_t Card::suitToSymbol() const {
    switch (suit) {
        case CardSuit::HEARTS:   return L'♥'; // O 'L' antes da string indica que é um caractere "wide" (Unicode)
        case CardSuit::DIAMONDS: return L'♦';
        case CardSuit::CLUBS:    return L'♣';
        case CardSuit::SPADES:   return L'♠';
        default:                 return L'J'; // Símbolo para o Coringa
    }
}

// Retorna a cor correta para o naipe, usando o enum 'Color' da engine
Color Card::getSuitColor() const {
    switch (suit) {
        case CardSuit::HEARTS:
        case CardSuit::DIAMONDS:
            return Color::RED;
        case CardSuit::CLUBS:
        case CardSuit::SPADES:
            return Color::WHITE; // Usamos branco para naipes pretos, para melhor visibilidade no terminal
        default:
            return Color::MAGENTA; // Uma cor distinta para o Coringa
    }
}

// Retorna a representação em string do valor da carta
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
