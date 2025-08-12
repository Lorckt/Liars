#include "BarScene.hpp"
#include "ASCII_Engine/input/Keyboard.hpp"
#include <sstream>
#include <thread>
#include <chrono>

BarScene::BarScene() :
    Fase("BarScene", Sprite("rsc/fundo.img")),
    selectedCardIndex(0),
    lastPlayerIndex(-1),
    needsRedraw(true),
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
    lastPlayerIndex = -1;
    lastPlayedCards.clear();
    for (auto obj : tableCardObjects) delete obj;
    tableCardObjects.clear();
    currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
    updateHandObjects();
    needsRedraw = true;
}

void BarScene::render(SpriteBuffer& tela) {
    tela.clear();
    background->draw(tela, 0, 0);
    
    for (auto& cardObj : tableCardObjects) {
        cardObj->draw(tela, cardObj->getPosL(), cardObj->getPosC());
    }
    for (auto& cardObj : handCardObjects) {
        cardObj->draw(tela, cardObj->getPosL(), cardObj->getPosC());
    }
    
    drawUI(tela);
    
    show(tela);
}

unsigned BarScene::run(SpriteBuffer& tela) {
    if (needsRedraw) {
        render(tela);
        needsRedraw = false;
    }

    if (currentState == GameState::PLAYER_TURN) {
        char input = Keyboard::read();
        if (input == 27) return Fase::END_GAME;
        processInput(input);
    } else if (currentState == GameState::AI_TURN) {
        processAITurn(tela);
    } else if (currentState == GameState::SHOW_RESULT) {
        processShowResult(tela);
    } else if (currentState == GameState::GAME_OVER) {
        resultString = "Fim de Jogo! Pressione Enter para voltar ao menu.";
        needsRedraw = true;
        render(tela);
        char input = Keyboard::read();
        if (input == 13 || input == 10) return Fase::MENU;
    }
    
    return Fase::PLAYING;
}

void BarScene::processInput(char input) {
    Player* player = table->getCurrentPlayer();
    if (!player || !player->isHuman()) return;

    bool actionTaken = false;
    switch(input) {
        case 'd':
        case 'D':
            if (!player->getHand().empty() && (size_t)selectedCardIndex < player->getHand().size() - 1) {
                selectedCardIndex++;
                needsRedraw = true;
            }
            break;
        case 'a':
        case 'A':
            if (selectedCardIndex > 0) {
                selectedCardIndex--;
                needsRedraw = true;
            }
            break;
        case 13:
        case 10:
            if (!player->getHand().empty()) {
                lastPlayedCards = player->playCards({selectedCardIndex});
                lastPlayerIndex = table->getCurrentPlayerIndex();
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
                    if(table->performRussianRoulette(lastPlayerIndex)) resultString += " E morreu!";
                } else {
                    resultString = accused->getName() + " falou a verdade! Roleta russa para voce...";
                    if(table->performRussianRoulette(table->getCurrentPlayerIndex())) resultString += " E morreu!";
                }
                currentState = GameState::SHOW_RESULT;
                needsRedraw = true;
            }
            break;
    }

    if (actionTaken) {
        tableCardObjects.clear();
        for(size_t i = 0; i < lastPlayedCards.size(); ++i) {
            ObjetoDeJogo* cardObj = new ObjetoDeJogo("tablecard", cardBackSprite, 50, 45 + i * 20);
            tableCardObjects.push_back(cardObj);
        }
        selectedCardIndex = 0;
        table->nextTurn();
        currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
        needsRedraw = true;
    }
    updateHandObjects();
}

void BarScene::processAITurn(SpriteBuffer& tela) {
    resultString = table->getCurrentPlayer()->getName() + " esta a pensar...";
    needsRedraw = true;
    render(tela);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    Player* player = table->getCurrentPlayer();
    if(player && !player->isHuman() && !player->getHand().empty()) {
        lastPlayedCards = player->playCards({0});
        lastPlayerIndex = table->getCurrentPlayerIndex();
        
        tableCardObjects.clear();
        for(size_t i = 0; i < lastPlayedCards.size(); ++i) {
            ObjetoDeJogo* cardObj = new ObjetoDeJogo("tablecard", cardBackSprite, 50, 45 + i * 20);
            tableCardObjects.push_back(cardObj);
        }
    }
    
    resultString = "";
    table->nextTurn();
    currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
    needsRedraw = true;
    updateHandObjects();
}

void BarScene::processShowResult(SpriteBuffer& tela) {
    needsRedraw = true;
    render(tela);
    std::this_thread::sleep_for(std::chrono::seconds(4));

    if(table->getLivingPlayerCount() <= 1) {
        currentState = GameState::GAME_OVER;
    } else {
        setupNewRound();
    }
    needsRedraw = true;
}

void BarScene::updateHandObjects() {
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
    int linhaAtual = 10;
    for (int i = 0; i < table->getPlayerCount(); ++i) {
        Player* player = table->getPlayer(i);
        if (player) {
            std::string info = player->getName() + " - Roletas: " + std::to_string(player->getRouletteCount());
            statusText->setText(info);
            statusText->draw(tela, linhaAtual, 120);
            linhaAtual += 2;
        }
    }
    
    linhaAtual += 2;
    Card tempCard(table->getTableCardValue(), CardSuit::NONE);
    tableCardText->setText("Carta da Mesa: " + tempCard.valueToString());
    tableCardText->draw(tela, linhaAtual, 120);

    Player* player = table->getCurrentPlayer();
    if (player && player->isHuman()) {
        promptText->setText("Use [A] e [D] para selecionar.");
        promptText->draw(tela, 80, 120);
        
        promptText->setText("[ENTER] para jogar.");
        promptText->draw(tela, 81, 120);

        if(lastPlayerIndex != -1) {
             promptText->setText("[L] para chamar mentiroso!");
             promptText->draw(tela, 82, 120);
        }
    }
    
    resultText->setText(resultString);
    resultText->draw(tela, 40, 120);
}