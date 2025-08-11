#include "BarScene.hpp"
#include "ASCII_Engine/input/Keyboard.hpp"
#include "MainMenuScene.hpp"
#include <sstream>
#include <thread>
#include <chrono>

BarScene::BarScene() : Fase(), selectedCardIndex(0), lastPlayerIndex(-1) {
    table = std::make_unique<Table>(std::vector<std::string>{"Voce", "Billy", "Anna"});
    
    statusText = new FontSprite();
    promptText = new FontSprite();
    tableCardText = new FontSprite();
    resultText = new FontSprite();

    // Carrega os sprites base
    try {
        backgroundSprite = Sprite("rsc/sprites/fundo.img");
        cardFrontTemplate = Sprite("rsc/sprites/carta_frente.img");
    } catch (const std::exception& e) {
        // Lida com o erro se os ficheiros não forem encontrados
    }
    
    setupNewRound();
}

BarScene::~BarScene() {
    // Limpa a memória dos objetos de carta criados dinamicamente
    for (auto obj : cardObjects) delete obj;
    cardObjects.clear();
    delete statusText;
    delete promptText;
    delete tableCardText;
    delete resultText;
}

void BarScene::setupNewRound() {
    table->setupNewRound();
    resultText->setText("");
    lastPlayerIndex = -1;
    lastPlayedCards.clear();
    currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
    updateCardObjects(); 
}

unsigned BarScene::run(SpriteBuffer& tela) {
    // Lógica da máquina de estados
    switch(currentState) {
        case GameState::PLAYER_TURN:
            handlePlayerInput(tela);
            break;
        case GameState::AI_TURN:
            handleAITurn(tela);
            break;
        // ... (outros estados como antes)
    }

    // Desenho a cada frame
    tela.clear();
    backgroundSprite.draw(tela, 0, 0); // 1. Desenha o fundo

    for (auto& cardObj : cardObjects) { // 2. Desenha cada carta por cima do fundo
        cardObj->draw(tela);
    }

    drawUI(tela); // 3. Desenha os textos da UI por cima de tudo
    tela.display();

    return Fase::CONTINUE;
}

void BarScene::updateCardObjects() {
    for (auto obj : cardObjects) delete obj;
    cardObjects.clear();

    Player* player = table->getPlayer(0);
    const auto& hand = player->getHand();
    if (hand.empty()) return;

    // --- O PASSO MAIS IMPORTANTE: DEFINIR AS COORDENADAS ---
    // Defina aqui as coordenadas (linha, coluna) para cada carta.
    // Terá de ajustar estes valores para ficarem perfeitos!
    std::vector<std::pair<int, int>> positions;
    if (hand.size() == 4) {
        // Posição para 4 cartas (a selecionada fica uma linha acima)
        positions = {{19, 20}, {19, 50}, {19, 80}, {19, 110}};
        if(selectedCardIndex >= 0 && selectedCardIndex < positions.size()){
            positions[selectedCardIndex].first -= 1; // Sobe a carta selecionada
        }
    }
    // Adicione aqui 'if (hand.size() == 5)' etc. para outras quantidades

    // Cria um ObjetoDeJogo para cada carta na mão
    for (size_t i = 0; i < hand.size(); ++i) {
        Sprite cardSprite = cardFrontTemplate; // Cria uma cópia do molde
        const auto& card = hand[i];
        
        // "Pinta" o número e o naipe no molde
        std::string val = card.valueToString();
        wchar_t suit = card.suitToSymbol();
        
        cardSprite.setChar(1, 2, val[0], card.getSuitColor());
        if(val.length() > 1) cardSprite.setChar(1, 3, val[1], card.getSuitColor());
        cardSprite.setChar(3, 2, suit, card.getSuitColor());

        // Cria o objeto de jogo com o sprite pintado e na posição correta
        ObjetoDeJogo* cardObj = new ObjetoDeJogo("card" + std::to_string(i), cardSprite, positions[i].first, positions[i].second);
        cardObjects.push_back(cardObj);
    }
}

// ... (handlePlayerInput e handleAITurn permanecem funcionalmente iguais, apenas chamam updateCardObjects) ...
// ... (drawUI desenha os TextObjects) ...
