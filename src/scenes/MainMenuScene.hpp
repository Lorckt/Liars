#ifndef MAIN_MENU_SCENE_HPP
#define MAIN_MENU_SCENE_HPP

#include "Fase.hpp" // Corrigido
#include "Sprite.hpp" // Corrigido
#include "FontSprite.hpp" // Corrigido

class MainMenuScene : public Fase {
public:
    MainMenuScene();
    virtual ~MainMenuScene();
    
    virtual unsigned run(SpriteBuffer& tela) override;
    virtual void init() override;
    
private:
    int selectedOption;
    FontSprite* startText;
    FontSprite* exitText;
};

#endif // MAIN_MENU_SCENE_HPP