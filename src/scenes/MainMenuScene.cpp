#include "MainMenuScene.hpp"
#include "input/Keyboard.hpp"
#include "core/BaseColor.hpp"
#include "BarScene.hpp"

MainMenuScene::MainMenuScene() : Fase("MainMenu", Sprite("rsc/fundo.img")), selectedOption(0) {

    startText = new Sprite("rsc/iniciar.img");
    exitText = new Sprite("rsc/sair.img");
}

MainMenuScene::~MainMenuScene() {
    delete startText;
    delete exitText;
}

void MainMenuScene::init() {
    // Inicialização da cena, se necessário.
}

// Corrigido: Removido o 'T' extra no nome da classe
unsigned MainMenuScene::run(SpriteBuffer& tela) {
    tela.clear();
    background->draw(tela, 0, 0);

    // Corrigido: Chamando setCor diretamente no TextSprite
    if (selectedOption == 0) {
        startText->setCor(COR::AMARELA);
        exitText->setCor(COR::BRANCA);
    } else {
        startText->setCor(COR::BRANCA);
        exitText->setCor(COR::AMARELA);
    }

    // Desenha o texto na tela
    startText->draw(tela, 15, 35);
    exitText->draw(tela, 17, 35);

    show(tela); // Mostra o buffer na tela

    char input = Keyboard::read(); // Espera por input

    switch(input) {
        case 's':
        case 'S':
            selectedOption = 1;
            break;
        case 'w':
        case 'W':
            selectedOption = 0;
            break;
        case 10: // Enter (Nova Linha)
        case 13: // Enter (Retorno de Carro)
            if (selectedOption == 0) {
                return Fase::LEVEL_1; // Muda para a cena do jogo
            } else {
                return Fase::END_GAME; // Sai do jogo
            }
        case 27: // ESC
            return Fase::END_GAME;
    }

    return Fase::PLAYING;
}