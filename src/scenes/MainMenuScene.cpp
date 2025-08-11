#include "MainMenuScene.hpp"
#include "ASCII_Engine/input/Keyboard.hpp"
#include "ASCII_Engine/core/BaseColor.hpp"
#include "BarScene.hpp"

MainMenuScene::MainMenuScene() : Fase("MainMenu", Sprite("rsc/fundo.img")), selectedOption(0) {
    startText = new FontSprite("Iniciar Jogo");
    exitText = new FontSprite("Sair");
}

MainMenuScene::~MainMenuScene() {
    delete startText;
    delete exitText;
}

void MainMenuScene::init() {
    // Inicialização da cena, se necessário.
}

unsigned MainMenuScene::run(SpriteBuffer& tela) {
    // A engine que você está usando parece não ter um método estático update() ou isKeyPressed().
    // A leitura do teclado é feita diretamente com Keyboard::read().
    // Vamos adaptar para um modo de input bloqueante simples.
    
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

    show(tela); // Mostra o estado atual da tela

    char input = Keyboard::read(); // Leitura bloqueante do teclado

    switch(input) {
        case 's':
        case 'S':
            selectedOption = 1;
            break;
        case 'w':
        case 'W':
            selectedOption = 0;
            break;
        case 13: // Código ASCII para Enter
            if (selectedOption == 0) {
                return Fase::LEVEL_1; // Muda para a cena do jogo
            } else {
                return Fase::END_GAME; // Fecha o jogo
            }
        case 27: // Código ASCII para ESC
            return Fase::END_GAME;
    }

    return Fase::PLAYING;
}