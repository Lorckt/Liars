#ifndef BAR_SCENE_HPP
#define BAR_SCENE_HPP

#include "ASCII_Engine/Fase.hpp" // Herda de Fase, como no seu projeto
#include "gameObjects/Table.hpp"
#include "ASCII_Engine/FontSprite.hpp" // Usado para texto
#include "ASCII_Engine/Sprite.hpp"
#include <memory>
#include <vector>

// Enum para controlar o fluxo do jogo.
enum class GameState {
    PLAYER_TURN,
    AI_TURN,
    SHOW_RESULT,
    GAME_OVER
};

class BarScene : public Fase {
public:
    BarScene();
    virtual ~BarScene() = default;

    // A função principal da cena, que substitui 'update' e 'draw'
    virtual unsigned run(SpriteBuffer& tela) override;

private:
    // --- Lógica e Estado ---
    std::unique_ptr<Table> table;
    GameState currentState;

    // --- Elementos de UI ---
    FontSprite* statusText;
    FontSprite* promptText;
    FontSprite* tableCardText;
    FontSprite* resultText;

    Sprite playerHandSprite;

    // --- Controlo do Jogador ---
    int selectedCardIndex;
    int lastPlayerIndex;
    std::vector<Card> lastPlayedCards;
    
    // --- Funções de Ajuda ---
    void handlePlayerInput();
    void handleAITurn();
    void updatePlayerHandSprite();
    void drawUITexts();
    void setupNewRound();
};

#endif // BAR_SCENE_HPP
