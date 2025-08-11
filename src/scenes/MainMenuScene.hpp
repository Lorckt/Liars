#ifndef MAIN_MENU_SCENE_HPP
#define MAIN_MENU_SCENE_HPP

\#include "ASCII\_Engine/Scene.hpp"
\#include "ASCII\_Engine/Sprite.hpp"
\#include "ASCII\_Engine/TextObject.hpp"

class MainMenuScene : public Scene {
public:
// Construtor que inicializa a cena
MainMenuScene();

// Destrutor padrão
virtual ~MainMenuScene() = default;

// Função chamada a cada frame para processar a lógica (input do jogador)
virtual void update(AEngine* engine, float deltaTime) override;


private:
int selectedOption;      // Guarda a opção selecionada (0 para Iniciar, 1 para Sair)
Sprite titleSprite;      // O sprite com a arte do título "Liar's Bar"
TextObject\* startText;   // O objeto de texto para "Iniciar Jogo"
TextObject\* exitText;    // O objeto de texto para "Sair"
};

\#endif // MAIN\_MENU\_SCENE\_HPP
