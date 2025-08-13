#include "BarScene.hpp"
#include "input/Keyboard.hpp"
#include <sstream>
#include <thread>
#include <chrono>
#include <locale>     
#include <codecvt>

// O construtor é a chave para resolver o erro "no default constructor".
// Inicializamos TODOS os membros do tipo Sprite aqui.
BarScene::BarScene() :
    Fase("BarScene", SpriteBuffer(1,1)),
    selectedCardIndex(0),
    lastPlayerIndex(-1),
    needsRedraw(true),
    cardFrontTemplate("rsc/carta_frente.img") // <-- Inicializa o template da frente
{
    background_topo = new Sprite("rsc/fundo_bar_camadao1.img");
    background_baixo = new Sprite("rsc/fundo_bar_camadao2.img");

    table = std::make_unique<Table>(std::vector<std::string>{"Voce", "Billy", "Anna"});     
    statusText = new TextSprite("");
    promptText = new TextSprite("");
    tableCardText = new TextSprite("");
    resultText = new TextSprite("");
    
    setupNewRound();
}

BarScene::~BarScene() {
    delete background_topo;
    delete background_baixo;
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

// A função updateHandObjects desenha as cartas da mão.
void BarScene::updateHandObjects() {
    // 1. Limpa os objetos de carta antigos da mão
    for (auto obj : handCardObjects) delete obj;
    handCardObjects.clear();

    Player* player = table->getPlayer(0); // Jogador humano
    if (!player) return;
    const auto& hand = player->getHand();
    if (hand.empty()) {
        selectedCardIndex = -1;
        return;
    }
    if ((size_t)selectedCardIndex >= hand.size()) {
        selectedCardIndex = hand.size() - 1;
    }

    // Posições das cartas na mão
    std::vector<std::pair<int, int>> positions;
    if (hand.size() == 5) positions = {{78, 5}, {78, 25}, {78, 45}, {78, 65}, {78, 85}};
    else if (hand.size() == 4) positions = {{78, 10}, {78, 35}, {78, 60}, {78, 85}};
    else if (hand.size() == 3) positions = {{78, 15}, {78, 45}, {78, 75}};
    else if (hand.size() == 2) positions = {{78, 25}, {78, 55}};
    else if (hand.size() == 1) positions = {{78, 45}};
    
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    // 2. Cria os novos objetos de carta para a mão
    for (size_t i = 0; i < hand.size(); ++i) {
        int posX = positions[i].first;
        if(i == (size_t)selectedCardIndex) {
            posX -= 2; // Eleva a carta selecionada
        }

        // 2a. Cria o ObjetoDeJogo com um sprite em branco (o template)
        ObjetoDeJogo* cardObj = new ObjetoDeJogo(("card" + std::to_string(i)).c_str(), cardFrontTemplate, posX, positions[i].second);
        
        // 2b. Pega o ponteiro para o sprite que está DENTRO do objeto
        Sprite* spriteNoObjeto = (Sprite*)cardObj->getSprite();

        // 2c. Desenha o texto diretamente no sprite do objeto
        if (spriteNoObjeto) {
            const auto& cardData = hand[i];
            std::string val = cardData.valueToString();
            std::string sym = converter.to_bytes(cardData.suitToSymbol());
            TextSprite valText(val, cardData.getSuitColor());
            TextSprite symText(sym, cardData.getSuitColor());
            spriteNoObjeto->putAt(valText, 1, 1);
            spriteNoObjeto->putAt(symText, 3, 2);
        }
        
        // 2d. Adiciona o objeto já desenhado à lista da mão
        handCardObjects.push_back(cardObj);
    }
    needsRedraw = true;
}

// A função render desenha apenas o que queremos ver
void BarScene::render(SpriteBuffer& tela) {
    tela.clear();
    background_topo->draw(tela, 0, 0);
    int linha_inicio_baixo = background_topo->getAltura() + 15;
    background_baixo->draw(tela, linha_inicio_baixo, 0);

    // *** AQUI ESTÁ A LÓGICA DO SEU PEDIDO ***
    // Apenas o loop que desenha as cartas da MÃO é executado.
    for (auto& cardObj : handCardObjects) {
        cardObj->draw(tela, cardObj->getPosL(), cardObj->getPosC());
    }

    drawUI(tela);
    show(tela);
}

// O resto do seu ficheiro com a lógica do jogo...
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
            if (!player->isAlive()) { info += " (Morto)"; }
            statusText->setText(info);
            statusText->draw(tela, linhaAtual, coluna_ui);
            linhaAtual += 2;
        }
    }
    linhaAtual += 4;
    Card tempCard(table->getTableCardValue(), CardSuit::SPADES);
    tableCardText->setText("Carta da Mesa: " + tempCard.valueToString());
    tableCardText->draw(tela, linhaAtual, coluna_ui);
    Player* currentPlayer = table->getCurrentPlayer();
    if (currentPlayer && currentPlayer->isHuman()) {
         promptText->setText("Use [A] e [D] para selecionar.");
         promptText->draw(tela, 90, coluna_ui);
         promptText->setText("[ENTER] para jogar.");
         promptText->draw(tela, 91, coluna_ui);
         if(lastPlayerIndex != -1) {
              promptText->setText("[L] para chamar mentiroso!");
              promptText->draw(tela, 92, coluna_ui);
         }
    }
    resultText->setText(resultString);
    resultText->draw(tela, 60, coluna_ui);
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
        case 'd': case 'D':
            if (!player->getHand().empty() && (size_t)selectedCardIndex < player->getHand().size() - 1) {
                selectedCardIndex++;
                needsRedraw = true;
            }
            break;
        case 'a': case 'A':
            if (selectedCardIndex > 0) {
                selectedCardIndex--;
                needsRedraw = true;
            }
            break;
        case 13: case 10:
            if (!player->getHand().empty()) {
                lastPlayedCards = player->playCards({selectedCardIndex});
                lastPlayerIndex = table->getCurrentPlayerIndex();
                actionTaken = true;
            }
            break;
        case 'l': case 'L':
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
        selectedCardIndex = 0;
        table->nextTurn();
        currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
        updateHandObjects();
        needsRedraw = true;
    } else if(needsRedraw) {
        updateHandObjects();
    }
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
    }
    resultString = "";
    table->nextTurn();
    currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
    updateHandObjects();
    needsRedraw = true;
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