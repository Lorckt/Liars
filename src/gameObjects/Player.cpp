#include &quot;Player.hpp&quot;
#include &lt;algorithm&gt; // Usado para ordenação, se necessário no futuro
#include &lt;iostream&gt;  // Útil para debug

// Implementação do construtor
// Inicializa o jogador com um nome, define-o como vivo e se é humano ou IA
Player::Player(const std::string& name, bool isHuman)
: name(name), alive(true), human(isHuman) {}

// Adiciona uma carta ao final do vetor 'hand' (a mão do jogador)
void Player::addCard(Card card) {
hand.push\_back(card);
}

// Esvazia completamente o vetor 'hand'
void Player::clearHand() {
hand.clear();
}

// --- Implementação dos Getters ---

const std::string& Player::getName() const {
return name;
}

const std::vector&lt;Card&gt;& Player::getHand() const {
return hand;
}

bool Player::isAlive() const {
return alive;
}

bool Player::isHuman() const {
return human;
}

// --- Implementação das Ações ---

// Define o estado do jogador como "morto"
void Player::kill() {
alive = false;
}

// Lógica para jogar cartas:
// Cria uma nova mão ('remainingHand') e um vetor de cartas jogadas ('playedCards').
// Percorre a mão original e move as cartas para o lugar certo.
// No final, substitui a mão antiga pela nova mão.
std::vector&lt;Card&gt; Player::playCards(const std::vector&lt;int&gt;& indices) {
std::vector&lt;Card&gt; playedCards;
std::vector&lt;Card&gt; remainingHand;
std::vector&lt;bool&gt; toRemove(hand.size(), false);

// Marca as cartas que devem ser removidas com base nos índices recebidos
for (int index : indices) {
if (index >= 0 && index < hand.size()) {
playedCards.push_back(hand[index]);
toRemove[index] = true;
}
}

// Constrói a nova mão apenas com as cartas que não foram marcadas para remoção
for (size_t i = 0; i < hand.size(); ++i) {
if (!toRemove[i]) {
remainingHand.push_back(hand[i]);
}
}

// Atualiza a mão do jogador para a nova mão (sem as cartas jogadas)
hand = remainingHand;

// Retorna as cartas que foram jogadas para que a cena do jogo possa processá-las
return playedCards;


}
