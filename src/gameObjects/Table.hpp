#ifndef TABLE_HPP
#define TABLE_HPP

\#include &lt;vector&gt;
\#include &lt;memory&gt; // Necessário para std::unique\_ptr (gerenciamento inteligente de memória)
\#include "Player.hpp"
\#include "Card.hpp"

class Table {
public:
// Construtor: cria a mesa e os jogadores com os nomes fornecidos
Table(const std::vector[std::string](std::string)& playerNames);

// Prepara tudo para uma nova rodada (embaralha, distribui cartas, etc.)
void setupNewRound();

// --- Funções para obter o estado atual do jogo (Getters) ---
Player* getPlayer(int index) const;
Player* getCurrentPlayer() const;
CardValue getTableCardValue() const;
int getCurrentPlayerIndex() const;
int getLivingPlayerCount() const;

// --- Funções que controlam a lógica do jogo ---
void nextTurn(); // Passa a vez para o próximo jogador vivo
bool checkLie(const std::vector& playedCards) const; // Verifica se uma jogada foi mentirosa
bool performRussianRoulette(int playerIndex); // Executa a roleta russa para um jogador


private:
// --- Variáveis que guardam o estado do jogo ---
// Usamos unique\_ptr para que a memória dos jogadores seja liberada automaticamente
std::vector\<std::unique\_ptr&lt;Player&gt;\> players;
std::vector&lt;Card&gt; deck;
std::vector&lt;Card&gt; discardPile;
CardValue tableCardValue; // A carta que "vale" na rodada atual
int currentPlayerIndex;

// --- Funções de ajuda internas ---
void createDeck();
void shuffleDeck();
void dealCards(int numCards);


};

\#endif // TABLE\_HPP
