#include "Card.hpp"

Card::Card(CardValue v, CardSuit s) : value(v), suit(s) {}

std::string Card::getSpritePath() const {
    if (value == JOKER) {
        return "rsc/cartas/Joker.img";
    }

    std::string path = "rsc/cartas/";
    switch (suit) {
        case HEARTS:   path += "Copas(Heart)/"; break;
        case DIAMONDS: path += "Ouros(Diamons)/"; break;
        case CLUBS:    path += "Paus(Clubes)/"; break;
        case SPADES:   path += "Espadas(Spades)/"; break;
        default: break;
    }

    switch (value) {
        case ACE:   path += "A"; break;
        case JACK:  path += "J"; break;
        case QUEEN: path += "Q"; break;
        case KING:  path += "K"; break;
        default:    path += std::to_string(value); break; // Para outras cartas se necessário
    }
    
    path += "_";
    
    switch (suit) {
        case HEARTS:   path += "H"; break;
        case DIAMONDS: path += "D"; break;
        case CLUBS:    path += "C"; break;
        case SPADES:   path += "S"; break;
        default: break;
    }
    
    path += ".img";
    return path;
}

std::string Card::valueToString() const {
    switch (value) {
        case ACE:   return "AS";
        case JACK:  return "VALETE";
        case QUEEN: return "DAMA";
        case KING:  return "REI";
        case JOKER: return "CORINGA";
        default:    return "?";
    }
}

std::wstring Card::suitToSymbol() const {
    // Não é mais tão necessário com sprites, mas mantemos para a UI
    switch (suit) {
        case HEARTS:   return L"♥";
        case DIAMONDS: return L"♦";
        case CLUBS:    return L"♣";
        case SPADES:   return L"♠";
        default:       return L" ";
    }
}

COR::Cor Card::getSuitColor() const {
    switch (suit) {
        case HEARTS:
        case DIAMONDS:
            return COR::VERMELHA;
        default:
            return COR::PRETA;
    }
}