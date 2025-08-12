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
    cardFrontTemplate("rsc/carta_frente.img"),
    cardBackSprite("rsc/carta_frente.img")
{
    table = std::make_unique<Table>(std::vector<std::string>{"Voce", "Billy", "Anna"});
    
    statusText = new TextSprite("");
    promptText = new TextSprite("");
    tableCardText = new TextSprite("");
    resultText = new TextSprite("");
    
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
    resultString = "";
    resultText->setText(resultString);
    lastPlayerIndex = -1;
    lastPlayedCards.clear();
    for (auto obj : tableCardObjects) delete obj;
    tableCardObjects.clear();
    currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
    updateHandCardObjects(); 
}

unsigned BarScene::run(SpriteBuffer& tela) {
    if (currentState == GameState::PLAYER_TURN) {
        if (handlePlayerInput(tela) == Fase::END_GAME) {
            return Fase::END_GAME;
    }
} else if (currentState == GameState::AI_TURN) {
        handleAITurn(tela);
    } else if (currentState == GameState::SHOW_RESULT) {
        drawScreen(tela);
        std::this_thread::sleep_for(std::chrono::seconds(4));
        if(table->getLivingPlayerCount() <= 1) {
            currentState = GameState::GAME_OVER;
        } else {
            setupNewRound();
        }
    } else if (currentState == GameState::GAME_OVER) {
        resultString = "Fim de Jogo! Pressione Enter para voltar ao menu.";
        resultText->setText(resultString);
        drawScreen(tela);
        char input = Keyboard::read();
        if (input == 13 || input == 10) return Fase::MENU;
    }
    
    // A chamada para drawScreen foi movida para dentro dos handlers para evitar redesenhos desnecessários
    return Fase::PLAYING;
}

void BarScene::drawScreen(SpriteBuffer& tela) {
    tela.clear();
    background->draw(tela, 0, 0); 
    for (auto& cardObj : handCardObjects) {
        cardObj->draw(tela, cardObj->getPosL(), cardObj->getPosC());
    }
    for (auto& cardObj : tableCardObjects) {
        cardObj->draw(tela, cardObj->getPosL(), cardObj->getPosC());
    }
    drawUI(tela);
    show(tela);
}

unsigned BarScene::handlePlayerInput(SpriteBuffer& tela) {
    Player* player = table->getCurrentPlayer();
    if (!player || !player->isHuman()) {
        return;
    }
    
    updateHandCardObjects();
    drawScreen(tela); // Desenha a tela antes de esperar o input

    char input = Keyboard::read();

    bool actionTaken = false;
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
        case 13: 
        case 10: 
            if (!player->getHand().empty()) {
                lastPlayedCards = player->playCards({selectedCardIndex});
                lastPlayerIndex = table->getCurrentPlayerIndex();
                
                tableCardObjects.clear();
                // NOVAS COORDENADAS PARA AS CARTAS DA MESA
                for(size_t i = 0; i < lastPlayedCards.size(); ++i) {
                    ObjetoDeJogo* cardObj = new ObjetoDeJogo("tablecard", cardBackSprite, 50, 45 + i * 20);
                    tableCardObjects.push_back(cardObj);
                }
                
                selectedCardIndex = 0;
                actionTaken = true;
            }
            break;
        case 'l':
        case 'L':
            if (lastPlayerIndex != -1) {
                bool wasLie = table->checkLie(lastPlayedCards);
                Player* accused = table->getPlayer(lastPlayerIndex);
                
                if(wasLie) {
                    resultString = accused->getName() + " mentiu! Roleta russa para ele...";
                    if(table->performRussianRoulette(lastPlayerIndex)) {
                        resultString += " E morreu!";
                    }
                } else {
                    resultString = accused->getName() + " falou a verdade! Roleta russa para voce...";
                    if(table->performRussianRoulette(table->getCurrentPlayerIndex())) {
                         resultString += " E morreu!";
                    }
                }
                resultText->setText(resultString);
                currentState = GameState::SHOW_RESULT;
            }
            break;
        case 27: // Tecla ESC
            return Fase::END_GAME;
    }

    if (actionTaken) {
        table->nextTurn();
        Player* nextPlayer = table->getCurrentPlayer();
        currentState = nextPlayer->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
    }
        return Fase::PLAYING;
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

    // LÓGICA DE TRANSIÇÃO CORRIGIDA
    table->nextTurn();
    Player* nextPlayer = table->getCurrentPlayer();
    currentState = nextPlayer->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
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
    if (hand.size() == 5) positions = {{80, 5}, {80, 25}, {80, 45}, {80, 65}, {80, 85}};
    else if (hand.size() == 4) positions = {{80, 10}, {80, 35}, {80, 60}, {80, 85}};
    else if (hand.size() == 3) positions = {{80, 15}, {80, 45}, {80, 75}};
    else if (hand.size() == 2) positions = {{80, 25}, {80, 55}};
    else if (hand.size() == 1) positions = {{80, 45}};
    
    if(!positions.empty() && selectedCardIndex >= 0 && (size_t)selectedCardIndex < positions.size()){
        positions[selectedCardIndex].first -= 2;
    }

    // --- INÍCIO DA MUDANÇA (TESTE) ---
    // Nesta versão, nós não vamos mais escrever o valor na carta.
    // Vamos apenas criar o objeto com o sprite da carta em branco.
    for (size_t i = 0; i < hand.size(); ++i) {
        // A linha abaixo cria o objeto de jogo usando a carta em branco diretamente
        ObjetoDeJogo* cardObj = new ObjetoDeJogo("card" + std::to_string(i), cardFrontTemplate, positions[i].first, positions[i].second);
        handCardObjects.push_back(cardObj);
    }
    // --- FIM DA MUDANÇA (TESTE) ---
}

void BarScene::drawUI(SpriteBuffer& tela) {
    // NOVAS COORDENADAS PARA A INTERFACE (lado direito da tela)
    // A coluna (segundo número) agora é > 115.

    statusText->setText("Turno de: " + table->getCurrentPlayer()->getName());
    statusText->draw(tela, 10, 120); // Posição (linha, coluna)

    Card tempCard(table->getTableCardValue(), CardSuit::NONE); 
    tableCardText->setText("Carta da Mesa: " + tempCard.valueToString());
    tableCardText->draw(tela, 12, 120);

    // Mostrando o prompt na parte de baixo, à direita
    std::stringstream prompt;
    Player* player = table->getCurrentPlayer();
    if (player && player->isHuman()) {
        prompt << "Use [A] e [D] para selecionar.";
        promptText->setText(prompt.str());
        promptText->draw(tela, 80, 120);

        prompt.str(""); // Limpa o stream
        prompt << "[ENTER] para jogar.";
        promptText->setText(prompt.str());
        promptText->draw(tela, 81, 120);

        if(lastPlayerIndex != -1) {
             prompt.str("");
             prompt << "[L] para chamar mentiroso!";
             promptText->setText(prompt.str());
             promptText->draw(tela, 82, 120);
        }
    }
    
    // Texto de resultado no meio da área da UI
    resultText->draw(tela, 40, 120);
}