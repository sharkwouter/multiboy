#include "GameManager.hpp"

#include <stdexcept>

#include "FontType.hpp"
#include "constants.hpp"

GameManager::GameManager() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) != 0) {
        throw std::runtime_error(SDL_GetError());
    }
    this->font_manager = new FontManager();
    this->player_managers.reserve(4);
    for (size_t i = 0; i < 4; i++) {
        this->player_managers.push_back(new PlayerManager(font_manager));
    }
    this->player_managers[0]->connect(KEYBOARD_ID);

    this->input_manager = new InputManager(&player_managers);
}

GameManager::~GameManager() {
    for(size_t i = 0; i < player_managers.size(); i++) {
        delete player_managers[i];
        player_managers[i] = nullptr;
    }
    delete this->input_manager;
    delete this->font_manager;
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

void GameManager::run() {
    std::vector<Input> inputs;
    bool has_active_players = false;
    bool b_pressed = false;

    this->createWindowAndRenderer();
    while (!this->input_manager->has_quit_triggered() && !(!has_active_players && b_pressed)) {
        SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
        SDL_RenderClear(this->renderer);

        this->input_manager->getInputs(&inputs);
        this->updatePlayerManagers();

        b_pressed = false;
        for (Input input : inputs) {
            for(PlayerManager* player : this->player_managers) {
                if (input.type == InputType::B) {
                    b_pressed = true;
                }
                if (input.gamepad_id == player->getGamepadId()) {
                    player->handleInput(input);
                }
            }
        }

        has_active_players = false;
        for(PlayerManager* player : this->player_managers) {
            if (!player->isActive()) {
                continue;
            }
            has_active_players = true;

            player->update();
        }
        if (has_active_players) {
            this->drawPlayerScreens();
        } else {
            this->drawSplashScreen();
        }
        SDL_RenderPresent(this->renderer);
    }
}

void GameManager::createWindowAndRenderer() {
    this->window = SDL_CreateWindow("Multiboy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (this->window == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }

    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (this->renderer == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }
}

void GameManager::updatePlayerManagers() {
    if (!this->input_manager->has_controller_event_triggered()) {
        return;
    }

}

void GameManager::drawSplashScreen() {
    SDL_Rect window_rect;
    SDL_Rect splash_rect;

    if(this->splash_texture == nullptr) {
        this->splash_texture = font_manager->getTexture(this->renderer, "Press start/enter", FontType::TITLE,  {0, 0, 0, 255});
    }

    SDL_GetWindowSize(this->window, &window_rect.w, &window_rect.h);

    SDL_QueryTexture(this->splash_texture, NULL, NULL, &splash_rect.w, &splash_rect.h);
    splash_rect.x = window_rect.w/2 - splash_rect.w/2;
    splash_rect.y = window_rect.h/2 - splash_rect.h/2;
    SDL_RenderCopy(this->renderer, this->splash_texture, NULL, &splash_rect);
}


void GameManager::drawPlayerScreens() {
    int i;
    SDL_Rect window_rect;

    // Get the amount of active players
    int active_players = 0;
    for(PlayerManager* player : this->player_managers) {
        if(player->isActive()) {
            active_players++;
        }
    }

    window_rect.x = 0;
    window_rect.y = 0;
    SDL_GetWindowSize(this->window, &window_rect.w, &window_rect.h);
    for(PlayerManager* player : this->player_managers) {
        SDL_Rect dst;
        dst.x = 0;
        dst.y = 0;
        dst.w = window_rect.w;
        dst.h = window_rect.h;
        if(active_players > 1) {
            dst.w /= 2;
            dst.x += (i % 2) * dst.w; 
        }
        if(active_players > 2) {
            dst.h /= 2;
            if(i > 1) {
                dst.y += dst.h;
            }
        }
        
        player->draw(this->renderer, &dst);
        i++;
    }
}
