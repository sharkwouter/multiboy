#include "ScreenPlay.hpp"

#include <memory.h>

#include "../InputType.hpp"

#include <mgba/core/core.h>
#include <mgba/core/config.h>

ScreenPlay::ScreenPlay(std::string rom, std::string player_name) {
    core = mCoreFind(rom.c_str());
    if (!core) {
        SDL_Log("Could not load game");
        return;
    }
    if(!core->init(core)) {
        SDL_Log("Init failed");
        return;
    }

    if (!mCoreLoadFile(core, rom.c_str())) {
        SDL_Log("Failed to load file");
        core->deinit(core);
        core = nullptr;
        return;
    }

    mCoreConfigInit(&core->config, player_name.c_str());
    mCoreConfigLoad(&core->config);

    mCoreLoadSaveFile(core, std::string(rom + "." + player_name + ".sav").c_str(), false);

    mCoreLoadConfig(core);

    core->baseVideoSize(core, &render_width, &render_height);
    screen_buffer = (mColor *) malloc(render_width * render_height * BYTES_PER_PIXEL);
}

ScreenPlay::~ScreenPlay() {
    free(screen_buffer);
    SDL_DestroyTexture(screen);
    mCoreConfigDeinit(&core->config);
    core->deinit(core);
}

void ScreenPlay::handleInput(Input input) {
    switch (input.type)
    {
        case InputType::A:
            keys_pressed |= 1;
            break;
        case InputType::A_RELEASED:
            keys_pressed &= ~1;
            break;
        case InputType::B:
            keys_pressed |= 2;
            break;
        case InputType::B_RELEASED:
            keys_pressed &= ~2;
            break;
        case InputType::SELECT:
            keys_pressed |= 4;
            break;
        case InputType::SELECT_RELEASED:
            keys_pressed &= ~4;
            break;
        case InputType::START:
            keys_pressed |= 8;
            break;
        case InputType::START_RELEASED:
            keys_pressed &= ~8;
            break;
        case InputType::RIGHT:
            keys_pressed |= 16;
            break;
        case InputType::RIGHT_RELEASED:
            keys_pressed &= ~16;
            break;
        case InputType::LEFT:
            keys_pressed |= 32;
            break;
        case InputType::LEFT_RELEASED:
            keys_pressed &= ~32;
            break;
        case InputType::UP:
            keys_pressed |= 64;
            break;
        case InputType::UP_RELEASED:
            keys_pressed &= ~64;
            break;
        case InputType::DOWN:
            keys_pressed |= 128;
            break;
        case InputType::DOWN_RELEASED:
            keys_pressed &= ~128;
            break;
        case InputType::R:
            keys_pressed |= 256;
            break;
        case InputType::R_RELEASED:
            keys_pressed &= ~256;
            break;
        case InputType::L:
            keys_pressed |= 512;
            break;
        case InputType::L_RELEASED:
            keys_pressed &= ~512;
            break;
        default:
            return;
    }
}

void ScreenPlay::update() {
    core->setKeys(core, keys_pressed);
}

void ScreenPlay::draw(SDL_Renderer * renderer, SDL_Rect * dst_rect) {
    if (!screen) {
        screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, render_width, render_height);
        SDL_SetTextureScaleMode(screen, SDL_ScaleModeNearest);
        SDL_LockTexture(screen, NULL, (void**) &screen_buffer, &render_pitch);
    
        core->setVideoBuffer(core, screen_buffer, render_pitch / BYTES_PER_PIXEL);
        core->reset(core);
    }

    SDL_LockTexture(screen, NULL, (void**) &screen_buffer, &render_pitch);
    core->runFrame(core);
    SDL_UnlockTexture(screen);
    
    // Calculate how to display screen texture
    unsigned int real_width, real_height;
    this->core->currentVideoSize(core, &real_width, &real_height);
    int size_multiplier = std::min(dst_rect->w / (int) real_width, dst_rect->h / (int) real_height);

    SDL_Rect src_rect = {0, 0, (int) real_width, (int) real_height};

    dst_rect->x += dst_rect->w / 2 - (int) real_width * size_multiplier / 2;
    dst_rect->y += dst_rect->h / 2 - (int) real_height * size_multiplier / 2;
    dst_rect->w = (int) real_width * size_multiplier;
    dst_rect->h = (int) real_height * size_multiplier;
    
    SDL_RenderCopy(renderer, screen, &src_rect, dst_rect);
}
