#include "BarScene.hpp"
#include "input/Keyboard.hpp"
#include "MainMenuScene.hpp"
#include <sstream>
#include <thread>
#include <chrono>

BarScene::BarScene() : 
    Fase("BarScene", Sprite("rsc/fundo.img")), 
    selectedCardIndex(0), 
    lastPlayerIndex(-1),
    // Corrigido Erro 1: Inicializando os sprites aqui
    cardFrontTemplate("rsc/carta_frente.img"),
    cardBackSprite("rsc/carta_frente.img") // Usando a frente por enquanto
{
    table = std::make_unique<Table>(std::vector<std::string>{"Voce", "Billy", "Anna"});
    
    statusText = new FontSprite("");
    promptText = new FontSprite("");
    tableCardText = new FontSprite("");
    resultText = new FontSprite("");
    
    setupNewRound();
}

BarScene::~BarScene() {
    for (auto obj : handCardObjects) delete obj;
    for (auto obj : tableCardObjects) delete obj;
    delete statusText;
    delete promptText;
    delete tableCardText;
    delete resultText;
}

void BarScene::init() {}

void BarScene::setupNewRound() {
    table->setupNewRound();
    resultText->setText("");
    lastPlayerIndex = -1;
    lastPlayedCards.clear();
    for (auto obj : tableCardObjects) delete obj;
    tableCardObjects.clear();
    currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
    updateHandCardObjects(); 
}

unsigned BarScene::run(SpriteBuffer& tela) {
    if (currentState == GameState::AI_TURN) {
        handleAITurn(tela);
    } else if (currentState == GameState::SHOW_RESULT) {
        drawScreen(tela); // Mostra o resultado
        std::this_thread::sleep_for(std::chrono::seconds(4));
        if(table->getLivingPlayerCount() <= 1) {
            currentState = GameState::GAME_OVER;
        } else {
            setupNewRound();
        }
    } else if (currentState == GameState::GAME_OVER) {
        resultText->setText("FIM DE JOGO! Pressione ENTER para voltar ao menu.");
        drawScreen(tela);
        char input = Keyboard::read();
        if (input == 13) return Fase::MENU;
    }

    if (currentState == GameState::PLAYER_TURN) {
        handlePlayerInput(tela);
    }
    
    drawScreen(tela);
    return Fase::PLAYING;
}

void BarScene::drawScreen(SpriteBuffer& tela) {
    tela.clear();
    // Corrigido Erro 2: Adicionado x=0, y=0
    background->draw(tela, 0, 0); 
    for (auto& cardObj : handCardObjects) {
        // Corrigido Erro 2: Usando as posições do próprio objeto
        cardObj->draw(tela, cardObj->getPosL(), cardObj->getPosC());
    }
    for (auto& cardObj : tableCardObjects) {
        cardObj->draw(tela, cardObj->getPosL(), cardObj->getPosC());
    }
    drawUI(tela);
    show(tela); // Imprime a tela no console
}

void BarScene::handlePlayerInput(SpriteBuffer& tela) {
    Player* player = table->getCurrentPlayer();
    if (!player || !player->isHuman()) {
        return;
    }
    
    updateHandCardObjects();

    char input = Keyboard::read();

    switch(input) {
        case 'd':
        case 'D':
            if (!player->getHand().empty() && (size_t)selectedCardIndex < player->getHand().size() - 1) {
                selectedCardIndex++;
            }
            break;
        case 'a':
        case 'A':
            if (selectedCardIndex > 0) {
                selectedCardIndex--;
            }
            break;
        case 13: // Enter
            if (!player->getHand().empty()) {
                lastPlayedCards = player->playCards({selectedCardIndex});
                lastPlayerIndex = table->getCurrentPlayerIndex();
                
                tableCardObjects.clear();
                for(size_t i = 0; i < lastPlayedCards.size(); ++i) {
                    ObjetoDeJogo* cardObj = new ObjetoDeJogo("tablecard", cardBackSprite, 10, 55 + i * 5);
                    tableCardObjects.push_back(cardObj);
                }
                
                selectedCardIndex = 0;
                table->nextTurn();
                currentState = GameState::AI_TURN;
            }
            break;
        case 'l':
        case 'L':
            if (lastPlayerIndex != -1) {
                bool wasLie = table->checkLie(lastPlayedCards);
                Player* accused = table->getPlayer(lastPlayerIndex);
                
                if(wasLie) {
    resultString = accused->getName() + " MENTIU! Roleta Russa para ele...";
    if(table->performRussianRoulette(lastPlayerIndex)) {
        resultString += " E MORREU!"; // Usamos a nossa string
    }
} else {
    resultString = accused->getName() + " falou a verdade! Roleta Russa para voce...";
    if(table->performRussianRoulette(table->getCurrentPlayerIndex())) {
         resultString += " E MORREU!"; // Usamos a nossa string
    }
}
resultText->setText(resultString); // Agora definimos o texto no FontSprite
                currentState = GameState::SHOW_RESULT;
            }
            break;
    }
}

void BarScene::handleAITurn(SpriteBuffer& tela) {
    promptText->setText(table->getCurrentPlayer()->getName() + " esta a pensar...");
    drawScreen(tela);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    Player* player = table->getCurrentPlayer();
    if(player && !player->isHuman() && !player->getHand().empty()) {
        lastPlayedCards = player->playCards({0});
        lastPlayerIndex = table->getCurrentPlayerIndex();
        
        tableCardObjects.clear();
        for(size_t i = 0; i < lastPlayedCards.size(); ++i) {
            ObjetoDeJogo* cardObj = new ObjetoDeJogo("tablecard", cardBackSprite, 10, 55 + i*5);
            tableCardObjects.push_back(cardObj);
        }
    }
    
    promptText->setText("");
    table->nextTurn();
    currentState = GameState::PLAYER_TURN;
}

void BarScene::updateHandCardObjects() {
    for (auto obj : handCardObjects) delete obj;
    handCardObjects.clear();

    Player* player = table->getPlayer(0);
    if (!player) return;
    const auto& hand = player->getHand();

    if ((size_t)selectedCardIndex >= hand.size() && !hand.empty()) {
        selectedCardIndex = hand.size() - 1;
    }

    std::vector<std::pair<int, int>> positions;
    if (hand.size() == 5) positions = {{25, 10}, {25, 35}, {25, 60}, {25, 85}, {25, 110}};
    else if (hand.size() == 4) positions = {{25, 20}, {25, 50}, {25, 80}, {25, 110}};
    else if (hand.size() == 3) positions = {{25, 35}, {25, 60}, {25, 85}};
    else if (hand.size() == 2) positions = {{25, 45}, {25, 75}};
    else if (hand.size() == 1) positions = {{25, 60}};
    
    if(!positions.empty() && selectedCardIndex >= 0 && (size_t)selectedCardIndex < positions.size()){
        positions[selectedCardIndex].first -= 2;
    }

    for (size_t i = 0; i < hand.size(); ++i) {
        Sprite cardSprite = cardFrontTemplate;
        const auto& card = hand[i];
        
        std::string val = card.valueToString();
        cardSprite.putAt(TextSprite(val, card.getSuitColor()), 1, 2);
        
        ObjetoDeJogo* cardObj = new ObjetoDeJogo("card" + std::to_string(i), cardSprite, positions[i].first, positions[i].second);
        handCardObjects.push_back(cardObj);
    }
}

void BarScene::drawUI(SpriteBuffer& tela) {
    statusText->setText("Turno de: " + table->getCurrentPlayer()->getName());
    statusText->draw(tela, 3, 2);

    Card tempCard(table->getTableCardValue(), CardSuit::NONE); 
    tableCardText->setText("Carta da Mesa: " + tempCard.valueToString());
    tableCardText->draw(tela, 3, 40);

    std::stringstream prompt;
    Player* player = table->getCurrentPlayer();
    if (player && player->isHuman()) {
        prompt << "Use [A] e [D] para selecionar. [ENTER] para jogar.";
        if(lastPlayerIndex != -1) {
             prompt << " [L] para chamar MENTIROSO!";
        }
    }
    promptText->setText(prompt.str());
    promptText->draw(tela, 33, 3);

    resultText->draw(tela, 12, 25);
}