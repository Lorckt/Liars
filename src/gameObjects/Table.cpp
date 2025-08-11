Este é o arquivo de implementação. Ele contém o "cérebro" de cada função declarada no .hpp.

// Implementação do construtor
Table::Table(const std::vectorstd::string& playerNames) : currentPlayerIndex(0) {
for(size_t i = 0; i < playerNames.size(); ++i) {
// Cria um novo jogador. O primeiro (índice 0) é definido como humano.
players.push_back(std::make_unique(playerNames[i], i == 0));
}
}

// Cria um baralho padrão de 52 cartas + 2 coringas
void Table::createDeck() {
deck.clear();
const std::vector suits = {CardSuit::HEARTS, CardSuit::DIAMONDS, CardSuit::CLUBS, CardSuit::SPADES};

for (const auto& suit : suits) {
    for (int i = 0; i < 13; ++i) { // Percorre de Ás (0) a Rei (12)
        deck.push_back(Card(static_cast<CardValue>(i), suit));
    }
}
deck.push_back(Card(CardValue::JOKER, CardSuit::NONE));
deck.push_back(Card(CardValue::JOKER, CardSuit::NONE));
}

// Embaralha o baralho usando o tempo atual como semente para garantir aleatoriedade
void Table::shuffleDeck() {
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
}

// Distribui um número específico de cartas para cada jogador vivo
void Table::dealCards(int numCards) {
for (auto& player : players) {
player->clearHand(); // Limpa a mão da rodada anterior
if(player->isAlive()) {
for(int i = 0; i < numCards; ++i) {
if (!deck.empty()) {
player->addCard(deck.back()); // Pega a carta do topo do baralho
deck.pop_back();              // Remove a carta do baralho
}}}}}

// Função principal que organiza o início de cada rodada
void Table::setupNewRound() {
discardPile.clear();
createDeck();
shuffleDeck();

// Define a "carta da mesa" para a rodada
tableCardValue = deck.back().getValue();
deck.pop_back();

dealCards(5); // Distribui 5 cartas para cada jogador

// Garante que o primeiro jogador da rodada esteja vivo
currentPlayerIndex = 0;
if(!players[currentPlayerIndex]->isAlive()) {
    nextTurn();
}
}

// --- Implementação dos Getters ---

Player* Table::getPlayer(int index) const {
if (index >= 0 && index < players.size()) {
return players[index].get();
}
return nullptr;
}

Player* Table::getCurrentPlayer() const {
return players[currentPlayerIndex].get();
}

CardValue Table::getTableCardValue() const {
return tableCardValue;
}

int Table::getCurrentPlayerIndex() const {
return currentPlayerIndex;
}

int Table::getLivingPlayerCount() const {
int count = 0;
for(const auto& p : players) {
if (p->isAlive()) {
count++;
}
}
return count;
}

// --- Implementação da Lógica do Jogo ---

// Avança para o próximo jogador que ainda está no jogo
void Table::nextTurn() {
do {
currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
} while (!players[currentPlayerIndex]->isAlive());
}

// Verifica se alguma das cartas jogadas é uma "mentira"
bool Table::checkLie(const std::vector& playedCards) const {
for (const auto& card : playedCards) {
// Uma mentira é qualquer carta que não seja a carta da mesa ou um coringa
if (card.getValue() != tableCardValue && card.getValue() != CardValue::JOKER) {
return true; // Mentira encontrada!
}
}
return false; // Todas as cartas são válidas (verdade)
}

// Simula a roleta russa com 1 chance em 6
bool Table::performRussianRoulette(int playerIndex) {
if (playerIndex < 0 || playerIndex >= players.size()) return false;

int roll = rand() % 6; // Gera um número de 0 a 5
if (roll == 0) { // Se for 0, o jogador perde
    players[playerIndex]->kill();
    return true; // Retorna true para indicar que o jogador morreu
}
return false; // O jogador sobreviveu
}


&lt;/immersive&gt;
