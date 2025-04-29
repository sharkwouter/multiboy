#ifndef SCREENPLAY_HPP
#define SCREENPLAY_HPP

#include <SDL.h>

#define ENABLE_VFS
#define ENABLE_DIRECTORIES

#include <mgba/core/core.h>
#include <string>

#include "ScreenBase.hpp"

class ScreenPlay : public ScreenBase {
private:
    struct mCore * core = nullptr;
    SDL_Texture * screen = nullptr;
    mColor * screen_buffer = nullptr;
    unsigned int render_width, render_height;
    int render_pitch;
public:
    ScreenPlay(std::string rom);
    ~ScreenPlay();

    void handleInput(Input input);
    void update();
    void draw(SDL_Renderer * renderer, SDL_Rect * dst_rect);
};

#endif // SCREENPLAY_HPP