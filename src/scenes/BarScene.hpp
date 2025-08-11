#ifndef BAR_SCENE_HPP
#define BAR_SCENE_HPP

#include "ASCII_Engine/Fase.hpp"
#include "gameObjects/Table.hpp"
#include "ASCII_Engine/FontSprite.hpp"
#include "ASCII_Engine/Sprite.hpp"
#include "ASCII_Engine/ObjetoDeJogo.hpp" // Importante incluir
#include <memory>
#include <vector>

enum class GameState {
    PLAYER_TURN, AI_TURN, SHOW_RESULT, GAME_OVER
};

class BarScene : public Fase {
public:
    BarScene();
    virtual ~BarScene(); // Destrutor para limpar a memória dos objetos de carta

    virtual unsigned run(SpriteBuffer& tela) override;

private:
    std::unique_ptr<Table> table;
    GameState currentState;

    // --- Elementos de UI e Arte ---
    FontSprite* statusText;
    FontSprite* promptText;
    FontSprite* tableCardText;
    FontSprite* resultText;

    Sprite backgroundSprite;      // Sprite para o fundo (personagens + mesa)
    Sprite cardFrontTemplate;     // Sprite "molde" para a frente de uma carta

    // Vetor para guardar os objetos de jogo de cada carta na mão
    std::vector<ObjetoDeJogo*> cardObjects;

    // --- Controlo do Jogador ---
    int selectedCardIndex;
    int lastPlayerIndex;
    std::vector<Card> lastPlayedCards;
    
    // --- Funções de Ajuda ---
    void handlePlayerInput(SpriteBuffer& tela);
    void handleAITurn(SpriteBuffer& tela);
    void updateCardObjects(); // Nova função para criar e posicionar as cartas
    void drawUI(SpriteBuffer& tela);
    void setupNewRound();
};

#endif // BAR_SCENE_HPP
