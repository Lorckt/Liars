#include "BarScene.hpp"
#include "ASCII_Engine/input/Keyboard.hpp"
#include "ASCII_Engine/SpriteBuffer.hpp"
#include "MainMenuScene.hpp" // Para poder voltar ao menu
#include <sstream>
#include <thread>
#include <chrono>

BarScene::BarScene() : Fase(), selectedCardIndex(0), lastPlayerIndex(-1) {
    table = std::make_unique<Table>(std::vector<std::string>{"Voce", "Billy", "Anna"});
    
    // Usa FontSprite para texto, como na engine
    statusText = new FontSprite();
    promptText = new FontSprite();
    tableCardText = new FontSprite();
    resultText = new FontSprite();
    
    setupNewRound();
}

void BarScene::setupNewRound() {
    table->setupNewRound();
    resultText->setText("");
    lastPlayerIndex = -1;
    lastPlayedCards.clear();
    currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
    updatePlayerHandSprite(); 
}

unsigned BarScene::run(SpriteBuffer& tela) {
    // Lógica principal da máquina de estados
    switch(currentState) {
        case GameState::PLAYER_TURN:
            handlePlayerInput();
            break;
        case GameState::AI_TURN:
            handleAITurn();
            break;
        case GameState::SHOW_RESULT:
            tela.clear();
            drawUITexts();
            resultText->draw(tela);
            tela.display();
            std::this_thread::sleep_for(std::chrono::seconds(3));
            
            if(table->getLivingPlayerCount() <= 1) {
                currentState = GameState::GAME_OVER;
            } else {
                setupNewRound(); 
            }
            break;
        case GameState::GAME_OVER:
            resultText->setText("FIM DE JOGO! Pressione ENTER para voltar ao menu.");
            if(Keyboard::isKeyPressed(Keyboard::Key::ENTER)) {
                return Fase::MENU; // Sinaliza para voltar ao menu
            }
            break;
    }

    // Desenho a cada frame
    tela.clear();
    playerHandSprite.draw(tela, 1, 15); // Desenha a mão
    drawUITexts(); // Desenha os textos da UI
    resultText->draw(tela); // Desenha o texto de resultado (se houver)
    tela.display();

    return Fase::CONTINUE; // Continua na cena atual
}

void BarScene::handlePlayerInput() {
    Player* player = table->getCurrentPlayer();
    bool selectionChanged = false;

    if (Keyboard::isKeyPressed(Keyboard::Key::D) && !player->getHand().empty() && selectedCardIndex < player->getHand().size() - 1) {
        selectedCardIndex++;
        selectionChanged = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::Key::A) && selectedCardIndex > 0) {
        selectedCardIndex--;
        selectionChanged = true;
    }

    if (selectionChanged) {
        updatePlayerHandSprite();
    }

    if (Keyboard::isKeyPressed(Keyboard::Key::ENTER) && !player->getHand().empty()) {
        lastPlayedCards = player->playCards({selectedCardIndex});
        lastPlayerIndex = table->getCurrentPlayerIndex();
        selectedCardIndex = 0;
        table->nextTurn();
        currentState = GameState::AI_TURN;
        updatePlayerHandSprite();
    }
    
    if (Keyboard::isKeyPressed(Keyboard::Key::L) && lastPlayerIndex != -1) {
        bool wasLie = table->checkLie(lastPlayedCards);
        Player* accused = table->getPlayer(lastPlayerIndex);
        
        if(wasLie) {
            resultText->setText(accused->getName() + " MENTIU! Roleta Russa para ele...");
            if(table->performRussianRoulette(lastPlayerIndex)) {
                resultText->setText(resultText->getText() + " E MORREU!");
            }
        } else {
            resultText->setText(accused->getName() + " falou a verdade! Roleta Russa para voce...");
            if(table->performRussianRoulette(table->getCurrentPlayerIndex())) {
                 resultText->setText(resultText->getText() + " E VOCE MORREU!");
            }
        }
        currentState = GameState::SHOW_RESULT;
    }
}

void BarScene::handleAITurn() {
    // Pausa para simular pensamento
    tela.clear();
    drawUITexts();
    playerHandSprite.draw(tela, 1, 15);
    promptText->setText(table->getCurrentPlayer()->getName() + " esta a pensar...");
    promptText->draw(tela);
    tela.display();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    Player* player = table->getCurrentPlayer();
    if(!player->getHand().empty()) {
        lastPlayedCards = player->playCards({0});
        lastPlayerIndex = table->getCurrentPlayerIndex();
    }

    table->nextTurn();
    currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
    updatePlayerHandSprite();
}

void BarScene::updatePlayerHandSprite() {
    Player* player = table->getPlayer(0);
    const auto& hand = player->getHand();

    if (hand.empty()) {
        playerHandSprite = Sprite();
        return;
    }

    if (selectedCardIndex >= hand.size()) {
        selectedCardIndex = hand.size() - 1;
    }

    std::string path = "rsc/sprites/hands/hand_" + std::to_string(hand.size()) + "_cards_sel_" + std::to_string(selectedCardIndex) + ".img";
    
    try {
        playerHandSprite = Sprite(path);
    } catch (const std::exception& e) {
        playerHandSprite = Sprite();
        promptText->setText("Erro: Sprite nao encontrado: " + path);
        return;
    }

    for (size_t i = 0; i < hand.size(); ++i) {
        const auto& card = hand[i];
        std::string val = card.valueToString();
        wchar_t suit_symbol = card.suitToSymbol();
        
        int card_x_offset = i * 30 + 5; 
        int card_y_offset = (i == selectedCardIndex) ? 2 : 3;

        for(size_t char_idx = 0; char_idx < val.length(); ++char_idx) {
            playerHandSprite.setChar(card_x_offset + char_idx, card_y_offset, val[char_idx], card.getSuitColor());
        }
        playerHandSprite.setChar(card_x_offset, card_y_offset + 1, suit_symbol, card.getSuitColor());
    }
}

void BarScene::drawUITexts() {
    statusText->setText("Turno de: " + table->getCurrentPlayer()->getName());
    statusText->setPosition(3, 2);
    statusText->draw(tela);

    Card tempCard(table->getTableCardValue(), CardSuit::NONE); 
    tableCardText->setText("Carta da Mesa: " + tempCard.valueToString());
    tableCardText->setPosition(40, 3);
    tableCardText->draw(tela);

    std::stringstream prompt;
    prompt << "Use [A] e [D] para selecionar. [ENTER] para jogar. ";
    if(lastPlayerIndex != -1) {
         prompt << "[L] para chamar MENTIROSO!";
    }
    promptText->setText(prompt.str());
    promptText->setPosition(3, 28);
    promptText->draw(tela);
}
