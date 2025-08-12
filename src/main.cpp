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
        // Aumentamos a tela para 200 colunas de largura e 120 de altura
        SpriteBuffer tela(200, 120);
        Fase* cenaAtual = new MainMenuScene();
        unsigned estado = Fase::MENU;

        while (true) {
            if (cenaAtual) {
                estado = cenaAtual->run(tela);
            }

            if (estado == Fase::END_GAME) {
                break; // Sai do loop para encerrar o jogo
            }

            Fase* proximaCena = nullptr;
            if (estado == Fase::LEVEL_1) {
                proximaCena = new BarScene();
            } else if (estado == Fase::MENU) {
                proximaCena = new MainMenuScene();
            }

            if (proximaCena) {
                delete cenaAtual;
                cenaAtual = proximaCena;
            }
        }
        delete cenaAtual;

    } catch (const std::exception& e) {
        std::cerr << "Ocorreu um erro fatal: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}