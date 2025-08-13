#include "BarScene.hpp"
#include "gameObjects/Table.hpp"
#include "gameObjects/Player.hpp"
#include "SpriteBuffer.hpp"
#include "input/Keyboard.hpp"
#include <thread>
#include <chrono>

BarScene::BarScene() : Fase("BarScene", SpriteBuffer(120, 360)) {
    background_topo = new Sprite("rsc/fundo_bar_camadao1.img");
    background_baixo = new Sprite("rsc/fundo_bar_camadao2.img");
    cardBackSprite = new Sprite("rsc/cartas/Joker.img"); 

    statusText = new TextSprite("", COR::BRANCA);
    tableCardText = new TextSprite("", COR::BRANCA);
    promptText = new TextSprite("", COR::BRANCA);
    resultText = new TextSprite("", COR::AMARELA);

    table = new Table(4);
    init();
}

BarScene::~BarScene() {
    delete background_topo;
    delete background_baixo;
    delete cardBackSprite;
    delete statusText;
    delete tableCardText;
    delete promptText;
    delete resultText;
    delete table;

    for (auto obj : handCardObjects) delete obj;
    for (auto obj : tableCardObjects) delete obj;
}

void BarScene::init() {
    setupNewRound();
}

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

void BarScene::updateHandObjects() {
    for (auto obj : handCardObjects) delete obj;
    handCardObjects.clear();

    Player* player = table->getPlayer(0); 
    if (!player || !player->isAlive()) return;

    const auto& hand = player->getHand();
    if (hand.empty()) {
        selectedCardIndex = -1;
        needsRedraw = true;
        return;
    }

    if (selectedCardIndex < 0) selectedCardIndex = 0;
    if ((size_t)selectedCardIndex >= hand.size()) {
        selectedCardIndex = hand.size() - 1;
    }

    std::vector<std::pair<int, int>> positions;
    if (hand.size() >= 5) for (size_t i = 0; i < hand.size(); ++i) positions.push_back({78, 5 + (int)i * 20});
    else if (hand.size() == 4) positions = {{78, 10}, {78, 35}, {78, 60}, {78, 85}};
    else if (hand.size() == 3) positions = {{78, 15}, {78, 45}, {78, 75}};
    else if (hand.size() == 2) positions = {{78, 25}, {78, 55}};
    else if (hand.size() == 1) positions = {{78, 45}};

    for (size_t i = 0; i < hand.size(); ++i) {
        const auto& cardData = hand[i];
        int posX = positions[i].first;
        
        if(i == (size_t)selectedCardIndex) posX -= 2; 

        Sprite* cardSprite = new Sprite(cardData.getSpritePath());
        ObjetoDeJogo* cardObj = new ObjetoDeJogo("player_card", *cardSprite, posX, positions[i].second);
        
        handCardObjects.push_back(cardObj);
        delete cardSprite;
    }
    needsRedraw = true;
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
        std::this_thread::sleep_for(std::chrono::seconds(4));
        processShowResult();
    } else if (currentState == GameState::GAME_OVER) {
        resultString = "Fim de Jogo! Pressione Enter.";
        needsRedraw = true;
        render(tela);
        char input = Keyboard::read();
        if (input == 13 || input == 10) return Fase::MENU;
    }

    return Fase::PLAYING;
}

void BarScene::processInput(char input) {
    Player* player = table->getCurrentPlayer();
    if (!player || !player->isHuman() || !player->isAlive()) return;
    bool actionTaken = false;

    switch(input) {
        case 'd': case 'D':
            if (!player->getHand().empty()) {
                selectedCardIndex = (selectedCardIndex + 1) % player->getHand().size();
                updateHandObjects();
            }
            break;
        case 'a': case 'A':
            if (!player->getHand().empty()) {
                selectedCardIndex = (selectedCardIndex - 1 + player->getHand().size()) % player->getHand().size();
                updateHandObjects();
            }
            break;
        case 13: case 10: // Enter
            if (selectedCardIndex != -1) {
                lastPlayedCards = player->playCards({selectedCardIndex});
                lastPlayerIndex = table->getCurrentPlayerIndex();
                actionTaken = true;
            }
            break;
        case 'l': case 'L':
            if (lastPlayerIndex != -1) {
                bool wasLie = table->checkLie(lastPlayedCards);
                Player* accused = table->getPlayer(lastPlayerIndex);
                if (wasLie) {
                    resultString = accused->getName() + " mentiu! Roleta russa para ele...";
                    if (table->performRussianRoulette(lastPlayerIndex)) resultString += " E morreu!";
                } else {
                    resultString = accused->getName() + " falou a verdade! Roleta para voce...";
                    if (table->performRussianRoulette(table->getCurrentPlayerIndex())) resultString += " E morreu!";
                }
                currentState = GameState::SHOW_RESULT;
                needsRedraw = true;
            }
            break;
    }

    if (actionTaken) {
        for (auto obj : tableCardObjects) delete obj;
        tableCardObjects.clear();
        for(size_t i = 0; i < lastPlayedCards.size(); ++i) {
            ObjetoDeJogo* cardObj = new ObjetoDeJogo("tablecard", *cardBackSprite, 50, 45 + i * 20);
            tableCardObjects.push_back(cardObj);
        }
        
        table->nextTurn();
        currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
        updateHandObjects(); 
    }
}

void BarScene::processAITurn(SpriteBuffer& tela) {
    Player* player = table->getCurrentPlayer();
    if (!player || player->isHuman() || !player->isAlive()) return;

    resultString = player->getName() + " esta a pensar...";
    render(tela);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    if (!player->getHand().empty()) {
        lastPlayedCards = player->playCards({0}); 
        lastPlayerIndex = table->getCurrentPlayerIndex();
        
        for (auto obj : tableCardObjects) delete obj;
        tableCardObjects.clear();
        for(size_t i = 0; i < lastPlayedCards.size(); ++i) {
            ObjetoDeJogo* cardObj = new ObjetoDeJogo("tablecard", *cardBackSprite, 50, 45 + i * 20);
            tableCardObjects.push_back(cardObj);
        }
    }
    
    resultString = "";
    table->nextTurn();
    currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
    updateHandObjects();
    needsRedraw = true;
}

void BarScene::processShowResult() {
    if (table->getLivingPlayerCount() <= 1) {
        currentState = GameState::GAME_OVER;
    } else {
        setupNewRound();
    }
    needsRedraw = true;
}

void BarScene::render(SpriteBuffer& tela) {
    tela.clear();
    background_topo->draw(tela, 0, 0);
    background_baixo->draw(tela, background_topo->getAltura() + 15, 0);

    for (auto& cardObj : tableCardObjects) cardObj->draw(tela, cardObj->getPosL(), cardObj->getPosC());
    for (auto& cardObj : handCardObjects) cardObj->draw(tela, cardObj->getPosL(), cardObj->getPosC());

    drawUI(tela);
    show(tela);
}

void BarScene::drawUI(SpriteBuffer& tela) {
    int coluna_ui = 305;
    int linhaAtual = 10;
    
    statusText->setText("--- JOGADORES ---");
    statusText->draw(tela, linhaAtual, coluna_ui);
    linhaAtual += 2;

    for (int i = 0; i < table->getPlayerCount(); ++i) {
        Player* player = table->getPlayer(i);
        if (player) {
            std::string info = player->getName() + " - Roletas: " + std::to_string(player->getRouletteCount());
            if (!player->isAlive()) info += " (Morto)";
            
            if (i == table->getCurrentPlayerIndex()) statusText->setCor(COR::AMARELA);
            else statusText->setCor(COR::BRANCA);

            statusText->setText(info);
            statusText->draw(tela, linhaAtual, coluna_ui);
            linhaAtual += 2;
        }
    }
    
    linhaAtual += 4;
    statusText->setCor(COR::BRANCA);
    Card tempCard(table->getAnnouncedValue(), SPADES); // Usamos SPADES só para poder chamar a função
    tableCardText->setText("Mesa de: " + tempCard.valueToString());
    tableCardText->draw(tela, linhaAtual, coluna_ui);

    Player* currentPlayer = table->getCurrentPlayer();
    if (currentState == GameState::PLAYER_TURN && currentPlayer && currentPlayer->isHuman()) {
         promptText->setText("Use [A] e [D] para selecionar.");
         promptText->draw(tela, 90, coluna_ui);
         promptText->setText("[ENTER] para jogar.");
         promptText->draw(tela, 91, coluna_ui);
         if(lastPlayerIndex != -1) {
              promptText->setText("[L] para chamar mentiroso!");
              promptText->draw(tela, 92, coluna_ui);
         }
    } else {
        promptText->setText("");
        promptText->draw(tela, 90, coluna_ui);
        promptText->draw(tela, 91, coluna_ui);
        promptText->draw(tela, 92, coluna_ui);
    }

    resultText->setText(resultString);
    resultText->draw(tela, 60, coluna_ui);
}