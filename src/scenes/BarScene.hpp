#ifndef BAR_SCENE_HPP
#define BAR_SCENE_HPP

#include "Fase.hpp"
#include "TextSprite.hpp"
#include "Sprite.hpp"
#include "ObjetoDeJogo.hpp"
#include "gameObjects/Table.hpp"
#include "gameObjects/Card.hpp" // <-- Include crucial para a classe Card
#include <memory>
#include <vector>
#include <iostream>

enum class GameState {
    PLAYER_TURN,
    AI_TURN,
    SHOW_RESULT,
    GAME_OVER
};

class BarScene : public Fase {
public:
    BarScene();
    virtual ~BarScene();

    virtual unsigned run(SpriteBuffer& tela) override;
    virtual void init() override;

private:
    void processInput(char input);
    void processAITurn(SpriteBuffer& tela);
    void processShowResult(SpriteBuffer& tela);
    void render(SpriteBuffer& tela);
    void updateHandObjects();
    void drawUI(SpriteBuffer& tela);
    void setupNewRound();

    std::unique_ptr<Table> table;
    GameState currentState;
    bool needsRedraw;

    TextSprite* statusText;
    TextSprite* promptText;
    TextSprite* tableCardText;
    TextSprite* resultText;
    std::string resultString;
    
    Sprite* background_topo;
    Sprite* background_baixo;
    
    // Um "molde" para a frente das cartas
    Sprite cardFrontTemplate;

    // Vetores que guardam os objetos visíveis no jogo
    std::vector<ObjetoDeJogo*> handCardObjects;
    std::vector<ObjetoDeJogo*> tableCardObjects; // Mantemos para a lógica, mas não desenhamos

    int selectedCardIndex;
    int lastPlayerIndex;
    std::vector<Card> lastPlayedCards;
};

#endif // BAR_SCENE_HPP