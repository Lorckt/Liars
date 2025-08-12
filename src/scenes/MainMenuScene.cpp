#include "MainMenuScene.hpp"
#include "input/Keyboard.hpp" // Corrigido
#include "core/BaseColor.hpp" // Corrigido
#include "BarScene.hpp"

MainMenuScene::MainMenuScene() : Fase("MainMenu", Sprite("rsc/fundo.img")), selectedOption(0) {
    startText = new FontSprite("INICIAR JOGO");
    exitText = new FontSprite("SAIR");
}

MainMenuScene::~MainMenuScene() {
    delete startText;
    delete exitText;
}

void MainMenuScene::init() {
    // Inicialização da cena, se necessário.
}

unsigned MainMenuScene::run(SpriteBuffer& tela) {
    tela.clear();
    background->draw(tela, 0, 0);

    if (selectedOption == 0) {
        startText->setCor(COR::AMARELA);
        exitText->setCor(COR::BRANCA);
    } else {
        startText->setCor(COR::BRANCA);
        exitText->setCor(COR::AMARELA);
    }

    startText->draw(tela, 15, 35);
    exitText->draw(tela, 17, 35);

    show(tela);

    char input = Keyboard::read();

    switch(input) {
    case 's':
    case 'S':
        selectedOption = 1;
        break;
    case 'w':
    case 'W':
        selectedOption = 0;
        break;
    case 10: // Enter (código de Nova Linha)
    case 13: // Enter (código de Retorno de Carro)
        if (selectedOption == 0) {
            return Fase::LEVEL_1; // Muda para a cena do jogo
        } else {
            return Fase::END_GAME; // Sai do jogo se "SAIR" estiver selecionado
        }
    case 27: // ESC
        return Fase::END_GAME;
}

    return Fase::PLAYING;
}