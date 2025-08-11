// O construtor inicializa a cena, criando os objetos e começando a primeira ronda
BarScene::BarScene() : Scene("BarScene"), selectedCardIndex(0), lastPlayerIndex(-1) {
table = std::make_unique(std::vectorstd::string{"Voce", "Billy", "Anna"});

statusText = new TextObject("status", "", {3, 2});
tableCardText = new TextObject("tablecard", "", {40, 3});
promptText = new TextObject("prompt", "", {3, 28});
resultText = new TextObject("result", "", {25, 12}, Color::YELLOW);

this->addGameObject(statusText);
this->addGameObject(tableCardText);
this->addGameObject(promptText);
this->addGameObject(resultText);

setupNewRound();
}

// Prepara uma nova ronda, reiniciando as variáveis
void BarScene::setupNewRound() {
table->setupNewRound();
resultText->setText("");
lastPlayerIndex = -1;
lastPlayedCards.clear();
currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
updatePlayerHandSprite();
}

// A função 'draw' é chamada a cada frame para desenhar tudo no ecrã
void BarScene::draw(Drawer& drawer) {
this->clearVisualElements(); // Limpa os elementos visuais da frame anterior

// Adiciona o sprite da mão (já personalizado) para ser desenhado
this->addVisualElement(&playerHandSprite, {1, 15});

drawTableInfo(); // Adiciona os textos da mesa

// A função base da engine que desenha todos os elementos adicionados
Scene::draw(drawer);
}

// A função 'update' é a máquina de estados principal do jogo
void BarScene::update(AEngine* engine, float deltaTime) {
switch(currentState) {
case GameState::PLAYER_TURN:
handlePlayerTurn();
break;
case GameState::AI_TURN:
handleAITurn();
break;
case GameState::SHOW_RESULT:
// Pausa o jogo por 3 segundos para mostrar o resultado
std::this_thread::sleep_for(std::chrono::seconds(3));
resultText->setText("");
if(table->getLivingPlayerCount() <= 1) {
currentState = GameState::GAME_OVER;
} else {
setupNewRound();
}
break;
case GameState::GAME_OVER:
resultText->setText("FIM DE JOGO! Pressione ENTER para voltar ao menu.");
if(Keyboard::isKeyPressed(Keyboard::Key::Enter)) {
engine->setCurrentScene("MainMenu");
}
break;
}
}

// Lógica executada quando é a vez do jogador humano
void BarScene::handlePlayerTurn() {
Player* player = table->getCurrentPlayer();
statusText->setText("Turno de: " + player->getName());

std::stringstream prompt;
prompt << "Use [A] e [D] para selecionar. [ENTER] para jogar. ";
if(lastPlayerIndex != -1) {
     prompt << "[L] para chamar MENTIROSO!";
}
promptText->setText(prompt.str());

bool selectionChanged = false;
if (Keyboard::isKeyPressed(Keyboard::Key::D) && !player->getHand().empty() && selectedCardIndex < player->getHand().size() - 1) {
    selectedCardIndex++;
    selectionChanged = true;
}
if (Keyboard::isKeyPressed(Keyboard::Key::A) && selectedCardIndex > 0) {
    selectedCardIndex--;
    selectionChanged = true;
}

// Se a seleção mudou, atualiza o sprite da mão
if (selectionChanged) {
    updatePlayerHandSprite();
}

// Se o jogador pressionar Enter, joga a carta selecionada
if (Keyboard::isKeyPressed(Keyboard::Key::Enter) && !player->getHand().empty()) {
    lastPlayedCards = player->playCards({selectedCardIndex});
    lastPlayerIndex = table->getCurrentPlayerIndex();
    selectedCardIndex = 0;
    table->nextTurn();
    currentState = GameState::AI_TURN;
    updatePlayerHandSprite();
}

// Se o jogador pressionar L, acusa o jogador anterior de mentir
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

// Lógica (muito simples) para o turno da IA
void BarScene::handleAITurn() {
Player* player = table->getCurrentPlayer();
statusText->setText("Turno de: " + player->getName());
promptText->setText(player->getName() + " esta a pensar...");

std::this_thread::sleep_for(std::chrono::seconds(2)); // Pausa para simular pensamento

if(!player->getHand().empty()) {
    lastPlayedCards = player->playCards({0}); // IA sempre joga a primeira carta
    lastPlayerIndex = table->getCurrentPlayerIndex();
}

table->nextTurn();
currentState = table->getCurrentPlayer()->isHuman() ? GameState::PLAYER_TURN : GameState::AI_TURN;
updatePlayerHandSprite();
}

// A função "mágica" que desenha a mão do jogador
void BarScene::updatePlayerHandSprite() {
Player* player = table->getPlayer(0); // O jogador 0 é sempre o humano
const auto& hand = player->getHand();

if (hand.empty()) {
    playerHandSprite = Sprite(); // Define um sprite vazio se não houver cartas
    return;
}

if (selectedCardIndex >= hand.size()) {
    selectedCardIndex = hand.size() - 1;
}

// 1. Constrói o nome do ficheiro do sprite da mão dinamicamente
std::string path = "rsc/sprites/hands/hand_" + std::to_string(hand.size()) + "_cards_sel_" + std::to_string(selectedCardIndex) + ".img";

try {
    // 2. Carrega o sprite "molde" a partir do ficheiro
    playerHandSprite = Sprite(path);
} catch (const std::exception& e) {
    playerHandSprite = Sprite();
    promptText->setText("Erro: Sprite nao encontrado: " + path);
    return;
}

// 3. "Pinta" os detalhes de cada carta no sprite da mão
for (size_t i = 0; i < hand.size(); ++i) {
    const auto& card = hand[i];
    std::string val = card.valueToString();
    wchar_t suit_symbol = card.suitToSymbol();
    Color suit_color = card.getSuitColor();

    // ATENÇÃO: Estes valores de offset são "chutes" baseados nos seus sprites.
    // Pode precisar de os ajustar para alinhar perfeitamente.
    int card_x_offset = i * 30 + 5; 
    int card_y_offset = (i == selectedCardIndex) ? 2 : 3;

    // Pinta o valor (canto superior esquerdo)
    for(size_t char_idx = 0; char_idx < val.length(); ++char_idx) {
        playerHandSprite.setChar(card_x_offset + char_idx, card_y_offset, val[char_idx]);
        playerHandSprite.setColor(card_x_offset + char_idx, card_y_offset, suit_color);
    }
    
    // Pinta o naipe (logo abaixo do valor)
    playerHandSprite.setChar(card_x_offset, card_y_offset + 1, suit_symbol);
    playerHandSprite.setColor(card_x_offset, card_y_offset + 1, suit_color);
}
}

// Desenha as informações de texto da mesa
void BarScene::drawTableInfo() {
CardValue tableVal = table->getTableCardValue();
Card tempCard(tableVal, CardSuit::NONE);
tableCardText->setText("Carta da Mesa: " + tempCard.valueToString());

if(!lastPlayedCards.empty()) {
    std::string playText = "Ultima Jogada: " + std::to_string(lastPlayedCards.size()) + "x " + tempCard.valueToString();
    // Adiciona um objeto de texto temporário para mostrar a última jogada
    this->addVisualElement(new TextObject("lastplay", playText, {55, 5}), true);
}
}


&lt;/immersive&gt;
