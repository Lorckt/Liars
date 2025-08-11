#include "ASCII_Engine/Fase.hpp"
#include "scenes/MainMenuScene.hpp"
#include "scenes/BarScene.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <locale.h>

void setupConsole() {
    setlocale(LC_ALL, "");
}

int main() {
    srand(time(0));
    setupConsole();

    try {
        Fase* cenaAtual = new MainMenuScene();
        SpriteBuffer tela(150, 40);

        unsigned estado = Fase::MENU; // Estado inicial
        while(estado != Fase::END_GAME)
        {
            if (cenaAtual)
                estado = cenaAtual->run(tela);

            if(estado == Fase::LEVEL_1) { // Usando enum da engine
                delete cenaAtual;
                cenaAtual = new BarScene();
            } else if (estado == Fase::MENU) {
                 delete cenaAtual;
                cenaAtual = new MainMenuScene();
            }
        }
        delete cenaAtual;

    } catch (const std::exception& e) {
        std::cerr << "Ocorreu um erro fatal: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}