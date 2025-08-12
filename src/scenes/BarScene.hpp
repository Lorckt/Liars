#ifndef BAR_SCENE_HPP
#define BAR_SCENE_HPP

#include "Fase.hpp"
#include "gameObjects/Table.hpp"
#include "TextSprite.hpp"
#include "Sprite.hpp"
#include "ObjetoDeJogo.hpp"
#include "utils/Drawer.hpp"
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
    // Funções de Lógica
    void processInput(char input);
    void processAITurn();
    void processShowResult();

    // Função Central de Desenho
    void render(SpriteBuffer& tela);

    // Funções Auxiliares
    void updateHandObjects();
    void drawUI(SpriteBuffer& tela);
    void setupNewRound();

    // Variáveis de Estado
    std::unique_ptr<Table> table;
    GameState currentState;
    bool needsRedraw;

    // Sprites e Textos
    TextSprite* statusText;
    TextSprite* promptText;
    TextSprite* tableCardText;
    TextSprite* resultText;
    std::string resultString;

    Sprite cardFrontTemplate;
    Sprite cardBackSprite;

    std::vector<ObjetoDeJogo*> handCardObjects;
    std::vector<ObjetoDeJogo*> tableCardObjects;

    int selectedCardIndex;
    int lastPlayerIndex;
    std::vector<Card> lastPlayedCards;
};

#endif // BAR_SCENE_HPP