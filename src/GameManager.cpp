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
    bool should_quit = false;

    this->createWindowAndRenderer();
    while (!this->input_manager->has_quit_triggered() && !should_quit) {
        SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
        SDL_RenderClear(this->renderer);

        this->input_manager->getInputs(&inputs);
        this->updatePlayerManagers();

        for (Input input : inputs) {
            for(PlayerManager* player : this->player_managers) {
                if (input.gamepad_id == player->getGamepadId()) {
                    player->handleInput(input);
                }
            }
        }

        this->active_players = 0;
        for(PlayerManager* player : this->player_managers) {
            if (!player->isActive()) {
                continue;
            }
            this->active_players++;

            player->update();
        }
        if (this->active_players > 0) {
            this->drawPlayerScreens();
        } else {
            for (Input input : inputs) {
                if (input.type == InputType::B) {
                    should_quit = true;
                }
            }
            if (should_quit) {
                continue;
            }
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
    SDL_ShowCursor(0);
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
    int i = 0;
    SDL_Rect window_rect;

    window_rect.x = 0;
    window_rect.y = 0;
    SDL_GetWindowSize(this->window, &window_rect.w, &window_rect.h);
    for(PlayerManager* player : this->player_managers) {
        if (!player->isActive()) {
            continue;
        }
        SDL_Rect dst;
        dst.x = 0;
        dst.y = 0;
        dst.w = window_rect.w;
        dst.h = window_rect.h;
        if(this->active_players > 1) {
            dst.w /= 2;
            dst.x += (i % 2) * dst.w;
        }
        if(this->active_players > 2) {
            dst.h /= 2;
            if(i > 1) {
                dst.y += dst.h;
            }
        }
        
        player->draw(this->renderer, &dst);
        i++;
    }
}
