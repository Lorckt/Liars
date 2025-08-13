#ifndef BAR_SCENE_HPP
#define BAR_SCENE_HPP

#include "Fase.hpp"
#include "ObjetoDeJogo.hpp"
#include "Sprite.hpp"
#include "TextSprite.hpp"
#include "gameObjects/Card.hpp"
#include <vector>
#include <string>

// Forward declaration da classe Table
class Table; 

// Enum para controlar o estado do jogo na cena
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
    
    virtual void init() override;
    virtual unsigned run(SpriteBuffer& tela) override;

private:
    // Métodos de lógica interna
    void setupNewRound(); // <-- ADICIONADO DE VOLTA
    void updateHandObjects();
    void render(SpriteBuffer& tela);
    void drawUI(SpriteBuffer& tela);
    void processInput(char input);
    void processAITurn(SpriteBuffer& tela);
    void processShowResult();

    // Estado do jogo
    GameState currentState;
    Table* table;
    bool needsRedraw;
    int selectedCardIndex;
    std::string resultString;

    // Cartas (objetos visuais)
    std::vector<ObjetoDeJogo*> handCardObjects;
    std::vector<ObjetoDeJogo*> tableCardObjects;
    
    // Cartas (dados lógicos da última jogada)
    std::vector<Card> lastPlayedCards;
    int lastPlayerIndex;

    // Sprites e Textos da UI
    Sprite* background_topo;
    Sprite* background_baixo;
    Sprite* cardBackSprite; // Para o verso das cartas na mesa
    TextSprite* statusText;
    TextSprite* tableCardText;
    TextSprite* promptText;
    TextSprite* resultText;
};

#endif // BAR_SCENE_HPP