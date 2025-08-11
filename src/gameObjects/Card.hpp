#ifndef CARD_HPP
#define CARD_HPP

\#include &lt;string&gt;
\#include &lt;vector&gt;
\#include "ASCII\_Engine/Core/Color.hpp" // Incluir para usar as cores da engine

// Enum para os valores das cartas
enum class CardValue {
ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, JOKER
};

// Enum para os naipes
enum class CardSuit {
HEARTS, DIAMONDS, CLUBS, SPADES, NONE // NONE para Coringas
};

class Card {
public:
// Construtor que cria uma carta com um valor e um naipe
Card(CardValue value, CardSuit suit);
