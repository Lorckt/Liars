#ifndef PLAYER_HPP
#define PLAYER_HPP

\#include &lt;string&gt;
\#include &lt;vector&gt;
\#include "Card.hpp"

class Player {
public:
// Construtor: cria um jogador com um nome e define se é humano ou IA
Player(const std::string& name, bool isHuman = true);
