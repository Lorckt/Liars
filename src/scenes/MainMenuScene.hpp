#ifndef MAIN_MENU_SCENE_HPP
#define MAIN_MENU_SCENE_HPP

#include "Fase.hpp"
#include "Sprite.hpp"
#include "TextSprite.hpp"

class MainMenuScene : public Fase {
public:
    MainMenuScene();
    virtual ~MainMenuScene();
    
    virtual unsigned run(SpriteBuffer& tela) override;
    virtual void init() override;
    
private:
    int selectedOption;
    // Corrigido: Voltando a usar TextSprite diretamente para simplicidade
    TextSprite* startText;
    TextSprite* exitText;
};

#endif // MAIN_MENU_SCENE_HPP