#ifndef BAR_SCENE_HPP
#define BAR_SCENE_HPP

#include "Fase.hpp" // Corrigido
#include "gameObjects/Table.hpp"
#include "FontSprite.hpp" // Corrigido
#include "Sprite.hpp" // Corrigido
#include "ObjetoDeJogo.hpp" // Corrigido
#include "utils/Drawer.hpp" // Corrigido
#include <memory>
#include <vector>

enum class GameState {
    PLAYER_TURN, AI_TURN, SHOW_RESULT, GAME_OVER
};

class BarScene : public Fase {
public:
    BarScene();
    virtual ~BarScene();

    virtual unsigned run(SpriteBuffer& tela) override;
    virtual void init() override;

private:
    std::unique_ptr<Table> table;
    GameState currentState;

    FontSprite* statusText;
    FontSprite* promptText;
    FontSprite* tableCardText;
    FontSprite* resultText;

    Sprite cardFrontTemplate;
    Sprite cardBackSprite;

    std::vector<ObjetoDeJogo*> handCardObjects;
    std::vector<ObjetoDeJogo*> tableCardObjects;

    int selectedCardIndex;
    int lastPlayerIndex;
    std::vector<Card> lastPlayedCards;
    
    void handlePlayerInput(SpriteBuffer& tela);
    void handleAITurn(SpriteBuffer& tela);
    void updateHandCardObjects();
    void drawUI(SpriteBuffer& tela);
    void setupNewRound();
    void drawScreen(SpriteBuffer& tela); // Adicionei a declaração que faltava
};

#endif // BAR_SCENE_HPP