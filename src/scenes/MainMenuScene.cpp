// Implementação do construtor
MainMenuScene::MainMenuScene() : Scene("MainMenu"), selectedOption(0) {
// Carrega a arte do título a partir do arquivo de recurso
titleSprite = Sprite("rsc/sprites/ui/title.img");

// Cria os objetos de texto para as opções do menu
startText = new TextObject("start", "Iniciar Jogo", {35, 12});
exitText = new TextObject("exit", "Sair", {35, 14});

// Adiciona os objetos de texto à cena para que sejam desenhados
this->addGameObject(startText);
this->addGameObject(exitText);

// Adiciona o sprite do título como um elemento visual na cena
this->addVisualElement(&titleSprite, {10, 2});
}

// Implementação da lógica de atualização
void MainMenuScene::update(AEngine* engine, float deltaTime) {
// Verifica se as teclas de navegação foram pressionadas
if (Keyboard::isKeyPressed(Keyboard::Key::Down)) {
selectedOption = 1;
}
if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
selectedOption = 0;
}

// Atualiza a cor do texto para destacar a opção selecionada
if (selectedOption == 0) {
    startText->setTextColor(Color::YELLOW);
    exitText->setTextColor(Color::WHITE);
} else {
    startText->setTextColor(Color::WHITE);
    exitText->setTextColor(Color::YELLOW);
}

// Verifica se a tecla Enter foi pressionada
if (Keyboard::isKeyPressed(Keyboard::Key::Enter)) {
    if (selectedOption == 0) {
        // Se "Iniciar Jogo" estiver selecionado, cria e muda para a cena do bar
        engine->addScene(new BarScene());
        engine->setCurrentScene("BarScene");
    } else {
        // Se "Sair" estiver selecionado, para a execução da engine
        engine->stop();
    }
}
}


&lt;/immersive&gt;
