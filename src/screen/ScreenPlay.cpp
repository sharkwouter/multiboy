#include "ScreenPlay.hpp"

#include <filesystem>
#include <memory.h>

#include "../utils.hpp"
#include "../InputType.hpp"
#include "../constants.hpp"

#include <mgba/core/cheats.h>
#include <mgba/core/config.h>
#include <mgba/core/core.h>
#include <mgba/core/log.h>
#include <mgba/core/serialize.h>
#include <mgba/debugger/debugger.h>

ScreenPlay::ScreenPlay(std::string rom) {
    core = mCoreFind(rom.c_str());
    core->init(core);
}

ScreenPlay::~ScreenPlay() {
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

}
