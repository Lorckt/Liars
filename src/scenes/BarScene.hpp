#ifndef BAR_SCENE_HPP
#define BAR_SCENE_HPP

\#include "ASCII\_Engine/Scene.hpp"
\#include "gameObjects/Table.hpp"
\#include "ASCII\_Engine/TextObject.hpp"
\#include "ASCII\_Engine/Sprite.hpp"
\#include &lt;memory&gt;
\#include &lt;vector&gt;

// Enum para controlar o fluxo do jogo. A cena estará sempre num destes estados.
enum class GameState {
PLAYER\_TURN,    // A aguardar a ação do jogador humano
AI\_TURN,        // A executar a lógica do oponente (IA)
SHOW\_RESULT,    // A mostrar o resultado de uma acusação (quem mentiu, quem bebe)
GAME\_OVER       // A mostrar a tela de fim de jogo
};

class BarScene : public Scene {
public:
BarScene();
virtual \~BarScene() = default; // O destrutor padrão é suficiente

// Funções principais da engine, chamadas a cada ciclo do jogo
virtual void update(AEngine* engine, float deltaTime) override;
virtual void draw(Drawer& drawer) override;


private:
// --- Variáveis de Lógica e Estado ---
std::unique\_ptr&lt;Table&gt; table; // O "cérebro" com as regras do jogo
GameState currentState;       // O estado atual do jogo

// --- Variáveis de Interface (UI) ---
TextObject* statusText;       // Mostra de quem é o turno
TextObject* promptText;       // Mostra as ações disponíveis (A, D, Enter, L)
TextObject* tableCardText;    // Mostra qual é a carta da mesa
TextObject* resultText;       // Mostra o resultado de uma acusação

Sprite playerHandSprite;      // O sprite completo da mão do jogador

// --- Variáveis de Controlo do Jogador ---
int selectedCardIndex;        // O índice (0, 1, 2...) da carta selecionada na mão
int lastPlayerIndex;          // Guarda quem foi o último a jogar, para ser acusado
std::vector lastPlayedCards; // Guarda as últimas cartas jogadas

// --- Funções de Ajuda Internas ---
void handlePlayerTurn();      // Lógica para o turno do jogador
void handleAITurn();          // Lógica para o turno da IA
void updatePlayerHandSprite(); // A função "mágica" que desenha a mão
void drawTableInfo();         // Atualiza as informações da mesa
void setupNewRound();         // Prepara o início de uma nova ronda


};

\#endif // BAR\_SCENE\_HPP
