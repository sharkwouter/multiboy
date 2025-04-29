#include "ScreenPlay.hpp"

#include <memory.h>

#include "../InputType.hpp"

#include <mgba/core/core.h>
#include <mgba/core/config.h>

ScreenPlay::ScreenPlay(std::string rom) {
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

    mCoreConfigInit(&core->config, NULL);
    mCoreConfigLoad(&core->config);

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
        case InputType::B:
        case InputType::LEFT:
        case InputType::RIGHT:
        case InputType::UP:
        case InputType::DOWN:
        default:
            break;
    }
}

void ScreenPlay::update() {

}

void ScreenPlay::draw(SDL_Renderer * renderer, SDL_Rect * dst_rect) {
    if (!screen) {
        screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, render_width, render_height);
        SDL_LockTexture(screen, NULL, (void**) &screen_buffer, &render_pitch);
    
        core->setVideoBuffer(core, screen_buffer, render_pitch / BYTES_PER_PIXEL);
        core->reset(core);
    }

    SDL_LockTexture(screen, NULL, (void**) &screen_buffer, &render_pitch);
    core->runFrame(core);
    SDL_UnlockTexture(screen);
    SDL_RenderCopy(renderer, screen, NULL, dst_rect);
}
