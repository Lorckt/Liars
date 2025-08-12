#ifndef MAIN_MENU_SCENE_HPP
#define MAIN_MENU_SCENE_HPP

#include "Fase.hpp"
#include "Sprite.hpp"
#include "TextSprite.hpp" // Garante que o tipo correto seja incluído

class MainMenuScene : public Fase {
public:
    MainMenuScene();
    virtual ~MainMenuScene();
    
    virtual unsigned run(SpriteBuffer& tela) override;
    virtual void init() override;
    
private:
    int selectedOption;
    // Corrigido: As variáveis agora são do tipo TextSprite*
    TextSprite* startText;
    TextSprite* exitText;
};

#endif // MAIN_MENU_SCENE_HPP