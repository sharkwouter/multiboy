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

    InputManager * input_manager = nullptr;
    FontManager * font_manager = nullptr;

    std::vector<PlayerManager*> player_managers;

    SDL_Texture * splash_texture = nullptr;

    int active_players = 0;

    void createWindowAndRenderer();

    void drawSplashScreen();
    void drawPlayerScreens();
public:
    GameManager();
    ~GameManager();

    void run();
};

#endif // GAMEMANAGER_HPP