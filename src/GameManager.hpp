#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <SDL.h>
#include <vector>

#include "InputManager.hpp"
#include "FontManager.hpp"
#include "PlayerManager.hpp"

class GameManager {

private:
    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;

    InputManager input_manager;
    FontManager font_manager;

    std::vector<PlayerManager*> player_managers;

    SDL_Texture * splash_texture = nullptr;

    void createWindowAndRenderer();
    void updatePlayerManagers();

    void drawSplashScreen();
    void drawPlayerScreens();
public:
    GameManager();
    ~GameManager();

    void run();
};

#endif // GAMEMANAGER_HPP